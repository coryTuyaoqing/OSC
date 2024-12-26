#include "connmgr.h"

/**
 * Server listening thread runner
 */
void *connmgr_listener(void *param);

sbuffer_t *gateway_buffer;
sensor_data_t END_MSG = {
    .id = 0,
    .value = 0,
    .ts = 0
};

int connmgr_start(sbuffer_t *buffer, int MAX_CONN, int PORT){
    gateway_buffer = buffer;

    int conn_counter = 0;
    tcpsock_t *server;
    tcpsock_t *client[MAX_CONN]; // client array with length of max_conn

    // thread initialization
    pthread_t tid[MAX_CONN];   /* the thread identifier */
    pthread_attr_t attr;       /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) return CONNMGR_FAILURE;
    do {
        if (tcp_wait_for_connection(server, &client[conn_counter]) !=
            TCP_NO_ERROR)
            return CONNMGR_FAILURE;
        printf("Incoming client-%d connection\n", conn_counter);
        /* create the thread */
        pthread_create(&tid[conn_counter], &attr, connmgr_listener, client[conn_counter]);

        conn_counter++;
    } while (conn_counter < MAX_CONN);

    /* wait for the thread to exit */
    for (int i = 0; i < MAX_CONN; i++) {
        pthread_join(tid[i], NULL);
    }

    if (tcp_close(&server) != TCP_NO_ERROR) return CONNMGR_FAILURE;

    // if all sensor nodes close connections
    // tell other thread to stop through buffer (send end message)
    printf("connmgr sent end msg\n");
    sbuffer_insert(gateway_buffer, &END_MSG);

    printf("Test server is shutting down\n");
    return CONNMGR_SUCCESS;
}

void *connmgr_listener(void *param){
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
            sbuffer_insert(gateway_buffer, &data);

        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);

    pthread_exit(0);
}
