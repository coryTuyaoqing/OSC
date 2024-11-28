/**
 * \author Bert Lagaisse
 */

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "logger.h"

#include <stdbool.h>

/** open csv file and return FILE pointer
* - overwrite csv file or not depending on boolean value 'append' 
* - creat a child process to handle log events
* - generate a log event when a new csv file is created or an existing file has been opened
* \param filename file name of csv file
* \param append if 'append' is true, open with 'a' mode. Otherwise use 'w' mode (overwrite)
* return file pointer
*/
FILE * open_db(char * filename, bool append);

/** insert sensor data into csv file
* - generate log when the following events happens:
* - data insertion succeeded
* - an error occurred when writing to the csv file
* \param f csv file pointer
* \param id sensor id
* \param value sensor value
* \param ts snesor last modified time
* \return return 0 if everything goes well
*/
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);

/** close the csv file
* - generate a log event when the csv file has been closed
* \param f a pointer point to the csv file
* \return return 0 if everthing is ok
*/
int close_db(FILE * f);


#endif /* _SENSOR_DB_H_ */
