#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include <stdio.h>

void print_arglist(char **arglist) {
    if (arglist == NULL) {
        printf("arglist is NULL\n");
        return;
    }

    for (int i = 0; arglist[i] != NULL; i++) {
        printf("arglist[%d]: %s\n", i, arglist[i]);
    }
}


int prepare(void) {
    // Set up SIGINT to be ignored in the parent process
    if(signal(SIGINT, SIG_IGN) == SIG_ERR){
        perror("SIGINT handler failed");
        return -1;
    }
    // Set up the SIGCHLD handler to prevent zombies
    if(signal(SIGCHLD, SIG_IGN) == SIG_ERR){
        perror("SIGINT handler failed");
        return -1;
    }

    return 0;  // Success
}



int process_arglist(int count, char **arglist) {

    print_arglist(arglist);

    //check if and where a shell symbol &\<\>\| locate in arglist
    char symbol[2] = "";
    int symbol_index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(arglist[i], "&") == 0) {
            strcpy(symbol, "&");
            symbol_index = i;
            break;
        }
        else if (strcmp(arglist[i], "<") == 0) {
            strcpy(symbol, "<");
            symbol_index = i;
            break;
        }
        else if (strcmp(arglist[i], ">") == 0) {
            strcpy(symbol, ">");
            symbol_index = i;
            break;
        }
        else if (strcmp(arglist[i], "|") == 0) {
            strcpy(symbol, "|");
            symbol_index = i;
            break;
        }
    }

    // Handle "&" case - background process (parent not need to wait, the same SIGINT handler as parent - ignore)
    if (strcmp(symbol, "&") == 0) {

        arglist[count - 1] = NULL; // Remove "&" from arglist

        pid_t pid = fork(); 
        
        if (pid == -1) {
            perror("fork");
            return 0;  
        } 
        else if (pid == 0) { 
            // child process
            if (execvp(arglist[0], arglist) == -1) {
                perror("execvp");
                exit(1);  // Exit child process if execvp fails
            }
        }
        // No need to wait for the child process as it's a background process
    }

    // Handle "", "<", ">" cases - foreground processes (parent need to wait,a different SIGINT handler from parent - the original default), no pipe
    else if (strcmp(symbol, "|") != 0) {

        pid_t pid = fork(); 

        if (pid == -1) {
            perror("fork");
            return 0;  // or handle the error as appropriate
        }
         
        else if (pid == 0) { 
            // child process
            // set up signal handler to NOT ignore SIGINT in child - set to original default
            if(signal(SIGINT, SIG_DFL) == SIG_ERR){
                perror("SIGINT handler failed");
                exit(1);
            }

            // Handle "<" case (input redirection)
            if (strcmp(symbol, "<") == 0){
                int fd_in = open(arglist[symbol_index + 1], O_RDONLY);
                if (fd_in < 0) {
                    perror("Input redirection error");
                    exit(1);
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
                arglist[symbol_index] = NULL; // Remove "<" and the filename from arglist
            }
            // Handle ">" case (output redirection)
            else if(strcmp(symbol, ">") == 0){

                int fd_out = open(arglist[symbol_index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out < 0) {
                    perror("Output redirection error");
                    exit(1);
                }
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
                arglist[symbol_index] = NULL; // Remove ">" and the filename from arglist
            }
            // Execute the command for cases: "", "<", ">" 
            if (execvp(arglist[0], arglist) == -1) {
                perror("execvp");
                exit(1);  // Exit child process if execvp fails
            }
            
        }
        else{
            //parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
    // Handle "|" case (single piping) - foreground processes (parent need to wait,a different SIGINT handler from parent - the original default), piping!
    else if (strcmp(symbol, "|") == 0){
        //create a pipe
        int fds[2];
        pipe(fds);

        pid_t pid1 = fork(); 

        if (pid1 == -1) {
            perror("fork");
            return 0;  // or handle the error as appropriate
        }
         
        else if (pid1 == 0) { 
            // child1 process
            // set up signal handler to NOT ignore SIGINT in child - set to original default
            if(signal(SIGINT, SIG_DFL) == SIG_ERR){
                perror("SIGINT handler failed");
                exit(1);
            }

            close(fds[0]);
            dup2(fds[1], STDOUT_FILENO);
            close(fds[1]);
            arglist[symbol_index] = NULL; // Remove "|" from arglist
            if (execvp(arglist[0], arglist) == -1) {
                perror("execvp");
                exit(1);  // Exit child process if execvp fails
            }
            
        }
        else{
            //parent process
            pid_t pid2 = fork();

            if (pid2 == -1) {
                perror("fork");
                return 0;  // or handle the error as appropriate
            }
         
            else if (pid2 == 0) { 
                // child2 process
                // set up signal handler to NOT ignore SIGINT in child - set to original default
                if(signal(SIGINT, SIG_DFL) == SIG_ERR){
                    perror("SIGINT handler failed");
                    exit(1);
                }

                close(fds[1]);
                dup2(fds[0], STDIN_FILENO);
                close(fds[0]);

                if (execvp(arglist[symbol_index+1], &arglist[symbol_index+1]) == -1) {
                    perror("execvp");
                    exit(1);  // Exit child process if execvp fails
                }
            }
            else{
                //parent process
                close(fds[0]);
                close(fds[1]);
                int status;
                waitpid(pid1, &status, 0);
                waitpid(pid2, &status, 0);
            }       
        }
    }
    return 1;
}

              

int finalize(void){
    return 0;
}

