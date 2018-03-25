#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void any_signal_handler(int sig_num)
{
    static int counter = 0;
    printf("Signal #%i: [%i]\n", counter, sig_num);
    counter++;
}

int main() {
    // attach handler to all signals
    // range 0 - 64 including
    for(int i = 1; i < 65; i++) {
        printf("Creating handler for signal No.%i\n", i);
        signal(i, any_signal_handler);
    }

    printf("Process listener PID: %i\n", getpid());

    while(1) {
        sleep(1);
    }

    return 0;
}