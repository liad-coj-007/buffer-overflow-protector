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

void authneticateUser(){
int canary1 = globalCanary;

    char password[4];
    int x = 3;

int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);


    x += 22*3 +15;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    printf("Enter password: ");
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    scanf("%s", password);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    if(strcmp(password, "A") == 0){
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

        printf("Authenticated\n");
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    }else{
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

        printf("Authentication failed\n");
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    }
}

int main(){
globalCanary = urandom();

int canary1 = globalCanary;

int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    authneticateUser();
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

    return 0;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}

