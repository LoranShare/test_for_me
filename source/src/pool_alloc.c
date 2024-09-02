/**
 *******************************************************************************
 * @file        pool_alloc.c.c
 * @author      Gevorgyan Ovik
 * @brief       A module implementing a simple pool allactor
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "pool_alloc.h"
#include <stdint.h>

/* Private define ------------------------------------------------------------*/
#if !defined(MEM_SIZE)
#error Parameter "MEM_SIZE" is not defined
#endif

#if !defined(CHUNK_SIZE)
#error Parameter "CHUNK_SIZE" is not defined
#endif

#if (MEM_SIZE < CHUNK_SIZE)
#error Parameter "MEM_SIZE" cannot be less than "CHUNK_SIZE"
#endif

#if ((MEM_SIZE % CHUNK_SIZE) != 0)
#error Parameter "MEM_SIZE" must be multiple "CHUNK_SIZE"
#endif

#if (MEM_SIZE <= 0) || (CHUNK_SIZE <= 0)
#error Parameters "MEM_SIZE", "CHUNK_SIZE" must be greater than zero
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct PoolAlloc
{
	void *start;
	void *end;
	size_t total_num_chunks;
	size_t free_num_chunks;

} PoolAlloc_t;

typedef struct PoolChunk
{
	struct PoolChunk *next_ptr;
	uint8_t free;
} PoolChunk_t;

/* Private functions ---------------------------------------------------------*/
int init_alloc(PoolAlloc_t *pool_allocator) {
	return -1;
}

void *pool_alloc(PoolAlloc_t *pool_allocator) {
	return NULL;
}

int pool_free(PoolAlloc_t *pool_allocator, void *mem_ptr) {
	return -1;
}


size_t get_free_capacity(PoolAlloc_t *pool_allocator) {
	return 0;
}

size_t get_total_capacity(PoolAlloc_t *pool_allocator) {
	return 0;
}
