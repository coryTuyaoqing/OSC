#include "dplist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void* element;
};

struct dplist {
    dplist_node_t* head;

    void* (*element_copy)(void* src_element);

    void (*element_free)(void** element);

    int (*element_compare)(void* x, void* y);
};

dplist_t* dpl_create( // callback functions
    void* (*element_copy)(void* src_element),
    void (*element_free)(void** element),
    int (*element_compare)(void* x, void* y))
{
    dplist_t* list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t** list, bool free_element)
{
    dplist_t* dplist = *list;
    if (dplist != NULL) {
        dplist_node_t* dplist_node = dplist->head;
        if (dplist_node != NULL) {
            while (dplist_node->next != NULL) {
                dplist_node = dplist_node->next;
                if (free_element == true) {
                    dplist->element_free(&(dplist_node->prev->element));
                }
                free(dplist_node->prev);
            }
            if (free_element == true) {
                dplist->element_free(&dplist_node->element);
            }
            free(dplist_node);
        }
        free(*list);
        *list = NULL;
    }
}

dplist_t* dpl_insert_at_index(dplist_t* list, void* element, int index, bool insert_copy)
{
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) {
        return list;
    }
    list_node = malloc(sizeof(dplist_node_t));
    if (insert_copy == true) {
        list_node->element = list->element_copy(element);
    } else {
        list_node->element = element;
    }

    if (list->head == NULL) { // if the list is empty, add the element as the first node
        list->head = list_node;
        list_node->prev = NULL;
        list_node->next = NULL;
    } else if (index <= 0) { // if the index<=0, add the element to the head of the list
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        if (index < dpl_size(list)) {
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
        } else {
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
        }
    }

    return list;
}

dplist_t* dpl_remove_at_index(dplist_t* list, int index, bool free_element)
{
    int size = dpl_size(list);
    dplist_node_t* dummy = NULL;
    if (list != NULL && size != 0) {
        dummy = list->head;
        if (index <= 0) {
            list->head = dummy->next;
            if(dummy->next != NULL){
                dummy->next->prev = NULL;
            }
            if (free_element == true) {
                list->element_free(&dummy->element);
            }
            free(dummy);
            dummy = NULL;
        } else {
            dummy = dpl_get_reference_at_index(list, index);
            assert(dummy != NULL);
            if (index >= size - 1) {
                dummy->prev->next = NULL;
                if (free_element == true) {
                    list->element_free(&dummy->element);
                }
                free(dummy);
                dummy = NULL;
            } else {
                dummy->prev->next = dummy->next;
                dummy->next->prev = dummy->prev;
                if (free_element == true) {
                    list->element_free(&dummy->element);
                }
                free(dummy);
                dummy = NULL;
            }
        }
    }

    return list;
}

int dpl_size(dplist_t* list)
{
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

void* dpl_get_element_at_index(dplist_t* list, int index)
{
    void* element = NULL;
    dplist_node_t* dummy = dpl_get_reference_at_index(list, index);
    if (dummy != NULL) {
        element = dummy->element;
    }

    return element;
}

int dpl_get_index_of_element(dplist_t* list, void* element)
{
    if (list == NULL) {
        return -1;
    }
    int index = -1;
    dplist_node_t* node = list->head;
    while (node != NULL) {
        index++;
        if (list->element_compare(node->element, element) == 0) {
            break;
        }
        node = node->next;
    }

    return index;
}

dplist_node_t* dpl_get_reference_at_index(dplist_t* list, int index)
{
    dplist_node_t* dummy = NULL;
    int size = dpl_size(list);

    if (list != NULL) {
        dummy = list->head;
        if (index > 0) {
            for (int i = 0; i < index && i < size - 1; i++) {
                dummy = dummy->next;
            }
        }
    }

    return dummy;
}

void* dpl_get_element_at_reference(dplist_t* list, dplist_node_t* reference)
{
    if (list == NULL) {
        return NULL;
    } else if (list->head == NULL) {
        return NULL;
    }
    int index = dpl_get_index_of_element(list, reference);
    if (index == -1) {
        return NULL;
    }
    return reference->element;
}

dplist_node_t* dpl_get_head(dplist_t* list)
{
    if(list == NULL || list->head == NULL)
        return NULL;
    else
        return list->head;
}
