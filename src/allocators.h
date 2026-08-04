#pragma once
#include <stddef.h>
#include "datatypes.h"

typedef struct MemoryPool MemoryPool;


MemoryPool *pool_init(size_t block_size, ui32 block_amount);

void* pool_alloc(MemoryPool *pool);

void pool_free(MemoryPool *pool, void *addr);

void pool_destroy(MemoryPool *pool);