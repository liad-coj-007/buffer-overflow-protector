#pragma once
#include <stdlib.h>

/**
 * @brief reads a file and returns its contents as a string
 * @param fileName the name of the file to read
 * @return the contents of the file as a string
 */
char* readFile(const char* fileName);

/**
 * @brief finds the first occurrence of a pattern in a string
 * @param str the string to search
 * @param pattern the pattern to find
 * @return the index of the first occurrence of the pattern in the string, or -1 if not found
 */
size_t findPattern(const char* str, const char* pattern);