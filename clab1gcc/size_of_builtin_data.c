#include <stdio.h>

int main(int argc, char* argv[]){
	char ch = -1;

	printf("size of char: %ld\n", sizeof(char));
	printf("size of unsigned char: %ld\n", sizeof(unsigned char));
	printf("size of int: %ld\n", sizeof(int));
	printf("size of double: %ld\n", sizeof(double));
	printf("size of short: %ld\n", sizeof(short));
	printf("size of long: %ld\n", sizeof(long));
	printf("size of void: %ld\n", sizeof(void));
	printf("size of int*: %ld\n", sizeof(int*));

	if(ch < 0)
		printf("char is signed.");
	else
		printf("char is unsigned."); 
	printf("ch = %d\n", (int)ch);
}
