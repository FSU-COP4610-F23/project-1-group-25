#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char * s = "exit";
	char currdir[100];
	char * prevDir;
	tokenlist last_three;
	while (1) {
		//getenv() should be machine for linprog//
		printf("%s@%s:%s", getenv("USER"), getenv("MACHINE"), getcwd(currdir, 100));
		printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */
		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);

		if (tokens->size == 1)
		{
			if (!strcmp(tokens->items[0], s))
			{
				printf("exit declared\n");
				free(input);
				free_tokens(tokens);
				free(prevDir);
				break;
			}
		}
		
		if (!(strcmp(tokens->items[0], "cd")))
		{
			if (tokens->size == 2)
			{
				char * dir = (char *)calloc(tokens->size - 1, sizeof(char));
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
					if (prevDir == NULL)
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
				printf("Prev Dir: %s\n", prevDir);
				chdir(getenv("HOME"));
			}
			else
			{
				printf("Too many arguments\n");
			}
		}
		else
		{
			//JAKE PIPING SECTION (PARSING AND SINGLE COMMANDS)
			/////////////////////////////////////////////////////////////////////////////////////
			char ***Cargs = (char ***)calloc(10, sizeof(char**));
			////////////////////////////////////////////////////////////////////////////////////
			// create an array of search paths, a path for each command in input
			////////////////////////////////////////////////////////////////////////////////////
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
					Cargs[count] = (char **)calloc(5, sizeof(char*));
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
			//////////////////////////////////////////////////////////////////////
			// maybe set an isPiping flag so i can only call piping function if piping
			// only call input output if input outout not null
			// and only call singler command if neither input output or piping


			//Should now have a list of commands with arguments as each token
			// and a list of path searches that correspond to each command argument token
			// REMEMBER
			// arglist right now is a list of char string tokens with each command and argument appended to it, remember to parse each token out into its own array for exec function
			// include some type of delimiter for each cmnd and argument so i can easily do this

			if(!(strcmp(Cargs[count][argToks-1], "&")))
			{
				Cargs[count][argToks-1] = NULL;
				if(pipeCount == 1){
					b_singlePiping(csp, Cargs, count+1);
				}
				else if (pipeCount == 2)
				{
					b_doublePiping(csp, Cargs, count+1);
				}
				else{
					pid_t pid = fork();
					int status;
					if(pid == 0){
						execv(csp[0], Cargs[0]);
						exit(1);

					}
					else{
						waitpid(pid, &status, WNOHANG);
					}
				}
			}
			else
			{
				if(pipeCount == 1){
					singlePiping(csp, Cargs, count+1);
				}
				else if (pipeCount == 2)
				{
					doublePiping(csp, Cargs, count+1);
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

			//END OF JAKES CODE
		}
		free(input);
		free_tokens(tokens);
	}
	

	return 0;
}

void singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount){
	int fd[2];
	pipe(fd);
	pid_t pid1 = fork();

	if(pid1 == 0){
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execv(cmdPaths[0], cmdArgs[0]);
	}


	int pid2 = fork();

	if(pid2 == 0){

		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execv(cmdPaths[1], cmdArgs[1]);

	}

	close(fd[0]);
	close(fd[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void b_singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount){
	int fd[2];
	pipe(fd);
	pid_t pid1 = fork();
	int status1;

	if(pid1 == 0){
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execv(cmdPaths[0], cmdArgs[0]);
	}


	int pid2 = fork();
	int status2;

	if(pid2 == 0){

		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execv(cmdPaths[1], cmdArgs[1]);

	}

	close(fd[0]);
	close(fd[1]);

	waitpid(pid1, &status1, WNOHANG);
	waitpid(pid2, &status2, WNOHANG);
}

void doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount){
	pid_t pid1, pid2, pid3;
	int pipe1[2];
	int pipe2[2];

	pipe(pipe1);

	pid1 = fork();
	if(pid1 == 0){
		// input from stdin, output to pipe1
		dup2(pipe1[1], 1);

		close(pipe1[0]);
		close(pipe1[1]);

		execv(cmdPaths[0], cmdArgs[0]);

		exit(0);
	}

	pipe(pipe2);

	pid2 = fork();
	if(pid2 == 0){
		// input pipe1
		dup2(pipe1[0], 0);
		//output pipe2
		dup2(pipe2[1], 1);

		//close fd's
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		execv(cmdPaths[1], cmdArgs[1]);
		exit(0);
	}


	close(pipe1[0]);
	close(pipe1[1]);

	pid3 = fork();
	if(pid3 == 0){
		//input from pipe2
		dup2(pipe2[0], 0);

		//output to stdout (already done) close fds
		close(pipe2[0]);
		close(pipe2[1]);

		execv(cmdPaths[2], cmdArgs[2]);
		exit(0);
	}

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	//waitpid(pid3, NULL, 0);
}

void b_doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount){
	pid_t pid1, pid2, pid3;
	int status1, status2, status3;
	int pipe1[2];
	int pipe2[2];

	pipe(pipe1);

	pid1 = fork();
	if(pid1 == 0){
		// input from stdin, output to pipe1
		dup2(pipe1[1], 1);

		close(pipe1[0]);
		close(pipe1[1]);

		execv(cmdPaths[0], cmdArgs[0]);

		exit(0);
	}

	pipe(pipe2);

	pid2 = fork();
	if(pid2 == 0){
		// input pipe1
		dup2(pipe1[0], 0);
		//output pipe2
		dup2(pipe2[1], 1);

		//close fd's
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		execv(cmdPaths[1], cmdArgs[1]);
		exit(0);
	}


	close(pipe1[0]);
	close(pipe1[1]);

	pid3 = fork();
	if(pid3 == 0){
		//input from pipe2
		dup2(pipe2[0], 0);

		//output to stdout (already done) close fds
		close(pipe2[0]);
		close(pipe2[1]);

		execv(cmdPaths[2], cmdArgs[2]);
		exit(0);
	}

	waitpid(pid1, &status1, WNOHANG);
	waitpid(pid2, &status2, WNOHANG);
}

