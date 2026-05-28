#include <stdio.h>
#include <stdlib.h>

/* best random source on UNIX-like systems. */
int urandom() {
#ifdef __unix__
int var;
FILE *fd = fopen("/dev/urandom", "r");
fread(&var, sizeof(int), 1, fd);
fclose(fd);
return var;
#else
return 4;
#endif
}

void verifyOverflow(int a, int b) {
    // Implementation for overflow verification
    if(a != b){
        printf("Alert! Buffer Overflow detected.\n");
        exit(1);
    }
}


int globalCanary = 0;
#include <stdio.h>
#include <string.h>

int loop(int n){
int canary1 = globalCanary;

    char buffer[4];
    int i = 0;

int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);


    for(i = 0; i < n; i++){
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

        scanf("%s", buffer);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

        if(strcmp(buffer,"A") == 0){
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

            printf("Authenticated\n");
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

        }
    }
    return 0;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}

int main(){
globalCanary = urandom();

int canary1 = globalCanary;

int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    loop(4);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    return 0;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}

