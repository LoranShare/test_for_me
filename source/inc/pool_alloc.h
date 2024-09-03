/**
 *******************************************************************************
 * @file        pool_alloc.h
 * @author      Gevorgyan Ovik
 * @brief       Header of pool_alloc.c
 *******************************************************************************
 */

/**
 * @note		Будем считать, что память выделяется путем размещения вектора
 * 				в памяти, на деле же это будет указатель из Linker script
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POOL_ALLOC_H__
#define __POOL_ALLOC_H__

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct PoolAlloc PoolAlloc_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int init_alloc(PoolAlloc_t **pool_allocator);

void *pool_alloc(PoolAlloc_t *pool_allocator);

int pool_free(PoolAlloc_t *pool_allocator, void *mem_ptr);

/* @note в принципе можно и дефайными задать */
size_t pool_get_chunksize(PoolAlloc_t *pool_allocator);

size_t get_free_capacity(PoolAlloc_t *pool_allocator);

size_t get_total_capacity(PoolAlloc_t *pool_allocator);

#ifdef TEST
void print_structure(PoolAlloc_t *pool_allocator);
#endif

#endif /* __POOL_ALLOC_H__ */
