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

int write_to_log_process(char *msg);
int create_log_process();
int end_log_process();
void process_log_msg(char *log_msg);

#endif
