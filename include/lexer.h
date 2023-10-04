#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
    char ** items;
    size_t size;
} tokenlist;

char * get_input(void);
tokenlist * get_tokens(char *input);
tokenlist * new_tokenlist(void);
char * path_Search(tokenlist* tokens);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
