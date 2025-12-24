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
            const unsigned long id = *(unsigned long*) entry_ids->value;
            const unsigned long range_min = ((ID_Range*) entry_id_ranges->value)->min;
            const unsigned long range_max = ((ID_Range*) entry_id_ranges->value)->max;
            entry_id_ranges = entry_id_ranges->next;
            if (id >= range_min && id <= range_max) {
                ++valid_id_count;
                break;
            }
        }
        entry_ids = entry_ids->next;
    }
    return valid_id_count;
}

int comp_id_range(const void* a, const void* b) {
    const unsigned long arg_1 = (*(ID_Range**) a)->min;
    const unsigned long arg_2 = (*(ID_Range**) b)->min;
    if (arg_1 < arg_2) return -1;
    if (arg_1 > arg_2) return 1;
    return 0;
}

void qsort_id_ranges(Linked_List* list_id_ranges) {
    ID_Range* id_ranges[list_id_ranges->size];
    Linked_List_Entry* curr = list_id_ranges->head;
    for (size_t i = 0; i < list_id_ranges->size; ++i) {
        id_ranges[i] = (ID_Range*) curr->value;
        curr = curr->next;
    }
    qsort(id_ranges, list_id_ranges->size, sizeof(ID_Range*), comp_id_range);
    curr = list_id_ranges->head;
    for (size_t i = 0; i < list_id_ranges->size; ++i) {
        curr->value = id_ranges[i];
        curr = curr->next;
    }
}

void reduce_id_ranges(Linked_List* list_id_ranges) {
    qsort_id_ranges(list_id_ranges);
    Linked_List_Entry* curr = list_id_ranges->head;
    while (curr) {
        Linked_List_Entry* next = curr->next;
        unsigned long* curr_range_max = &((ID_Range*) curr->value)->max;
        while (next) {
            const unsigned long* next_range_min = &((ID_Range*) next->value)->min;
            const unsigned long* next_range_max = &((ID_Range*) next->value)->max;
            if (*curr_range_max >= *next_range_min) {
                if (*curr_range_max < *next_range_max) *curr_range_max = *next_range_max;
                curr->next = next->next;
                if (next->next) next->next->prev = curr;
                --list_id_ranges->size;
                free(next->value);
                free(next);
                next = curr->next;
                continue;
            }
            next = next->next;
        }
        curr = curr->next;
    }
}

unsigned long count_all_valid_ids(Linked_List* list_id_ranges) {
    unsigned long valid_id_count = 0;
    reduce_id_ranges(list_id_ranges);
    Linked_List_Entry* entry_id_ranges = list_id_ranges->head;
    while (entry_id_ranges) {
        unsigned long curr_range_min = ((ID_Range*) entry_id_ranges->value)->min;
        unsigned long curr_range_max = ((ID_Range*) entry_id_ranges->value)->max;
        valid_id_count += curr_range_max - curr_range_min + 1;
        entry_id_ranges = entry_id_ranges->next;
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

unsigned long part_2(void) {
    Linked_List* list_id_ranges = linked_list_init();
    Linked_List* list_ids = linked_list_init();
    parse_intput(list_id_ranges, list_ids);
    const unsigned long valid_id_count = count_all_valid_ids(list_id_ranges);
    linked_list_destroy_full(list_id_ranges, free);
    linked_list_destroy_full(list_ids, free);
    return valid_id_count;
}

int main(void) {
    printf("Part 1: %d\n", part_1());
    printf("Part 2: %lu\n", part_2());
    return 0;
}