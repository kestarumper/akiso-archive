#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIGITS_BUFSIZE 512

int mystrlen(const char * str) {
    int result = 0;
    while(*str != '\0') {
        str++;
        result++;
    }
    return result;
}

char * paddingLeft(char * str, char token, int len) {
    char * result = NULL;

    if(mystrlen(str) < len) {
        result = malloc(sizeof(char) * len);
    } else {
        return str;
    }

    int diff = len - mystrlen(str);

    for(int i = 0; i < diff; i++) {
        result[i] = token;
    }
    
    for(int i = 0; i < mystrlen(str); i++) {
        result[i + diff] = str[i];
    }

    return result;
}

int stringToInt(char * a, int base) {
    int result = 0;
    int sign = 1;

    int length = mystrlen(a);

    if(a[length-1] == '\n') {
        a[length-1] = '\0';
        length--;
    }

    if(a[0] == '-') {
        a++;
        sign = -1;
        length--;
    }

    char digit;
    int power;

    for(int i = 0; i < length; i++) {
        digit = a[length-i-1];
        if(digit >= 'A') {
            digit -= 'A';
            digit += 10;
        } else {
            digit -= '0';
        }

        result += pow(base, i) * ((int)digit);
    }

    return result * sign;
}

char * intToString(int a, int base) {
    char * result = malloc(DIGITS_BUFSIZE * sizeof(char));

    int * digits = malloc(DIGITS_BUFSIZE * sizeof(int));

    if(result == NULL || digits == NULL) {
        perror("inttostring:malloc");
        exit(1);
    }

    int sign = 1;
    if(a < 0) {
        sign = -1;
    }

    a *= sign;

    int i = 0;
    while(a > 0) {
        digits[i] = a % base;
        a /= base;
        i++;
    }

    if( sign == -1 ) {
        digits[i] = -3;
        i++;
    }

    char tmp;
    int digit;
    for(int j = i; j > 0; j--) {
        digit = digits[j-1];

        if(digit > 9) {
            tmp = 'A';
            digit -= 10;
        } else {
            tmp = '0';
        }

        result[i-j] = (char)(tmp + digit);
    }

    result[i] = '\0';

    free(digits);
    return result;
}

char * opcodeToBinary(char * opcode) {
    if(strcmp(opcode, "JNS") == 0) {
        return "0000";
    } 
    else if(strcmp(opcode, "LOAD") == 0) {
        return "0001";
    }
    else if(strcmp(opcode, "STORE") == 0) {
        return "0010";
    }
    else if(strcmp(opcode, "ADD") == 0) {
        return "0011";
    }
    else if(strcmp(opcode, "SUBT") == 0) {
        return "0100";
    }
    else if(strcmp(opcode, "INPUT") == 0) {
        return "0101";
    }
    else if(strcmp(opcode, "OUTPUT") == 0) {
        return "0110";
    }
    else if(strcmp(opcode, "HALT") == 0) {
        return "0111";
    }
    else if(strcmp(opcode, "SKIPCOND") == 0) {
        return "1000";
    }
    else if(strcmp(opcode, "JUMP") == 0) {
        return "1001";
    }
    else if(strcmp(opcode, "CLEAR") == 0) {
        return "1010";
    }
    else if(strcmp(opcode, "ADDI") == 0) {
        return "1011";
    }
    else if(strcmp(opcode, "JUMPI") == 0) {
        return "1100";
    }
    else {
        return opcode;
    }
}

int main(int argc, char ** argv) {
    FILE * file;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * command = NULL;
    char * cmdaddr = NULL;

    if(argc != 2) {
        printf("Bad arguments");
        exit(1);
    }

    file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Error opening file");
        exit(1);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        command = strtok(line, " \n");
        cmdaddr = strtok(NULL, " \n");
        if(cmdaddr == NULL) {
            cmdaddr = "";
        }

        if(strcmp(opcodeToBinary(command), command) == 0) {
            printf("%s\n", paddingLeft(intToString(stringToInt(command, 16), 2), '0', 16));
        } else {
            printf("%s%s\n", paddingLeft(opcodeToBinary(command), '0', 4), paddingLeft(intToString(stringToInt(cmdaddr, 16), 2), '0', 12));
        }        
        
    }
    printf("\n");

    free(line);

    return 0;
}
