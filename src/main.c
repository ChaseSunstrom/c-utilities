#include "../tests/test_main.h"
#include "util/hash.h"
#include "util/system.h"
#include "util/unordered_map.h"
#include "util/alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_INSERTIONS 1000000
#define MAX_KEY_LENGTH 20

size_t custom_key_hash(const void* key, size_t len) {
    const char* str = (const char*)key;

    // Check if the string starts with "key_"
    if (len < 4 || strncmp(str, "key_", 4) != 0) {
        // If not, fall back to a simple hash
        size_t hash = 5381;
        for (size_t i = 0; i < len; i++) {
            hash = ((hash << 5) + hash) + str[i];
        }
        return hash;
    }

    // Extract the number after "key_"
    size_t num = 0;
    for (size_t i = 4; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
        }
        else {
            break;
        }
    }

    // Combine the "key_" part and the number
    return (size_t)((5381 * 33 * 33 * 33 * 33) + num);
}

void key_destructor(void* key) {
    // Simplified destructor
    free(key);
}

void value_destructor(void* value) {
    // Simplified destructor
    free(value);
}

i32 main() {
#ifdef CUTIL_TEST
    Tests_T* p_tests = Tests_T_new();
    Tests_T_run(p_tests);
#endif
    printf("Creating Allocator\n");
    Allocator* allocator = Allocator_new(GENERAL_ALLOCATOR);
    if (!allocator) {
        fprintf(stderr, "Failed to create Allocator\n");
        return 1;
    }
    printf("Creating Unordered_Map\n");
    Unordered_Map* map = Unordered_Map_new(custom_key_hash, key_destructor, value_destructor, allocator);
    if (!map) {
        fprintf(stderr, "Failed to create Unordered_Map\n");
        Allocator_free_everything(allocator);
        return 1;
    }
    clock_t start, end;
    f64 cpu_time_used;
    start = clock();
    char key_buffer[MAX_KEY_LENGTH];
    for (i32 i = 0; i < NUM_INSERTIONS; i++) {
        snprintf(key_buffer, MAX_KEY_LENGTH, "key_%d", i);
        // Explicitly use the two-argument version of Allocator_alloc
        char* key = Allocator_alloc(allocator, strlen(key_buffer) + 1);
        char* value = Allocator_alloc(allocator, 3); // "hi" + null terminator
        strcpy(key, key_buffer);
        strcpy(value, "hi");
        Unordered_Map_insert(map, key, value, strlen(key) + 1);
    }
    end = clock();
    cpu_time_used = ((f64)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to insert %d elements: %f seconds\n", NUM_INSERTIONS, cpu_time_used);
    printf("Final size of the map: %zu\n", map->u_size);
    printf("Freeing Unordered_Map\n");
    Unordered_Map_free(map);
    printf("Unordered_Map freed successfully\n");
    printf("Freeing Allocator\n");
    Allocator_free_everything(allocator);
    printf("Allocator freed successfully\n");
    return 0;
}