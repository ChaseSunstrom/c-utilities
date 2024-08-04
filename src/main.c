#include "util/thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_TASKS 100
#define NUM_THREADS 50

void initialize_random_seed() {
    srand((unsigned int)time(NULL));
}

void* increment_task(void* p_arg) {
    int* p_num = (int*)p_arg;

    // Generate a random delay between 0 and 1000 milliseconds
    int delay_ms = rand() % 1001;

    // Sleep for the random duration
#ifdef _WIN32
    Sleep(delay_ms);
#else
    usleep(delay_ms * 1000);  // usleep takes microseconds
#endif

    (*p_num)++;
    printf("Task %d completed after %d ms.\n", *p_num, delay_ms);
    return p_num;
}

int main() {
    for (int k = 0; k < 10; k++)
    {
        printf("Starting Thread Pool test...\n");

        // Create thread pool
        Thread_Pool* p_pool = Thread_Pool_new(NUM_THREADS);
        if (!p_pool) {
            printf("Failed to create thread pool.\n");
            return 1;
        }

        printf("Thread Pool created successfully.\n");

        // Prepare tasks
        int numbers[NUM_TASKS];
        Task* p_tasks[NUM_TASKS];

        // Add tasks to the pool
        for (int i = 0; i < NUM_TASKS; i++) {
            numbers[i] = i;
            p_tasks[i] = Thread_Pool_add_task(p_pool, increment_task, &numbers[i]);
            if (!p_tasks[i]) {
                printf("Failed to add task %d.\n", i);
                Thread_Pool_destroy(p_pool);
                return 1;
            }
        }

        printf("All tasks added to the pool.\n");

        // Wait for all tasks to complete
        Thread_Pool_wait(p_pool);

        printf("All tasks completed. Checking results...\n");

        // Check results
        for (int i = 0; i < NUM_TASKS; i++) {
            int* p_result = (int*)Thread_Pool_get_result(p_tasks[i]);
            if (*p_result != i + 1) {
                printf("Unexpected result for task %d: expected %d, got %d\n", i, i + 1, *p_result);
                Thread_Pool_destroy(p_pool);
                return 1;
            }
        }

        printf("All results are correct.\n");

        // Clean up
        Thread_Pool_destroy(p_pool);

        printf("Thread Pool destroyed. Test completed successfully!\n");
    }
}