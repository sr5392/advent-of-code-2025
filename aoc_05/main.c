#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct {
    unsigned long min;
    unsigned long max;
} ID_Range;

void parse_intput(Linked_List* list_id_ranges, Linked_List* list_ids) {
    FILE* fp = fopen("input_test.txt", "r");
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
    for (size_t i = 0; i < list_id_ranges->size; ++i) {
        printf("%lu-%lu\n", ((ID_Range*) linked_list_get_item(list_id_ranges, i))->min, ((ID_Range*) linked_list_get_item(list_id_ranges, i))->max);
    }
    for (size_t i = 0; i < list_ids->size; ++i) {
        printf("%lu\n", *(unsigned long*) linked_list_get_item(list_ids, i));
    }
}

int part_1(void) {
    Linked_List* list_id_ranges = linked_list_init();
    Linked_List* list_ids = linked_list_init();

    parse_intput(list_id_ranges, list_ids);
    print_lists(list_id_ranges, list_ids);

    linked_list_destroy_full(list_id_ranges, free);
    linked_list_destroy_full(list_ids, free);
    return 0;
}

int main(void) {
    part_1();
    return 0;
}