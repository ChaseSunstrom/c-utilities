#include "alloc.h"
#include "hash.h"

static size_t hash_pointer(void* ptr) {
    return ((uintptr_t)ptr) >> 3;
}

static void block_table_init(Block_Hash_Table* table, size_t initial_capacity) {
    table->buckets = calloc(initial_capacity, sizeof(Allocated_Block*));
    table->capacity = initial_capacity;
    table->size = 0;
}

static void block_table_insert(Block_Hash_Table* table, Allocated_Block* block) {
    size_t index = block->u_hash % table->capacity;
    block->next = table->buckets[index];
    table->buckets[index] = block;
    table->size++;

    // Check if resizing is needed
    if ((float)table->size / table->capacity > HASH_TABLE_LOAD_FACTOR) {
        size_t new_capacity = table->capacity * 2;
        Allocated_Block** new_buckets = calloc(new_capacity, sizeof(Allocated_Block*));

        if (!new_buckets) {
            fprintf(stderr, "Failed to resize hash table. Continuing with current size.\n");
            return;
        }

        // Rehash all existing entries
        for (size_t i = 0; i < table->capacity; i++) {
            Allocated_Block* current = table->buckets[i];
            while (current) {
                Allocated_Block* next = current->next;
                size_t new_index = current->u_hash % new_capacity;
                current->next = new_buckets[new_index];
                new_buckets[new_index] = current;
                current = next;
            }
        }

        free(table->buckets);
        table->buckets = new_buckets;
        table->capacity = new_capacity;
    }
}

static Allocated_Block* block_table_find(Block_Hash_Table* table, void* ptr) {
    size_t hash = hash_pointer(ptr);
    size_t index = hash % table->capacity;
    Allocated_Block* block = table->buckets[index];
    while (block) {
        if (block->p_data == ptr) {
            return block;
        }
        block = block->next;
    }
    return NULL;
}

static void block_table_remove(Block_Hash_Table* table, void* ptr) {
    size_t hash = hash_pointer(ptr);
    size_t index = hash % table->capacity;
    Allocated_Block** pp_block = &table->buckets[index];
    while (*pp_block) {
        if ((*pp_block)->p_data == ptr) {
            Allocated_Block* temp = *pp_block;
            *pp_block = (*pp_block)->next;
            free(temp);
            table->size--;
            return;
        }
        pp_block = &(*pp_block)->next;
    }
}


static void block_table_free(Block_Hash_Table* table) {
    for (size_t i = 0; i < table->capacity; i++) {
        Allocated_Block* block = table->buckets[i];
        while (block) {
            Allocated_Block* temp = block;
            block = block->next;
            free(temp);
        }
    }
    free(table->buckets);
    table->buckets = NULL;
    table->capacity = 0;
    table->size = 0;
}

Allocator* Allocator_default(void) {
    return Allocator_new_1(GENERAL_ALLOCATOR);
}

Allocator* Allocator_new_1(Alloc_T type) {
    Allocator* p_allocator = malloc(sizeof(Allocator));
    if (!p_allocator) return NULL;

    p_allocator->type = type;
    p_allocator->u_size = 0;
    p_allocator->s_capacity = -1;
    p_allocator->u_used = 0;
    p_allocator->u_ref_count = 1;
    block_table_init(&p_allocator->block_table, INITIAL_HASH_TABLE_SIZE);

    switch (type) {
    case GENERAL_ALLOCATOR:
        p_allocator->alloc = general_alloc;
        p_allocator->realloc = general_realloc;
        break;
    case ARENA_ALLOCATOR:
        p_allocator->alloc = arena_alloc;
        p_allocator->realloc = arena_realloc;
        break;
    case PAGE_ALLOCATOR:
        p_allocator->alloc = page_alloc;
        p_allocator->realloc = page_realloc;
        break;
    }

    return p_allocator;
}

Allocator* Allocator_new_2(Alloc_T type, ssize_t s_capacity) {
    Allocator* p_allocator = Allocator_new_1(type);
    if (p_allocator) {
        p_allocator->s_capacity = s_capacity;
    }
    return p_allocator;
}

Allocator* Allocator_new_4(Alloc_T type, ssize_t s_capacity, Alloc_Fn alloc, Realloc_Fn realloc) {
    Allocator* p_allocator = Allocator_new_2(type, s_capacity);
    if (p_allocator) {
        p_allocator->alloc = alloc;
        p_allocator->realloc = realloc;
    }
    return p_allocator;
}

