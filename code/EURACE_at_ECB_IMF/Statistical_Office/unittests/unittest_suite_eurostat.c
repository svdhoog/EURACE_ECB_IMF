/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../header.h"
#include <CUnit/Basic.h>
#include <gsl/gsl_rng.h>

//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************/
void unittest_asserts();

void unittest_Eurostat_compute_house_price_index();

void unittest_Eurostat_reset_data();
void unittest_Eurostat_compute_mean_price();

void unittest_Eurostat_read_firm_data();

void unittest_Eurostat_compute_region_firm_data();
void unittest_Eurostat_compute_global_firm_data();

void unittest_Eurostat_compute_subsistence_level();

void unittest_Eurostat_read_household_data();
void unittest_Eurostat_read_household_data_2();

void unittest_Eurostat_compute_region_household_data();
void unittest_Eurostat_compute_global_household_data_1();
void unittest_Eurostat_compute_global_household_data_2();

void unittest_Eurostat_calculate_data();

void unittest_Eurostat_calc_macro_data_1();
void unittest_Eurostat_calc_macro_data_2();

void unittest1_Eurostat_measure_recession();
void unittest2_Eurostat_measure_recession();
void unittest3_Eurostat_measure_recession();
void unittest4_Eurostat_measure_recession();


void unittest_sum_array();
void unittest_sum_array_backwards();
void unittest_ratio_qoq();
void unittest_ratio_yoy();
void unittest_pct_change_qoq();
void unittest_pct_change_yoy();

void unittest_compute_statistics_economy();
void unittest_compute_statistics_regions();
void unittest_update_timeseries_economy();
void unittest_update_timeseries_regions();
void unittest_Eurostat_compute_timeseries_statistics();

void unittest_Eurostat_compute_macro_indicators();


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
/*
        NULL == CU_add_test(pSuite, "unittest_asserts", unittest_asserts) ||

        NULL == CU_add_test(pSuite, "Eurostat_compute_house_price_index", unittest_Eurostat_compute_house_price_index) ||
     	NULL == CU_add_test(pSuite, "Eurostat_reset_data", unittest_Eurostat_reset_data) ||

     	NULL == CU_add_test(pSuite, "Eurostat_compute_mean_price", unittest_Eurostat_compute_mean_price) ||
     	NULL == CU_add_test(pSuite, "Eurostat_read_firm_data", unittest_Eurostat_read_firm_data)  ||
     	NULL == CU_add_test(pSuite, "Eurostat_compute_region_firm_data", unittest_Eurostat_compute_region_firm_data) ||
     	NULL == CU_add_test(pSuite, "Eurostat_compute_global_firm_data", unittest_Eurostat_compute_global_firm_data) ||

        NULL == CU_add_test(pSuite, "Eurostat_compute_subsistence_level", unittest_Eurostat_compute_subsistence_level) ||
     	NULL == CU_add_test(pSuite, "Eurostat_read_household_data", unittest_Eurostat_read_household_data) ||
        NULL == CU_add_test(pSuite, "Eurostat_read_household_data_2", unittest_Eurostat_read_household_data_2) ||

     	NULL == CU_add_test(pSuite, "Eurostat_compute_region_household_data", unittest_Eurostat_compute_region_household_data) ||
        NULL == CU_add_test(pSuite, "Eurostat_compute_global_household_data 1", unittest_Eurostat_compute_global_household_data_1) // ||
*/
        NULL == CU_add_test(pSuite, "Eurostat_compute_global_household_data 2", unittest_Eurostat_compute_global_household_data_2) // ||
/*
     	NULL == CU_add_test(pSuite, "Eurostat_calc_macro_data 1", unittest_Eurostat_calc_macro_data_1) ||
        NULL == CU_add_test(pSuite, "Eurostat_calc_macro_data 2", unittest_Eurostat_calc_macro_data_2) ||        
    	
    	NULL == CU_add_test(pSuite, "Eurostat_measure_recession 1: Detect recession", unittest1_Eurostat_measure_recession) ||
    	NULL == CU_add_test(pSuite, "Eurostat_measure_recession 2: Recession in progress, duration updated by 1, recession continues", unittest2_Eurostat_measure_recession) ||
    	NULL == CU_add_test(pSuite, "Eurostat_measure_recession 3: Recession in progress, duration updated by 1, recession stops", unittest3_Eurostat_measure_recession) ||
    	NULL == CU_add_test(pSuite, "Eurostat_measure_recession 4: No recession occurs", unittest4_Eurostat_measure_recession) ||

       NULL == CU_add_test(pSuite, "sum_array", unittest_sum_array) ||
       NULL == CU_add_test(pSuite, "sum_array_backwards", unittest_sum_array_backwards) ||

        NULL == CU_add_test(pSuite, "ratio_qoq", unittest_ratio_qoq) ||
        NULL == CU_add_test(pSuite, "ratio_yoy", unittest_ratio_yoy) ||
        NULL == CU_add_test(pSuite, "pct_change_qoq", unittest_pct_change_qoq) ||
        NULL == CU_add_test(pSuite, "pct_change_yoy", unittest_pct_change_yoy) ||

        NULL == CU_add_test(pSuite, "compute_statistics_economy", unittest_compute_statistics_economy) ||
        NULL == CU_add_test(pSuite, "compute_statistics_regions", unittest_compute_statistics_regions) ||
        NULL == CU_add_test(pSuite, "update_timeseries_economy", unittest_update_timeseries_economy) ||
        NULL == CU_add_test(pSuite, "update_timeseries_regions", unittest_update_timeseries_regions) ||
        NULL == CU_add_test(pSuite, "Eurostat_compute_timeseries_statistics", unittest_Eurostat_compute_timeseries_statistics) ||
        NULL == CU_add_test(pSuite, "Eurostat_compute_macro_indicators", unittest_Eurostat_compute_macro_indicators)
*/
    )
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
