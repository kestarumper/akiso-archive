#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    // attach handler to all signals
    // range 0 - 64 including
    int num = atoi(argv[2]);
    for(int i = 0; i < num; i++) {
        printf("Sending signal to PID:[%s] No.%i\n", argv[1], i);
        if(kill(atoi(argv[1]), 5) < 0) {
            perror("kill: ");
        }
    }
    printf("Sent %i signals to PIS:%s", num, argv[1]);

    return 0;
}