/**
 * \author Yaoqing
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>

#define SIZE 512 
#define READ_END 0
#define WRITE_END 1
#define LOG_FILE_NAME "gateway.log"
#define ENG_MSG "END"

/**
 * Write a log message to log file. 
 * Called by parent process and send msg to child process. 
 * \param msg message to be sent to child process
 * \return 0 if nothing goes wrong
 */
int write_to_log_process(char *msg);
/**
 * Create a log process. 
 * Child process keep listening to log message. 
 * \return 1 if parent process, 0 if children process, -1 if fail to fork
 */
int create_log_process();
/**
 * Close pip, close log file and end log process
 */
int end_log_process();
/**
 * Write log message to log file. 
 * Called by child (log) process. 
 */
void process_log_msg(char *log_msg);

#endif
