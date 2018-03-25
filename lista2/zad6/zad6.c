#include "stdio.h"

int main() {
    for(int i = 0; i < 256; i++) {
        printf("\e[38;5;%imHello World\e[0m\t\e[48;5;%im Hello World \e[0m\n", i, i);
    }
    return 0;
}