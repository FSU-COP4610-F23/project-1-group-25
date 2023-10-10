#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


void singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile){
        int fd[2];	// pipe input and output array
	
	//check if valid pipe
        if( pipe(fd) == -1){
		perror("bad pipe");
		exit(1);
	}
	
        pid_t pid1;	// first process id 

	// check if valid fork()
	if((pid1 = fork()) == -1){
		perror("bad fork pid1");
		exit(1);
	}

	// if fork valid, execute child process
        if(pid1 == 0){
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);

                if(inputFile != NULL){

                        // Open inputfile with user permissions
                        int fd;
                        if((fd = open(inputFile, O_RDWR, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening input");
				exit(1);
			}

                        dup2(fd, 0);    // 0 means opening for input

                        // close input file
                        close(fd);
                        // Execute command
                }

                execv(cmdPaths[0], cmdArgs[0]);
		// if exec does not work
		perror("bad execution");
		exit(1);
        }


        pid_t pid2;	// second process id 
	
	//check if fork is valid
	if((pid2 = fork()) == -1){
		perror("bad fork pid2");
		exit(1);
	}
	
	// if fork valid, execute child process
        if(pid2 == 0){

                dup2(fd[0], 0);
                close(fd[0]);
                close(fd[1]);

                // if char * outputFile is not NULL
                if(outputFile != NULL){

                        // Open outputFile with user permissions
                        int fd;
                        if((fd = open(outputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening output");
				exit(1);
			}
                        dup2(fd, 1);    // 1 means opening for output
                        close(fd);
                }

                execv(cmdPaths[1], cmdArgs[1]);
		// if execution error
		perror("bad execution");
		exit(1);

        }

        close(fd[0]);
        close(fd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
	exit(0);
}

void b_singlePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile){
        int fd[2];	// pipe input and output array

	//check if valid pipe
        if( pipe(fd) == -1){
		perror("bad pipe");
		exit(1);
	}

	pid_t pid1;	// first process id 
        
	if((pid1 = fork()) == -1){
		perror("bad fork pid1");
		exit(1);
	}
        int status1;

	// if fork is valid, execute child process
        if(pid1 == 0){
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);

                if(inputFile != NULL){

                        // Open inputfile with user permissions
                        int fd;
                        if((fd = open(inputFile, O_RDWR, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening input");
				exit(1);
			}

                        dup2(fd, 0);    // 0 means opening for input

                        // close input file
                        close(fd);
                        // Execute command
                }


                execv(cmdPaths[0], cmdArgs[0]);
		// if exec does not work
		perror("bad execution");
		exit(1);
        }


        pid_t pid2;	// second process id 
	
	//check if fork is valid
	if((pid2 = fork()) == -1){
		perror("bad fork2");
		exit(1);
	}

        int status2;

	// if fork valid, execute child process
        if(pid2 == 0){

                dup2(fd[0], 0);
                close(fd[0]);
                close(fd[1]);

                // if char * outputFile is not NULL
                if(outputFile != NULL){

                        // Open outputFile with user permissions
                        int fd;
                        if((fd = open(outputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening output");
				exit(1);
			}

                        dup2(fd, 1);    // 1 means opening for output
                        close(fd);
                }


                execv(cmdPaths[1], cmdArgs[1]);
		// if execution error
		perror("bad execution");
		exit(1);

        }

        close(fd[0]);
        close(fd[1]);

        waitpid(pid1, &status1, WNOHANG);
        waitpid(pid2, &status2, WNOHANG);
}

void doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile){
        pid_t pid1, pid2, pid3;	// process ids for all processes 
	
        int pipe1[2];	// first pipe
        int pipe2[2];	// second pipe

	
        if (pipe(pipe1) == -1){
		perror("bad pipe1");
		exit(1);
	}

        if((pid1 = fork()) == -1){
		perror("bad fork pid1");
		exit(1);
	}
	
	// if fork valid, execute child process
        if(pid1 == 0){
                // input from stdin, output to pipe1
                dup2(pipe1[1], 1);

                close(pipe1[0]);
                close(pipe1[1]);

                if(inputFile != NULL){

                        // Open inputfile with user permissions
                        int fd;
                        if((fd = open(inputFile, O_RDWR, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening input");
				exit(1);
			}

                        dup2(fd, 0);    // 0 means opening for input

                        // close input file
                        close(fd);
                        // Execute command
                }



                execv(cmdPaths[0], cmdArgs[0]);
		// if exec does not work
		perror("bad execution");

                exit(0);
        }

        if( pipe(pipe2) == -1){
		perror("bad pipe2");
		exit(1);
	}

	
        if ((pid2 = fork()) == -1){
		perror("bad fork pid2");
		exit(1);
	}

	// if pid2 is valid, execute child process 
        if(pid2 == 0){
                // input pipe1
                dup2(pipe1[0], 0);
                // output pipe2
                dup2(pipe2[1], 1);

                //close fd's
                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);


                execv(cmdPaths[1], cmdArgs[1]);
		// if exec does not work
		perror("bad execution");
                exit(0);
        }


        close(pipe1[0]);
        close(pipe1[1]);


        if((pid3 = fork()) == -1){
		perror("bad fork pid3");
                exit(0);
	}
	
	// if pid3 is valid, execute child process
        if(pid3 == 0){
                //input from pipe2
                dup2(pipe2[0], 0);

                //output to stdout (already done) close fds
                close(pipe2[0]);
                close(pipe2[1]);


                //// if char * outputFile is not NULL
                if(outputFile != NULL){

                        // Open outputFile with user permissions
                        int fd;
                        if((fd = open(outputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening output");
				exit(1);
			}
                        dup2(fd, 1);    // 1 means opening for output
                        close(fd);
                }


                execv(cmdPaths[2], cmdArgs[2]);
		// if execution error
		perror("bad execution");
                exit(0);
        }

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
}

void b_doublePiping(char** cmdPaths, char*** cmdArgs, int cmdCount, char * inputFile, char * outputFile){
        pid_t pid1, pid2, pid3;	// process ids for all processes

        int status1, status2;

        int pipe1[2];	// first pipe
        int pipe2[2];	// second pipe

        if (pipe(pipe1) == -1){
		perror("bad pipe1");
		exit(1);
	}

        if((pid1 = fork()) == -1){
		perror("bad fork pid1");
		exit(1);
	}


	// if fork valid, execute child process
        if(pid1 == 0){
                // input from stdin, output to pipe1
                dup2(pipe1[1], 1);

                close(pipe1[0]);
                close(pipe1[1]);

                if(inputFile != NULL){

                        // Open inputfile with user permissions
                        int fd;
                        if((fd = open(inputFile, O_RDWR, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening input");
				exit(1);
			}

                        dup2(fd, 0);    // 0 means opening for input

                        // close input file
                        close(fd);
                        // Execute command
                }



                execv(cmdPaths[0], cmdArgs[0]);
		// if exec does not work
		perror("bad execution");

                exit(0);
        }

        if( pipe(pipe2) == -1){
		perror("bad pipe2");
		exit(1);
	}

        if ((pid2 = fork()) == -1){
		perror("bad fork pid2");
		exit(1);
	}

	// if pid2 is valid, execute child 
        if(pid2 == 0){
                // input pipe1
                dup2(pipe1[0], 0);
                //output pipe2
                dup2(pipe2[1], 1);

                //close fd's
                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);

                execv(cmdPaths[1], cmdArgs[1]);
		// if exec does not work
		perror("bad execution");
                exit(0);
        }


        close(pipe1[0]);
        close(pipe1[1]);

        if((pid3 = fork()) == -1){
		perror("bad fork pid3");
                exit(0);
	}
	
	// if pid3 is valid, execute child process
        if(pid3 == 0){
                //input from pipe2
                dup2(pipe2[0], 0);

                //output to stdout (already done) close fds
                close(pipe2[0]);
                close(pipe2[1]);


                //// if char * outputFile is not NULL
                if(outputFile != NULL){

                        // Open outputFile with user permissions
                        int fd;
                        if((fd = open(outputFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
				perror("error opening output");
				exit(1);
			}
                        dup2(fd, 1);    // 1 means opening for output
                        close(fd);
                }



                execv(cmdPaths[2], cmdArgs[2]);
		// if execution error
		perror("bad execution");
                exit(0);
        }

        waitpid(pid1, &status1, WNOHANG);
        waitpid(pid2, &status2, WNOHANG);
}
