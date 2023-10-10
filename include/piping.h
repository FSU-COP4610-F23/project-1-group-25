#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile);
void b_singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile);
void doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile);
void b_doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile);
