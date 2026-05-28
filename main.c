#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief adds a canary value 
 * for every function in the program
 * @param program the program 
 * @param programPath the path to the program
 */
void addCanary(const char* program,const char* programPath);

/**
 * @brief adds a call to the verifyOverflow function before 
 * every return statement and 
 * before every closing curly brace in the program
 */
void addVerifyOverflow( char* program,const char* programPath);

/**
 * @brief finds the first return statement or closing curly brace in the program
 * @param program the program to search in
 * @return a pointer to the first return statement or closing curly brace in the program, or
 *  NULL if none of them are found
 * 
 */
char* findFinishFunction(const char* program,FunctionEndPositions positions);

/**
 * @brief injects the security infrastructure to 
 * the program by adding the code of the overflow_utils.c file to the top of the program
 * @param program the program to inject the security infrastructure to
 * @param programPath the path to the program
 */
void injectSecurityInfrastructure(const char* program,const char* programPath);

/**
 * @brief finds the last init of local varible in the function 
 * @return the position of the last init of local varible in the function
 */
char* findLastInit( char* program);

char* findPlaceVerfication(char* program,FunctionEndPositions positions);




int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }

    char* programPath = argv[1];
    char* program = readFile(programPath);
    char* protectedProgramPath = getProtectedPath(programPath);
    addCanary(program, protectedProgramPath);
    free(program);
    program = readFile(protectedProgramPath);
    addVerifyOverflow(program, protectedProgramPath);
    free(program);
    program = readFile(protectedProgramPath);
    injectSecurityInfrastructure(program, protectedProgramPath);
    free(program);
    free(protectedProgramPath);

    return 0;
}

void addCanary(const char* program, const char* programPath) {
    FILE* file = fopen(programPath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }


    const char* canaryGlobal = "int globalCanary = 0;\n";
    const char* canary1 = "\nint canary1 = globalCanary;\n";
    const char* canary2 = "\nint canary2 = globalCanary;\n";

    const char* init = "\nglobalCanary = urandom();\n";

    fwrite(canaryGlobal, sizeof(char), strlen(canaryGlobal), file);
    while (1) {
        FunctionInfo funcInfo = findFunction(program);
        if (funcInfo.name == NULL) {
            // Process the function
            fwrite(program, sizeof(char), strlen(program), file);
            free(funcInfo.name);
            break;
        }
        fwrite(program, sizeof(char), funcInfo.program - program +1, file);
        if(strcmp(funcInfo.name, "main") == 0){
            fwrite(init, sizeof(char), strlen(init), file);
        }
        fwrite(canary1, sizeof(char), strlen(canary1), file);
        funcInfo.program += 1;
        char* lastInitPos = findLastInit(funcInfo.program );
        if(lastInitPos != NULL){
            fwrite(funcInfo.program, sizeof(char), lastInitPos - funcInfo.program + 1 , file);
        }
        fwrite(canary2, sizeof(char), strlen(canary2), file);
        program = (lastInitPos ? lastInitPos  : funcInfo.program) + 1;
        free(funcInfo.name);
    }
    fclose(file);
}


char* findPlaceVerfication(char* program,FunctionEndPositions positions){
    char* keywords[] = {";","}","{","for","while"};
    char* res = NULL;
    const char* canary = "\nint canary2 = globalCanary;\n";

 
    char* pos = findFirstPattern(program,keywords,5);
    if(pos == NULL){
        return NULL;
    }

    if(*pos != '}' && *pos != 'f' && *pos !='w'){
        return pos;
    }

    if(*pos == 'f' || *pos =='w' ){
        return findPlaceVerfication(strstr(pos,")"),positions);
    }


    for(int i = 0; i < positions.count; i++){
        if(positions.positions[i] == pos){
            char* tmp = strstr(pos,canary);
            if(tmp == NULL){
                return NULL;
            }
            return strstr(tmp,";");
        }
    }

    return findPlaceVerfication(pos + 1 ,positions);

    

}


void addVerifyOverflow( char* program,const char* programPath){

    const char* verifyOverflow = "\nverifyOverflow(canary1, globalCanary);\nverifyOverflow(canary2, globalCanary);\n";
    FILE* file = fopen(programPath, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    // const char* returnExp = "return";
    FunctionEndPositions positions = findFunctionEndBraces(program);
    // while(1){
    //     char* tmp = findFinishFunction(program,positions);
    //     if(tmp == NULL){
    //         fwrite(program, sizeof(char), strlen(program), file);
    //         break;
    //     }
    //     fwrite(program, sizeof(char), tmp - program, file);
    //     fwrite(verifyOverflow, sizeof(char), strlen(verifyOverflow), file);
    //     if(*tmp == '}'){
    //         fwrite("}\n", sizeof(char), 2, file);
    //         program = tmp + 1;
    //     }else{
    //         fwrite(returnExp, sizeof(char), strlen(returnExp), file);
    //         program = tmp + strlen(returnExp);
    //     }
    // }
    // free(positions.positions);
    // fclose(file);
    const char* canary = "\nint canary2 = globalCanary;\n";
    char* pos = strstr(program,canary);
    fwrite(program,sizeof(char),pos - program + 1,file);
    program = pos;
    while(1){
        pos = findPlaceVerfication(program,positions);
        if(pos == NULL){
            fwrite(program,sizeof(char),strlen(program),file);
            break;
        }
        fwrite(program,sizeof(char),pos - program + 1,file);
        fwrite(verifyOverflow,sizeof(char),strlen(verifyOverflow),file);
        program = pos + 1;
    }
    free(positions.positions);
    fclose(file);

}

char* findFinishFunction(const char* program,FunctionEndPositions positions) {
    char* finishKeywords[] = {"return", "}"};

    while(1){
        char* lastPos = findFirstPattern(program,finishKeywords,2);
        if(lastPos  == NULL){
            return NULL; 
        }

        if(*lastPos == 'r'){
            return lastPos;
        }

        for(int i = 0 ; i < positions.count;i++){
            if(positions.positions[i] == lastPos){
                return lastPos;
            }
        }
        program = lastPos + 1;
    }
    return NULL;
}

void injectSecurityInfrastructure(const char* program,const char* programPath){
    FILE* file = fopen(programPath, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    char* verifyFunctions = readFile("overflow_utils.c");
    fwrite(verifyFunctions, sizeof(char), strlen(verifyFunctions), file);
    free(verifyFunctions);
    fwrite(program, sizeof(char), strlen(program), file);
    fclose(file);
}

char* findLastInit( char* program){
    char* lastPos = NULL;
    char* keywords[] = {"int", "char", "float", 
        "double", "long", "short", "bool"};
    while(1){
        char* line = copyUntilNewLine(program);
        char* tmp = deleteSpaces(line);
        free(line);
        line = tmp;
        
        if(strlen(line) == 0){
            free(line);
            program = strstr(program, "\n") + 1;
            continue;
        }

        

        if(line == NULL || !beginWith(line, keywords, 7)){
            free(line);
            break;
        }
        lastPos = strstr(program, ";");
        program = strstr(program, "\n") + 1;
        free(line);
    }

    return lastPos;

}

