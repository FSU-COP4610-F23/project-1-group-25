[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/wtw9xmrw)
# Shell
This is a comprehensive shell interface created in the C language. The goal of this shell is to enhance process control, user interaction, and error handling mechanisms. 
## Group Members
- **John McLeod**: jtm21p@fsu.edu
- **Kaleb Szaro**: kms19t@fsu.edu
- **Jake Herren**: jmh18m@fsu.edu
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Created a prompt that the user will be greeted with. Indicates the absolute working directory, user name, and machine name.
- **Assigned to**: John Mcleod, Jake Herren

### Part 2: Environment Variables
- **Responsibilities**: Replaced tokens prefixed with the "$" character with their corresponding values.
- **Assigned to**: Jake Herren, Kaleb Szaro

### Part 3: Tilde Expansion
- **Responsibilities**: Expanded the tilde "~" to the environment variable $HOME whenever shown at the beginning o a path.
- **Assigned to**: Kaleb Szaro, John McLeod

### Part 4: $PATH Search
- **Responsibilities**: Created list of directories using the $PATH environment variable that is searched until the argument is found or not.
- **Assigned to**: John McLeod, Jake Herren

### Part 5: External Command Execution
- **Responsibilities**: Took command from input and created a child process that would execute said command using fork() and execv().
- **Assigned to**: Jake Herren, Kaleb Szaro

### Part 6: I/O Redirection
- **Responsibilities**: Provided implementation of file input/output redirection. Combining the two when necessary.
- **Assigned to**: Kaleb Szaro, John McLeod

### Part 7: Piping
- **Responsibilities**: Implemented simultaneous execution of multiple commands, with the input and output of these commands interconnected via Piping.
- **Assigned to**: John McLeod, Jake Herren

### Part 8: Background Processing
- **Responsibilities**: Created ability for shell to execute external commands without waiting for their completion. Keeping track of said commands was also created.
- **Assigned to**: Jake Herren, Kaleb Szaro

### Part 9: Internal Command Execution
- **Responsibilities**: Created exit, cd PATH, and jobs internal commands for the shell. 
- **Assigned to**: Kaleb Szaro, John McLeod

### Part 10: External Timeout Executable
- **Responsibilities**: Implemented the external executable "mytimeout" which offers enhanced control over command execution duration.
- **Assigned to**: John McLeod, Jake Herren

### Extra Credit
- **Responsibilities**: Create support for an unlimited number of pipes, support Piping and I/O redirection in a single command, and be able to execute your shell from within a running shell process.
- **Assigned to**: John McLeod, Jake Herren, Kaleb Szaro

## File Listing
```
project-1-group-25/
│
├── src/
│ └── lexer.c
| └── pathsearch.c
| └── mytimeout.c
│
├── include/
│ └── lexer.h
| └── pathsearch.h
│
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: e.g., `gcc` for C/C++, `rustc` for Rust.
- **Dependencies**: N/A (Programmed in C)

### Compilation
For linprog:
```bash
make
```
This will build the executable "shell" in bin
### Execution
```bash
./bin/shell
```
This will run the program ...

## Bugs
- **Bug 1**: When attempting to use I/O Redirection and Piping at the same time, double piping seems to skip the center command.
- I.E. - `ls | grep .md | cat > output.txt`
- This command would only place the output of ls into output.txt
- **Bug 2**: Background Processes will not show the PID and Queue after a command is executing in background.
- **Bug 3**: This is bug 3.

## Extra Credit
- **Extra Credit 1**: Support unlimited number of pipes.
- **Extra Credit 1 Documentation**: Only supports 2 pipes at the moment.
- **Extra Credit 2**: Support piping and I/O redirection in a single command.
- **Extra Credit 2 Documentation**: Piping and I/O redirection works for single pipes, but not 2 pipes.
- **Extra Credit 3**: Execute your shell from within a running shell process repeatedly.
- **Extra Credit 3 Documentation**: Shell-ception is currently not available.

## Considerations
- Combining I/O Redirection and Piping only works for single piping background processes. Double piping seems to skip the center command.
