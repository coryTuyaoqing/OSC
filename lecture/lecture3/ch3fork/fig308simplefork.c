#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
	printf("hello from %s!\n", "SimpleLinuxFork");
	pid_t pid = fork();

	if (pid < 0)	{ // ERROR
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	if (pid == 0){ // CHILD
		printf("I am the child %d\n", pid);
	
	
		execlp("/usr/bin/pstree", "pstree", NULL);

		printf("I'm back");
		return 0;
	}
	printf("I am the parent %d\n", pid);
	wait(NULL); // THIS PARENT WILL WAIT for CHILD
	printf("Child complete\n");
	return 0;
}
