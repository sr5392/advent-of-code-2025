#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size_x;
    size_t size_y;
    char** data;
} Grid;

void print_grid(const Grid* grid) {
    for (size_t y = 0; y < grid->size_y; ++y) {
        for (size_t x= 0; x < grid->size_x; ++x) {
            printf("%c", grid->data[y][x]);
        }
        printf("\n");
    }
}

Grid parse_input() {
    Grid grid = {0};
    FILE* fp = fopen("input_test.txt", "r");
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
            grid.data[grid_index_y][grid_index_x] = (char) c;
            ++grid_index_x;
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
            grid_index_x = 0;
        }
        if (grid_index_x > grid_size_x) grid_size_x = grid_index_x;
        if (grid_index_y > grid_size_y) grid_size_y = grid_index_y;
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

int main(void) {
    Grid grid = parse_input();
    print_grid(&grid);
    destroy_grid(&grid);
    return 0;
}