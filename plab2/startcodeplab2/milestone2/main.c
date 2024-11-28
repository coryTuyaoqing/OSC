#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

#include "config.h"
#include "logger.h"
#include "sensor_db.h"

int main()
{
    FILE* f = open_db("sensor_db.csv", true);

    sleep(1);
    sensor_id_t id = 1;
    sensor_value_t v = 0.001;
    sensor_ts_t ts = time(NULL);
    insert_sensor(f, id, v, ts);

    id = 2;
    v = 0.002;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    id = 3;
    v = 0.003;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    sleep(5);
    insert_sensor(f, 4, v, ts);

	for(int i=0; i<100; i++){
	    insert_sensor(f, 4, v, ts);
	}

    close_db(f);
    return 0;
}
