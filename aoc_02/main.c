#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_valid_id(const unsigned long id) {
    char buffer[16] = {0};
    snprintf(buffer, sizeof(buffer), "%lu", id);
    const size_t id_length = strlen(buffer);
    if ((id_length % 2) != 0)
        return true;
    char* buffer_begin = &buffer[0];
    char* buffer_mid = &buffer[(id_length / 2)];
    char* buffer_end = &buffer[id_length - 1] + 1;
    while (buffer_mid != buffer_end) {
        if (*buffer_begin != *buffer_mid)
            return true;
        ++buffer_begin;
        ++buffer_mid;
    }
    return false;
}

unsigned long sum_invalid_ids(void) {
    FILE* fp = fopen("./input.txt", "r");
    if (!fp) {
        exit(EXIT_FAILURE);
    }
    unsigned long sum = 0;
    while (true) {
        unsigned long id_range_begin = 0;
        unsigned long id_range_end = 0;
        int arg_count = 0;
        arg_count = fscanf(fp, "%lu-%lu,", &id_range_begin, &id_range_end);
        if (arg_count == EOF)
            break;
        if (arg_count != 2) {
            exit(EXIT_FAILURE);
        }
        for (unsigned long i = id_range_begin; i <= id_range_end; ++i) {
            if (!is_valid_id(i))
                sum += i;
        }
    }
    fclose(fp);
    return sum;
}

unsigned long part_1(void) {
    return sum_invalid_ids();
}

int main(void) {
    printf("Sum of invalid IDs: %lu\n", part_1());
    return 0;
}