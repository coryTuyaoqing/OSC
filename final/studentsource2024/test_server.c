/**
 * \author {Yaoqing}
 */

#include "config.h"
#include "lib/tcpsock.h"
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Implements a sequential test server (only one connection at the same time)
 */

int main(int argc, char *argv[]) {
    tcpsock_t *server;
    int conn_counter = 0;

    if (argc < 3) {
        printf("Please provide the right arguments: first the port, then the "
               "max nb of clients");
        return -1;
    }

    int MAX_CONN = atoi(argv[2]);
    int PORT = atoi(argv[1]);
    // client array with length of max_conn
    tcpsock_t *client[MAX_CONN];

    // thread initialization
    void *runner(void *param); /* threads call this function */
    pthread_t tid[MAX_CONN];   /* the thread identifier */
    pthread_attr_t attr;       /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client[conn_counter]) !=
            TCP_NO_ERROR)
            exit(EXIT_FAILURE);
        printf("Incoming client-%d connection\n", conn_counter);
        /* create the thread */
        pthread_create(&tid[conn_counter], &attr, runner, client[conn_counter]);

        conn_counter++;
    } while (conn_counter < MAX_CONN);

    /* wait for the thread to exit */
    for (int i = 0; i < MAX_CONN; i++) {
        pthread_join(tid[i], NULL);
    }

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    return 0;
}

void *runner(void *param) {
    int bytes, result;
    sensor_data_t data;
    tcpsock_t *client = param;

    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(client, (void *)&data.id, &bytes);
        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(client, (void *)&data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client, (void *)&data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            printf("sensor id = %" PRIu16
                   " - temperature = %g - timestamp = %ld\n",
                   data.id, data.value, (long int)data.ts);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);

    pthread_exit(0);
}
