#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int peek(FILE* fp) {
    const int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

int get_joltage(char* buffer_line) {
    int joltage = 0;
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

int get_total_joltage(void) {
    FILE* fp = fopen("./input.txt", "r");
    if (!fp) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    int total_joltage = 0;
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

int part_1(void) {
    return get_total_joltage();
}

int main(void) {
    printf("%d\n", part_1());
    return 0;
}
