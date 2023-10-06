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
				break;
			}
		}
		char *outp;
		char *inp;
		int cntr = 0;
		int cntr2 = 0;	
		for (int i = 0; i < tokens->size; i++)
		{
			if (!(strcmp(tokens->items[i],">")))
			{
				cntr++;
				if (cntr > 1)
				{
					printf("Too many arguments for >\n");
				}
				outp = tokens->items[i+1];
				printf("%s is what we're writing to!\n", outp);
			}
			else if (!(strcmp(tokens->items[i], "<")))
			{
				cntr2++;
				if (cntr2 > 1)
				{
					printf("Too many arguments for >\n");
				}
				inp = tokens->items[i+1];
				printf("%s is what we're writing from!\n", inp);
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
					printf("Change one dir back\n");
					prevDir = getcwd(currdir, 100);
					printf("Prev Dir: %s", prevDir);
					chdir("..");
				}
				else if(!(strcmp(dir, "/")))
				{
					printf("Change to root directory\n");
					prevDir = getcwd(currdir, 100);
					chdir("/");
				}
				else if (!(strcmp(dir, "-")))
				{
					if (prevDir == NULL)
					{
						printf("No such file or directory.\n");
					}
					else
					{
						printf("Change to previous directory\n");
						chdir("-");
					}
				}
				else
				{
					prevDir = getcwd(currdir, 100);
					if(chdir(dir) == -1)
					{
						printf("Failure\n");
					}
					else
					{
						printf("Success\n");
					}
				}
			}
			else if (tokens->size == 1)
			{
				printf("just cd\n");
				chdir(getenv("HOME"));
			}
			else
			{
				printf("Too many arguments\n");
			}
		}
		else
		{
			char *pSearch = path_Search(tokens);
			if (!strcmp(pSearch, "failure"))
			{
				printf("%s:", tokens->items[0]);
				printf("Command not found\n");
			}
			else
			{
				pid_t pid;
				pid = fork();
				if (pid == 0)
				{
					char ** argC = (char **)calloc(tokens->size - 1, sizeof(char));
					for (int i = 0; i < tokens->size; i++)
					{
						argC[i] = tokens->items[i];
					}
					execv(pSearch, argC);
					exit(0);
				}
				else
				{
					wait(NULL);
				}
			}
		}
		free(input);
		free_tokens(tokens);
	}

	return 0;
}

char *path_Search(tokenlist* tokens)
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
		strlen(tokens->items[0]) + 1, sizeof(char));
		char *bs_adder = "/";
		strcpy(accTest, directories->items[i]);
		strcat(accTest,bs_adder);
		strcat(accTest,tokens->items[0]);
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
