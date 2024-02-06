# linux-shell
This repository hosts an Operating Systems course project: a custom shell in C. It illustrates process management, signal handling, and IPC with pipes, showcasing the creation and control of foreground and background processes, and data flow management between programs in a shell environment.
Certainly! Here's an expanded version of the README with additional relevant information:

---

# Custom Shell for Operating Systems Course

## Overview
This project is a custom shell implemented in C, developed as part of an Operating Systems course. It serves as a practical application of core OS concepts such as process management, signal handling, and inter-process communication.

## Features
- **Command Execution:** Executes most external commands available in the system PATH.
- **Background Execution (`&`):** Allows running processes in the background, enabling the shell to accept new commands immediately.
- **Piping (`|`):** Supports single-level piping between commands, enabling the output of one command to be used as the input to another.
- **Redirection (`<`, `>`):** Implements input and output redirections, allowing the shell to read input from a file or write output to a file.
- **Signal Handling:** Custom handling of SIGINT and SIGCHLD signals for process control and zombie process prevention.
- **Zombie Process Management:** Automatically reaps zombie processes to maintain system health.

## Assumptions and Limitations
- Single-instance of special symbols: Only one of each (`|`, `&`, `<`, `>`) is handled per command.
- Proper syntax: Assumes correct spacing and syntax for commands and redirections.
- External commands only: The shell does not implement shell built-in commands like `cd`.
- Basic error handling: Provides error feedback for common issues like command not found or file access errors.

## Compilation and Usage
- **Compilation:** Use `gcc` to compile the shell program. Example: `gcc -o myshell myshell.c`.
- **Execution:** Run the compiled executable and enter commands as in a standard shell.

## Contributing
Contributions to enhance the shell, add new features, or improve error handling are welcome. Please follow the standard GitHub pull request process.

## About shell.c

The shell.c file serves as the foundational driver for the custom shell. It is responsible for the overall lifecycle of the shell application, including:

Reading User Input: It captures command-line input from the user.
Command Parsing: It breaks down the input into recognizable commands and parameters.
Invocation of process_arglist(): It delegates the execution of parsed commands to the process_arglist() function defined in myshell.c.
Initial Setup and Cleanup: It calls prepare() before entering the main command loop and finalize() upon exiting, ensuring any necessary initialization and cleanup are performed.

shell.c acts as the interface between the user and the underlying functionalities implemented in myshell.c, orchestrating the flow of the shell program.
---

This README provides a comprehensive overview of your project, its features, and how it can be used and expanded. Feel free to adjust it to better fit the specifics of your project or any additional details you think are important.
