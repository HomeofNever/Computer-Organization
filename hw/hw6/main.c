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
    int *array;
    int num;
};

// Globals
struct array *cache;
int set = -1;
int mode;
int set_num;
int mem_address[MAX_ITEM];
int item_num;

int cache_index(int target) {
  return target % set;
}

// Find an element in the array
// Return -1 if no found
int find(struct array *arr, int target) {
  for (int i = 0; i < arr->num; i++) {
    if (arr->array[i] == target) {
      return i;
    }
  }

  return -1;
}

int distance(int target, int start_index) {
  int max = -1;
  for (int i = start_index; i < item_num; i++) {
    if (target == mem_address[i]) {
      max = i;
    }
  }

  return max;
}

int find_smallest(struct array * arr) {
  int small = -1;
  for (int i = 0; i < arr->num; i++) {
    if (arr->array[i] < small) {
      small = arr->array[i];
    }
  }

  return small;
}

int find_largest(struct array * arr) {
  int large = -1;
  for (int i = 0; i < arr->num; i++) {
    if (arr->array[i] > large) {
      large = arr->array[i];
    }
  }

  return large;
}

void find_indexes(struct array * arr, struct array * result, int value) {
  for (int i = 0; i < arr->num; i++) {
    if (value == arr->array[i]) {
      result->array[result->num] = i;
      result->num++;
    }
  }
}

void extract_index(struct array * source, struct array * index, struct array * result) {
  for (int i = 0; i < index->num; i++) {
    result->array[result->num] = source->array[index->array[i]];
    result->num++;
  }
}

void push_lru(struct array *arr, int target) {
  int fd = find(arr, target);
  if (fd != -1) {
    // If found, swap
    for (int i = fd; i < arr->num - 1; i++) {
      int temp = arr->array[i + 1];
      arr->array[i + 1] = arr->array[i];
      arr->array[i] = temp;
    }
  } else {
    if (arr->num < set_num) {
      arr->array[arr->num] = target;
      arr->num++;
    } else {
      // Remove the least one, and insert target to the end
      for (int i = 0; i < arr->num - 1; i++) {
        arr->array[i] = arr->array[i + 1];
      }
      arr->array[arr->num - 1] = target;
    }
  }
}

void push_belady(int index, int cindex, int fd) {
  if (fd == -1) {
    if (cache[cindex].num < set_num) {
      cache[cindex].array[cache[cindex].num] = mem_address[index];
      cache[cindex].num++;
    } else {
      // Find furthest distance
      struct array distances = {
              .num = 0,
              .array = calloc(set_num, sizeof(int))
      };
      for (int i = 0; i < set_num; i++) {
        distances.array[i] = distance(cache[cindex].array[i], index);
        distances.num++;
      }

      // How many never access(-1） do we have?
      struct array result = {
              .num = 0,
              .array = calloc(set_num, sizeof(int))
      };
      find_indexes(&distances, &result, -1);

      // More than 2
      if (result.num > 1) {
        struct array negative = {
                .num = 0,
                .array = calloc(set_num, sizeof(int))
        };
        extract_index(&cache[cindex], &distances, &negative);
        cache[cindex].array[find_smallest(&negative)] = mem_address[index];
      } else if (result.num == 1){
        // Replace the one with -1
        cache[cindex].array[result.array[0]] = mem_address[index];
      } else {
        // We dont have any, remove the one with largest distance
        cache[cindex].array[find(&distances, find_largest(&distances))] = mem_address[index];
      }
    }
  }
}

bool access_lru(int target) {
  int index = cache_index(target);
  int fd = find(&cache[index], target);
  push_lru(&cache[index], target);
  return fd != -1;
}

bool access_belady(int index) {
  int cindex = cache_index(mem_address[index]);
  int fd = find(&cache[cindex], mem_address[index]);
  push_belady(index, cindex, fd);
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
  set = MAX_SIZE / set_num;
  printf("Cache sets: %d\n", set);
  printf("Cache algorithm: %s\n", argv[2]);

  // Init
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
  item_num = i;
  int hit_num = 0;
  // Init data structure based on input
  cache = calloc(set, sizeof(struct array));
  for (int j = 0; j < set; j++) {
    cache[j].array = calloc(set_num, sizeof(int));
    cache[j].num = 0;
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
    for (i = 0; i < item_num; i++) {
      if (access_belady(i)) {
        printf("%d %s\n", mem_address[i], STR_HIT);
        hit_num++;
      } else {
        printf("%d %s\n", mem_address[i], STR_MISS);
      }
    }
  } else {
    fprintf(stderr, "You should not be here!");
  }

  // print Summary
  printf("Cache accesses: %d\n", item_num);
  printf("Cache hits: %d\n", hit_num);
  printf("Cache misses: %d\n", item_num - hit_num);
  float hit_rate = (float) hit_num / (float) item_num;
  printf("Overall hit rate: %f\n", hit_rate);

  return EXIT_SUCCESS;
}
