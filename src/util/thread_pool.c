#include "thread_pool.h"

#ifdef _WIN32
DWORD WINAPI thread_function(LPVOID p_arg) {
#else
void* thread_function(void* p_arg) {
#endif
    Thread_Pool* p_pool = (Thread_Pool*)p_arg;
    Task* p_task;

    while (true) {
#ifdef _WIN32
        EnterCriticalSection(&p_pool->lock);
#else
        pthread_mutex_lock(&p_pool->lock);
#endif

        while (p_pool->u_task_count == 0 && !p_pool->b_stop) {
#ifdef _WIN32
            SleepConditionVariableCS(&p_pool->task_available, &p_pool->lock, INFINITE);
#else
            pthread_cond_wait(&p_pool->task_available, &p_pool->lock);
#endif
        }

        if (p_pool->b_stop) {
#ifdef _WIN32
            LeaveCriticalSection(&p_pool->lock);
#else
            pthread_mutex_unlock(&p_pool->lock);
#endif
            break;
        }

        p_task = p_pool->arr_tasks[p_pool->u_head];
        p_pool->u_head = (p_pool->u_head + 1) % p_pool->u_task_capacity;
        p_pool->u_task_count--;
        p_pool->u_tasks_in_progress++;

#ifdef _WIN32
        LeaveCriticalSection(&p_pool->lock);
#else
        pthread_mutex_unlock(&p_pool->lock);
#endif

        // Execute the task
        p_task->p_result = p_task->function(p_task->p_argument);

#ifdef _WIN32
        EnterCriticalSection(&p_pool->lock);
#else
        pthread_mutex_lock(&p_pool->lock);
#endif

        p_task->b_is_completed = true;
        p_pool->u_tasks_in_progress--;

        // Signal if this was the last task
        if (p_pool->u_task_count == 0 && p_pool->u_tasks_in_progress == 0) {
#ifdef _WIN32
            WakeAllConditionVariable(&p_pool->queue_empty);
#else
            pthread_cond_broadcast(&p_pool->queue_empty);
#endif
        }

#ifdef _WIN32
        LeaveCriticalSection(&p_pool->lock);
#else
        pthread_mutex_unlock(&p_pool->lock);
#endif
    }

    return 0;
}

Thread_Pool* Thread_Pool_new_1(size_t u_num_threads) {
	return Thread_Pool_new_2(u_num_threads, NULL);
}

Thread_Pool* Thread_Pool_new_2(size_t u_num_threads, Allocator* p_allocator) {
    bool b_external_allocator = true;
    if (!p_allocator) {
        p_allocator = Allocator_default();
        if (!p_allocator) {
            return NULL;
        }
        b_external_allocator = false;
    }

    Thread_Pool* p_pool = Allocator_alloc(p_allocator, sizeof(Thread_Pool));
    if (!p_pool) {
        if (!b_external_allocator) {
            Allocator_free_everything(p_allocator);
        }
        return NULL;
    }

    // Initialize p_pool members...
    p_pool->p_allocator = p_allocator;
    p_pool->u_task_count = 0;
    p_pool->u_task_capacity = MAX_TASKS;
    p_pool->u_head = 0;
    p_pool->u_tail = 0;
    p_pool->u_num_threads = u_num_threads;
    p_pool->u_tasks_in_progress = 0;
    p_pool->b_external_allocator = b_external_allocator;
    p_pool->b_stop = false;

#ifdef _WIN32
    InitializeCriticalSection(&p_pool->lock);
    InitializeConditionVariable(&p_pool->task_available);
    InitializeConditionVariable(&p_pool->queue_empty);
    p_pool->p_threads = Allocator_alloc(p_pool->p_allocator, u_num_threads * sizeof(HANDLE));
#else
    pthread_mutex_init(&p_pool->lock, NULL);
    pthread_cond_init(&p_pool->task_available, NULL);
    pthread_cond_init(&p_pool->queue_empty, NULL);
    p_pool->p_threads = Allocator_alloc(p_pool->p_allocator, u_num_threads * sizeof(pthread_t));
#endif

    if (!p_pool->p_threads) {
        Allocator_free(p_allocator, p_pool);
        if (!b_external_allocator) {
            Allocator_free_everything(p_allocator);
        }
        return NULL;
    }
    for (size_t i = 0; i < u_num_threads; i++) {
#ifdef _WIN32
        p_pool->p_threads[i] = CreateThread(NULL, 0, thread_function, p_pool, 0, NULL);
        if (p_pool->p_threads[i] == NULL) {
            // Clean up previously created threads
            for (size_t j = 0; j < i; j++) {
                CloseHandle(p_pool->p_threads[j]);
            }
            Allocator_free(p_allocator, p_pool->p_threads);
            Allocator_free(p_allocator, p_pool);
            if (!b_external_allocator) {
                Allocator_free_everything(p_allocator);
            }
            return NULL;
        }
#else
        i32 result = pthread_create(&p_pool->p_threads[i], NULL, thread_function, p_pool);
        if (result != 0) {
            // Clean up previously created threads
            for (size_t j = 0; j < i; j++) {
                pthread_cancel(p_pool->p_threads[j]);
                pthread_join(p_pool->p_threads[j], NULL);
            }
            Allocator_free(p_allocator, p_pool->p_threads);
            Allocator_free(p_allocator, p_pool);
            if (!b_external_allocator) {
                Allocator_free_everything(p_allocator);
            }
            return NULL;
        }
#endif
    }

    return p_pool;
}

void Thread_Pool_destroy(Thread_Pool * p_pool) {
    if (!p_pool) return;

#ifdef _WIN32
    EnterCriticalSection(&p_pool->lock);
#else
    pthread_mutex_lock(&p_pool->lock);
#endif

    p_pool->b_stop = true;

#ifdef _WIN32
    WakeAllConditionVariable(&p_pool->task_available);
    LeaveCriticalSection(&p_pool->lock);

    WaitForMultipleObjects(p_pool->u_num_threads, p_pool->p_threads, TRUE, INFINITE);
    for (size_t i = 0; i < p_pool->u_num_threads; i++) {
        CloseHandle(p_pool->p_threads[i]);
    }

    DeleteCriticalSection(&p_pool->lock);
#else
    pthread_cond_broadcast(&p_pool->task_available);
    pthread_mutex_unlock(&p_pool->lock);

    for (size_t i = 0; i < p_pool->u_num_threads; i++) {
        pthread_join(p_pool->p_threads[i], NULL);
    }

    pthread_mutex_destroy(&p_pool->lock);
    pthread_cond_destroy(&p_pool->task_available);
    pthread_cond_destroy(&p_pool->queue_empty);
#endif

    for (size_t i = 0; i < p_pool->u_task_count; i++) {
        Allocator_free(p_pool->p_allocator, p_pool->arr_tasks[(p_pool->u_head + i) % p_pool->u_task_capacity]);
    }

    bool b_external_allocator = p_pool->b_external_allocator;

    if (!b_external_allocator) Allocator_free_everything(p_pool->p_allocator);
	else Allocator_free(p_pool->p_allocator, p_pool);
}

Task* Thread_Pool_add_task(Thread_Pool * p_pool, Task_Fn task, void* p_arg) {
    if (!p_pool) return NULL;

#ifdef _WIN32
    EnterCriticalSection(&p_pool->lock);
#else
    pthread_mutex_lock(&p_pool->lock);
#endif

    if (p_pool->u_task_count == p_pool->u_task_capacity) {
#ifdef _WIN32
        LeaveCriticalSection(&p_pool->lock);
#else
        pthread_mutex_unlock(&p_pool->lock);
#endif
        return NULL;
    }

    Task* p_new_task = Allocator_alloc(p_pool->p_allocator, sizeof(Task));
    if (!p_new_task) {
#ifdef _WIN32
        LeaveCriticalSection(&p_pool->lock);
#else
        pthread_mutex_unlock(&p_pool->lock);
#endif
        return NULL;
    }

    p_new_task->function = task;
    p_new_task->p_argument = p_arg;
    p_new_task->p_result = NULL;
    p_new_task->b_is_completed = false;

    p_pool->arr_tasks[p_pool->u_tail] = p_new_task;
    p_pool->u_tail = (p_pool->u_tail + 1) % p_pool->u_task_capacity;
    p_pool->u_task_count++;

#ifdef _WIN32
    WakeConditionVariable(&p_pool->task_available);
    LeaveCriticalSection(&p_pool->lock);
#else
    pthread_cond_signal(&p_pool->task_available);
    pthread_mutex_unlock(&p_pool->lock);
#endif

    return p_new_task;
}

void Thread_Pool_wait(Thread_Pool* p_pool) {
    if (!p_pool) return;

#ifdef _WIN32
    EnterCriticalSection(&p_pool->lock);
#else
    pthread_mutex_lock(&p_pool->lock);
#endif

    while (p_pool->u_task_count > 0) {
#ifdef _WIN32
        SleepConditionVariableCS(&p_pool->queue_empty, &p_pool->lock, INFINITE);
#else
        pthread_cond_wait(&p_pool->queue_empty, &p_pool->lock);
#endif
    }

#ifdef _WIN32
    LeaveCriticalSection(&p_pool->lock);
#else
    pthread_mutex_unlock(&p_pool->lock);
#endif
}

void* Thread_Pool_get_result(Task * p_task) {
    if (!p_task) return NULL;

    while (!p_task->b_is_completed) {
        // Busy wait or use a condition variable if needed
    }

    return p_task->p_result;
}