#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        printf("Parent process, child PID: %d\n", pid);
        sleep(20);  // Sleep for 60 seconds
    } else if (pid == 0) {
        // Child process
        exit(0);  // Child exits immediately
    } else {
        // Fork failed
        perror("fork");
        exit(1);
    }

    return 0;
}
