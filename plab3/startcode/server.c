/**
 * \author {Yaoqing}
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "config.h"
#include "lib/tcpsock.h"

/**
 * Implements a multi-thread server
 */

int main(int argc, char* argv[]){
	tcpsock_t *server, *client;
	sensor_data_t data;
	int bytes, result;
	int conn_counter = 0;

	if(argc < 3) {
    	printf("Please provide the right arguments: first the port, then the max nb of clients");
    	return -1;
    }

	int MAX_CONN = atoi(argv[2]);
	int PORT = atoi(argv[1]);

	if(tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is started\n");
	do{
		if(tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
		printf("Imcoming client connection\n");
		conn_counter++;

		// connection set up successfully. start reading data from client
		do{
			// if no error occur and no bytes return, print data and close connection
			if((result == TCP_NO_ERROR) && bytes){
				printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int) data.ts);
			}
		}while(result == TCP_NO_ERROR);

	}while (conn_conter < MAX_CONN);
	 

	return 0;
}
