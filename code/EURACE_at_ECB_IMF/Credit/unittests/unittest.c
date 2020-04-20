/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../header.h"
#include <CUnit/Basic.h>
#include <gsl/gsl_rng.h>

//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************
void unittest_Bank_read_loan_request_send_offers();
void unittest_Bank_read_interest_payments();
void unittest_Bank_read_debt_installment_payments();
/*************************** end prototypes ***************************/

//void unittest_Bank_decide_credit_conditions();
void unittest1_Firm_request_loans();
void unittest2_Firm_request_loans();
void unittest_Bank_decide_credit_conditions();
void unittest_Bank_receive_installment();
void unittest_Bank_accounting_1();
void unittest_Bank_accounting_2();
void unittest_Bank_accounting_3();

void unittest_Bank_send_data_to_Eurostat();

void unittest_Bank_read_policy_rate();
void unittest_Central_Bank_monetary_policy();

void unittest_Central_Bank_read_recapitalization_requests();

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
    	//NULL == CU_add_test(pSuite, "Firm_request_loans 1", unittest1_Firm_request_loans) == NULL)) //||
    	//NULL == CU_add_test(pSuite, "Firm_request_loans 2", unittest2_Firm_request_loans) == NULL)) //||    	
        //NULL == CU_add_test(pSuite, "Bank_decide_credit_conditions", unittest_Bank_decide_credit_conditions)) //||
        //NULL == CU_add_test(pSuite, "Bank_receive_installment", unittest_Bank_receive_installment)) //||
        //NULL == CU_add_test(pSuite, "Bank_accounting_1", unittest_Bank_accounting_1) ||
        //NULL == CU_add_test(pSuite, "Bank_accounting_2", unittest_Bank_accounting_2) ||
        //NULL == CU_add_test(pSuite, "Bank_accounting_3", unittest_Bank_accounting_3)) //||
        NULL == CU_add_test(pSuite, "Bank_send_data_to_Eurostat", unittest_Bank_send_data_to_Eurostat)) //||

        //NULL == CU_add_test(pSuite, "Bank_read_policy_rate", unittest_Bank_read_policy_rate)) //||

        //NULL == CU_add_test(pSuite, "Central_Bank_monetary_policy", unittest_Central_Bank_monetary_policy)) //||

        //NULL == CU_add_test(pSuite, "Central_Bank_read_recapitalization_requests", unittest_Central_Bank_read_recapitalization_requests)) //||
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
