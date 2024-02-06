#include <stdio.h>

int main() {
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("Received: %s", buffer);
    }
    printf("prog2: stdin closed\n");
    return 0;
}
