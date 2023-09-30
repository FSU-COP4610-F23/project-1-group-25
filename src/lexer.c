#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
			
		}


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
		
		free(input);
		free_tokens(tokens);
	}

	return 0;
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
		if(tok[0] == '~')
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
