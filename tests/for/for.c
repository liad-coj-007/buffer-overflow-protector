#include <stdio.h>
#include <string.h>

int loop(int n){
    char buffer[4];
    int i = 0;
    for(i = 0; i < n; i++){
        scanf("%s", buffer);
        if(strcmp(buffer,"A") == 0){
            printf("Authenticated\n");
        }
    }
    return 0;
}

int main(){
    loop(4);
    return 0;
}

