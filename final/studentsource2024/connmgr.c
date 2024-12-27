#include "connmgr.h"

/**
 * Server listening thread runner
 */
void *connmgr_listener(void *param);

int conn_counter = 0;
pthread_mutex_t counter_mutex;
tcpsock_t *server;
sbuffer_t *gateway_buffer;
sensor_data_t END_MSG = {
    .id = 0,
    .value = 0,
    .ts = 0
};

int connmgr_start(sbuffer_t *buffer, int MAX_CONN, int PORT){
    gateway_buffer = buffer;

    tcpsock_t *client[MAX_CONN]; // client array with length of max_conn

    // thread initialization
    pthread_t tid[MAX_CONN];   /* the thread identifier */
    pthread_attr_t attr;       /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);
    pthread_mutex_init(&counter_mutex, NULL);

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) {
        printf("tcp_passive_open error.\n");
        return CONNMGR_FAILURE;
    }
        
    for(int i=0; i<MAX_CONN; i++){
        /* create the thread */
        pthread_create(&tid[i], &attr, connmgr_listener, client[i]);
    }


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
    int bytes, result, sequence = 0;
    sensor_data_t data;
    tcpsock_t *client = param;

    if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR){
        printf("wait for connection error.");
        pthread_exit(NULL);
    }
    printf("Incoming client-%d connection\n", conn_counter);
    pthread_mutex_lock(&counter_mutex);
    conn_counter++;
    pthread_mutex_unlock(&counter_mutex);

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
            printf("Connection manager: sensor id = %" PRIu16
                   " - temperature = %g - timestamp = %ld, the %d times\n",
                   data.id, data.value, (long int)data.ts, sequence+1);
            sbuffer_insert(gateway_buffer, &data);
        }
        if(sequence == 0){
            // create log for first data package arrived
            char msg[50];
            sprintf(msg, "Sensor node %d has opened a new connection.", data.id);
            write_to_log_process(msg);
        }
        sequence++;
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED){
        printf("Peer has closed connection\n");
        char msg[50];
        sprintf(msg, "Sensor node %d has closed the connection.", data.id);
        write_to_log_process(msg);
    }
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);

    pthread_exit(0);
}
