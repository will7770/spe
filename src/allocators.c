#include <stdlib.h>
#include "datatypes.h"


typedef struct PoolNode {
    struct PoolNode *next;
} PoolNode;


typedef struct {
    void* buffer;
    PoolNode *free_block;
    size_t block_size;
    ui32 block_amount;
} MemoryPool;


MemoryPool *pool_init(size_t block_size, ui32 block_amount) {
    size_t aligned_block = (block_size + 8 - 1) & ~(8 - 1); // assuming we run on x64. well, who'll run this on 32 bit anyway?

    MemoryPool *pool = malloc(aligned_block*block_amount + sizeof(MemoryPool));
    if (pool == NULL) {
        return NULL;
    }

    pool->block_amount = block_amount;
    pool->block_size = aligned_block;
    pool->buffer = (ui8*)pool + sizeof(MemoryPool);
    pool->free_block = pool->buffer;

    PoolNode *last = pool->free_block;
    for (ui32 i = 0; i < block_amount-1; i++) {
        PoolNode *next_addr = (PoolNode*)((ui8*)last + aligned_block);
        last->next = next_addr;
        last = next_addr;
    }
    last->next = NULL; // set the last element to point to NULL

    return pool;
}


void* pool_alloc(MemoryPool *pool) {
    PoolNode *ret_addr = pool->free_block;

    if (ret_addr == NULL) {
        return NULL;
    }

    pool->free_block = ret_addr->next;
    return (void*)ret_addr;
}


void pool_free(MemoryPool *pool, void *addr) {
    PoolNode *next = pool->free_block;
    PoolNode *node = (PoolNode*)addr;
    node->next = next;
    pool->free_block = node;
}


void pool_destroy(MemoryPool *pool) {
    free(pool);
}
