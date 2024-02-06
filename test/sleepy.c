#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    for(int i = 1; i<5; i++){
        printf("%d\n", i);
        sleep(2);
    }

    return 0;
}