char *path_Search(char* tokens)
{
	char *buf = (char *)calloc(strlen(getenv("PATH")) + 1, sizeof(char));
	strcpy(buf, getenv("PATH"));
	tokenlist *directories = new_tokenlist();
	char *tok = strtok(buf, ":");

	while(tok != NULL)
	{
		add_token(directories, tok);
		tok = strtok(NULL, ":");
	}
	free(buf);

	int counter = 0;
	for (int i = 0; i < directories->size; i++)
	{
		char *accTest = (char *)calloc(strlen(directories->items[i]) + 2 + 
		strlen(tokens) + 1, sizeof(char));
		char *bs_adder = "/";
		strcpy(accTest, directories->items[i]);
		strcat(accTest,bs_adder);
		strcat(accTest,tokens);
		if(access(accTest, F_OK) == 0)
		{
			counter++;
			return accTest;
		}
	}
	if (counter == 0)
	{
		char * val = "failure";
		return val;
	}
	else
	{
		char * val = "failure";
		return val;
	}
}

char *get_input(void) {
	char *buffer = NULL;
	int bufsize = 0;
	char line[5];
	while (fgets(line, 5, stdin) != NULL)
	{
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;
		buffer = (char *)realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;
		if (newln != NULL)
			break;
	}
	buffer = (char *)realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;
	return buffer;
}

tokenlist *new_tokenlist(void) {
	tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **)malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item) {
	int i = tokens->size;

	tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *)malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

tokenlist *get_tokens(char *input) {
	char *buf = (char *)malloc(strlen(input) + 1);
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	char *tok = strtok(buf, " ");
	
	while (tok != NULL)
	{
		/*Parts 2 and 3, make both into function calls for cleaner code*/

		/*Checks if first char in token is a '$', then changes it to the
		 * getenv() version of that $. If the getenv() returns NULL, the
		 * an error message will appear.
		 */
		if(tok[0] == '$' || (tok[0] == '/' && tok[1] == '$'))
		{
			if(tok[0] == '$')
			{
				char *envCheck = (char *)calloc(strlen(tok) - 1, sizeof(char));

				for (int i = 1; i < strlen(tok); i++)
				{
					envCheck[i-1] = tok[i];
					
				}
				if (getenv(envCheck) == NULL)
				{
					printf("%s: Undefined variable.\n", envCheck);
					free(envCheck);
					break;
				}
				tok = getenv(envCheck);
			}
			else
			{
				char *envCheck = (char *)calloc(strlen(tok) - 2, sizeof(char));

				for (int i = 2; i < strlen(tok); i++)
				{
					envCheck[i-2] = tok[i];
				}
				if (getenv(envCheck) == NULL)
				{
					printf("%s: Undefined variable.\n", envCheck);
					free(envCheck);
					break;
				}
				tok = getenv(envCheck);
			}

		}
		/*Checks if first char in token is a '~', then changes it to the
		 * full path "/home/$USER" and moves it into the tok[0] value.
		 * No error checking as of now, may not need any.
		 */
		if(tok[0] == '~' || (tok[0] == '~' && tok[1] == '/'))
		{
			char *tilde = getenv("HOME");
			char *combine = (char *)calloc((strlen(tilde) + (strlen(tok) - 1)), sizeof(char));

			for (int i = 0; i < strlen(tilde); i++)
			{
				combine[i] = tilde[i];
			}
			int j = 1;
			for (int i = strlen(tilde); j <= strlen(tok); i++)
			{
				combine[i] = tok[j];
				j++;
			}
			tok = combine;
		}
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}
	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}
