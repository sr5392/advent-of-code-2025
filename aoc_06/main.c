#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size_x;
    size_t size_y;
    unsigned long* data;
} Grid;

typedef struct {
    size_t size;
    char* data;
} Operators;

typedef struct {
    Grid grid;
    Operators operators;
} Input_Data;

Input_Data create_input_data(FILE* fp) {
    size_t grid_size_x = 0;
    size_t grid_size_y = 0;
    char line_buffer[4096] = {0};
    while (fgets(line_buffer, 4096, fp) != NULL) {
        ++grid_size_y;
    }
    const char* c = line_buffer;
    while (*c) {
        if (*c == '+' || *c == '*') ++grid_size_x;
        ++c;
    }
    Operators operators = {
        .size = grid_size_x,
        .data = malloc(grid_size_x)
    };
    rewind(fp);
    grid_size_y -= 1;
    Grid grid = {
        .size_x = grid_size_x,
        .size_y = grid_size_y,
        .data = malloc(grid_size_x * grid_size_y * sizeof(unsigned long))
    };
    Input_Data input_data = {
        .grid = grid,
        .operators = operators
    };
    return input_data;
}

void destroy_input_data(Input_Data* input_data) {
    free(input_data->grid.data);
    free(input_data->operators.data);
}

void fill_operators(Operators* operators, const char* line_buffer) {
    size_t i = 0;
    const char* c = line_buffer;
    while (*c) {
        if (i >= operators->size) break;
        if (*c == '+' || *c == '*') {
            operators->data[i] = *c;
            ++i;
        }
        ++c;
    }
}

void fill_input_data(FILE* fp, Input_Data* input_data) {
    char line_buffer[4096] = {0};
    size_t current_line = 0;
    while (fgets(line_buffer, 4096, fp) != NULL) {
        if (current_line == input_data->grid.size_y) {
            fill_operators(&input_data->operators, line_buffer);
            break;
        }
        const char* start = line_buffer;
        char* end = NULL;
        for (size_t i = 0; i < input_data->grid.size_x; ++i) {
            input_data->grid.data[input_data->grid.size_x * current_line + i] = strtoull(start, &end, 10);
            start = end;
        }
        ++current_line;
    }
}

void print_grid(const Grid* grid) {
    for (size_t y = 0; y < grid->size_y; ++y) {
        for (size_t x = 0; x < grid->size_x; ++x) {
            printf("%lu ", grid->data[grid->size_x * y + x]);
        }
        printf("\n");
    }
}

void print_operators(const Operators* operators) {
    for (size_t i = 0; i < operators->size; ++i) {
        printf("%c", operators->data[i]);
    }
    printf("\n");
}

Input_Data parse_input() {
    FILE* fp = fopen("input.txt", "r");
    Input_Data input_data = create_input_data(fp);
    fill_input_data(fp, &input_data);
    fclose(fp);
    return input_data;
}

unsigned long problem_total(const Input_Data* input_data) {
    unsigned long problem_total = 0;
    for (size_t x = 0; x < input_data->grid.size_x; ++x) {
        unsigned long problem = 0;
        for (size_t y = 0; y < input_data->grid.size_y; ++y) {
            if (input_data->operators.data[x] == '+')
                problem += input_data->grid.data[input_data->grid.size_x * y + x];
            if (input_data->operators.data[x] == '*') {
                if (problem == 0) problem = 1;
                problem *= input_data->grid.data[input_data->grid.size_x * y + x];
            }
        }
        problem_total += problem;
    }
    return problem_total;
}

unsigned long part_1(void) {
    Input_Data input_data = parse_input();
    const unsigned long total = problem_total(&input_data);
    destroy_input_data(&input_data);
    return total;
}

int main(void) {
    printf("Part 1: %lu\n", part_1());
    return 0;
}
