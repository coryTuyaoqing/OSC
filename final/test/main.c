#include <pthread.h>
#include <stdio.h>
#include "sbuffer.h"
#include "datamgr.h"
#include "sensor_db.h"
#include "connmgr.h"

sbuffer_t *buffer;
int MAX_CONN, PORT;

void *connmgr_thread(void *param);
void *datamgr_thread(void *param);
void *sensor_db_thread(void *param);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide the right arguments: first the port, then the "
               "max nb of clients");
        return -1;
    }
    MAX_CONN = atoi(argv[2]);
    PORT = atoi(argv[1]);
    
    int result = create_log_process();
    if (result == -1) {
        puts("child process generating failed.");
        exit(1);
    }
    else if(result == 1){ //parent process
        sbuffer_init(&buffer);
        
        pthread_t tid_connmgr, tid_datamgr, tid_sensor_db;
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        pthread_create(&tid_connmgr, &attr, connmgr_thread, NULL);
        pthread_create(&tid_datamgr, &attr, datamgr_thread, NULL);
        pthread_create(&tid_sensor_db, &attr, sensor_db_thread, NULL);

        pthread_join(tid_connmgr, NULL);
        pthread_join(tid_datamgr, NULL);
        pthread_join(tid_sensor_db, NULL);

        sbuffer_free(&buffer);
    }
    
    end_log_process();
    printf("Gateway server is shutdown.\n");

    return 0;
}

void *connmgr_thread(void *param){
    if(connmgr_start(buffer, MAX_CONN, PORT) != CONNMGR_SUCCESS)
        printf("connmgr fail.\n");

    pthread_exit(0);
}

void *datamgr_thread(void *param){
    FILE *map = fopen("room_sensor.map", "r");
    
    datamgr_init(map);
    datamgr_start(buffer);
    datamgr_free();
    fclose(map);

    pthread_exit(0);
}
void *sensor_db_thread(void *param){
    FILE *db_file = open_db("data.csv", false);
    // read data from buffer in a loop
    // put data into sensor_db.csv
    start_sensor_db(db_file, buffer);

    close_db(db_file);

    pthread_exit(0);
}


