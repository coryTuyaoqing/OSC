#include <stdio.h>
#include <stdlib.h>
#include "sensor_db.h"

FILE * open_db(char * filename, bool append){
	char* indicator = append ? "w": "w+"; 
	FILE* file = fopen(filename, indicator);

	return file;
}
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
	fprintf(f, "%d,%f,%ld\n", id, value, (long)ts);

	return 0;
}
int close_db(FILE * f){
	fclose(f);

	return 0;
}


