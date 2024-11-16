#include "datamgr.h"
#include "lib/dplist.h"

// the list of datamgr
static dplist_t* temp_avg_list = NULL;

// the element type of list
typedef struct {
    sensor_id_t sensor_id;
    room_id_t room_id;
    sensor_value_t temp_avg;
    sensor_ts_t last_modified;
}temp_element_t;

void* element_copy(void* element){
    temp_element_t* temp_element_cp = malloc(sizeof(temp_element_t));
    temp_element_cp->room_id = ((temp_element_t*)element)->room_id;
    temp_element_cp->sensor_id = ((temp_element_t*)element)->sensor_id;
    temp_element_cp->temp_avg = ((temp_element_t*)element)->temp_avg;
    temp_element_cp->last_modified = ((temp_element_t*)element)->last_modified;
    return (void*)temp_element_cp;
}

void element_free(void** element){
    free(*element);
    *element = NULL;
}

int element_compare(void* el1, void* el2){
    sensor_value_t delta_temp_avg = ((temp_element_t*)el1)->temp_avg - ((temp_element_t*)el2)->temp_avg;
    return (delta_temp_avg < 0) ? -1 : ((delta_temp_avg == 0) ? 0 : 1);
}

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    temp_avg_list = dpl_create(element_copy, element_free, element_compare);
    // read map file

    // read data file and calculate 
}

void datamgr_free(){
    
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){

}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){

}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){

}

int datamgr_get_total_sensors(){

}
