/**
 *******************************************************************************
 * @file        test_pool_alloc.c
 * @author      Gevorgyan Ovik
 * @brief       Unit tests
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "CUnit/Basic.h"
#include "pool_alloc.h"

/* Private define ------------------------------------------------------------*/
// #define PRINT_MEM ///< Отладочный вывод всех чанков и хекдамп памяти
#define MAX_NUM_OF_CHUNKS (MEM_SIZE / CHUNK_SIZE)

int setUp(void) {
    return 0;
}

int tearDown(void) {
    return 0;
}

/* Создание экземпляра, выделение блока, освобождение блока */
void test_alloc_1_chunk(void) {
    PoolAlloc_t *pool_allocator = NULL;
    size_t chunk_size;
    void *ptr;

    CU_ASSERT_FATAL(init_alloc(&pool_allocator) == 0);

    chunk_size = pool_get_chunksize(pool_allocator);
    ptr        = pool_alloc(pool_allocator);
    memset(ptr, 0xAA, chunk_size);

    CU_ASSERT_PTR_NOT_NULL_FATAL(ptr);
    CU_ASSERT_FATAL(pool_free(pool_allocator, ptr) == 0);

#if defined(PRINT_MEM) && defined(TEST)
    print_structure(pool_allocator);
#endif
}

/* Создание экземпляра, выделение всех блоков, выделение еще блоков, освобождение блоков */
void test_alloc_all_chunks(void) {
    PoolAlloc_t *pool_allocator  = NULL;
    void *ptr[MAX_NUM_OF_CHUNKS] = {NULL};
    size_t chunk_size;

    CU_ASSERT_FATAL(init_alloc(&pool_allocator) == 0);
    chunk_size = pool_get_chunksize(pool_allocator);

    for (int i = 0; i < get_total_capacity(pool_allocator); ++i) {
        ptr[i] = pool_alloc(pool_allocator);
        CU_ASSERT_PTR_NOT_NULL_FATAL(ptr[i]);
        memset(ptr[i], 0xBB, chunk_size);
    }

    for (int i = 0; i < get_total_capacity(pool_allocator); ++i) {
        CU_ASSERT_FATAL(pool_free(pool_allocator, ptr[i]) == 0);
    }

#if defined(PRINT_MEM) && defined(TEST)
    print_structure(pool_allocator);
#endif
}

/* Создание экземпляра, выделение всех блоков, освобождение блоков, не входящих в данный адрес
    блоков не являющихся началом блоков, повторное освобождение блоков */

/* Тест с невыравненым размером чанков, с невыравненым адресом памяти */

int main(int argc, char const *argv[]) {
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("common_suilt", setUp, tearDown);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "allocating one chunk of memory", test_alloc_1_chunk)) ||
        (NULL == CU_add_test(pSuite, "allocate all memory", test_alloc_all_chunks))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
