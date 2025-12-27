#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    FILE* fp = fopen("input_test.txt", "r");
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

int part_1(void) {
    Junction_Boxes jbs = parse_input();
    print_junction_boxes(jbs);
    printf("%f\n", get_distance(jbs.boxes[0], jbs.boxes[7]));
    free(jbs.boxes);
    return 0;
}

int main(void) {
    printf("%d\n", part_1());
    return 0;
}
