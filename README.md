[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/wtw9xmrw)
# Shell
This is a comprehensive shell interface created in the C language. The goal of this shell is to enhance process control, user interaction, and error handling mechanisms. 
## Group Members
- **John McLeod**: jtm21p@fsu.edu
- **Kaleb Szaro**: kms19t@fsu.edu
- **Jake Herren**: jmh18m@fsu.edu
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Created a prompt that the user will be greeted with. Indicates the absolute working directory, user name, and machine name. Implementation research done by John, Jake, and Kaleb. Coded by John and Kaleb and committed to github by Kaleb.
- **Assigned to**: John Mcleod, Jake Herren, Kaleb Szaro

### Part 2: Environment Variables
- **Responsibilities**: Replaced tokens prefixed with the "$" character with their corresponding values. Implementation research done by John, Jake, and Kaleb. Coded by John and Kaleb and committed to github by Kaleb. 
- **Assigned to**: John McLeod, Jake Herren, Kaleb Szaro

### Part 3: Tilde Expansion
- **Responsibilities**: Expanded the tilde "~" to the environment variable $HOME whenever shown at the beginning of a path. Implementation research done by Jake, John, and Kaleb. Coded by John and Kaleb and committed to github by Kaleb.
- **Assigned to**: Kaleb Szaro, John McLeod, Jake Herren

### Part 4: $PATH Search
- **Responsibilities**: Created list of directories using the $PATH environment variable that is searched until the argument is found or not. Implementation research by Kaleb and Jake. Coded by Jake and committed to github by Kaleb.
- **Assigned to**: Kaleb Szaro, Jake Herren

### Part 5: External Command Execution
- **Responsibilities**: Took command from input and created a child process that would execute said command using fork() and execv(). Research implementation by John, Jake, and Kaleb. Coded by Kaleb and John and committed to github by Kaleb.
- **Assigned to**: Jake Herren, Kaleb Szaro, John McLeod

### Part 6: I/O Redirection
- **Responsibilities**: Provided implementation of file input/output redirection. Combining the two when necessary. Implementation research by Jake and John. Coded by Jake and John and committed to github by Kaleb.
- **Assigned to**: Jake Herren, John McLeod

### Part 7: Piping
- **Responsibilities**: Implemented simultaneous execution of multiple commands, with the input and output of these commands interconnected via Piping. Implementation research and coding done by Jake. Committed to github by Kaleb.
- **Assigned to**: Jake Herren

### Part 8: Background Processing
- **Responsibilities**: Created ability for shell to execute external commands without waiting for their completion. Keeping track of said commands was also created. Implementation research and coding done by Kaleb. Committed to github by Kaleb.
- **Assigned to**: Kaleb Szaro

### Part 9: Internal Command Execution
- **Responsibilities**: Created exit, cd PATH, and jobs internal commands for the shell. 
- **Assigned to**: Kaleb Szaro, John McLeod

### Part 10: External Timeout Executable
- **Responsibilities**: Implemented the external executable "mytimeout" which offers enhanced control over command execution duration. Implementation research and coding done by John. Committed to github by Kaleb.
- **Assigned to**: John McLeod

### Extra Credit
- **Responsibilities**: Create support for an unlimited number of pipes, support Piping and I/O redirection in a single command, and be able to execute your shell from within a running shell process.
- **Assigned to**: John McLeod, Jake Herren, Kaleb Szaro

## File Listing
```
project-1-group-25/
│
├── src/
│ └── lexer.c
| └── main.c
| └── mytimeout.c
| └── piping.c
│
├── include/
│ └── lexer.h
| └── piping.h
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
and the executable mytimeout in ~/.bin
### Execution
```bash
./bin/shell
```
This will run the program ...

## Bugs
- **Bug 1**: Background Processes will not show the PID and Queue after a command is executing in background.
- **Bug 2**: 
- **Bug 3**: 

## Extra Credit
- **Extra Credit 1**: Support unlimited number of pipes.
- **Extra Credit 1 Documentation**: Only supports 2 pipes at the moment.
- **Extra Credit 2**: Support piping and I/O redirection in a single command.
- **Extra Credit 2 Documentation**: Piping and I/O redirection works correctly.
- **Extra Credit 3**: Execute your shell from within a running shell process repeatedly.
- **Extra Credit 3 Documentation**: Shell-ception is currently not available.

## Considerations
-Github shows almost all commits were by Kaleb. This was by design, as Kaleb was able to get everything running on VScode with an easy way to upload to github. Jake and John wrote and ran code on linprog to make sure it would run correctly there, and then would pass code over to Kaleb to commit to github.
