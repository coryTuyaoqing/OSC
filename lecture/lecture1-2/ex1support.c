#include <stdio.h>

extern int externvar;

void externwrite() {
	printf("externvar is %d\n", externvar);
}