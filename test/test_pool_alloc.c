/**
 *******************************************************************************
 * @file        test_pool_alloc.c
 * @author      Gevorgyan Ovik
 * @brief       Unit tests
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "CUnit/Basic.h"
#include "pool_alloc.h"

/* Private define ------------------------------------------------------------*/
#define NUM_OF_CHUNKS (MEM_SIZE / CHUNK_SIZE)

int setUp(void) {
    return 0;
}

int tearDown(void) {
    /// @todo Проверка что блоки освобождены
    return 0;
}

/* Создание экземпляра, выделение блока, освобождение блока */
void test_alloc_1_chunk(void) {
    PoolAlloc_t *pool_allocator = NULL;
    void *ptr;

    CU_ASSERT_FATAL(init_alloc(pool_allocator) == 0);
    ptr = pool_alloc(pool_allocator);

    CU_ASSERT_PTR_NULL_FATAL(ptr);
    CU_ASSERT_FATAL(pool_free(pool_allocator, ptr) == 0);
}

/* Создание экземпляра, выделение всех блоков, выделение еще блоков, освобождение блоков */
void test_alloc_all_chunks(void) {
    PoolAlloc_t *pool_allocator = NULL;
    void *ptr[NUM_OF_CHUNKS];

    CU_ASSERT_FATAL(init_alloc(pool_allocator) == 0);

    for (int i = 0; i < NUM_OF_CHUNKS; ++i)
    {
        ptr[i] = pool_alloc(pool_allocator);
        CU_ASSERT_PTR_NULL_FATAL(ptr[i]);
    }

    CU_ASSERT_FATAL(init_alloc(pool_allocator) != 0);

    for (int i = 0; i < NUM_OF_CHUNKS; ++i)
    {
        CU_ASSERT_FATAL(pool_free(pool_allocator, ptr[i]) == 0);
    }
}

/* Создание экземпляра, выделение всех блоков, освобождение блоков, не входящих в данный адрес
    блоков не являющихся началом блоков, повторное освобождение блоков */

int main(int argc, char const *argv[])
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("common_suilt", setUp, tearDown);
    if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "allocating one chunk of memory", test_alloc_1_chunk)) ||
       (NULL == CU_add_test(pSuite, "allocate all memory", test_alloc_all_chunks))
       )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
