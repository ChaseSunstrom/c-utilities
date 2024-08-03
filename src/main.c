#include "../tests/test_main.h"
#include "util/hash.h"
#include "util/system.h"
#include "util/unordered_map.h"
#include "util/alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_INSERTIONS 1000
#define MAX_KEY_LENGTH 20

void key_destructor(void* key) {
    // Simplified destructor
    free(key);
}

void value_destructor(void* value) {
    // Simplified destructor
    free(value);
}

int main() {
#ifdef CUTIL_TEST
    Tests_T *p_tests = Tests_T_new();
    Tests_T_run(p_tests);
#endif
    printf("Creating Allocator\n");
    Allocator* allocator = Allocator_new(GENERAL_ALLOCATOR);
    if (!allocator) {
        fprintf(stderr, "Failed to create Allocator\n");
        return 1;
    }

    printf("Creating Unordered_Map\n");
    Unordered_Map* map = Unordered_Map_new_4(jenkins_hash, key_destructor, value_destructor, allocator);
    if (!map) {
        fprintf(stderr, "Failed to create Unordered_Map\n");
        Allocator_free_everything(allocator);
        return 1;
    }

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    char key_buffer[MAX_KEY_LENGTH];
    for (int i = 0; i < NUM_INSERTIONS; i++) {
        
        snprintf(key_buffer, MAX_KEY_LENGTH, "key_%d", i);
        char* key = Allocator_alloc(allocator, strlen(key_buffer) + 1);
        char* value = Allocator_alloc(allocator, 3); // "hi" + null terminator
        if (!key || !value) {
            fprintf(stderr, "Failed to allocate memory for key or value\n");
            Unordered_Map_free(map);
            Allocator_free_everything(allocator);
            return 1;
        }
        strcpy(key, key_buffer);
        strcpy(value, "hi");

        Unordered_Map_insert(map, key, value, strlen(key) + 1);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to insert %d elements: %f seconds\n", NUM_INSERTIONS, cpu_time_used);
    printf("Final size of the map: %zu\n", map->u_size);

    printf("Freeing Unordered_Map\n");
    Unordered_Map_free(map);
    printf("Unordered_Map freed successfully\n");

    printf("Freeing Allocator\n");
    printf("Allocator freed successfully\n");

    return 0;
}
