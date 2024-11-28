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
    temp_element_t temp_element;

    // read map file
    int room_id, sensor_id;
    while(fscanf(fp_sensor_map, "%d %d\n", &room_id, &sensor_id) != -1){ //scanf return -1 at EOF
        //printf("room id: %d, sensor id: %d\n", room_id, sensor_id);
        temp_element.room_id = room_id;
        temp_element.sensor_id = sensor_id;
        temp_element.temp_avg = 0;
        dpl_insert_at_index(temp_avg_list, &temp_element, 0, true);
    } 

    // print out the list to see if insert correctly
	temp_element_t* element;
	int list_size = dpl_size(temp_avg_list);
    //for(int i=list_size; i>=0; i--){
    //    element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
    //    printf("No%d: room id = %d, sensor id = %d, sensor value = %f\n", i, element->room_id, element->sensor_id, element->temp_avg);
    //}
    
    // read data file and calculate
	sensor_value_t temp_buffer = 0;
	sensor_ts_t timestamp_buffer = 0;

	while(!feof(fp_sensor_data)){
		// read data into buffer
		fread(&sensor_id, sizeof(sensor_id_t), 1, fp_sensor_data);
		fread(&temp_buffer, sizeof(sensor_value_t), 1, fp_sensor_data);
		fread(&timestamp_buffer, sizeof(sensor_ts_t), 1, fp_sensor_data);
		//printf("sensor_id = %d, temp_buffer = %f, timestamp_buffer = %s", sensor_id, temp_buffer, asctime(gmtime(&timestamp_buffer)));

		// put data into the linklist
		int i;
		for(i=0; i<list_size; i++){
			element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
			if(element->sensor_id == sensor_id){
				element->temp_avg = temp_buffer;
				element->last_modified = timestamp_buffer;

				//print log message
				if(temp_buffer > SET_MAX_TEMP){
					puts("Warning: the avg temp is larger than the max temp.");
				}
				else if(temp_buffer < SET_MIN_TEMP){
					puts("Warning: the avg temp is smaller than the min temp.");
				}

				break;
			}
		}
		if(i == list_size)
			puts("sensor id not found");
	}

	// print out all contain in link list
	for(int i=list_size; i>=0; i--){
		element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
		printf("No%d: room id = %d, sensor id = %d, sensor value = %f\n", i, element->room_id, element->sensor_id, element->temp_avg);
		printf("Last modified time: %s", asctime(gmtime(&element->last_modified)));
	}
}

void datamgr_free(){
    dpl_free(&temp_avg_list, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
	int size = dpl_size(temp_avg_list);
	temp_element_t* element = NULL;
	for(int i=0; i<size; i++){
		element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
		if(element->sensor_id == sensor_id){
			return element->room_id;
		}
	}
	ERROR_HANDLER(true, "invalid sensor id");
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
	int size = dpl_size(temp_avg_list);
	temp_element_t* element = NULL;
	for(int i=0; i<size; i++){
		element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
		if(element->sensor_id == sensor_id){
			return element->temp_avg;
		}
	}
	ERROR_HANDLER(true, "invalid sensor id");
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
	int size = dpl_size(temp_avg_list);
	temp_element_t* element = NULL;
	for(int i=0; i<size; i++){
		element = (temp_element_t*)dpl_get_element_at_index(temp_avg_list, i);
		if(element->sensor_id == sensor_id){
			return element->last_modified;
		}
	}
	ERROR_HANDLER(true, "invalid sensor id");
}

int datamgr_get_total_sensors(){
	return dpl_size(temp_avg_list);
}
