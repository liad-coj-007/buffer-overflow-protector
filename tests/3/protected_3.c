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
#include <stdlib.h>

void jalil_keep(int i, ...) {
int canary1 = globalCanary;


int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}

void func_jalil_0() {
int canary1 = globalCanary;

char _str[10];
int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);


jalil_keep(0, _str);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

scanf("%s", _str);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

printf("%s", _str);
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}

void main(){
globalCanary = urandom();

int canary1 = globalCanary;

int canary2 = globalCanary;
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

func_jalil_0();
verifyOverflow(canary1, globalCanary);
verifyOverflow(canary2, globalCanary);

}
