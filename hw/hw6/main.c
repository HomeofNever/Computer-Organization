#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 256
#define MAX_ITEM 1024
#define MAX_INT_SIZE 30
#define STR_HIT "(hit)"
#define STR_MISS "(miss)"

// Types
struct array {
    int * array;
    int num;
};

// Globals
struct array * cache;
int set_num = 0;

int cache_index(int target) {
    return target % set_num;
}

// Find an element in the array
// Return -1 if no found
int find(struct array * arr, int target) {
    for (int i = 0; i < arr->num; i++) {
        if (arr->array[i] == target) {
            return i;
        }
    }

    return -1;
}

void push_lru(struct array * arr, int target) {
    int fd = find(arr, target);
    if (fd != -1) {
        // If found, swap
        for (int i = fd; i < arr->num - 1; i++) {
            // Swap
            int temp = arr->array[i + 1];
            arr->array[i + 1] = arr->array[i];
            arr->array[i] = temp;
        }
    } else {
        if (arr->num < set_num) {
            arr->array[arr->num] = target;
            arr->num++;
        } else {
            arr->array[0] = target;
        }
    }
}

bool access_lru(int target) {
    int index = cache_index(target);
    int fd = find(&cache[index], target);
    push_lru(&cache[index], target);
    return fd != -1;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "ERR: Missing Parameters.");
        return EXIT_FAILURE;
    }

    set_num = atoi(argv[1]);
    if (!(set_num == 1 || set_num == 2 || set_num == 4)) {
        fprintf(stderr, "ERR: Set number should be one of the following: 1, 2, 4. Find %s", argv[1]);
        return EXIT_FAILURE;
    }

    int mode = -1;
    if (strcmp(argv[2], "LRU") == 0) {
        mode = 1;
    } else if (strcmp(argv[2], "Belady") == 0) {
        mode = 2;
    } else {
        fprintf(stderr, "ERRL Mode should be one of the following: LRU or Belady. Find %s", argv[2]);
        return EXIT_FAILURE;
    }

    // Open input file
    FILE *file;
    file = fopen(argv[3], "r");

    // check if the file can be opened
    if (!file) {
        fprintf(stderr, "ERR: Cannot open file: %s.", argv[3]);
        return EXIT_FAILURE;
    }

    // Print Basic
    printf("Cache size: %d\n", MAX_SIZE);
    printf("Cache associativity: %d\n", set_num);
    int set = MAX_SIZE / set_num;
    printf("Cache sets: %d\n", set);
    printf("Cache algorithm: %s\n", argv[2]);

    // Init
    int mem_address[MAX_ITEM];
    for (int i = 0; i < MAX_ITEM; i++) {
        mem_address[i] = 0;
    }

    // Read file
    int i = 0;
    char line[50];
    while (fgets(line, MAX_INT_SIZE, file)) {
        // Remove the last newline
        line[strcspn(line, "\n")] = '\0';
        mem_address[i] = atoi(line);
        i++;
    }


    // Init Iteration
    fclose(file);
    int item_num = i;
    int hit_num = 0;
    // Init data structure based on input
    cache = calloc(set, sizeof(struct array));
    for (int i = 0; i < set; i++) {
        cache->array = calloc(set_num, sizeof(int));
        cache->num = 0;
    }

    if (mode == 1) {
        for (i = 0; i < item_num; i++) {
            if (access_lru(mem_address[i])) {
                printf("%d %s\n", mem_address[i], STR_HIT);
                hit_num++;
            } else {
                printf("%d %s\n", mem_address[i], STR_MISS);
            }
        }
    } else if (mode == 2) {

    } else {
        fprintf(stderr, "You should not be here!");
    }

    // print Summary
    printf("Cache accesses: %d\n", item_num);
    printf("Cache hits: %d\n", hit_num);
    printf("Cache misses: %d\n", item_num - hit_num);
    float hit_rate = (float)hit_num / (float)item_num;
    printf("Overall hit rate: %f\n", hit_rate);

    return EXIT_SUCCESS;
}
