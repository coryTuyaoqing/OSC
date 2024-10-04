#include <stdio.h>
#include "hellofunc.h"

int main(int argc, char** argv){
	printf("argc: %d\n", argc);
	for(int i=0; i<argc; i++){
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	// call a function in another file
	if (argc == 2)
		myPrintHelloMake(argv[1]);
	else
		myPrintHelloMake("nobody");
	return 0;
}
