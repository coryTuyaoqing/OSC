#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main()
{
	printf("%d*\n",getpid());

	fork();

	printf("%d**\n",getpid());

	fork();

	printf("%d***\n",getpid());
    
	fork();
	
	printf("%d****\n",getpid());


	return 0;
}

