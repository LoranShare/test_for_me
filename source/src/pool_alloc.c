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
#ifdef TEST
    #include <stdio.h>
#endif

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

/// @todo Можно еще опцию заполнения добавить
#define FILL_MEM  ///<

/* Private typedef -----------------------------------------------------------*/
typedef struct Mutex {
} Mutex_t;

typedef struct PoolAlloc {
    void *start;
    void *end;
    void *base_addr;
    size_t total_num_chunks;
    size_t free_num_chunks;
    Mutex_t mutex;

} PoolAlloc_t;

typedef struct PoolChunk {
    struct PoolChunk *next_ptr;
    uint8_t free;
} PoolChunk_t;

/* Private macro -------------------------------------------------------------*/
#define init_mutex(x)
#define lock_mutex(x)
#define free_mutex(x)

/* Private variables ---------------------------------------------------------*/
/**
 * @note    Память для структуры выделяется тут, можно дать это право пользователю
 *          или выделять ее в одном из блоков/начале памяти
 */

static PoolAlloc_t allocator;
static uint8_t mem[MEM_SIZE];
static uint8_t *mem_ptr = &mem[1];

/* Private functions ---------------------------------------------------------*/
static size_t get_aligned_value(size_t n, size_t k) {
    size_t aligment_mask = ~(k - 1);
    return (n + k - 1) & aligment_mask;
}

int init_alloc(PoolAlloc_t **pool_allocator) {
    /// @todo Т.к. это embedded код, то можно добавить ассерты
    /// на нулевой указатель и прочие условия, который в резиле будет убран
    /// пока мало времени, сделаю, если успею

    // Смещаемся для выравнения адреса
    uintptr_t base_addr        = get_aligned_value((uintptr_t)mem_ptr, sizeof(void *));
    uintptr_t start_addr       = (uintptr_t)mem_ptr;
    uintptr_t end_addr         = start_addr + MEM_SIZE;
    allocator.total_num_chunks = (MEM_SIZE - (base_addr - (uintptr_t)mem_ptr)) / CHUNK_SIZE;
    allocator.free_num_chunks  = allocator.total_num_chunks;
    PoolChunk_t *prev_chuck    = NULL;
    PoolChunk_t *cur_chuck;

    if ((base_addr > end_addr) || (CHUNK_SIZE < sizeof(PoolChunk_t)) ||
        ((CHUNK_SIZE != get_aligned_value(CHUNK_SIZE, sizeof(void *))) != 0) ||
        (allocator.total_num_chunks == 0)) {
        return -1;
    }

    /// @note если размер блока не кратен размеру указателя, по идее можно смещение
    /// сделать внутри блока
    allocator.start     = (void *)base_addr;
    allocator.base_addr = (void *)base_addr;
    allocator.end       = (void *)end_addr;
    init_mutex(&allocator.mutex);

    for (int i = 0; i < allocator.total_num_chunks; ++i) {
        cur_chuck           = (PoolChunk_t *)(base_addr + i * CHUNK_SIZE);
        cur_chuck->free     = 1;
        cur_chuck->next_ptr = NULL;

        if (prev_chuck) {
            prev_chuck->next_ptr = cur_chuck;
        }

        prev_chuck = cur_chuck;
    }

    *pool_allocator = &allocator;

    return 0;
}

void *pool_alloc(PoolAlloc_t *pool_allocator) {
    /// @todo ассерт на нулевой указатель

    PoolChunk_t *head_ptr = (PoolChunk_t *)pool_allocator->base_addr;

    lock_mutex(&pool_allocator->mutex);

    if (head_ptr) {
        head_ptr->free            = 0;
        pool_allocator->base_addr = (void *)head_ptr->next_ptr;

        /// @todo ассерт на данный счетчик, т.к. в релизе проверка мб не интересна
        --pool_allocator->free_num_chunks;
        head_ptr->next_ptr = NULL;

        /// @note можно заполнением выравнить адрес по нужной границе
        head_ptr = (PoolChunk_t *)((uintptr_t)head_ptr + sizeof(*head_ptr));
    }
    free_mutex(&pool_allocator->mutex);

    free_mutex(&pool_allocator->mutex);

    return (void *)head_ptr;
}

int pool_free(PoolAlloc_t *pool_allocator, void *mem_ptr) {
    /// @todo ассерт на нулевой указатель

    PoolChunk_t *chunk;
    mem_ptr = (void *)((uintptr_t)mem_ptr - sizeof(*chunk));
    chunk   = (PoolChunk_t *)mem_ptr;

    if (((uintptr_t)mem_ptr < (uintptr_t)pool_allocator->start) ||
        ((uintptr_t)mem_ptr > (uintptr_t)pool_allocator->end)) {
        return -1;
    }

    if ((((uintptr_t)mem_ptr - (uintptr_t)pool_allocator->start) % CHUNK_SIZE) != 0) {
        return -1;
    }

    if (chunk->free) {
        return -1;
    }

    lock_mutex(&pool_allocator->mutex);

    chunk->next_ptr           = pool_allocator->base_addr;
    chunk->free               = 1;
    pool_allocator->base_addr = (void *)chunk;
    ++pool_allocator->free_num_chunks;

    free_mutex(&pool_allocator->mutex);

    return 0;
}

size_t get_free_capacity(PoolAlloc_t *pool_allocator) {
    return pool_allocator->free_num_chunks;
}

size_t get_total_capacity(PoolAlloc_t *pool_allocator) {
    return pool_allocator->total_num_chunks;
}

#ifdef TEST

int hexdump(const void *data, size_t size) {
    const unsigned char *ptr;

    if (!data) {
        return -1;
    }

    ptr = (const unsigned char *)data;

    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", ptr[i]);

        if ((i + 1) % 16 == 0 || i == size - 1) {
            for (size_t j = 0; j < 16 - (i % 16); j++) {
                printf("   ");  // Three spaces for each unprinted byte
            }

            printf("| ");

            for (size_t j = (i / 16) * 16; j <= i; j++) {
                unsigned char c = ptr[j];
                if (c < 32 || c > 126) {
                    c = '.';  // Replace non-printable characters with a dot
                }
                printf("%c", c);
            }

            printf("\n");
        }
    }
    return 0;
}

void print_structure(PoolAlloc_t *pool_allocator) {
    PoolChunk_t *chunk = (PoolChunk_t *)pool_allocator->base_addr;
    size_t i           = 0;

    lock_mutex(&pool_allocator->mutex);
    printf("\n");
    while (chunk) {
        printf("\n%ld. Addr %p, free %d\n", i++, chunk, chunk->free);
        hexdump(((void *)(uintptr_t)chunk + sizeof(*chunk)), CHUNK_SIZE - sizeof(*chunk));
        chunk = chunk->next_ptr;
    }

    free_mutex(&pool_allocator->mutex);
}

size_t pool_get_chunksize(PoolAlloc_t *pool_allocator) {
    return (size_t)CHUNK_SIZE - sizeof(PoolChunk_t);
}

void *get_start_mem_addr(void) {
    return (void *)mem;
}

void *get_end_mem_addr(void) {
    return (void *)((uintptr_t)mem + sizeof(mem));
}

#endif
