#pragma once

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
tokenlist ** prevArgs(tokenlist* tokens, tokenlist ** test);
tokenlist * get_tokens(char *input);
tokenlist * new_tokenlist(void);
char * path_Search(char* tokens);
char *get_outputfile(char *input);
char *get_inputfile(char *input);
char *expandEnv(char *tok);
void singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount);
void b_singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount);
void doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount);
void b_doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
