#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "linked_list.h"

typedef struct {
    size_t pos_x;
    size_t pos_y;
    size_t pos_z;
} Junction_Box;

typedef struct {
    size_t size;
    Junction_Box* boxes;
} Junction_Boxes;

void print_junction_box(const Junction_Box jb) {
    printf("x: %zu, y: %zu, z: %zu\n", jb.pos_x, jb.pos_y, jb.pos_z);
}

void print_junction_boxes(const Junction_Boxes jbs) {
    for (size_t i = 0; i < jbs.size; ++i) {
        print_junction_box(jbs.boxes[i]);
    }
}

double get_distance(const Junction_Box a, const Junction_Box b) {
    const double dx = (double) a.pos_x - (double) b.pos_x;
    const double dy = (double) a.pos_y - (double) b.pos_y;
    const double dz = (double) a.pos_z - (double) b.pos_z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Junction_Boxes parse_input() {
    FILE* fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Unable to open file");
        exit(EXIT_FAILURE);
    }

    Junction_Boxes jbs = {0};
    size_t jbs_capacity = 0;
    while (true) {
        size_t pos_x = 0;
        size_t pos_y = 0;
        size_t pos_z = 0;
        const int ret = fscanf(fp, "%zu,%zu,%zu", &pos_x, &pos_y, &pos_z);
        if (ret == EOF) break;
        if (ret != 3) {
            fprintf(stderr, "Invalid file format");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        if (jbs_capacity == 0) {
            jbs.boxes = malloc(sizeof(Junction_Box));
            if (!jbs.boxes) {
                fprintf(stderr, "Error allocating junction boxes");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            jbs_capacity = 1;
        }
        if (jbs.size >= jbs_capacity) {
            jbs_capacity = 2 * jbs_capacity;
            Junction_Box* tmp_jbs = realloc(jbs.boxes, jbs_capacity * sizeof(Junction_Box));;
            if (!tmp_jbs) {
                fprintf(stderr, "Error allocating junction boxes");
                free(jbs.boxes);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            jbs.boxes = tmp_jbs;
        }
        const Junction_Box jb = {
            .pos_x = pos_x,
            .pos_y = pos_y,
            .pos_z = pos_z
        };
        jbs.boxes[jbs.size] = jb;
        ++jbs.size;
    }
    fclose(fp);
    return jbs;
}

typedef struct {
    size_t idx_junction_box_a;
    size_t idx_junction_box_b;
    double distance;
} Junction_Box_Distance;

typedef struct {
    size_t size;
    Junction_Box_Distance* jbds;
} Junction_Box_Distances;


void print_junctions_box_distances(const Junction_Box_Distances jbds) {
    for (size_t i = 0; i < jbds.size; ++i) {
        printf("[%zu][%zu]%f\n",
               jbds.jbds[i].idx_junction_box_a, jbds.jbds[i].idx_junction_box_b, jbds.jbds[i].distance
        );
    }
}

int comp_junction_box_distance(const void* a, const void* b) {
    if (((Junction_Box_Distance*) a)->distance > ((Junction_Box_Distance*) b)->distance) return 1;
    if (((Junction_Box_Distance*) a)->distance < ((Junction_Box_Distance*) b)->distance) return -1;
    return 0;
}

Junction_Box_Distances map_junction_box_distances(const Junction_Boxes jbs) {
    Junction_Box_Distances jbds = {0};
    jbds.size = jbs.size * (jbs.size - 1) / 2;
    jbds.jbds = malloc(jbds.size * sizeof(Junction_Box_Distance));
    if (!jbds.jbds) {
        fprintf(stderr, "Invalid file format");
        exit(EXIT_FAILURE);
    }
    size_t index_jbds = 0;
    for (size_t i = 0; i < jbs.size; ++i) {
        for (size_t j = i + 1; j < jbs.size; ++j) {
            Junction_Box_Distance jbd = {
                .idx_junction_box_a = i,
                .idx_junction_box_b = j,
                .distance = get_distance(jbs.boxes[i], jbs.boxes[j])
            };
            jbds.jbds[index_jbds] = jbd;
            ++index_jbds;
        }
    }
    qsort(jbds.jbds, jbds.size, sizeof(Junction_Box_Distance), comp_junction_box_distance);
    return jbds;
}

typedef struct {
    size_t size;
    Linked_List** circuits;
} Circuits;

int comp_circuits(const void* a, const void* b) {
    if ((*(Linked_List**) a)->size < (*(Linked_List**) b)->size) return 1;
    if ((*(Linked_List**) a)->size > (*(Linked_List**) b)->size) return -1;
    return 0;
}
typedef struct {
    Junction_Box jb;
    size_t crct_idx;
} Junction_Box_Circuit;

typedef struct {
    size_t size;
    Junction_Box_Circuit* jbcrcts;
} Junction_Box_Circuits;

Circuits map_circuits(const Junction_Boxes jbs, Junction_Box_Distances jbds) {
    Junction_Box_Circuits jbcrcts = {0};
    jbcrcts.size = jbs.size;
    jbcrcts.jbcrcts = calloc(jbcrcts.size, sizeof(Junction_Box_Circuit));
    if (!jbcrcts.jbcrcts) {
        fprintf(stderr, "Error allocating junction box circuits");
        exit(EXIT_FAILURE);
    }
    Linked_List* crcts = linked_list_init();
    size_t crcts_idx = 1;
    for (size_t i = 0; i < 1000; ++i) {
        const size_t idx_jbcrct_a = jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_a].crct_idx;
        const size_t idx_jbcrct_b = jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_b].crct_idx;
        if (idx_jbcrct_a == 0 && idx_jbcrct_b == 0) {
            Linked_List* crct = linked_list_init();
            linked_list_append_item(crct, &jbs.boxes[jbds.jbds[i].idx_junction_box_a]);
            linked_list_append_item(crct, &jbs.boxes[jbds.jbds[i].idx_junction_box_b]);
            jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_a].crct_idx = crcts_idx;
            jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_b].crct_idx = crcts_idx;
            linked_list_append_item(crcts, crct);
            ++crcts_idx;
            continue;
        }
        if (idx_jbcrct_a != 0 && idx_jbcrct_b == 0) {
            linked_list_append_item(linked_list_get_item(crcts, idx_jbcrct_a - 1), &jbs.boxes[jbds.jbds[i].idx_junction_box_b]);
            jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_b].crct_idx = jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_a].crct_idx;
            continue;
        }
        if (idx_jbcrct_a == 0 && idx_jbcrct_b != 0) {
            linked_list_append_item(linked_list_get_item(crcts,idx_jbcrct_b - 1), &jbs.boxes[jbds.jbds[i].idx_junction_box_a]);
            jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_a].crct_idx = jbcrcts.jbcrcts[jbds.jbds[i].idx_junction_box_b].crct_idx;
            continue;
        }
        if (idx_jbcrct_a != 0 && idx_jbcrct_b != 0) {
            if (idx_jbcrct_a == idx_jbcrct_b) continue;
            Linked_List* list_a;
            Linked_List* list_b;
            size_t new_crct_index;
            size_t old_crct_index;
            if (idx_jbcrct_a > idx_jbcrct_b) {
                list_a = linked_list_get_item(crcts, idx_jbcrct_b - 1);
                list_b = linked_list_get_item(crcts, idx_jbcrct_a - 1);
                old_crct_index = idx_jbcrct_a;
                new_crct_index = idx_jbcrct_b;
            } else {
                list_a = linked_list_get_item(crcts, idx_jbcrct_a - 1);
                list_b = linked_list_get_item(crcts, idx_jbcrct_b - 1);
                old_crct_index = idx_jbcrct_b;
                new_crct_index = idx_jbcrct_a;
            }
            Linked_List_Entry* curr = list_b->head;
            while (curr) {
                linked_list_append_item(list_a, curr->value);
                curr = curr->next;
            }
            linked_list_remove_item(crcts, old_crct_index - 1);
            for (size_t j = 0; j < jbcrcts.size; ++j) {
                if (jbcrcts.jbcrcts[j].crct_idx == old_crct_index) {
                    jbcrcts.jbcrcts[j].crct_idx = new_crct_index;
                    continue;
                }
                if (jbcrcts.jbcrcts[j].crct_idx > old_crct_index)
                    jbcrcts.jbcrcts[j].crct_idx = jbcrcts.jbcrcts[j].crct_idx - 1;
            }
            --crcts_idx;
        }
    }
    Circuits ret_crcts = {0};
    ret_crcts.size = crcts->size;
    ret_crcts.circuits = calloc(ret_crcts.size, sizeof(Linked_List*));
    if (!ret_crcts.circuits) {
        fprintf(stderr, "Error allocating circuits array");
        exit(EXIT_FAILURE);
    }
    Linked_List_Entry* curr = crcts->head;
    size_t i = 0;
    while (curr) {
        ret_crcts.circuits[i] = curr->value;
        curr = curr->next;
        ++i;
    }
    qsort(ret_crcts.circuits, ret_crcts.size, sizeof(Linked_List*), comp_circuits);
    free(crcts);
    free(jbcrcts.jbcrcts);
    return ret_crcts;
}

void print_circuits(Circuits crcts) {
    for (int i = 0; i < crcts.size; ++i) {
        Linked_List_Entry* curr = crcts.circuits[i]->head;
        while (curr) {
            print_junction_box(*(Junction_Box*) curr->value);
            curr = curr->next;
        }
        printf("------\n");
    }
}

void destroy_circuits(Circuits crcts) {
    for (int i = 0; i < crcts.size; ++i) {
        linked_list_destroy(crcts.circuits[i]);
    }
    free(crcts.circuits);
}

unsigned long get_circuit_size(Circuits crcts) {
    size_t crct_size = 1;
    for (size_t i = 0; i < 3; ++i) {
        if (i < crcts.size) crct_size *= crcts.circuits[i]->size;
    }
    return crct_size;
}

int part_1(void) {
    Junction_Boxes jbs = parse_input();
    Junction_Box_Distances jbds = map_junction_box_distances(jbs);
    Circuits crcts = map_circuits(jbs, jbds);
    const unsigned long crct_size = get_circuit_size(crcts);
    destroy_circuits(crcts);
    free(jbds.jbds);
    free(jbs.boxes);
    return crct_size;
}

int main(void) {
    printf("%d\n", part_1());
    return 0;
}
