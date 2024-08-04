#ifndef CUTIL_THREAD_POOL_H
#define CUTIL_THREAD_POOL_H
#include "alloc.h"
#include "system.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define MAX_TASKS 1000

typedef void* (*Task_Fn)(void* arg);

typedef struct
{
    Task_Fn function;
    void* p_argument;
    void* volatile p_result;
    volatile bool b_is_completed;
} Task;

typedef struct
{
    Task* volatile arr_tasks[MAX_TASKS];
    volatile size_t u_task_count;
    size_t u_task_capacity;
    volatile size_t u_head;
    volatile size_t u_tasks_in_progress;
    volatile size_t u_tail;
    size_t u_num_threads;
    volatile bool b_stop;
    bool b_external_allocator;
    Allocator* p_allocator;
#ifdef _WIN32
    HANDLE* p_threads;
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE task_available;
    CONDITION_VARIABLE queue_empty;
#else
    pthread_t* p_threads;
    pthread_mutex_t lock;
    pthread_cond_t task_available;
    pthread_cond_t queue_empty;
#endif
} Thread_Pool;

#define Thread_Pool_new(...) CONCAT(Thread_Pool_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

Thread_Pool* Thread_Pool_new_1(size_t u_num_threads);
Thread_Pool* Thread_Pool_new_2(size_t u_num_threads, Allocator* p_allocator);
void Thread_Pool_destroy(Thread_Pool* p_pool);
Task* Thread_Pool_add_task(Thread_Pool* p_pool, Task_Fn task, void* p_arg);
void Thread_Pool_wait(Thread_Pool* p_pool);
void* Thread_Pool_get_result(Task* p_task);

#endif // CUTIL_THREAD_POOL_H