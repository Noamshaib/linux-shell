#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        printf("Data\n");
        fflush(stdout);  // Ensure data is sent to stdout immediately
        sleep(1);        // Pause for a second
    }
    return 0;
}
