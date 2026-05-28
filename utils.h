#pragma once
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief reads a file and returns its contents as a string
 * @param fileName the name of the file to read
 * @return the contents of the file as a string
 */
char* readFile(const char* fileName);

char* getProtectedPath(const char* path);


typedef struct{
    char* program;
    char* name;
}FunctionInfo;


/**
 * @brief finds the first function in the program and returns its name 
 * and the start of the function as a string
 */
FunctionInfo findFunction(  char* program);


/**
 * @brief finds the first occurrence of any of the patterns in the string
 * @param str the string to search ing
 * @param patterns the patterns to search for
 * @param patternsSize the number of patterns
 * @return a pointer to the first occurrence of any of the patterns in the string, or 
 * NULL if none of the patterns are found
 */
char* findFirstPattern(const char* str,char** patterns,const int patternsSize);


/**
 * @brief copies the string until the first new line character
 * @param str the string to copy from
 * @return a new string that is a copy of the input string until the first new line character
 */
char* copyUntilNewLine(const char* str);

char* deleteSpaces(const char* str);

bool beginWith(const char* str, char** patterns, const int patternsSize);


typedef struct{
    char** positions;
    int count;
}FunctionEndPositions;

/**
 * @brief  find the braces that end function
 */
FunctionEndPositions findFunctionEndBraces(char* str);

