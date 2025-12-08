#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int peek(FILE* fp) {
    const int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

unsigned long get_joltage(const char* buffer_line) {
    unsigned long joltage = 0;
    const size_t line_length = strlen(buffer_line);
    int digit_1 = 0;
    int digit_2 = 0;
    const char* left_bound = buffer_line;
    for (size_t i = 0; i < line_length - 1; ++i) {
        const int tmp_digit_1 = buffer_line[i] - '0';
        if (tmp_digit_1 > digit_1) {
            digit_1 = tmp_digit_1;
            left_bound = &buffer_line[i];
        }
    }
    for (size_t i = left_bound - buffer_line + 1; i < line_length; ++i) {
        const int tmp_digit_2 = buffer_line[i] - '0';
        if (tmp_digit_2 > digit_2) {
            digit_2 = tmp_digit_2;
        }
    }
    joltage += 10 * digit_1 + digit_2;
    return joltage;
}

unsigned long get_joltage_2(const char* buffer_line) {
    unsigned long joltage = 0;
    const size_t line_length = strlen(buffer_line);
    size_t left_bound = 0;
    size_t right_bound = line_length - 11;
    for (int i = 11; i >= 0; --i) {
        int digit = 0;
        for (size_t j = left_bound; j < right_bound; ++j) {
            const int tmp_digit = buffer_line[j] - '0';
            if (tmp_digit > digit) {
                digit = tmp_digit;
                left_bound = j + 1;
            }
        }
        right_bound++;
        joltage += (unsigned long) pow(10, i) * digit;
    }
    return joltage;
}

unsigned long get_total_joltage(unsigned long (*get_joltage)(const char*)) {
    FILE* fp = fopen("./input.txt", "r");
    if (!fp) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    unsigned long total_joltage = 0;
    while (true) {
        size_t index = 0;
        char buffer_line[256] = {0};
        const int c = peek(fp);
        if (c == EOF) {
            if (feof(fp))
                break;
            if (ferror(fp)) {
                printf("Unable to read file");
                exit(EXIT_FAILURE);
            }
        }
        if (!isdigit(c)) {
            if (c == '\r' || c == '\n') {
                fgetc(fp);
                continue;
            }
            printf("Invalid file format");
            exit(EXIT_FAILURE);
        }
        while (isdigit(peek(fp))) {
            buffer_line[index] = (char) fgetc(fp);
            ++index;
            if (index >= sizeof(buffer_line) - 1) {
                printf("Buffer size exceeded!");
                exit(EXIT_FAILURE);
            }
        }
        total_joltage += get_joltage(buffer_line);
    }
    return total_joltage;
}

unsigned long part_1(void) {
    return get_total_joltage(get_joltage);
}

unsigned long part_2(void) {
    return get_total_joltage(get_joltage_2);
}

int main(void) {
    printf("%lu\n", part_1());
    printf("%lu\n", part_2());
    return 0;
}
