#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    // attach handler to all signals
    // range 0 - 64 including
    for(int i = 1; i < 65; i++) {
        if(i == 9 || i == 19) continue; // 9 - kills program, 19 - stops (Ctrl+Z)
        if(i > 31 && i < 34) continue;
        printf("Sending signal to PID:[%s] No.%i\n", argv[1], i);
        if(kill(atoi(argv[1]), i) < 0) {
            perror("kill: ");
        }
        // sleep(1);
    }
    return 0;
}