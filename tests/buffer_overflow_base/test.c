#include <stdio.h>
#include <string.h>

void authneticateUser(){
    char password[4];
    int x = 3;
    x += 22*3 +15;
    printf("Enter password: ");
    scanf("%s", password);
    if(strcmp(password, "A") == 0){
        printf("Authenticated\n");
    }else{
        printf("Authentication failed\n");
    }
}

int main(){
    authneticateUser();
    return 0;
}

