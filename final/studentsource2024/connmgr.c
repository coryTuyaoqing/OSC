#include <sys/select.h>
#include <errno.h>
#include "connmgr.h"

/**
 * Server listening thread runner
 */
void *connmgr_listener(void *param);

int conn_counter = 0;
pthread_mutex_t counter_mutex;
tcpsock_t *server;
sbuffer_t *gateway_buffer;

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
    sensor_data_t END_MSG = {
        .id = 0,
        .value = 0,
        .ts = 0
    };
    sbuffer_insert(gateway_buffer, &END_MSG);
    printf("Connection manager sent end msg\n");
    pthread_mutex_destroy(&counter_mutex);

    printf("Connection manager is shutting down\n");
    return CONNMGR_SUCCESS;
}

void *connmgr_listener(void *param){
    int bytes, result, sequence = 0;
    sensor_data_t data;
    tcpsock_t *client = param;
    // time_t last_data;

    // pthread_t tid_timer;
    // pthread_attr_t attr;

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
        result = connmgr_receive_time_limited(client, (void *)&data.id, &bytes, TIMEOUT);
        if(result == CONNMGR_TIMEOUT) break;
        // read temperature
        bytes = sizeof(data.value);
        result = connmgr_receive_time_limited(client, (void *)&data.value, &bytes, TIMEOUT);
        if(result == CONNMGR_TIMEOUT) break;
        // read timestamp
        bytes = sizeof(data.ts);
        result = connmgr_receive_time_limited(client, (void *)&data.ts, &bytes, TIMEOUT);
        if(result == CONNMGR_TIMEOUT) break;
        if ((result == TCP_NO_ERROR) && bytes) {
            printf("Connection manager: sensor id = %" PRIu16
                   " - temperature = %g - timestamp = %ld, the %d times\n",
                   data.id, data.value, (long int)data.ts, sequence+1);
                sbuffer_insert(gateway_buffer, &data);

            // create log for first data package arrived
            if(sequence == 0){
                char msg[100] = {0};
                sprintf(msg, "Sensor node %d has opened a new connection.", data.id);
                write_to_log_process(msg);
            }
            sequence++;
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED){
        printf("Peer has closed connection\n");
        char msg[100] = {0};
        sprintf(msg, "Sensor node %d has closed the connection.", data.id);
        write_to_log_process(msg);
    }
    else if(result == CONNMGR_TIMEOUT){
        printf("Client-%d hasn't sent data for %d seconds. TIMEOUT ERROR.\n", data.id, TIMEOUT);
        char msg[100] = {0};
        sprintf(msg, "Sensor %d connection overtime.", data.id);
        write_to_log_process(msg);
    }
    else    
        printf("Error occured on connection to peer\n");
    tcp_close(&client);

    pthread_exit(0);
}

int connmgr_receive_time_limited(tcpsock_t *socket, void *buffer, int *buf_size, int seconds){
    // pthread_t tid_timer;
    // pthread_

    // int result = tcp_receive(socket, buf_size, buf_size);

    // return result;
    if (!socket || !buffer || !buf_size || *buf_size <= 0) {
        return TCP_SOCKET_ERROR; // Invalid parameters
    }

    // Get the file descriptor from the socket
    int sock_fd = 0;
    tcp_get_sd(socket, &sock_fd);
    if (sock_fd < 0) {
        return TCP_SOCKET_ERROR; // Error retrieving file descriptor
    }

    // Set up the timeout for select()
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    // Set up the file descriptor set
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock_fd, &readfds);

    // Wait for data to become available
    int result = select(sock_fd + 1, &readfds, NULL, NULL, &timeout);
    if (result == 0) {
        // Timeout occurred
        return CONNMGR_TIMEOUT;
    } else if (result < 0) {
        // Error in select()
        perror("select error");
        return TCP_SOCKET_ERROR;
    }

    // Data is available, proceed with tcp_receive
    return tcp_receive(socket, buffer, buf_size);
}
