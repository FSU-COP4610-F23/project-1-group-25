#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	char * s = "exit";

	while (1) {
		//getenv() should be machine for linprog//
		printf("%s@%s:%s", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
		printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		/*for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
			
		}*/
		// PART 4
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
		
		char *pSearch = path_Search(tokens);
		if (!strcmp(pSearch, "failure"))
		{
			printf("%s:", tokens->items[0]);
			printf("Command not found\n");
		}
		else
		{
			printf("found in directory:\n");
			printf("(%s)\n", pSearch);
			if (fork() == 0)
			{
				execv(pSearch, tokens);
			}
		}
		
		if (!strcmp(tokens->items[0], "echo"))
		{
			for (int i = 1; i < tokens->size; i++)
			{
				printf("%s ", tokens->items[i]);
			}
			printf("\n");
		}

		/* temporary exit functionality to prevent exit through
		 * CTRL + Z
		 * if the token list is only of size 1, and that token
		 * is the word "exit", then break the loop and exit
		 */

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
		char *test = (char *)calloc(strlen(directories->items[i]) + 2 + strlen(tokens->items[0]) + 1, sizeof(char));
		char *test2 = "/";
		strcpy(test, directories->items[i]);
		strcat(test,test2);
		strcat(test,tokens->items[0]);
		if(access(test, F_OK) == 0)
		{
			//printf("found in directory:\n");
			//printf("(%s)\n", directories->items[i]);
			//add a flag to say "not found"//
			counter++;
			return test;
		}
		//printf("directories %d: (%s)\n", i, directories->items[i]);
	}
	if (counter == 0)
	{
		//printf("%s:", tokens->items[0]);
		//printf("Command not found.\n");
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
				char *test = (char *)calloc(strlen(tok) - 1, sizeof(char));

				for (int i = 1; i < strlen(tok); i++)
				{
					test[i-1] = tok[i];
					
				}
				if (getenv(test) == NULL)
				{
					printf("%s: Undefined variable.\n", test);
					free(test);
					break;
				}
				tok = getenv(test);
			}
			else
			{
				char *test = (char *)calloc(strlen(tok) - 2, sizeof(char));

				for (int i = 2; i < strlen(tok); i++)
				{
					test[i-2] = tok[i];
					
				}
				if (getenv(test) == NULL)
				{
					printf("%s: Undefined variable.\n", test);
					free(test);
					break;
				}
				tok = getenv(test);
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
