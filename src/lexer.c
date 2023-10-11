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

char * get_outputfile(char *input)
{
	char *buf = (char *)calloc(strlen(input) + 1, sizeof(char));
	strcpy(buf, input);
	char *tok = strtok(buf, " ");

	while (tok != NULL)
	{
		if(tok[0] == '>')
		{
			tok = strtok(NULL, " ");
			return tok;
		}
		tok = strtok(NULL, " ");
	}
	free(buf);
	return NULL;
}

char * get_inputfile(char *input)
{
	char *buf = (char *)calloc(strlen(input) + 1, sizeof(char));
	strcpy(buf, input);
	char *tok = strtok(buf, " ");

	while (tok != NULL)
	{
		if(tok[0] == '<')
		{
			tok = strtok(NULL, " ");
			return tok;
		}
		tok = strtok(NULL, " ");
	}
	free(buf);
	return NULL;
}

tokenlist *get_tokens(char *input) {
	char *buf = (char *)calloc(strlen(input) + 1, sizeof(char));
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	char *tok = strtok(buf, " ");
	bool outputExists = false;
	bool inputExists = false;

	while (tok != NULL)
	{
		if(tok[0] == '>')
		{
			if(outputExists)
			{
                		printf("Ambiguous output redirect.\n");
				free_tokens(tokens);
				free(buf);
				return NULL;
			}
			outputExists = true;
			tok = strtok(NULL, " ");
			tok = strtok(NULL, " ");
			continue;
		}
		if(tok[0] == '<')
		{
			if(inputExists)
			{
                		printf("Ambiguous input redirect.\n");
				free_tokens(tokens);
				free(buf);
				return NULL;
			}
			inputExists = true;
			tok = strtok(NULL, " ");
			tok = strtok(NULL, " ");
			continue;
		}
		if((tok[0] == '~') || (tok[0] == '$'))
		{
			char * envVar = expandEnv(tok);
			if(envVar == NULL)
			{
				free_tokens(tokens);
				return NULL;
			}
			add_token(tokens, envVar);
			tok = strtok(NULL, " ");
			continue;
		}
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}
	free(buf);
	return tokens;
}

char * expandEnv(char * tok)
{
	if(tok[0] == '$')
        {
        	char *envVar = (char *)calloc(strlen(tok) - 1, sizeof(char));

                for(int i = 1; i < strlen(tok); i++)
                {
                	envVar[i-1] = tok[i];
                }
                if(getenv(envVar) == NULL)
                {
                	printf("%s: Undefined variable.\n", envVar);
                        free(envVar);
                        return NULL;
                }
                tok = getenv(envVar);
                free(envVar);
		return tok;
	}
        if(tok[0] == '~')
        {
        	char *tilde = getenv("HOME");
                char *combine = (char *)calloc(strlen(tilde) + strlen(tok) - 1, sizeof(char));
                for(int i = 0; i < strlen(tilde); i++)
                {
                	combine[i] = tilde[i];
                }
                int j = 1;
                for(int i = strlen(tilde); j <= strlen(tok); i++)
                {
                	combine[i] = tok[j];
                        j++;
                }
                tok = combine;
		return combine;
	}
	return NULL;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}
