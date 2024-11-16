/**
 * \author Jeroen Van Aken, Bert Lagaisse, Ludo Bruynseels
 */

#include "dplist.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * The real definition of struct list / struct node
 */
struct dplist_node {
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist {
    dplist_node_t* head;
    // more fields will be added later
};

dplist_t* dpl_create()
{
    dplist_t* list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    return list;
}

void dpl_free(dplist_t** list)
{
    // TODO: add your code here
    // Do extensive testing with valgrind.
    dplist_t* dplist = *list;
    dplist_node_t* dplist_node = dplist->head;
    if (dplist != NULL && dplist_node != NULL) {
        while (dplist_node->next != NULL) {
            dplist_node = dplist_node->next;
            free(dplist_node->prev);
        }
        free(dplist_node);
    }
    free(dplist);
	dplist = NULL;
}

/* Important note: to implement any list manipulation operator (insert, append,
 *delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special
 *pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special
 *pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default
 *pointer manipulation ALWAYS check that you implementation works correctly in
 *all these cases (check this on paper with list representation drawings!)
 **/

dplist_t* dpl_insert_at_index(dplist_t* list, element_t element, int index)
{
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL)
        return NULL;

    list_node = malloc(sizeof(dplist_node_t));

    list_node->element = element;
    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t* dpl_remove_at_index(dplist_t* list, int index)
{
    // TODO: add your code here
	int size = dpl_size(list);
	dplist_node_t* dummy = NULL;
	if(list != NULL && size != 0){
		dummy = list->head;
		if(index<=0){
			list->head = dummy->next;
			dummy->next->prev = NULL;
			free(dummy);
			dummy = NULL;
		}
		else{
			dummy = dpl_get_reference_at_index(list, index);
			assert(dummy != NULL);
			if(index >= size-1){
				dummy->prev->next = NULL;
				free(dummy);
				dummy = NULL;
			}
			else{
				dummy->prev->next = dummy->next;
				dummy->next->prev = dummy->prev;
				free(dummy);
				dummy = NULL;
			}
		}
	}

    return list;
}

int dpl_size(dplist_t* list)
{
    // TODO: add your code here
    int size = 0;
    if (list == NULL)
        size = -1;
    else {
        dplist_node_t* dplist_node = list->head;
        while (dplist_node != NULL) {
            dplist_node = dplist_node->next;
            size++;
        }
    }
    return size;
}

dplist_node_t* dpl_get_reference_at_index(dplist_t* list, int index)
{
    dplist_node_t* dummy = NULL;
	int size = dpl_size(list);

    // TODO: add your code here
	if(list != NULL){
		dummy = list->head;
		if(index > 0){
			for(int i=0; i<index && i<size-1; i++){
				dummy = dummy->next;
			}
		}
	}

    return dummy;
}


element_t dpl_get_element_at_index(dplist_t *list, int index){
    // TODO: add your code here
	element_t e = '\0';
	dplist_node_t* dummy = dpl_get_reference_at_index(list, index);
	if(dummy != NULL){
		e = dummy->element;
	}

    return e;
}

int dpl_get_index_of_element(dplist_t* list, element_t element)
{
    // TODO: add your code here
	int index = -1;
	dplist_node_t* node = NULL;
	if(list == NULL){
		return -1;
	}
	else{
		node = list->head;
		while(node != NULL){
			index++;
			if(node->element == element){
				return index;	
			}
			node = node->next;
		}
	}
	return -1;
}
