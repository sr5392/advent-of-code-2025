#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size_x;
    size_t size_y;
    void** data;
} Grid;

void print_grid_beams(const Grid* grid) {
    for (size_t y = 0; y < grid->size_y; ++y) {
        for (size_t x = 0; x < grid->size_x; ++x) {
            printf("%c", ((char**) grid->data)[y][x]);
        }
        printf("\n");
    }
}

void print_grid_timelines(const Grid* grid) {
    for (size_t y = 0; y < grid->size_y; ++y) {
        for (size_t x = 0; x < grid->size_x; ++x) {
            printf("%lu", ((unsigned long**) grid->data)[y][x]);
        }
        printf("\n");
    }
}

Grid parse_input() {
    Grid grid = {0};
    FILE* fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Unable to open file");
        exit(EXIT_FAILURE);
    }
    size_t grid_size_x = 0;
    size_t grid_size_y = 0;
    size_t grid_index_x = 0;
    size_t grid_index_y = 0;
    int c = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == 'S' || c == '.' || c == '^') {
            if (grid.size_y == 0) {
                grid.data = malloc(sizeof(char*));
                if (!grid.data) {
                    fprintf(stderr, "Error allocating grid data");
                    exit(EXIT_FAILURE);
                }
                ++grid.size_y;
            }
            if (grid.size_x == 0) {
                grid.data[grid_index_y] = malloc(sizeof(char));
                if (!grid.data[grid_index_y]) {
                    fprintf(stderr, "Error allocating grid data");
                    exit(EXIT_FAILURE);
                }
                ++grid.size_x;
            }
            if (grid_index_x >= grid.size_x - 1) {
                const size_t new_grid_size_x = 2 * grid.size_x;
                for (size_t i = 0; i < grid.size_y; ++i) {
                    grid.data[i] = realloc(grid.data[i], new_grid_size_x * sizeof(char));
                    if (!grid.data[i]) {
                        fprintf(stderr, "Error reallocating grid data");
                        exit(EXIT_FAILURE);
                    }
                }
                grid.size_x = new_grid_size_x;
            }
            ((char**) grid.data)[grid_index_y][grid_index_x] = (char) c;
            ++grid_index_x;
            if (grid_index_x > grid_size_x) grid_size_x = grid_index_x;
            continue;
        }
        if (c == '\n') {
            if (grid.size_x == 0 || grid.size_y == 0) break;
            if (grid_index_y >= grid.size_y - 1) {
                const size_t new_grid_size_y = 2 * grid.size_y;
                grid.data = realloc(grid.data, new_grid_size_y * sizeof(char*));
                if (!grid.data) {
                    fprintf(stderr, "Error reallocating grid data");
                    exit(EXIT_FAILURE);
                }
                for (size_t i = grid_index_y + 1; i < new_grid_size_y; ++i) {
                    grid.data[i] = malloc(grid.size_x * sizeof(char));
                    if (!grid.data[i]) {
                        fprintf(stderr, "Error allocating grid data");
                        exit(EXIT_FAILURE);
                    }
                }
                grid.size_y = new_grid_size_y;
            }
            ++grid_index_y;
            if (grid_index_y > grid_size_y) grid_size_y = grid_index_y;
            grid_index_x = 0;
        }
    }
    grid.size_x = grid_size_x;
    grid.size_y = grid_size_y;
    fclose(fp);
    return grid;
}

void destroy_grid(Grid* grid) {
    for (size_t y = 0; y < grid->size_y; ++y) {
        free(grid->data[y]);
    }
    free(grid->data);
}

unsigned long count_beam_splits(Grid* grid) {
    unsigned long split_count = 0;
    char** grid_data = ((char**) grid->data);
    for (size_t y = 1; y < grid->size_y; ++y) {
        for (size_t x = 0; x < grid->size_x; ++x) {
            if (grid_data[y - 1][x] != 'S' && grid_data[y - 1][x] != '|') continue;
            if (grid_data[y][x] == '^') {
                if (x > 0) grid_data[y][x - 1] = '|';
                if (x < grid->size_x - 1) grid_data[y][x + 1] = '|';
                ++split_count;
                continue;
            }
            if (grid_data[y][x] == '.') {
                grid_data[y][x] = '|';
            }
        }
    }
    return split_count;
}

unsigned long count_beam_timelines(Grid* grid_beams) {
    unsigned long timeline_count = 0;
    Grid grid_timelines = {
        .size_x = grid_beams->size_x,
        .size_y = grid_beams->size_y,
        .data = NULL
    };
    grid_timelines.data = malloc(grid_timelines.size_y * sizeof(unsigned long*));
    if (!grid_timelines.data) {
        fprintf(stderr, "Error allocating timeline data");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < grid_beams->size_y; ++i) {
        grid_timelines.data[i] = calloc(grid_timelines.size_x, sizeof(unsigned long));
        if (!grid_timelines.data[i]) {
            fprintf(stderr, "Error allocating timeline data");
            exit(EXIT_FAILURE);
        }
    }
    char** grid_beams_data = ((char**) grid_beams->data);
    unsigned long** grid_timelines_data = ((unsigned long**) grid_timelines.data);
    for (size_t y = 1; y < grid_beams->size_y; ++y) {
        for (size_t x = 0; x < grid_beams->size_x; ++x) {
            if (grid_beams_data[y - 1][x] != 'S' && grid_beams_data[y - 1][x] != '|') continue;
            if (grid_beams_data[y - 1][x] == 'S') {
                grid_timelines_data[y - 1][x] += 1;
            }
            if (grid_beams_data[y][x] == '^') {
                if (x > 0) {
                    grid_beams_data[y][x - 1] = '|';
                    grid_timelines_data[y][x - 1] += grid_timelines_data[y - 1][x];
                }
                if (x < grid_beams->size_x - 1) {
                    grid_beams_data[y][x + 1] = '|';
                    grid_timelines_data[y][x + 1] += grid_timelines_data[y - 1][x];
                }
                continue;
            }
            if (grid_beams_data[y][x] == '.' || grid_beams_data[y][x] == '|') {
                grid_beams_data[y][x] = '|';
                grid_timelines_data[y][x] += grid_timelines_data[y - 1][x];
            }
        }
    }
    for (size_t i = 0; i < grid_timelines.size_x; ++i) {
        timeline_count += grid_timelines_data[grid_timelines.size_y - 1][i];
    }
    destroy_grid(&grid_timelines);
    return timeline_count;
}

unsigned long part_1(void) {
    Grid grid = parse_input();
    const unsigned long beam_split_count = count_beam_splits(&grid);
    destroy_grid(&grid);
    return beam_split_count;
}

unsigned long part_2(void) {
    Grid grid = parse_input();
    const unsigned long beam_timeline_count = count_beam_timelines(&grid);
    destroy_grid(&grid);
    return beam_timeline_count;
}

int main(void) {
    printf("%lu\n", part_1());
    printf("%lu\n", part_2());
    return 0;
}
