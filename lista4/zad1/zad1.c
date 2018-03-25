#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    int current_id = getuid();
    printf("uid=%i\n", current_id);
    if(setuid(0)) {
        perror("setuid(0)");
        exit(1);
    } else {
        printf("after setuid(0)\n");
    }
    current_id = getuid();
    printf("uid=%i\n", current_id);

    printf("DOSTEP PRZYZNANY XD\n");

    char * args[] = {"/bin/bash", NULL};

    // system("sudo /bin/bash");
    execvp(args[0], args);

    perror("execvp: ");

    return 0;
}