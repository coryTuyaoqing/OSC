/**
 * \author {AUTHOR}
 */

#include "sbuffer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * basic node for the buffer, these nodes are linked together to create the
 * buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next; /**< a pointer to the next node*/
    sensor_data_t data;        /**< a structure containing the data */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head; /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail; /**< a pointer to the last node in the buffer */
    pthread_mutex_t mutex;
};

int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    pthread_mutex_init(&(*buffer)->mutex, NULL);
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;

    pthread_mutex_lock(&(*buffer)->mutex);
    // lock the mutex of buffer, preventing other threadto access buffer
    // critical session
    if ((buffer == NULL) || (*buffer == NULL)) {
        pthread_mutex_unlock(&(*buffer)->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    pthread_mutex_unlock(&(*buffer)->mutex); // unlock mutex
    free(*buffer);
    *buffer = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;

    // printf("thread%ld: trying to read from buffer\n", pthread_self());
    pthread_mutex_lock(&buffer->mutex); // lock the mutex of buffer, preventing
                                        // other thread to access buffer
    // critical session
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    if (buffer->head == NULL) { // no data
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_NO_DATA;
    }
    *data = buffer->head->data;
    dummy = buffer->head;
    if (buffer->head == buffer->tail) // buffer has only one node
    {
        buffer->head = buffer->tail = NULL;
    } else // buffer has many nodes empty
    {
        buffer->head = buffer->head->next;
    }

    pthread_mutex_unlock(&buffer->mutex); // unlock mutex
    // printf("thread%ld: after read from buffer\n", pthread_self());

    free(dummy);

    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;

    // printf("thread%ld: trying to insert data to buffer\n", pthread_self());
    pthread_mutex_lock(&buffer->mutex); // lock the mutex of buffer, preventing
                                        // other thread to access buffer
    // critical session
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    dummy = malloc(sizeof(sbuffer_node_t));
    if (dummy == NULL) {
        pthread_mutex_unlock(&buffer->mutex); // unlock mutex
        return SBUFFER_FAILURE;
    }
    dummy->data = *data;
    dummy->next = NULL;
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        buffer->head = buffer->tail = dummy;
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    pthread_mutex_unlock(&buffer->mutex); // unlock mutex
    // printf("thread%ld: after insert data\n", pthread_self());

    return SBUFFER_SUCCESS;
}
