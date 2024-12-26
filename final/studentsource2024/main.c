#include <pthread.h>
#include <stdio.h>
#include "sbuffer.h"
#include "datamgr.h"
#include "sensor_db.h"

void *connmgr_thread(void *param);
void *datamgr_thread(void *param);
void *sensor_db_thread(void *param);

int main(int argc, char *argv[]) {
    puts("Hello from main\n");

    sbuffer_t *buffer;
    sbuffer_init(&buffer);

    open_db("sensor_db.csv", true);

    pthread_t tid_connmgr, tid_datamgr, tid_sensor_db;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid_connmgr, &attr, connmgr_thread, buffer);
    pthread_create(&tid_datamgr, &attr, datamgr_thread, buffer);
    pthread_create(&tid_sensor_db, &attr, sensor_db_thread, buffer);

    pthread_join(tid_connmgr, NULL);
    pthread_join(tid_datamgr, NULL);
    pthread_join(tid_sensor_db, NULL);

    return 0;
}

void *connmgr_thread(void *param){

    pthread_exit(0);
}
void *datamgr_thread(void *param){
    FILE *map = fopen("room_sensor.map", "r");
    sbuffer_t *buffer = param;
    datamgr_init(map);
    datamgr_read_buffer(buffer);

    pthread_exit(0);
}
void *sensor_db_thread(void *param){

    pthread_exit(0);
}
