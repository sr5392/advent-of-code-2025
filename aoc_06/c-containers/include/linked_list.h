#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stddef.h>

typedef void (*free_func)(void* ptr);

typedef struct Linked_List_Entry {
    void* value;
    struct Linked_List_Entry* next;
    struct Linked_List_Entry* prev;
} Linked_List_Entry;

typedef struct {
    size_t size;
    Linked_List_Entry* head;
    Linked_List_Entry* tail;
} Linked_List;

Linked_List* linked_list_init(void);
void linked_list_append_item(Linked_List* list, void* item);
void* linked_list_get_item(const Linked_List* list, size_t index);
void linked_list_set_item(Linked_List* list, size_t index, void* item);
void linked_list_remove_item(Linked_List* list, size_t index);
void linked_list_destroy(Linked_List* list);
void linked_list_destroy_full(Linked_List* list, free_func f);
#endif
