#include <stdio.h>
#include <stdlib.h>

int get_password(void) {
    FILE* fp = fopen("./input.txt", "r");
    if (!fp) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    int dial = 50;
    int password = 0;
    while (true) {
        char rotation_direction = '\0';
        int rotation_distance = 0;
        const int arg_count = fscanf(fp, "%c%d\r\n", &rotation_direction, &rotation_distance);
        if (arg_count == EOF)
            break;
        if (arg_count != 2) {
            fclose(fp);
            printf("Invalid file format");
            exit(EXIT_FAILURE);
        }
        switch (rotation_direction) {
            case 'L':
                dial -= rotation_distance;
                break;
            case 'R':
                dial += rotation_distance;
                break;
            default:
                fclose(fp);
                printf("Invalid file format");
                exit(EXIT_FAILURE);
        }
        if (dial < 0 || dial > 99)
            dial %= 100;
        if (dial == 0)
            ++password;
    }
    fclose(fp);
    return password;
}

int part_1() {
    return get_password();
}

int main(void) {
    printf("Part One: %d\n", part_1());
    return 0;
}