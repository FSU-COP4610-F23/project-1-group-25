#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

