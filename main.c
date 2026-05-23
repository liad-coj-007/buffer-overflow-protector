#include <stdio.h>
#include "utils.h"
int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }

    char* programName = argv[1];
    char* program = readFile(programName);
    printf("Program:\n%s\n", program);
    free(program);
    




    return 0;
}