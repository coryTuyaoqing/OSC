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
			msg = "Data file opened.";
		else
			msg = "Data file created.";
	}
	else{
		printf("Data file fail to create\n");
		return NULL;
	}
	write_to_log_process(msg);
	return file;
}
int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    char *msg;

    if (f != NULL) {
        int write_result = fprintf(f, "%d, %f, %ld\n", id, value, (long)ts);
        if (write_result != -1) {
            msg = "Data inserted.";
        } else {
            msg = "Data insert failed.";
        }
    }

    write_to_log_process(msg);

    return 0;
}
int close_db(FILE *f) {
    fclose(f);
    char *msg = "Data file closed.";
    write_to_log_process(msg);

    return 0;
}