void* Allocator_alloc_4(Allocator* p_allocator, size_t u_size, size_t u_amount, Dealloc_Fn dealloc) {
    if (!p_allocator || (p_allocator->s_capacity != -1 &&
        p_allocator->u_used + u_size * u_amount > p_allocator->s_capacity)) {
        return NULL;
    }

    size_t total_size = u_size * u_amount;
    void* p_data = p_allocator->alloc(total_size, 1);

    if (!p_data) return NULL;

    Allocated_Block* new_block = malloc(sizeof(Allocated_Block));
    if (!new_block) {
        p_allocator->alloc(p_data, total_size, 1);
        return NULL;
    }

    new_block->p_data = p_data;
    new_block->u_size = total_size;
    new_block->dealloc = dealloc;
    new_block->next = NULL;
    new_block->u_hash = hash_pointer(p_data);  // Store the hash

    block_table_insert(&p_allocator->block_table, new_block);
    p_allocator->u_size += total_size;
    p_allocator->u_used += total_size;

    return p_data;
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Allocator_free_(Allocator** pp_allocator) {
    if (!pp_allocator || !*pp_allocator) return;
    Allocator_free_everything(*pp_allocator);
    *pp_allocator = NULL;
}
#endif

void* Allocator_alloc_3(Allocator* p_allocator, size_t u_size, size_t u_amount) {
    return Allocator_alloc_4(p_allocator, u_size, u_amount, free);
}

void* Allocator_alloc_2(Allocator* p_allocator, size_t u_size) {
    return Allocator_alloc_3(p_allocator, u_size, 1);
}

void Allocator_free(Allocator* p_allocator, void* p_data) {
    if (!p_allocator || !p_data || p_allocator->type == ARENA_ALLOCATOR) {
        return;
    }

    Allocated_Block* block = block_table_find(&p_allocator->block_table, p_data);
    if (block) {
        p_allocator->u_size -= block->u_size;
        p_allocator->u_used -= block->u_size;

        if (p_allocator->type == PAGE_ALLOCATOR) {
            page_free(p_data, block->u_size, 1);
        }
        else if (block->dealloc) {
            block->dealloc(p_data);
        }

        block_table_remove(&p_allocator->block_table, p_data);
    }
    else {
        fprintf(stderr, "Warning: Attempted to free untracked pointer %p\n", p_data);
    }
}

void Allocator_free_array(Allocator* p_allocator, void* p_data, size_t u_count, Dealloc_Fn dealloc) {
    if (!p_allocator || !p_data || p_allocator->type == ARENA_ALLOCATOR) {
        return;
    }

    Allocated_Block* block = block_table_find(&p_allocator->block_table, p_data);
    if (block) {
        p_allocator->u_size -= block->u_size;
        p_allocator->u_used -= block->u_size;

        if (dealloc) {
            for (size_t i = 0; i < u_count; i++) {
                dealloc(((char*)p_data) + i * (block->u_size / u_count));
            }
        }

        if (p_allocator->type == PAGE_ALLOCATOR) {
            page_free(p_data, block->u_size, 1);
        }
        else if (block->dealloc) {
            block->dealloc(p_data);
        }

        block_table_remove(&p_allocator->block_table, p_data);
    }
    else {
        fprintf(stderr, "Warning: Attempted to free untracked array pointer %p\n", p_data);
    }
}

void Allocator_free_with_dealloc(Allocator* p_allocator, void* p_data, Dealloc_Fn dealloc) {
	if (!p_allocator || !p_data || p_allocator->type == ARENA_ALLOCATOR) {
		return;
	}

	Allocated_Block* block = block_table_find(&p_allocator->block_table, p_data);
	if (block) {
		p_allocator->u_size -= block->u_size;
		p_allocator->u_used -= block->u_size;

		if (dealloc) {
			dealloc(p_data);
		}

		block_table_remove(&p_allocator->block_table, p_data);
	}
	else {
		fprintf(stderr, "Warning: Attempted to free untracked pointer %p\n", p_data);
	}
}

void Allocator_free_all(Allocator* p_allocator) {
    if (!p_allocator) return;

    for (size_t i = 0; i < p_allocator->block_table.capacity; i++) {
        Allocated_Block* block = p_allocator->block_table.buckets[i];
        while (block) {
            if (p_allocator->type == PAGE_ALLOCATOR) {
                page_free(block->p_data, block->u_size, 1);
            }
            else if (block->dealloc) {
                block->dealloc(block->p_data);
            }
            block = block->next;
        }
    }

    block_table_free(&p_allocator->block_table);
    block_table_init(&p_allocator->block_table, INITIAL_HASH_TABLE_SIZE);
    p_allocator->u_size = 0;
    p_allocator->u_used = 0;
}

void Allocator_free_everything(Allocator* p_allocator) {
    if (!p_allocator) return;

    if (p_allocator->u_ref_count > 1) {
        fprintf(stderr, "Warning: Freeing allocator with multiple references\n");
    }

    Allocator_free_all(p_allocator);
    block_table_free(&p_allocator->block_table);

    if (p_allocator->type == PAGE_ALLOCATOR) {
        page_free(p_allocator, sizeof(Allocator), 1);
    }
    else {
        free(p_allocator);
    }
}

void* Allocator_realloc(Allocator* p_allocator, void* p_data, size_t u_size) {
    if (!p_allocator || !p_data) return NULL;
    if (!p_allocator->realloc) {
        fprintf(stderr, "Error: Realloc not supported for this allocator type\n");
        return NULL;
    }

    Allocated_Block* block = block_table_find(&p_allocator->block_table, p_data);
    if (!block) {
        fprintf(stderr, "Error: Attempted to realloc untracked pointer %p\n", p_data);
        return NULL;
    }

    void* p_new_data = p_allocator->realloc(p_data, u_size, 1);
    if (!p_new_data) return NULL;

    p_allocator->u_size += u_size - block->u_size;
    p_allocator->u_used += u_size - block->u_size;
    block->p_data = p_new_data;
    block->u_size = u_size;
    block->u_hash = hash_pointer(p_new_data);  // Update the hash if the pointer changes

    return p_new_data;
}

Allocator* Allocator_decref(Allocator* p_allocator) {
    if (!p_allocator) return NULL;
    if (--p_allocator->u_ref_count == 0) {
        Allocator_free_everything(p_allocator);
        return NULL;
    }
    return p_allocator;
}

Allocator* Allocator_move(Allocator* p_allocator) {
    if (p_allocator) {
        p_allocator->u_ref_count = 1;
    }
    return p_allocator;
}