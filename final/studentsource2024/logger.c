#include "logger.h"

pid_t pid;
int fd[2];
int log_num = 0;
FILE* log_file;

int write_to_log_process(char *msg){
	if(pid > 0){ //parent process
		char msg_delimited[SIZE];
		snprintf(msg_delimited, SIZE, "%s^", msg); // add delimiter '^' to each msg

		// TODO: make sure the pipe is thread safe!
		write(fd[WRITE_END], msg_delimited, strlen(msg_delimited));
	}

	return 0;
}

int create_log_process(){
	//create pip first
	if(pipe(fd) == -1){
		puts("Pipe failed.\n");
		return 1;
	}
	//fork child process
	pid = fork();
	if(pid < 0){
		fprintf(stderr, "Fork failed.");
		return -1;
	}
	else if(pid > 0){ //parent process
		close(fd[READ_END]);
		return 1;
	}
	else if(pid == 0){ //child process
		close(fd[WRITE_END]);
		log_file = fopen(LOG_FILE_NAME, "w"); // a new and empty gateway.log file should be created
		char log_msg[SIZE];
		char *start, *end;
		while(true){
			size_t byte_read = read(fd[READ_END], log_msg, SIZE);
			if(byte_read > 0){
				start = log_msg;
				
				while((end = strchr(start, '^')) != NULL){
					*end = '\0';
					process_log_msg(start);
					start = end + 1;
				}
			}
			else if(byte_read == 0) break;
		}
	}
	return 0;
}

void process_log_msg(char* log_msg){
	// TODO: check if it should be end or not
	time_t t = time(NULL);
	char *time_string = ctime(&t);
	time_string[strlen(time_string)-1] = '\0';
	fprintf(log_file, "%d - %s - %s\n", log_num, time_string, log_msg);
	log_num++;
	fflush(log_file);
}

int end_log_process(){
	if(pid > 0){ //parent process
		close(fd[WRITE_END]);
		// tell child process to stop listening
		write_to_log_process("END");
	}
	else if(pid == 0){ //child process
		close(fd[READ_END]);
		fclose(log_file);
	}

	return 0;
}


