#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define READ_END 0
#define WRITE_END 1
#define SIZE 50

int main(int argc, char* argv[]){
	puts("---multi_process---");

	char wmsg[SIZE] = "Greeting from the PARENT!";
	char rmsg[SIZE];
	
	pid_t pid;
	int fd[2];

	//creating pipe before fork
	if(pipe(fd) == -1){
		puts("pipe failed");
		return 1;
	}

	//fork
	pid = fork();

	if(pid < 0){ // fork failded.
		fprintf(stderr, "Fork failed.");
		return 1;
	}
	if(pid == 0){ // child process
		printf("I am a child process %d.\n", pid);

		close(fd[WRITE_END]);
		read(fd[READ_END], rmsg, SIZE);

		// reverse the letter cases of rmsg
		for(int i=0; rmsg[i]!=0; i++){
			if(islower(rmsg[i])){
				rmsg[i] -= 0x20;
			}
			else if(isupper(rmsg[i])){
				rmsg[i] += 0x20;
			}
		}

		printf("Child read %s\n", rmsg);
		close(fd[READ_END]);
	}
	else if(pid > 0){
		printf("I am a parent process %d.\n", pid);

		close(fd[READ_END]);
		write(fd[WRITE_END], wmsg, strlen(wmsg)+1);
		close(fd[WRITE_END]);
	}

	return 0;
}
