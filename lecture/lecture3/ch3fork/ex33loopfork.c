#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main()
{
    int i = 0;
    for(i=0;i<4;i++){


	fork();

	printf("%d - i=%d\n",getpid(),i);

    }


	return 0;
}
