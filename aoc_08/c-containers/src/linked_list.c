#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

Linked_List* linked_list_init(void) {
    Linked_List* list = malloc(sizeof(Linked_List));
    if (!list) {
        fprintf(stderr, "Error allocating memory for list");
        exit(EXIT_FAILURE);
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

static Linked_List_Entry* linked_list_create_entry(void* item) {
    assert(item);
    Linked_List_Entry* entry = malloc(sizeof(Linked_List_Entry));
    if (!entry) {
        fprintf(stderr, "Error allocating memory for list entry");
        exit(EXIT_FAILURE);
    }
    entry->next = NULL;
    entry->prev = NULL;
    entry->value = item;
    return entry;
}

void linked_list_append_item(Linked_List* list, void* item) {
    assert(list && item);
    Linked_List_Entry* entry = linked_list_create_entry(item);
    if (list->head) {
        list->tail->next = entry;
        entry->prev = list->tail;
        list->tail = entry;
    } else {
        list->head = entry;
        list->tail = entry;
    }
    ++list->size;
}

static Linked_List_Entry* linked_list_get_entry(const Linked_List* list, size_t index) {
    assert(list);
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds");
        exit(EXIT_FAILURE);
    }
    Linked_List_Entry* entry = NULL;
    if (index <= list->size / 2) {
        entry = list->head;
        for (size_t i = 0; i < index; ++i) {
            entry = entry->next;
        }
    } else {
        entry = list->tail;
        for (size_t i = 0; i < list->size - 1 - index; ++i) {
            entry = entry->prev;
        }
    }
    return entry;
}

void* linked_list_get_item(const Linked_List* list, size_t index) {
    return linked_list_get_entry(list, index)->value;
}

void linked_list_set_item(Linked_List* list, size_t index, void* item) {
    Linked_List_Entry* entry = linked_list_get_entry(list, index);
    entry->value = item;
}

void linked_list_remove_item(Linked_List* list, size_t index) {
    assert(list);
    Linked_List_Entry* entry = linked_list_get_entry(list, index);
    if (entry->prev)
        entry->prev->next = entry->next;
    else
        list->head = entry->next;
    if (entry->next) entry->next->prev = entry->prev;
    if (index == list->size - 1) list->tail = entry->prev;
    free(entry);
    --list->size;
}

void linked_list_destroy(Linked_List* list) {
    assert(list);
    Linked_List_Entry* curr = list->head;
    while (curr) {
        Linked_List_Entry* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

void linked_list_destroy_full(Linked_List* list, free_func f) {
    assert(list);
    Linked_List_Entry* curr = list->head;
    while (curr) {
        Linked_List_Entry* next = curr->next;
        f(curr->value);
        free(curr);
        curr = next;
    }
    free(list);
}
