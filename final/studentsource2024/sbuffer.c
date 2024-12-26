/**
 * \author {AUTHOR}
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sbuffer.h"

pid_t pid;
int fd[2];
int log_num = 0;
FILE* log_file;

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next; /**< a pointer to the next node*/
    sensor_data_t data;        /**< a structure containing the data */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head; /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail; /**< a pointer to the last node in the buffer */
    pthread_mutex_t mutex;
    pthread_cond_t condvar;
    bool removable;       /**<  a boolean variable to indicate if node can be removed or not*/
};

int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    (*buffer)->removable = false;
    pthread_mutex_init(&(*buffer)->mutex, NULL);
    pthread_cond_init(&(*buffer)->condvar, NULL);
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;

    pthread_mutex_lock(&(*buffer)->mutex);
    // lock the mutex of buffer, preventing other threadto access buffer
    // critical session
    if ((buffer == NULL) || (*buffer == NULL)) {
        pthread_mutex_unlock(&(*buffer)->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    pthread_mutex_unlock(&(*buffer)->mutex); // unlock mutex
    free(*buffer);
    *buffer = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;

    // printf("thread%ld: trying to read from buffer\n", pthread_self());
    pthread_mutex_lock(&buffer->mutex); // lock the mutex of buffer, preventing
                                        // other thread to access buffer
    // critical session
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    if (buffer->head == NULL) { // no data
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_NO_DATA;
    }

    // remove only when the buffer is removable
    while(!buffer->removable)
        pthread_cond_wait(&buffer->condvar, &buffer->mutex);

    *data = buffer->head->data;
    dummy = buffer->head;
    if (buffer->head == buffer->tail) // buffer has only one node
    {
        buffer->head = buffer->tail = NULL;
    } else // buffer has many nodes empty
    {
        buffer->head = buffer->head->next;
    }

    buffer->removable = false; // set buffer to not removalbe after remove
    printf("buffer is removed; buffer removable: %s\n", buffer->removable ? "removable" : "not removable");

    pthread_cond_signal(&buffer->condvar);
    pthread_mutex_unlock(&buffer->mutex); // unlock mutex
    // printf("thread%ld: after read from buffer\n", pthread_self());

    free(dummy);

    return SBUFFER_SUCCESS;
}

int sbuffer_peek(sbuffer_t *buffer, sensor_data_t *data){
    // printf("thread%ld: trying to read from buffer\n", pthread_self());
    pthread_mutex_lock(&buffer->mutex); // lock the mutex of buffer, preventing
                                        // other thread to access buffer
    // critical session
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    if (buffer->head == NULL) { // no data
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_NO_DATA;
    }

    // peek only when buffer is not removable
    while(buffer->removable)
        pthread_cond_wait(&buffer->condvar, &buffer->mutex);

    *data = buffer->head->data;

    buffer->removable = true; // set buffer to removable after peek
    printf("buffer is peeked; buffer is %s\n", buffer->removable ? "removable" : "not removable");

    pthread_cond_signal(&buffer->condvar);
    pthread_mutex_unlock(&buffer->mutex); // unlock mutex
    // printf("thread%ld: after read from buffer\n", pthread_self());

    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;

    // printf("thread%ld: trying to insert data to buffer\n", pthread_self());
    pthread_mutex_lock(&buffer->mutex); // lock the mutex of buffer, preventing
                                        // other thread to access buffer
    // critical session
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    dummy = malloc(sizeof(sbuffer_node_t));
    if (dummy == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    dummy->data = *data;
    dummy->next = NULL;
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        buffer->head = buffer->tail = dummy;
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    pthread_mutex_unlock(&buffer->mutex); // unlock mutex
    // printf("thread%ld: after insert data\n", pthread_self());

    return SBUFFER_SUCCESS;
}

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
			// parent process close write end and let child process read EOF and stop
		}
	}
	return 0;
}

void process_log_msg(char* log_msg){
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
		// close write end and let child process read EOF and stop
	}
	else if(pid == 0){ //child process
		close(fd[READ_END]);
		fclose(log_file);
	}

	return 0;
}
