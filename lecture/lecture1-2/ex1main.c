#include <stdio.h>

void f1();
static int globalvar = 10;
int externvar;
extern void externwrite();

int main(int argc, char** argv)
{
    /* comment */
    printf("hello from %s!\n", "TestAppInC");

    while (globalvar--) f1();

    externvar = 100;
    externwrite();

    return 0;
}

void f1() {

    static int localvar = 5;
    localvar++;
    printf("localvar is %d and globalvar is %d\n", localvar, globalvar);
}
