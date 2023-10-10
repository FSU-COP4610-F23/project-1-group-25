#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char **argList = (char **)calloc(argc, sizeof(char*));
	char * timeoutPath = path_Search("timeout");
	argList[0] = (char *)calloc(strlen(timeoutPath), sizeof(char));
	strcpy(argList[0], timeoutPath);
	for(int i = 1; i < argc; i++)
	{
		argList[i] = (char *)calloc(strlen(argv[i]), sizeof(char));
		argList[i] = argv[i];
	}
	pid_t pid = fork();
	int status;
	if(pid == 0)
	{
		execv(argList[0], argList);
		exit(1);
	}
	else
	{
		waitpid(pid, &status, WNOHANG);
	}
	return 0;
}
