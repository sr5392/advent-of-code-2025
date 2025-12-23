#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct {
    unsigned long min;
    unsigned long max;
} ID_Range;

void parse_intput(Linked_List* list_id_ranges, Linked_List* list_ids) {
    FILE* fp = fopen("input.txt", "r");
    fpos_t fpos;
    while (true) {
        ID_Range* id_range = malloc(sizeof(ID_Range));
        int arg_count = fscanf(fp, "%lu-%lu", &id_range->min, &id_range->max);
        if (arg_count != 2) {
            free(id_range);
            fsetpos(fp, &fpos);
            break;
        }
        linked_list_append_item(list_id_ranges, id_range);
        fgetpos(fp, &fpos);
    }
    while (true) {
        unsigned long* id = malloc(sizeof(unsigned long));
        int arg_count = fscanf(fp, "%lu", id);
        if (arg_count != 1) {
            free(id);
            break;
        }
        linked_list_append_item(list_ids, id);
    }
    fclose(fp);
}

void print_lists(const Linked_List* list_id_ranges, const Linked_List* list_ids) {
    Linked_List_Entry* entry = list_id_ranges->head;
    while (entry) {
        printf("%lu-%lu\n", ((ID_Range*) entry->value)->min, ((ID_Range*) entry->value)->max);
        entry = entry->next;
    }
    entry = list_ids->head;
    while (entry) {
        printf("%lu\n", *(unsigned long*) entry->value);
        entry = entry->next;
    }
}

int count_valid_ids(const Linked_List* list_id_ranges, const Linked_List* list_ids) {
    int valid_id_count = 0;
    Linked_List_Entry* entry_ids = list_ids->head;
    while (entry_ids) {
        Linked_List_Entry* entry_id_ranges = list_id_ranges->head;
        while (entry_id_ranges) {
            unsigned long id = *(unsigned long*) entry_ids->value;
            unsigned long range_min = ((ID_Range*) entry_id_ranges->value)->min;
            unsigned long range_max = ((ID_Range*) entry_id_ranges->value)->max;
            entry_id_ranges = entry_id_ranges->next;
            if (id >= range_min && id <= range_max) {
                ++valid_id_count;
                entry_id_ranges = NULL;
            }
        }
        entry_ids = entry_ids->next;
    }
    return valid_id_count;
}

int part_1(void) {
    Linked_List* list_id_ranges = linked_list_init();
    Linked_List* list_ids = linked_list_init();
    parse_intput(list_id_ranges, list_ids);
    const int valid_id_count = count_valid_ids(list_id_ranges, list_ids);
    linked_list_destroy_full(list_id_ranges, free);
    linked_list_destroy_full(list_ids, free);
    return valid_id_count;
}

int main(void) {
    printf("Part 1: %d\n", part_1());
    return 0;
}