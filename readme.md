# myShell

myShell is a simple Unix-like shell written in C that supports several built-in functions, background jobs, listing jobs, pipes, I/O redirection, and environment variables for both built-in functions and executable programs.

## Features

- Built-in functions:
  - `cd`: Change the current directory.
  - `exit` or `quit`: Exit the shell.
  - `export`: Set and manage environment variables.
  - `echo`: Display messages with support for variable substitution.
  - `pwd`: Print the current working directory.
  - `kill`: Send signals to processes.
  
- Background jobs: You can run commands in the background by appending `&` to the command.

- Job management: List and manage background jobs.

- Pipes: Execute commands with pipes (e.g., `command1 | command2`).

- I/O redirection: Redirect input and output for both built-in and external commands.

- Environment variables: Set and use environment variables for your shell sessions.

## Building and Running

To build the myShell project, you can use the provided [Makefile](Makefile):

```
make quash
```
To run the shell, execute:

```
./quash or make quash_test
```

To check for memory leaks using Valgrind, execute:

```
make quash_valgrind
```

To clean the executable file:
```
make clean
```

## Usage

Simply start myShell by running the quash binary. You can enter commands with various features like background execution, pipes, I/O redirection, and environment variables.

Example usage:

```
$ ./quash
[QUASH]$ export MY_VARIABLE=my_value
[QUASH]$ echo $MY_VARIABLE
my_value
[QUASH]$ ls | grep .txt
file1.txt
file2.txt
[QUASH]$ program1 &
Background job started: [1] 1234 program1 &
[QUASH]$ jobs
[1] 1234 program1 &
```

## Contributors

Bhavik Goplani & Suhaan Syed

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
