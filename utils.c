#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <regex.h>


char* findFirstPattern(const char* str,char** patterns,const int patternsSize){
    char* firstPos = NULL;
    for(int i = 0; i < patternsSize; i++){
        char* pos = strstr(str, patterns[i]);
        if(pos != NULL && (firstPos == NULL || pos < firstPos)){
            firstPos = pos;
        }
    }
    return firstPos;
}



char* readFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
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


char* getProtectedPath(const char* path) {
    const char* last_slash = strrchr(path, '/');

    const char* filename = last_slash ? last_slash + 1 : path;

    size_t dir_len = last_slash ? (size_t)(last_slash - path + 1) : 0;

    const char* prefix = "protected_";

    size_t new_name_len = strlen(prefix) + strlen(filename);
    size_t total_len = dir_len + new_name_len;

    char* result = malloc(total_len + 1);
    if (!result) return NULL;

    // copy directory part (including '/')
    if (dir_len > 0) {
        memcpy(result, path, dir_len);
    }

    // build new filename
    memcpy(result + dir_len, prefix, strlen(prefix));
    memcpy(result + dir_len + strlen(prefix), filename, strlen(filename));

    result[total_len] = '\0';

    return result;
}



bool isControlKeyword(const char* name) {
    return (strcmp(name, "if") == 0 || 
            strcmp(name, "while") == 0 || 
            strcmp(name, "switch") == 0 || 
            strcmp(name, "for") == 0);
}

FunctionInfo findFunction(char* program) {
    FunctionInfo funcinfo = {NULL, NULL};
    if (program == NULL || *program == '\0') return funcinfo;

    regex_t regex;
    // pmatch[0] שומר את כל הביטוי שמצאנו, pmatch[1] שומר רק את קבוצת הסוגריים הראשונה (שם הפונקציה)
    regmatch_t pmatch[2]; 

    // ביטוי רגולרי חסין ומדויק יותר:
    // תומך ברווחים וירידות שורה, עוצר בסוגר הימני של הפרמטרים, ומסתיים בסוגר מסולסל פתוח
    const char* pattern = "[a-zA-Z_][a-zA-Z0-9_*]*[ \t\n\r]+([a-zA-Z_][a-zA-Z0-9_]*)[ \t\n\r]*\\([^)]*\\)[ \t\n\r]*\\{";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return funcinfo; 
    }

    char* current_pos = program;

    // לולאה שמחפשת התאמות ומדלגת על מילים שמורות כמו if או while
    while (regexec(&regex, current_pos, 2, pmatch, 0) == 0) {
        int name_start = pmatch[1].rm_so;
        int name_end = pmatch[1].rm_eo;
        size_t name_len = name_end - name_start;

        // הקצאת זיכרון זמנית לבדיקת שם הפונקציה
        char* temp_name = (char*)malloc(name_len + 1);
        if (temp_name == NULL) break;
        
        strncpy(temp_name, current_pos + name_start, name_len);
        temp_name[name_len] = '\0';

        // אם זו לא מילת מפתח (כמו if/while) - מצאנו פונקציה אמיתית!
        if (!isControlKeyword(temp_name)) {
            funcinfo.name = temp_name;
            
            // pmatch[0].rm_eo מייצג את התו שאחרי ה- '{'
            // פחות 1 יביא אותנו בדיוק אל התו '{' עצמו בתוך המחרוזת
            int total_end = pmatch[0].rm_eo;
            funcinfo.program = current_pos + (total_end - 1); 
            
            break; // מצאנו, אפשר לעצור
        }

        // אם זו הייתה מילת מפתח, משחררים את הזיכרון הזמני וממשיכים לחפש הלאה בקוד
        free(temp_name);
        current_pos += pmatch[0].rm_eo;
    }

    regfree(&regex);
    return funcinfo;
}

char* copyUntilNewLine(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strcspn(str, "\n");

    char* new_str = (char*)malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        return NULL; 
    }

    strncpy(new_str, str, len);

    new_str[len] = '\0';

    return new_str;
}



bool beginWith(const char* str, char** patterns, const int patternsSize) {
    if (str == NULL || patterns == NULL || patternsSize <= 0) {
        return false;
    }

    for (int i = 0; i < patternsSize; i++) {
        if (patterns[i] == NULL) {
            continue;
        }

        size_t pattern_len = strlen(patterns[i]);

        if (strncmp(str, patterns[i], pattern_len) == 0) {
            return true; 
        }
    }

    return false;
}


char* deleteSpaces(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    int non_space_count = 0;
    int i = 0;

    // 1. שלב ראשון: ספירת התווים שאינם רווחים כדי לדעת כמה זיכרון להקצות
    while (str[i] != '\0') {
        if (!isspace((unsigned char)str[i])) {
            non_space_count++;
        }
        i++;
    }

    char* clean_str = (char*)malloc((non_space_count + 1) * sizeof(char));
    if (clean_str == NULL) {
        return NULL; 
    }

    int src_index = 0;
    int dest_index = 0;
    while (str[src_index] != '\0') {
        if (!isspace((unsigned char)str[src_index])) {
            clean_str[dest_index] = str[src_index];
            dest_index++;
        }
        src_index++;
    }

    clean_str[dest_index] = '\0';

    return clean_str;
}



int countChar(const char* str, char target) {
    int count = 0;

    while ((str = strchr(str, target)) != NULL) {
        count++;
        str++; // ממשיכים אחרי המופע שנמצא
    }

    return count;
}


FunctionEndPositions findFunctionEndBraces(char* str){
    FunctionEndPositions positions;
    positions.count = countChar(str,'}');
    positions.positions = (char**)malloc(positions.count*sizeof(char*));
    for(int i = 0; i < positions.count; i++){
        positions.positions[i] = NULL;
    }

    char* keywords[] = {"{","}"};
    int n = 0;
    int idx = 0;
    while(1){
        char* pos = findFirstPattern(str,keywords,2);
        if(pos == NULL){
            break;
        }

        if(*pos == '{'){
            n++;
        }else{
            n--;
        }

        if(n == 0){
            positions.positions[idx] = pos;
            idx++;
        }
        str = pos +1;
    }
    return positions;
}




