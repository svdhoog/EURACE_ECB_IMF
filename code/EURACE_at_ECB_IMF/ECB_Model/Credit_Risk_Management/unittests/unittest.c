/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../../header.h"
#include <CUnit/Basic.h>
#include <gsl/gsl_rng.h>

//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************/

void unittest_Bank_set_pd_lgd_yearly_household_loans();


void unittest_Bank_set_risk_weight_household_loans_00();
void unittest_Bank_set_risk_weight_household_loans_01();
void unittest_Bank_set_risk_weight_household_loans_02();

void unittest_Bank_set_risk_weight_household_loans_1();
void unittest_Bank_set_risk_weight_household_loans_2();
void unittest_Bank_set_risk_weight_household_loans_3();
void unittest_Bank_set_risk_weight_household_loans_4();
void unittest_Bank_set_risk_weight_household_loans_5();
void unittest_Bank_set_risk_weight_household_loans_6();

void unittest_Bank_update_IRB_shortfall_firm_loans_1();
void unittest_Bank_update_IRB_shortfall_firm_loans_2();
void unittest_Bank_update_IRB_shortfall_firm_loans_3();

void unittest_Bank_update_IRB_shortfall_mortgage_loans_1();
void unittest_Bank_update_IRB_shortfall_mortgage_loans_2();
void unittest_Bank_update_IRB_shortfall_mortgage_loans_3();
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
	/******* GSL INIT */
	const gsl_rng_type * T;

	gsl_rng_env_setup();

	//returns a pointer to a newly-created instance of gsl_rng
	T = gsl_rng_default;

	FLAME_GSL_RNG = gsl_rng_alloc (T);

	//Init on time
	unsigned long int gsl_seed = (unsigned long int)time(NULL); 
		
	gsl_rng_set(FLAME_GSL_RNG, gsl_seed);
	/******* END GSL INIT */

    CU_pSuite pSuite = NULL;
    
    /* Init FLAME */
    initialise_unit_testing();
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_Bank_unittests", init_suite1, clean_suite1);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* add extra tests using || */
    
    if(
//    CU_add_test(pSuite, "unittest_Bank_set_pd_lgd_yearly_household_loans", unittest_Bank_set_pd_lgd_yearly_household_loans) == NULL //||
/*
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_00", unittest_Bank_set_risk_weight_household_loans_00) == NULL ||
	CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_01", unittest_Bank_set_risk_weight_household_loans_01) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_02", unittest_Bank_set_risk_weight_household_loans_02) == NULL ||
*/

/*
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_1", unittest_Bank_set_risk_weight_household_loans_1) == NULL //||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_2", unittest_Bank_set_risk_weight_household_loans_2) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_3", unittest_Bank_set_risk_weight_household_loans_3) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_4", unittest_Bank_set_risk_weight_household_loans_4) == NULL //||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_5", unittest_Bank_set_risk_weight_household_loans_5) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_set_risk_weight_household_loans_6", unittest_Bank_set_risk_weight_household_loans_6) == NULL //||
*/
    CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_firm_loans Test 1", unittest_Bank_update_IRB_shortfall_firm_loans_1) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_firm_loans Test 2", unittest_Bank_update_IRB_shortfall_firm_loans_2) == NULL ||

    CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_mortgage_loans Test 3", unittest_Bank_update_IRB_shortfall_mortgage_loans_1) == NULL ||
    CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_mortgage_loans Test 4", unittest_Bank_update_IRB_shortfall_mortgage_loans_2) == NULL //||
/*
    //CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_firm_loans Test 5", unittest_Bank_update_IRB_shortfall_firm_loans_3) == NULL //||
    //CU_add_test(pSuite, "unittest_Bank_update_IRB_shortfall_mortgage_loans Test 6", unittest_Bank_update_IRB_shortfall_mortgage_loans_3) == NULL //||
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
