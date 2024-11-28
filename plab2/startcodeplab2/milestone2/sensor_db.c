#include <stdio.h>
#include <stdlib.h>
#include "sensor_db.h"

FILE * open_db(char * filename, bool append){
	// if in append mode, generate a log saying 'an existing file is opened'
	// if in overwrite mode, generate a log saying 'a new file is genereated'
	char* msg;
	if(append == true){
		msg = "Data file opened.";
	}
	else{
		msg = "Data file created.";
	}
	write_to_log_process(msg);

	int result = create_log_process();
	if(result == -1){
		puts("child process generating failed.");
		exit(1);
	}
	else if(result == 1){ // parent process
		char* indicator = append ? "a": "w"; 
		FILE* file = fopen(filename, indicator);
		return file;
	}
	else{ // child process
		return NULL;
	}
}
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
	char* msg;

	if(f != NULL){
		int write_result = fprintf(f, "%d, %f, %ld\n", id, value, (long)ts);
		if(write_result != -1){
			msg = "Data inserted.";
		}
		else{
			msg = "Data insert failed.";
		}
	}

	write_to_log_process(msg);

	return 0;
}
int close_db(FILE * f){
	fclose(f);
	char* msg = "Data file closed.";
	write_to_log_process(msg);
	end_log_process();

	return 0;
}


