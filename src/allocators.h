#pragma once
#include <stddef.h>

typedef struct MemoryPool MemoryPool;


MemoryPool *pool_init(size_t block_size, size_t block_amount);

void* pool_alloc(MemoryPool *pool);

void pool_free(MemoryPool *pool, void *addr);

void pool_destroy(MemoryPool *pool);