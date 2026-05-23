#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(sizeof(char) * (fileSize + 1));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

size_t findPattern(const char* str, const char* pattern){
    
}