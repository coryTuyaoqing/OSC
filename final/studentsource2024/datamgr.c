#include "datamgr.h"
#include "config.h"
#include "lib/dplist.h"

// the list of datamgr
static dplist_t *temp_avg_list = NULL;

// the element type of list
struct temp_element{
    sensor_id_t sensor_id;
    uint16_t room_id;
    sensor_ts_t last_modified_ts;
    sensor_value_t temp_avg;
    sensor_value_t temp[RUN_AVG_LENGTH]; // previous temp data: temp[0] is the newest, temp[4] is the oldest
	int n; //how many sensor data have been process
};

// if fail to malloc, return NULL
void *element_copy(void *element) {
    temp_element_t *temp_element_cp = malloc(sizeof(temp_element_t));
    if (temp_element_cp == NULL) return NULL;

    temp_element_cp->room_id = ((temp_element_t *)element)->room_id;
    temp_element_cp->sensor_id = ((temp_element_t *)element)->sensor_id;
    temp_element_cp->last_modified_ts = ((temp_element_t *)element)->last_modified_ts;
    temp_element_cp->temp_avg = ((temp_element_t *)element)->temp_avg;
    for (int i = 0; i < RUN_AVG_LENGTH; i++) {
        temp_element_cp->temp[i] = ((temp_element_t *)element)->temp[i];
    }
	temp_element_cp->n = ((temp_element_t *)element)->n;

    return (void *)temp_element_cp;
}

void element_free(void **element) {
    free(*element);
    *element = NULL;
}

int element_compare(void *el1, void *el2) {
    sensor_value_t delta_temp_avg =
        ((temp_element_t *)el1)->sensor_id - ((temp_element_t *)el2)->sensor_id;
    return (delta_temp_avg < 0) ? -1 : ((delta_temp_avg == 0) ? 0 : 1);
}

void datamgr_init(FILE *fp_sensor_map) {
    temp_avg_list = dpl_create(element_copy, element_free, element_compare);
    temp_element_t temp_element = {
        .room_id = 0, 
		.sensor_id = 0, 
		.last_modified_ts = 0, 
		.temp = {0},
		.n = 0
	};

    // read map file
    int room_id, sensor_id;
    while (fscanf(fp_sensor_map, "%d %d\n", &room_id, &sensor_id) != -1) {
        // scanf return -1 at EOF
        // printf("room id: %d, sensor id: %d\n", room_id, sensor_id);
        temp_element.room_id = room_id;
        temp_element.sensor_id = sensor_id;
        dpl_insert_at_index(temp_avg_list, &temp_element, 0, true);
    }

    // // print out the list to see if insert correctly
    // temp_element_t *element;
    // int list_size = dpl_size(temp_avg_list);
    // for(int i=list_size; i>=0; i--){
    //     element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list,
    //     i); printf("No%d: room id = %d, sensor id = %d, sensor value = %f\n",
    //     i, element->room_id, element->sensor_id, element->temp_avg);
    // }
}

void datamgr_free() { dpl_free(&temp_avg_list, true); }

uint16_t datamgr_get_room_id(sensor_id_t sensor_id) {
    temp_element_t *element = datamgr_get_temp_element(sensor_id);
	return element->room_id;
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id) {
    temp_element_t *element = datamgr_get_temp_element(sensor_id);
	return element->temp_avg;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id) {
    temp_element_t *element = datamgr_get_temp_element(sensor_id);
	return element->last_modified_ts;
}

temp_element_t *datamgr_get_temp_element(sensor_id_t sensor_id){
	int size = dpl_size(temp_avg_list);
	temp_element_t *element = NULL;
	for (int i = 0; i < size; i++) {
        element = (temp_element_t *)dpl_get_element_at_index(temp_avg_list, i);
        if (element->sensor_id == sensor_id) {
            return element;
        }
    }
    ERROR_HANDLER(true, "invalid sensor id");
}

int datamgr_get_total_sensors() { return dpl_size(temp_avg_list); }

sensor_value_t datamgr_read_buffer(sbuffer_t *buffer) {
    sensor_value_t temp_avg = 0;
	sensor_data_t sensor_data;

    while(sbuffer_peek(buffer, &sensor_data) == SBUFFER_NO_DATA){
		// printf("datamgr read no data\n");
	}
	temp_element_t *element = datamgr_get_temp_element(sensor_data.id);
	element->last_modified_ts = sensor_data.ts;
	// update temp[] array: remove the oldest one and add new one
	for(int i=RUN_AVG_LENGTH-1; i>0; i--){
		element->temp[i] = element->temp[i-1];
	}
	element->temp[0] = sensor_data.value;
	// calculate the avg temp
	sensor_value_t temp_sum = 0;
	for(int i=0; i<RUN_AVG_LENGTH; i++){
		temp_sum += element->temp[i];
	}
	temp_avg = temp_sum / RUN_AVG_LENGTH;
	
    return temp_avg;
}
