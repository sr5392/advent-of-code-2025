#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* data;
    size_t size_x;
    size_t size_y;
} Grid;

Grid init_grid(const size_t size_x, const size_t size_y) {
    Grid grid;
    grid.size_x = size_x;
    grid.size_y = size_y;
    grid.data = calloc(size_x * size_y, sizeof(char));
    if (!grid.data) {
        printf("Error allocating memory for grid");
        exit(EXIT_FAILURE);
    }
    return grid;
}

void free_grid(Grid* const grid) {
    free(grid->data);
}

int peek(FILE* const fp) {
    const int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

int count_adjacent_rolls(const Grid* const grid, const int pos_x, const int pos_y) {
    int adjacent_count = 0;
    for (int i = pos_y - 1; i <= pos_y + 1; ++i) {
        if (i < 0 || i >= grid->size_y) continue;
        for (int j = pos_x - 1; j <= pos_x + 1; ++j) {
            if (j < 0 || j >= grid->size_x) continue;
            if (i == pos_y && j == pos_x) continue;
            if (grid->data[i * grid->size_x + j] != '@' && grid->data[i * grid->size_x + j] != 'x') continue;
            ++adjacent_count;
        }
    }
    return adjacent_count;
}

Grid parse_input(void) {
    FILE* fp = fopen("./input.txt", "r");
    if (!fp) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
    }
    size_t line_length = 0;
    size_t grid_size_x = 0;
    size_t grid_size_y = 0;
    while (true) {
        const int c = fgetc(fp);
        if (c == EOF) break;
        if (c == '\r') continue;
        if (c == '\n') {
            if (grid_size_x != 0 && line_length != grid_size_x) {
                fclose(fp);
                printf("Invalid file format");
                exit(EXIT_FAILURE);
            }
            if (line_length > grid_size_x) grid_size_x = line_length;
            line_length = 0;
            ++grid_size_y;
            if (peek(fp) != '.' && peek(fp) != '@' && peek(fp) != EOF) {
                break;
            }
            continue;
        }
        if (c == '.' || c == '@') {
            ++line_length;
            continue;
        }
        fclose(fp);
        printf("Invalid file format");
        exit(EXIT_FAILURE);
    }
    rewind(fp);
    if (grid_size_x == 0 || grid_size_y == 0) {
        fclose(fp);
        printf("Invalid file format");
        exit(EXIT_FAILURE);
    }
    Grid grid = init_grid(grid_size_x, grid_size_y);
    size_t x = 0;
    size_t y = 0;
    while (true) {
        const int c = fgetc(fp);
        if (c == EOF) break;
        if (c == '\r') continue;
        if (c == '\n') {
            x = 0;
            ++y;
            continue;
        }
        if (c == '.' || c == '@') {
            grid.data[y * grid_size_x + x] = (char) c;
            ++x;
        }
    }
    fclose(fp);
    return grid;
}

int count_accessible_rolls(Grid* const grid) {
    int accessible_count = 0;
    for (int i = 0; i < grid->size_y; ++i) {
        for (int j = 0; j < grid->size_x; ++j) {
            if (grid->data[i * grid->size_x + j] == '.') continue;
            const int adjacent_count = count_adjacent_rolls(grid, j, i);
            if (adjacent_count < 4) ++accessible_count;
        }
    }
    return accessible_count;
}

void remove_accessible_rolls(Grid* const grid) {
    for (int i = 0; i < grid->size_y; ++i) {
        for (int j = 0; j < grid->size_x; ++j) {
            if (grid->data[i * grid->size_x + j] == 'x')
                grid->data[i * grid->size_x + j] = '.';
        }
    }
}

int count_accessible_rolls_2(Grid* const grid) {
    int accessible_count = 0;
    for (int i = 0; i < grid->size_y; ++i) {
        for (int j = 0; j < grid->size_x; ++j) {
            if (grid->data[i * grid->size_x + j] == '.') continue;
            const int adjacent_count = count_adjacent_rolls(grid, j, i);
            if (adjacent_count < 4) {
                ++accessible_count;
                grid->data[i * grid->size_x + j] = 'x';
            }
        }
    }
    remove_accessible_rolls(grid);
    if (accessible_count == 0) return accessible_count;
    return accessible_count + count_accessible_rolls_2(grid);
}

int part_1(void) {
    int accessible_count = 0;
    Grid grid = parse_input();
    accessible_count = count_accessible_rolls(&grid);
    free_grid(&grid);
    return accessible_count;
}

int part_2(void) {
    int accessible_count = 0;
    Grid grid = parse_input();
    accessible_count = count_accessible_rolls_2(&grid);
    free_grid(&grid);
    return accessible_count;
}

int main(void) {
    printf("Part 1: %d\n", part_1());
    printf("Part 2: %d\n", part_2());
    return 0;
}
