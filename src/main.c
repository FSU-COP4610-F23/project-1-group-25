#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	pid_t waitpid(pid_t pid, int *stat_loc, int options);
	char * s = "exit";	//used for exit
	char currdir[100];	//holds the current directory for cd
	char * prevDir = NULL;	//holds the previous directory for cd
	pid_t pids[10];
	int p_iterator = 0;
	int status=0;
	
	while (1) {
		//getenv() should be machine for linprog//

		printf("%s@%s:%s", getenv("USER"), getenv("MACHINE"), getcwd(currdir, 100));
		printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */
		char *input = get_input();
		if (!strcmp(input, ""))
		{
			continue;
		}

		//These hold the file names for I/O redirects
		char *inputFile = get_inputfile(input);
		char *outputFile = get_outputfile(input);

		//Retrieves token list
		tokenlist *tokens = get_tokens(input);
		if(tokens == NULL)
		{
			continue;
		}

		if (tokens->size == 1)
		{
			//exit command
			if (!strcmp(tokens->items[0], s))
			{
				printf("logout\n");
				free(input);
				free_tokens(tokens);
				if (prevDir != NULL)
				{
					free(prevDir);
				}
				break;
			}
		}

		//cd command
		if (!(strcmp(tokens->items[0], "cd")))
		{
			if (tokens->size == 2)
			{
				char * dir = (char *)calloc(tokens->size+1, sizeof(char));
				strcpy(dir, tokens->items[1]);

				if(!(strcmp(dir, "..")))
				{
					//Change one directory back
					prevDir = getcwd(prevDir, 100);
					chdir("..");
				}
				else if(!(strcmp(dir, "/")))
				{
					//Change to root directory
					prevDir = getcwd(prevDir, 100);
					chdir("/");
				}
				else if (!(strcmp(dir, "-")))
				{
					//If no previous directory, fail function
					if (prevDir[0] == '\0')
					{
						printf("No such file or directory.\n");
					}
					else
					{
						//Change to previous directory
						chdir(prevDir);
						prevDir = NULL;
					}
				}
				else
				{
					//For cd with path argument
					prevDir = getcwd(prevDir, 100);
					if(chdir(dir) == -1)
					{
						printf("No such file or directory.\n");
					}
				}
				free(dir);
			}
			else if (tokens->size == 1)
			{
				//if cd is alone//
				prevDir = getcwd(prevDir, 100);
				chdir(getenv("HOME"));
			}
			else
			{
				printf("Too many arguments\n");
				continue;
			}
		}
		//Executing commands
		else
		{
			//setup for piping when needed
			////////////////////////////////////////////////////////////////////////
			char ***Cargs = (char ***)calloc(10, sizeof(char**));
			////////////////////////////////////////////////////////////////////////
			// create an array of search paths, a path for each command in input
			////////////////////////////////////////////////////////////////////////
			char **csp = (char **)calloc(strlen(getenv("PATH")) + 1, sizeof(char));
			int count = 0;  //commandCount
			int argToks = 0;
			bool isCommand = true;
			int pipeCount = 0;
			for(int i = 0; i < tokens->size; i++){
				if(isCommand){
					csp[count] = path_Search(tokens->items[i]);
					if (!strcmp(csp[count], "failure"))
					{
						printf("%s:", tokens->items[i]);
						printf("Command not found\n");
					}
					Cargs[count] = (char **)calloc(tokens->size+1, sizeof(char*));
					isCommand = false;
				}

				if(!strcmp(tokens->items[i], "|")){
					count++;
					argToks = 0;
					isCommand = true;
					pipeCount++;
				}
				else{
					Cargs[count][argToks] = tokens->items[i];
					argToks++;
				}
			}
			//Runs background processes
			if(!(strcmp(Cargs[count][argToks-1], "&")))
			{
				Cargs[count][argToks-1] = NULL;
				if(pipeCount == 1){
					b_singlePiping(csp, Cargs, count+1, inputFile, outputFile);
				}
				else if (pipeCount == 2)
				{
					b_doublePiping(csp, Cargs, count+1, inputFile, outputFile);
				}
				else{
					pid_t pid;
					pid = fork();
					if(pid == 0){
						execv(csp[0], Cargs[0]);

						exit(1);
					}
					else{
						pids[p_iterator] = pid;
						printf("[%d] %d\n", p_iterator+1, pids[p_iterator]);
						waitpid(pid, &status, WNOHANG);
						
					}
				}
			}
			//runs I/O redirects
			else if((inputFile != NULL) || (outputFile != NULL)){
				pid_t pid = fork();
	 			if (pid == 0)
				{
					// If char * inputFile is not NULL 
					if(inputFile != NULL){

						// Open inputfile with user permissions 
						int fd = open(inputFile, O_RDWR, S_IRUSR | S_IWUSR);

						dup2(fd, 0);    // 0 means opening for input 

						// close input file
						close(fd);

						// Execute command
					}
					// if char * outputFile is not NULL
					if(outputFile != NULL){

						// Open outputFile with user permissions
						int fd = open(outputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
						dup2(fd, 1);    // 1 means opening for output
						close(fd);
					}
				execv(csp[0], Cargs[0]);
		
			}
			else{
				if(!strcmp(tokens->items[tokens->size-1], "&"))
				{	
					Cargs[count][argToks-1] = NULL;
					waitpid(pid, NULL, WNOHANG);
				}
				else
				{
					waitpid(pid, NULL, 0);
				}
			}	
		}
			else
			{
				if(pipeCount == 1){
					singlePiping(csp, Cargs, count+1, inputFile, outputFile);
				}
				else if (pipeCount == 2)
				{
					doublePiping(csp, Cargs, count+1, inputFile, outputFile);
				}
				else{
					pid_t pid = fork();
					if(pid == 0){
						execv(csp[0], Cargs[0]);
						exit(0);
					}
					else{
						waitpid(pid, NULL, 0);
					}
				}
			}
			//dont forget to free
			//
			free(csp);
			for(int i = 0; i < count; i++){
				free(Cargs[i]);
			}
			free(Cargs);

		}
		free(input);
		free_tokens(tokens);
	}

	return 0;
}
