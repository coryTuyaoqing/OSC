/**
 * \author Yaoqing
 */

#ifndef _CONNMGR_H_
#define _CONNMGR_H_

#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include "lib/tcpsock.h"
#include "sbuffer.h"

#define CONNMGR_SUCCESS 0
#define CONNMGR_FAILURE 1

/**
 * Open listening thread to get data from sensor and put data into buffer
 * \param gateway_buffer the shared data buffer
 * \return flag that indicate if the operation is success
 */
int connmgr_start(sbuffer_t *gateway_buffer, int MAX_CONN, int PORT);

#endif
