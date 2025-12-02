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

bool is_valid_id_2(const unsigned long id) {
    char buffer[16] = {0};
    snprintf(buffer, sizeof(buffer), "%lu", id);
    const size_t id_length = strlen(buffer);
    if (id_length < 2)
        return true;
    char buffer_search[16] = {0};
    size_t search_length = 0;
    bool invalid = false;
    for (size_t i = 0; i < (id_length / 2); ++i) {
        strncpy(buffer_search, buffer, i + 1);
        search_length = i + 1;
        if (id_length % search_length != 0) {
            continue;
        }
        size_t k = 0;
        bool match = true;
        for (size_t j = 0; j < id_length; ++j) {
            if (buffer[j] != buffer_search[k]) {
                match = false;
                break;
            }
            k = (k + 1) % search_length;
        }
        if (match) {
            invalid = match;
            break;
        }
    }
    return !invalid;
}

unsigned long sum_invalid_ids(bool (*is_valid_id)(unsigned long)) {
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
    return sum_invalid_ids(is_valid_id);
}

unsigned long part_2(void) {
    return sum_invalid_ids(is_valid_id_2);
}

int main(void) {
    printf("Sum of invalid IDs: %lu\n", part_1());
    printf("Sum of invalid IDs: %lu\n", part_2());
    return EXIT_SUCCESS;
}