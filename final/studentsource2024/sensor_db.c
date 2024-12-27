#include "sensor_db.h"
#include <stdio.h>
#include <stdlib.h>

FILE *open_db(char *filename, bool append) {
    // if in append mode, generate a log saying 'an existing file is opened'
    // if in overwrite mode, generate a log saying 'a new file is genereated'
    char *msg;

	char *indicator = append ? "a" : "w";
	FILE *file = fopen(filename, indicator);
	if (file != NULL) {
		if (append == true)
			msg = "The old data.csv file is opened.";
		else
			msg = "A new data.csv file has been created.";
	}
	else{
		printf("data.csv file fail to create\n");
		return NULL;
	}
	write_to_log_process(msg);
	return file;
}
int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    char msg[50];

    if (f != NULL) {
        int write_result = fprintf(f, "%d, %f, %ld\n", id, value, (long)ts);
        if (write_result != -1) {
            sprintf(msg, "Data insertion from sensor %d succeeded.", id);
        } else {
            sprintf(msg, "Data insertion from sensor %d failed.", id);
        }
    }

    write_to_log_process(msg);

    return 0;
}
int close_db(FILE *f) {
    fclose(f);
    char *msg = "The data.csv file has been closed.";
    write_to_log_process(msg);

    return 0;
}

int start_sensor_db(FILE *f, sbuffer_t *buffer){
	sensor_data_t sensor_data;
	int result;
	while(1){
		/*read data from buffer*/ 
		do{
			result = sbuffer_remove(buffer, &sensor_data);
			// printf("sensor_db read no data\n");
		}while(result == SBUFFER_NO_DATA);
		if(result == SBUFFER_FAILURE){
			printf("Sensor database fail to read data from buffer\n");
			return SENSOR_DB_FAILURE;
		}

		/*check if connmgr sent END_MSG (all zero sensor data)*/ 
		if(sensor_data.ts == 0 && sensor_data.id == 0 && sensor_data.value == 0){
			printf("Sensor database end\n");
			return SENSOR_DB_SUCCESS;
		}

		/*insert data into data.csv*/
		printf("Sensor database: %d - %f - %ld\n", sensor_data.id, sensor_data.value, (long int) sensor_data.ts);
		insert_sensor(f, sensor_data.id, sensor_data.value, sensor_data.ts);
	}
}
