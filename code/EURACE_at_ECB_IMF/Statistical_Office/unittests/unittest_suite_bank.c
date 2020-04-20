/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../header.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/*************************** unittest prototypes ***************************/

void unittest_asset_liability_add();
void unittest_asset_liability_subtract();
void unittest_liability_transformation();
void unittest_asset_transformation();

/*************************** end prototypes ***************************/

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{
    CU_pSuite pSuite = NULL;
    
    /* Init FLAME */
    initialise_unit_testing();
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_Eurostat_unittests", init_suite1, clean_suite1);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* add extra tests using || */
    
    if(
           NULL == CU_add_test(pSuite, "asset_liability_add", unittest_asset_liability_add) ||
           NULL == CU_add_test(pSuite, "asset_liability_subtract", unittest_asset_liability_subtract) ||
           NULL == CU_add_test(pSuite, "liability_transformation", unittest_liability_transformation) ||
           NULL == CU_add_test(pSuite, "asset_transformation", unittest_asset_transformation) )
        {
            CU_cleanup_registry();
            return CU_get_error();
        }
        
        /* Run all tests using the CUnit Basic interface */
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
        
        /* Free FLAME */
        clean_up(0);
        
        return CU_get_error();
    }
