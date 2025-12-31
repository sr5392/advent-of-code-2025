#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t pos_x;
    size_t pos_y;
} Tile;

typedef struct {
    size_t size;
    size_t capacity;
    Tile* data;
} Tiles;

Tiles tiles_init() {
    Tiles tiles = {0};
    tiles.size = 0;
    tiles.capacity = 1;
    tiles.data = malloc(tiles.capacity * sizeof(Tile));
    if (!tiles.data) {
        fprintf(stderr, "Error allocating tiles");
        exit(EXIT_FAILURE);
    }
    return tiles;
}

void tiles_destroy(Tiles* tiles) {
    free(tiles->data);
}

void tiles_append(Tiles* tiles, const Tile* tile, const size_t index) {
    if (tiles->capacity <= index) {
        tiles->capacity *= 2;
        Tile* new_data = realloc(tiles->data, tiles->capacity * sizeof(Tile));
        if (!new_data) {
            free(tiles->data);
            fprintf(stderr, "Error reallocating tiles");
            exit(EXIT_FAILURE);
        }
        tiles->data = new_data;
    }
    tiles->data[index] = *tile;
    ++tiles->size;
}

void tiles_print(const Tiles* tiles) {
    for (size_t i = 0; i < tiles->size; ++i) {
        printf("x: %zu, y: %zu\n", tiles->data[i].pos_x, tiles->data[i].pos_y);
    }
}

Tiles parse_input(void) {
    FILE* fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Unable to open file");
        exit(EXIT_FAILURE);
    }
    size_t tile_index = 0;
    Tiles tiles = tiles_init();
    while (true) {
        Tile tile = {0};
        int ret = fscanf(fp, "%zu,%zu", &tile.pos_x, &tile.pos_y);
        if (ret == EOF) {
            break;
        }
        if (ret != 2) {
            break;
        }
        tiles_append(&tiles, &tile, tile_index);
        ++tile_index;
    }
    fclose(fp);
    return tiles;
}

unsigned long get_largest_tile_area(Tiles* tiles) {
    size_t largest_tile_area = 0;
    for (size_t i = 0; i < tiles->size; ++i) {
        for (size_t j = 0; j < tiles->size; ++j) {
            const Tile tile_a = tiles->data[i];
            const Tile tile_b = tiles->data[j];
            size_t dx = 1;
            size_t dy = 1;
            if (tile_a.pos_x > tile_b.pos_x)
                dx = tile_a.pos_x - tile_b.pos_x + 1;
            if (tile_a.pos_x < tile_b.pos_x)
                dx = tile_b.pos_x - tile_a.pos_x + 1;
            if (tile_a.pos_y > tile_b.pos_y)
                dy = tile_a.pos_y - tile_b.pos_y + 1;
            if (tile_a.pos_y < tile_b.pos_y)
                dy = tile_b.pos_y - tile_a.pos_y + 1;
            size_t tile_area = dx * dy;
            if (tile_area > largest_tile_area) largest_tile_area = tile_area;
        }
    }
    return largest_tile_area;
}

unsigned long part_1() {
    Tiles tiles = parse_input();
    const unsigned long tile_area = get_largest_tile_area(&tiles);
    tiles_destroy(&tiles);
    return tile_area;
}

int main(void) {
    printf("%lu", part_1());
    return 0;
}
