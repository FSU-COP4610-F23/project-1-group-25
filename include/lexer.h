#pragma once
#include "piping.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

typedef struct {
    char ** items;
    size_t size;
} tokenlist;


char * get_input(void);
tokenlist * get_tokens(char *input);
tokenlist * new_tokenlist(void);
char * path_Search(char* tokens);
char *get_outputfile(char *input);
char *get_inputfile(char *input);
char *expandEnv(char *tok);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);