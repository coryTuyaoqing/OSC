#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sbuffer.h"

void *writer(void *);
void *reader(void *);

int main() {
    sbuffer_t *buffer;

    // multithread initilzaiton
    pthread_t pid_w, pid_r[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (sbuffer_init(&buffer) != SBUFFER_SUCCESS) {
        printf("sbuffer fail to init\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&pid_w, &attr, writer, buffer);
    // sleep(1);
    pthread_create(&pid_r[0], &attr, reader, buffer);
    pthread_create(&pid_r[1], &attr, reader, buffer);

    pthread_join(pid_w, NULL);
    pthread_join(pid_r[0], NULL);
    pthread_join(pid_r[1], NULL);

    if(sbuffer_free(&buffer) == SBUFFER_SUCCESS)
        printf("free buffer sucessfully\n");

    return 0;
}

void *writer(void *param) {
    printf("enter writer sub-thread\n");

    FILE *data = fopen("sensor_data", "rb");
    if (data == NULL) exit(EXIT_FAILURE);
    sbuffer_t *buffer = param;
    sensor_data_t sensor_data;

    // write data to buffer
    while (!feof(data)) {
        fread(&sensor_data.id, sizeof(sensor_id_t), 1, data);
        fread(&sensor_data.value, sizeof(sensor_value_t), 1, data);
        fread(&sensor_data.ts, sizeof(sensor_ts_t), 1, data);
        printf("writer%ld: id: %d, value: %f, ts: %s", pthread_self(),
               sensor_data.id, sensor_data.value,
               asctime(gmtime(&sensor_data.ts)));
        if (sbuffer_insert(buffer, &sensor_data) == SBUFFER_SUCCESS)
            printf("insert sucessfully\n");
        usleep(10);
    }

    if(feof(data)){
        sensor_data.id = 0;
        sensor_data.value = 0;
        sensor_data.ts = 0;
        for(int i=0; i<2; i++){
            if (sbuffer_insert(buffer, &sensor_data) == SBUFFER_SUCCESS)
            printf("EOF\n");
        }
    }

    pthread_exit(0);
}

void *reader(void *param) {
    printf("enter reader%ld sub-thread\n", pthread_self());

    sbuffer_t *buffer = param;
    int result;
    // read from buffer
    // todo: if SBUFFER_NO_DATA, block and wait
    sensor_data_t sensor_data;

    while(1){
        result = sbuffer_remove(buffer, &sensor_data);

        if(result == SBUFFER_FAILURE){
            printf("reader%ld: SBUFFER_FAILURE\n", pthread_self());
            break;
        }
        else if(result == SBUFFER_NO_DATA){
            printf("reader%ld: SBUFFER_NO_DATA\n", pthread_self());
            continue;
        }
        else if(result == SBUFFER_SUCCESS){
            // check if is EOF
            if(sensor_data.ts == 0) {
                printf("reader%ld: Detect EOF\n", pthread_self());
                break;
            }
            printf("reader%ld: id: %d, value: %f, ts: %s", pthread_self(), sensor_data.id,
            sensor_data.value, asctime(gmtime(&sensor_data.ts)));
            // write data to csv file
            usleep(25);
        }
    }
    
    pthread_exit(0);
}
