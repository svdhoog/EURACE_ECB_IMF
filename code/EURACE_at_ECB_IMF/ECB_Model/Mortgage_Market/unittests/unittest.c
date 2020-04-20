/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../../header.h"
#include <CUnit/Basic.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>


//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************/

/* Household functions */
void unittest_Household_function();

void unittest_Household_init_mortgages1();
void unittest_Household_init_mortgages2();
void unittest_Household_init_mortgages3();

void unittest_Household_check_mortgage_conditions();
void unittest_Household_service_mortgage_debt1();
void unittest_Household_service_mortgage_debt2();
void unittest_Household_service_mortgage_debt3();
void unittest_Household_service_mortgage_debt4();

void unittest_Household_npl_mortgage_debt();

void unittest_Household_writeoff_mortgage_debt_1();
void unittest_Household_writeoff_mortgage_debt_2();

void unittest_Household_request_mortgage_loan();
void unittest_Household_mortgage_loan_application_1();
void unittest_Household_mortgage_loan_application_2();

void unittest_Household_receive_mortgage_loan_1();
void unittest_Household_receive_mortgage_loan_2();


/* Household aux functions */
void unittest_recompute_mortgage_attributes();
void unittest_recompute_mortgage_attributes_2();

void unittest_recompute_mortgage_attributes_including_arrears();
void unittest_recompute_mortgage_attributes_including_arrears_1();
void unittest_recompute_mortgage_attributes_including_arrears_2();

/* Bank functions */
void unittest_Bank_init_mortgages();
void unittest_Bank_read_mortgage_messages_1();
void unittest_Bank_read_mortgage_messages_2();

void unittest_Bank_receive_mortgage_payments_1();
void unittest_Bank_receive_mortgage_payments_2();

void unittest_Bank_process_mortgage_requests_1();
//void unittest_Bank_process_mortgage_requests_2();

void unittest_Bank_check_recapitalization();

/* Aux functions: Bank_read_mortgage_defaults*/
void unittest_Bank_set_pd_lgd_daily_firm_loans();
void unittest_Bank_set_pd_lgd_quarterly_firm_loans();
void unittest_Bank_set_pd_lgd_yearly_firm_loans();

void unittest_Bank_set_pd_lgd_monthly_household_loans();
void unittest_Bank_set_pd_lgd_quarterly_household_loans();
void unittest_Bank_set_pd_lgd_yearly_household_loans();


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
    //NULL == CU_add_test(pSuite, "recompute_mortgage_attributes", unittest_recompute_mortgage_attributes) //||
    //NULL == CU_add_test(pSuite, "recompute_mortgage_attributes 2", unittest_recompute_mortgage_attributes_2) //||
    //NULL == CU_add_test(pSuite, "recompute_mortgage_attributes_including_arrears", unittest_recompute_mortgage_attributes_including_arrears) //||
    NULL == CU_add_test(pSuite, "recompute_mortgage_attributes_including_arrears_1", unittest_recompute_mortgage_attributes_including_arrears_1) //||
    //NULL == CU_add_test(pSuite, "recompute_mortgage_attributes_including_arrears 2", unittest_recompute_mortgage_attributes_including_arrears_2) //||

    //NULL == CU_add_test(pSuite, "Household_init_mortgages1", unittest_Household_init_mortgages1)) //||
	//NULL == CU_add_test(pSuite, "Household_init_mortgages2", unittest_Household_init_mortgages2)) //||
    //NULL == CU_add_test(pSuite, "Household_init_mortgages3", unittest_Household_init_mortgages3) ) //||
/*
	CU_add_test(pSuite, "Household_check_mortgage_conditions", unittest_Household_check_mortgage_conditions) == NULL ||
	CU_add_test(pSuite, "Household_service_mortgage_debt_1", unittest_Household_service_mortgage_debt1) == NULL ||        	
	CU_add_test(pSuite, "Household_service_mortgage_debt_2", unittest_Household_service_mortgage_debt2) == NULL ||        	    
*/	
//    CU_add_test(pSuite, "Household_service_mortgage_debt_3", unittest_Household_service_mortgage_debt3) == NULL) //||
//    CU_add_test(pSuite, "Household_service_mortgage_debt_4", unittest_Household_service_mortgage_debt4) == NULL) //||
    
//    CU_add_test(pSuite, "Household_npl_mortgage_debt", unittest_Household_npl_mortgage_debt) == NULL ||

//    CU_add_test(pSuite, "Household_writeoff_mortgage_debt_1", unittest_Household_writeoff_mortgage_debt_1) == NULL // ||
     // CU_add_test(pSuite, "Household_writeoff_mortgage_debt_2", unittest_Household_writeoff_mortgage_debt_2) == NULL // ||

//   	CU_add_test(pSuite, "Household_request_mortgage_loan", unittest_Household_request_mortgage_loan) == NULL ||    
     //CU_add_test(pSuite, "Household_mortgage_loan_application_1", unittest_Household_mortgage_loan_application_1) == NULL) // ||    
     //CU_add_test(pSuite, "Household_mortgage_loan_application_2", unittest_Household_mortgage_loan_application_2) == NULL) // ||    

/*
	CU_add_test(pSuite, "Household_receive_mortgage_loan_1", unittest_Household_receive_mortgage_loan_1) == NULL ||
    CU_add_test(pSuite, "Household_receive_mortgage_loan_2", unittest_Household_receive_mortgage_loan_2) == NULL) //||
*/
	//CU_add_test(pSuite, "Bank_init_mortgages", unittest_Bank_init_mortgages) == NULL ||
	
    //CU_add_test(pSuite, "Bank_read_mortgage_messages_1", unittest_Bank_read_mortgage_messages_1) == NULL // ||    
    //CU_add_test(pSuite, "Bank_read_mortgage_messages_2", unittest_Bank_read_mortgage_messages_2) == NULL // ||    
/*
	CU_add_test(pSuite, "Bank_receive_mortgage_payments_1", unittest_Bank_receive_mortgage_payments_1) == NULL ||
    CU_add_test(pSuite, "Bank_receive_mortgage_payments_2", unittest_Bank_receive_mortgage_payments_2) == NULL ||
*/
	
//    CU_add_test(pSuite, "Bank_process_mortgage_requests_1", unittest_Bank_process_mortgage_requests_1) == NULL) // ||    
//    CU_add_test(pSuite, "Bank_process_mortgage_requests_2", unittest_Bank_process_mortgage_requests_2) == NULL) //||    	
//    CU_add_test(pSuite, "Bank_check_recapitalization", unittest_Bank_check_recapitalization) == NULL) //||  
/*
	CU_add_test(pSuite, "Bank_set_pd_lgd_daily_firm_loans", unittest_Bank_set_pd_lgd_daily_firm_loans) == NULL ||
	CU_add_test(pSuite, "Bank_set_pd_lgd_quarterly_firm_loans", unittest_Bank_set_pd_lgd_quarterly_firm_loans) == NULL ||
	CU_add_test(pSuite, "Bank_set_pd_lgd_yearly_firm_loans", unittest_Bank_set_pd_lgd_yearly_firm_loans) == NULL ||

	CU_add_test(pSuite, "Bank_set_pd_lgd_monthly_household_loans", unittest_Bank_set_pd_lgd_monthly_household_loans) == NULL ||
	CU_add_test(pSuite, "Bank_set_pd_lgd_quarterly_household_loans", unittest_Bank_set_pd_lgd_quarterly_household_loans) == NULL ||
	CU_add_test(pSuite, "Bank_set_pd_lgd_yearly_household_loans", unittest_Bank_set_pd_lgd_yearly_household_loans) == NULL ) //||
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
