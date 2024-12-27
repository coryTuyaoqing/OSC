/**
 * \author {Yaoqing}
 */

#ifndef _DATAMGR_H_
#define _DATAMGR_H_

#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "sbuffer.h"

#ifndef RUN_AVG_LENGTH
#define RUN_AVG_LENGTH 5
#endif

#ifndef SET_MAX_TEMP
#define SET_MAX_TEMP 30
#endif

#ifndef SET_MIN_TEMP
#define SET_MIN_TEMP -10
#endif

#define DATAMGR_SUCCESS 0
#define DATAMGR_FAILURE 1

/*
 * Use ERROR_HANDLER() for handling memory allocation problems, invalid sensor IDs, non-existing files, etc.
 */
#define ERROR_HANDLER(condition, ...)    do {                       \
                      if (condition) {                              \
                        printf("\nError: in %s - function %s at line %d: %s\n", __FILE__, __func__, __LINE__, __VA_ARGS__); \
                        exit(EXIT_FAILURE);                         \
                      }                                             \
                    } while(0)

typedef struct temp_element temp_element_t;

/**
 *  Thes method parse the sensor-room map file and initialize the dplist. The data is leave empty.
 *  \param fp_sensor_map file pointer to the map file
 */
void datamgr_init(FILE *fp_sensor_map);

/**
 * This method should be called to clean up the datamgr, and to free all used memory. 
 * After this, any call to datamgr_get_room_id, datamgr_get_avg, datamgr_get_last_modified or datamgr_get_total_sensors will not return a valid result
 */
void datamgr_free();

/**
 * Gets the room ID for a certain sensor ID
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the corresponding room id
 */
uint16_t datamgr_get_room_id(sensor_id_t sensor_id);

/**
 * Gets the running AVG of a certain senor ID (if less then RUN_AVG_LENGTH measurements are recorded the avg is 0)
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the running AVG of the given sensor
 */
sensor_value_t datamgr_get_avg(sensor_id_t sensor_id);

/**
 * Returns the time of the last reading for a certain sensor ID
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the last modified timestamp for the given sensor
 */
time_t datamgr_get_last_modified(sensor_id_t sensor_id);

/**
 * Returns the temp_element for a certain sensor ID
 * Use ERROR_HANDLER() if sensor_id is invalid
 * \param sensor_id the sensor id to look for
 * \return the temp element for the given sensor
 */
temp_element_t *datamgr_get_temp_element(sensor_id_t sensor_id);

/**
 *  Return the total amount of unique sensor ID's recorded by the datamgr
 *  \return the total amount of sensors
 */
int datamgr_get_total_sensors();

/**
 * Read sensor data from a sbuffer and update the data in temp_avg_list (dplist)
 * \param buffer the shared data buffer in sensor gateway
 * \return new average temp
 */
int datamgr_start(sbuffer_t *buffer);

/**
 * Update temperature array and average temperature
 * \param element the temperature element of target room
 * \param new_temp new temperature data coming from sensor
 */
void datamgr_update_temp(temp_element_t *element, sensor_value_t new_temp);

#endif
