#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"
#include "../Household_aux_headers.h"
#include "../../Housing_Market/centralized_auction/Household_aux_header.h"
#include "../../../Sanity_Checks/sanity_checks_aux_headers.h"

/*
 * 1) initialize agent
 * 2) add memory pre-conditions
 * 3) initialise MBs
 * 4) add message pre-conditions
 * 5) create MB iterators
 * 6) evaluate function to be tested
 * 7) assertions on memory post-conditions
 * 8) assertions on message post-conditions
 * 9) free agent
 * 10) free MBs
 */

/*
 * \fn: void unittest_Household_function()
 * \brief: Unit test for: Household_function.
 * Status: NOT Tested
 */
//void unittest_Household_()
//{
	
    /************* At start of unit test, add one agent **************/
//	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
/*
	MB_Create(&b_<message>, sizeof(m_<message>));
*/	
	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Function evaluation ***************************************/
//	<Agentname>_function();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
//	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}


/*
 * \fn: void unittest_Household_init_mortgages1()
 * \brief: Unit test for: unittest_Household_init_mortgages.
 * \brief: Status: PASS 3.11.16
 */
void unittest_Household_init_mortgages1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_info, sizeof(m_mortgage_info));
	
	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_info, &i_mortgage_info);

    /***** Function evaluation ***************************************/
	Household_init_mortgages();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

	//Check avg duration
	double sum, weights;
	sum=100.0;
	weights=1.0;
	
	double avg_duration = (1/3.0)*sum/weights;
	int    avg_initial_duration = (int) ((1/3.0)*sum/weights);
	
	printf("\n\n sum=%f weights=%f avg_initial_duration=%f avg_initial_duration=%d\n", sum, weights, avg_duration, avg_initial_duration);
	
	CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
	CU_ASSERT_EQUAL(avg_initial_duration, 33);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_init_mortgages2()
 * \brief: Unit test for: unittest_Household_init_mortgages.
 * \brief: Status: PASS 3.11.16
 */
void unittest_Household_init_mortgages2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/

    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	
	mortgage.initial_duration = 1;
	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 0;
	mortgage.outstanding_principal = 0.0;
	mortgage.monthly_total_repayment = 0.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 0.0;

	//copy into memvar
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage);
    
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_info, sizeof(m_mortgage_info));
	
	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Household_init_mortgages();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_info, &i_mortgage_info);

	/***** Messages: Message post-conditions *****/
	START_MORTGAGE_INFO_MESSAGE_LOOP
		CU_ASSERT_EQUAL(mortgage_info_message->mortgage.household_id, 1);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_info_message->mortgage.outstanding_principal, 10.0, 1e-3);
	FINISH_MORTGAGE_INFO_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_init_mortgages3()
 * \brief: Unit test for: unittest_Household_init_mortgages.
 * \brief: Status: PASS 7.12.2016
 */
void unittest_Household_init_mortgages3()
{
	int i;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_mortgage_hh = 1;

    //For avg_initial_duration > 0
    //sum(MORTGAGE_SAMPLE_INPUTS[i].sampled_outstanding_principal * MORTGAGE_SAMPLE_INPUTS[i].sampled_initial_duration)
    //  / sum(MORTGAGE_SAMPLE_INPUTS[i].sampled_outstanding_principal)
    
    MEAN_NET_INCOME = 100.0;
    SAMPLE_INPUTS.sampled_total_gross_annual_income = 100.0;


    MORTGAGE_SAMPLE_INPUTS[0].sampled_outstanding_principal = 1.0;
    MORTGAGE_SAMPLE_INPUTS[1].sampled_outstanding_principal = 0.0;
    MORTGAGE_SAMPLE_INPUTS[2].sampled_outstanding_principal = 0.0;
    
    MORTGAGE_SAMPLE_INPUTS[0].sampled_initial_duration = 10;
    MORTGAGE_SAMPLE_INPUTS[1].sampled_initial_duration = 0;
    MORTGAGE_SAMPLE_INPUTS[2].sampled_initial_duration = 0;
    
    if (MORTGAGE_SAMPLE_INPUTS[0].sampled_initial_duration >0) MORTGAGE_SAMPLE_INPUTS[0].sampled_monthly_total_repayment = MORTGAGE_SAMPLE_INPUTS[0].sampled_outstanding_principal / MORTGAGE_SAMPLE_INPUTS[0].sampled_initial_duration;
    else MORTGAGE_SAMPLE_INPUTS[0].sampled_monthly_total_repayment = 0.0;

    if (MORTGAGE_SAMPLE_INPUTS[1].sampled_initial_duration >0) MORTGAGE_SAMPLE_INPUTS[1].sampled_monthly_total_repayment = MORTGAGE_SAMPLE_INPUTS[1].sampled_outstanding_principal / MORTGAGE_SAMPLE_INPUTS[1].sampled_initial_duration;
    else MORTGAGE_SAMPLE_INPUTS[1].sampled_monthly_total_repayment = 0.0;

    if (MORTGAGE_SAMPLE_INPUTS[2].sampled_initial_duration >0) MORTGAGE_SAMPLE_INPUTS[2].sampled_monthly_total_repayment = MORTGAGE_SAMPLE_INPUTS[2].sampled_outstanding_principal / MORTGAGE_SAMPLE_INPUTS[2].sampled_initial_duration;
    else MORTGAGE_SAMPLE_INPUTS[2].sampled_monthly_total_repayment = 0.0;


    //For initial_loan_to_value:
	MORTGAGE_SAMPLE_INPUTS[0].sampled_initial_principal = 1.0;
	MORTGAGE_SAMPLE_INPUTS[1].sampled_initial_principal = 1.0;
	MORTGAGE_SAMPLE_INPUTS[2].sampled_initial_principal = 1.0;

	MORTGAGE_SAMPLE_INPUTS[0].sampled_value_at_acquisition = 6.0;

	/***** Messages: initialize message boards **********************************/
	
	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Household_init_mortgages();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/

	/***** Messages: Message post-conditions *****/

	printf("\n HMR initial duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration, 30);

	printf("\n HMR current duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration, 10);

	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_check_mortgage_conditions()
 * \brief: Unit test for: unittest_Household_check_mortgage_conditions.
 * \brief: OUTPUT mortgage_arrears_message(ID, bank_id, loan_loss, loan_default, arrears_counter);
 * 
 * Status: PASS 6.12.16
 */
void unittest_Household_check_mortgage_conditions()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=1;
    OP_MORTGAGE_STATUS=1;

/*    
    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	mortgage.initial_duration = 2;
	mortgage.outstanding_principal = 20.0;

	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 2;
	
	mortgage.monthly_total_repayment = 11.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 1.0;

	mortgage.arrears_counter = 2;
	mortgage.npl_counter = 0;
	mortgage.status = 1;

			
	//copy into memvar
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage);
*/

	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;
	
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
	

	
	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 0;

	int arrears_counter = 0;
	int npl_counter = 1;
	int status = 2;
	int bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id;
	double loan_loss	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal;
	double loan_default	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal;

	
	printf("\n\n After fixture:\n"); 
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);

	
	/***** Messages: initialize message boards **********************************/
	rc = MB_Create(&b_mortgage_arrears, sizeof(m_mortgage_arrears));
	
	/***** Messages: pre-conditions **********************************/
	//add_mortgage_arrears_message(ID, 1, 1.0, 1.0, 0);
	//add_mortgage_arrears_message(0, bank_id, loan_loss, loan_default, arrears_counter);


    /***** Function evaluation ***************************************/
	Household_check_mortgage_conditions();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


	/***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_mortgage_arrears, &i_mortgage_arrears);

	/***** Messages: Message post-conditions *****/

	START_MORTGAGE_ARREARS_MESSAGE_LOOP
		printf("\n\t mortgage_arrears_message->mortgage.household_id=%d\n", mortgage_arrears_message->mortgage.household_id);
		printf("\n\t mortgage_arrears_message->mortgage.arrears_counter=%d\n", mortgage_arrears_message->mortgage.arrears_counter);
	
		CU_ASSERT_EQUAL(mortgage_arrears_message->mortgage.household_id, 1);
		CU_ASSERT_EQUAL(mortgage_arrears_message->mortgage.arrears_counter, 3);
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP
	
	printf("\n\n After running function:\n"); 

	//Accounting HMR mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter, 3);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status, 1);
	
	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter);	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter, 3);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status, 1);

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_service_mortgage_debt1()
 * \brief: Unit test for: unittest_Household_service_mortgage_debt.
 * \brief: Test 1: HH can pay both mortgage payments: PAYMENT_ACCOUNT > principal + interest  (for both HMR and OP)
 * \brief: OUTPUT mortgage_payment_message(ID, bank_id, principal, interest);
 * Status: PASS 6.12.16
 */
void unittest_Household_service_mortgage_debt1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;
    
    PAYMENT_ACCOUNT = 22.1;

/*
    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	mortgage.initial_duration = 2;

	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 2;
	mortgage.outstanding_principal = 20.0;
	mortgage.monthly_total_repayment = 11.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 1.0;

	//copy into memvar
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
	copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage);
*/

	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;

	
	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 0;

	
	
	printf("\n\n After fixture:\n"); 
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Household_service_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

	/***** Messages: Message post-conditions *****/

	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		printf("\n\t mortgage_payment_message->mortgage.household_id=%d\n", mortgage_payment_message->mortgage.household_id);
		printf("\n\t mortgage_payment_message->mortgage.monthly_principal=%2.3f\n", mortgage_payment_message->mortgage.monthly_principal);
		printf("\n\t mortgage_payment_message->mortgage.monthly_interest=%2.3f\n", mortgage_payment_message->mortgage.monthly_interest);
	
		CU_ASSERT_EQUAL(mortgage_payment_message->mortgage.household_id, 1);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_payment_message->mortgage.monthly_principal, 10.0, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_payment_message->mortgage.monthly_interest, 1.0, 1e-3);
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	
	printf("\n\n After running function:\n"); 

	//Accounting HMR mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration, 1);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal, 10.0, 1e-3);

	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration, 1);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal, 10.0, 1e-3);


    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_service_mortgage_debt2()
 * \brief: Unit test for: unittest_Household_service_mortgage_debt.
 * \brief: Test 1: HH cannot pay both mortgage payments: PAYMENT_ACCOUNT < principal + interest (for both HMR and OP)
 * \brief: OUTPUT mortgage_payment_message(ID, bank_id, principal, interest);
 * Status: PASS 6.12.16
 */
void unittest_Household_service_mortgage_debt2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;
    
    PAYMENT_ACCOUNT = 0.1;

/*
    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	mortgage.initial_duration = 2;

	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 2;
	mortgage.outstanding_principal = 20.0;
	mortgage.monthly_total_repayment = 11.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 1.0;

	//copy into memvar
	copy_mortgage_contract_adt(&(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage), &mortgage);
	copy_mortgage_contract_adt(&(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property.array[0]), &mortgage);
*/

	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
	
	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 0;

	
	
	printf("\n\n After fixture:\n"); 
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Household_service_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

	/***** Messages: Message post-conditions *****/

	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		printf("\n\t mortgage_payment_message->mortgage.household_id=%d\n", mortgage_payment_message->mortgage.household_id);
		printf("\n\t mortgage_payment_message->mortgage.monthly_principal=%2.3f\n", mortgage_payment_message->mortgage.monthly_principal);
		printf("\n\t mortgage_payment_message->mortgage.monthly_interest=%2.3f\n", mortgage_payment_message->mortgage.monthly_interest);
	
		CU_ASSERT_EQUAL(mortgage_payment_message->mortgage.household_id, 1);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_payment_message->mortgage.monthly_principal, 10.0, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_payment_message->mortgage.monthly_interest, 1.0, 1e-3);
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	
	printf("\n\n After running function:\n"); 

	//Accounting HMR mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration, 2);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal, 20.0, 1e-3);

	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration, 2);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal, 20.0, 1e-3);


    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}



/*
 * \fn: void unittest_Household_service_mortgage_debt3()
 * \brief: Unit test for: unittest_Household_service_mortgage_debt.
 * \brief: Test 3: 
 * \brief: - PAYMENT_ACCOUNT = 110.1
 * \brief: - HH cannot pay all OP mortgage payments: total_monthly_debt_service > PAYMENT_ACCOUNT
 * \brief: - HMR mortgage, monthly_total_repayment = 100.0
 * \brief: - OP [0] mortgage, monthly_total_repayment = 11.0
 * \brief: - OP [1] mortgage, monthly_total_repayment = 10.0
 * \brief: RESULT:
 * \brief: - HMR is paid
 * \brief: - OP [0] enters arrears
 * \brief: - OP [1] is paid
 * \brief: OUTPUT
 * \brief: add_mortgage_payment_message(ID, bank_id, loan_loss, loan_default, principal_op, interest_op);
 * \brief: add_mortgage_arrears_message(ID, bank_id, loan_loss, loan_default, arrears_counter);
 * Status: PASS 29.3.2017
 */
void unittest_Household_service_mortgage_debt3()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_mortgage_hh = 1;
    FLAME_environment_variable_print_debug_mortgage_hh_unittest = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;
    
    PAYMENT_ACCOUNT = 110.1;

    //set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 990.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest = 10.0;
	
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 100.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 99.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;

	//set some values
	//add_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.other_property.mortgage, 
	//0, 1, 2, 2, 0.0,	2,	100.0,	10.0,	11.0,	10.0,	1.0,	0,	0,	0);

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.object_id = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 100.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_interest = 10.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 0;

	//set some values
	//add_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property, 
	//0, 1,	2,	2,	0.0, 2,		80.0,	20.0,	11.0,	10.0,	1.0,	0,	0,	0);
		
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.object_id = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.bank_id = 2;
	
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.current_duration = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal = 80.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_interest = 20.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_principal = 8.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_interest = 2.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.npl_counter = 0;


	
	printf("\n\n After fixture:\n"); 
	
	printf("\n\t PAYMENT_ACCOUNT=%2.3f\n", PAYMENT_ACCOUNT);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	MB_Create(&b_mortgage_arrears, sizeof(m_mortgage_arrears));

    /***** Message: Adding message iterators ***************************************/

	/***** Messages: pre-conditions **********************************/

    /***** Function evaluation ***************************************/
	Household_service_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);
	MB_Iterator_Create(b_mortgage_arrears, &i_mortgage_arrears);

	/***** Messages: Message post-conditions *****/

	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		printf("\n\t MORTGAGE_PAYMENT_MESSAGE_LOOP:");
		printf("\n\t mortgage_payment_message->mortgage.household_id=%d\n", mortgage_payment_message->mortgage.household_id);
		printf("\n\t mortgage_payment_message->mortgage.monthly_principal=%2.3f\n", mortgage_payment_message->mortgage.monthly_principal);
		printf("\n\t mortgage_payment_message->mortgage.monthly_interest=%2.3f\n", mortgage_payment_message->mortgage.monthly_interest);
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	
	START_MORTGAGE_ARREARS_MESSAGE_LOOP
		printf("\n\t MORTGAGE_ARREARS_MESSAGE_LOOP:");
		printf("\n\t mortgage_arrears_message->mortgage.household_id=%d\n", mortgage_arrears_message->mortgage.household_id);
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP

	printf("\n\n After running function:\n"); 

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);

	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
//	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration, 2);
//	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal, 20.0, 1e-3);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status);
//	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.current_duration, 2);
//	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal, 20.0, 1e-3);



	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.1, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_PAYMENT_OP, 10.0, 1e-3);



    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_service_mortgage_debt4()
 * \brief: Unit test for: unittest_Household_service_mortgage_debt.
 * \brief: Test 3: 
 * \brief: - PAYMENT_ACCOUNT = 110.1
 * \brief: - HH cannot pay all OP mortgage payments: total_monthly_debt_service > PAYMENT_ACCOUNT
 * \brief: - OP [0] mortgage, monthly_total_repayment = 11.0
 * \brief: - OP [1] mortgage, monthly_total_repayment = 10.0
 * \brief: RESULT:
 * \brief: - OP [0] enters arrears
 * \brief: - OP [1] is paid
 * \brief: OUTPUT
 * \brief: add_mortgage_payment_message(ID, bank_id, loan_loss, loan_default, principal_op, interest_op);
 * \brief: add_mortgage_arrears_message(ID, bank_id, loan_loss, loan_default, arrears_counter);
 * Status: PASS 29.3.2017
 */
void unittest_Household_service_mortgage_debt4()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_mortgage_hh = 1;
    FLAME_environment_variable_print_debug_mortgage_hh_unittest = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;
    
    PAYMENT_ACCOUNT = 10.1;

	//set some values
	//add_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property, 
	//0, 1,	2,	2,	0.0,	2,	20.0,	20.0,	11.0,	10.0,	1.0,	0,	0,	0);

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 100.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_interest = 10.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 0;

	//set some values
	//add_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property, 
	//0, 1,	2,	2,	0.0,	2,	20.0,	20.0,	11.0,	10.0,	1.0,	0,	0,	0);
		
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.current_duration = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal = 80.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_interest = 20.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_principal = 8.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_interest = 2.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.arrears_counter = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.npl_counter = 0;


	
	printf("\n\n After fixture:\n"); 
	
	printf("\n\t PAYMENT_ACCOUNT=%2.3f\n", PAYMENT_ACCOUNT);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	MB_Create(&b_mortgage_arrears, sizeof(m_mortgage_arrears));

    /***** Message: Adding message iterators ***************************************/

	/***** Messages: pre-conditions **********************************/

    /***** Function evaluation ***************************************/
	Household_service_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);
	MB_Iterator_Create(b_mortgage_arrears, &i_mortgage_arrears);

	/***** Messages: Message post-conditions *****/

	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		printf("\n\t MORTGAGE_PAYMENT_MESSAGE_LOOP:");
		printf("\n\t mortgage_payment_message->mortgage.household_id=%d\n", mortgage_payment_message->mortgage.household_id);
		printf("\n\t mortgage_payment_message->mortgage.monthly_principal=%2.3f\n", mortgage_payment_message->mortgage.monthly_principal);
		printf("\n\t mortgage_payment_message->mortgage.monthly_interest=%2.3f\n", mortgage_payment_message->mortgage.monthly_interest);
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	
	START_MORTGAGE_ARREARS_MESSAGE_LOOP
		printf("\n\t MORTGAGE_ARREARS_MESSAGE_LOOP:");
		printf("\n\t mortgage_arrears_message->mortgage.household_id=%d\n", mortgage_arrears_message->mortgage.household_id);
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP


	printf("\n\n After running function:\n"); 

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);

	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
//	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration, 2);
//	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal, 20.0, 1e-3);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.monthly_total_repayment);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.status);
//	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.current_duration, 2);
//	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].mortgage.outstanding_principal, 20.0, 1e-3);



	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.1, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_PAYMENT_OP, 10.0, 1e-3);



    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_npl_mortgage_debt()
 * \brief: Unit test for: Household_npl_mortgage_debt.
 * \brief: OUTPUT mortgage_npl_message(ID, bank_id, );
 * Status: PASS 6.12.16
 */
void unittest_Household_npl_mortgage_debt()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=2;
    OP_MORTGAGE_STATUS=2;

/*
    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	mortgage.initial_duration = 2;

	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 2;
	mortgage.outstanding_principal = 20.0;
	mortgage.monthly_total_repayment = 11.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 1.0;

	//copy into memvar
	copy_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage, &mortgage);
	copy_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property.array[0], &mortgage);
*/

	//set some values: HMR is in arrears
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 2;

	//set some values: OP is in NPL
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 2;
	
	
	printf("\n\n After fixture:\n"); 
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter);	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter);	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);

    
	/***** Messages: initialize message boards **********************************/
	rc = MB_Create(&b_mortgage_npl, sizeof(m_mortgage_npl));
	    
	/***** Messages: pre-conditions **********************************/

    /***** Function evaluation ***************************************/
	Household_npl_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_mortgage_npl, &i_mortgage_npl);

    /***** Messages: Message post-conditions *****/
	int count=0;
	START_MORTGAGE_NPL_MESSAGE_LOOP
		count++;
		CU_ASSERT_EQUAL(mortgage_npl_message->mortgage.household_id, 1);
		CU_ASSERT_EQUAL(mortgage_npl_message->mortgage.bank_id, 2);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_npl_message->mortgage.loan_default, 20.0, 1e-3);
		CU_ASSERT_EQUAL(mortgage_npl_message->mortgage.npl_counter, 2);
	FINISH_MORTGAGE_NPL_MESSAGE_LOOP
	printf("\n\t Mesg: %d\n", count);

	printf("\n\n After running function:\n"); 

	//Accounting HMR mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter, 2);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status, 2);
	
	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter);	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter, 2);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status, 2);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_writeoff_mortgage_debt()
 * \brief: Unit test for: Household_writeoff_mortgage_debt.
 * \brief: Case: Only default on HMR mortgage, not on OP mortgage
 * \brief: OUTPUT mortgage_writeoff_message(ID, bank_id, bad_debt);
 * Status: PASS 6.12.16
 */
void unittest_Household_writeoff_mortgage_debt_1()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=1;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=3;
    OP_MORTGAGE_STATUS=3;

	PAYMENT_ACCOUNT = 10.0;

/*
    	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	//set some values
	mortgage.household_id = 1;
	mortgage.bank_id = 2;

	mortgage.initial_duration = 2;
	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 2;
	mortgage.outstanding_principal = 20.0;
	mortgage.monthly_total_repayment = 11.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 1.0;

	//copy into memvar
	copy_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage, &mortgage);
	copy_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgages_other_property.array[0], &mortgage);
*/

	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 3;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
	
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id	= 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.seller_id	= 11;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.buyer_id		= 12;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.price		= 1.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value		= 19.0;
	
	//set some values
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.household_id = 1;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.bank_id = 2;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.initial_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.interest_rate_annual = 0.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal = 20.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_total_repayment = 11.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_principal = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.monthly_interest = 1.0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status = 2;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter = 0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter = 2;
	
	printf("\n\n After fixture:\n"); 
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);

	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal=%2.3f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal);

    
	/***** Messages: initialize message boards **********************************/
	rc = MB_Create(&b_mortgage_writeoff, sizeof(m_mortgage_writeoff));

	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Household_writeoff_mortgage_debt();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);

    /***** Messages: Message post-conditions *****/

	//Prototype:
 	//add_mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default, seized_liquidity, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence);


	int count=0;
	START_MORTGAGE_WRITEOFF_MESSAGE_LOOP
		count++;
		CU_ASSERT_EQUAL(mortgage_writeoff_message->house.mortgage.household_id, 1);
		CU_ASSERT_EQUAL(mortgage_writeoff_message->house.mortgage.bank_id, 2);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_writeoff_message->house.mortgage.loan_default, 20.0, 1e-3);

		CU_ASSERT_EQUAL(mortgage_writeoff_message->house.object_id, 1);
		printf("\n\t object_id=%d", mortgage_writeoff_message->house.object_id);

	FINISH_MORTGAGE_WRITEOFF_MESSAGE_LOOP
	printf("\n\t Mesg: %d\n", count);

	printf("\n\n After running function:\n"); 

	//CHECK IF HMR MORTGAGE HAS BEEN RESET USING INIT:
	//Accounting HMR mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status, 0);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, 0);
	printf("\n\t object_id=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);


	//Accounting other property mortgage
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.current_duration);
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter);	
	printf("\n\t HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status=%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter, 2);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status, 2);

	//loan_loss=1.0, PAYMENT_ACCOUNT=10.0, so PAYMENT_ACCOUNT=9.0 after
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 9.0, 1e-3);
	
	printf("\n PAYMENT_ACCOUNT=%.2f\n", PAYMENT_ACCOUNT);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_writeoff_mortgage_debt()
 * \brief: Unit test for: Household_writeoff_mortgage_debt.
 * \brief: Test: cleaing up array in reverse, removing all elements/removing some elements
 * \brief: 
 * Status: NOT TESTED
 */
void unittest_Household_writeoff_mortgage_debt_2()
{
	int i,n;
	int_array A;
	init_int_array(&A);

	for (i=0; i<4; i++)
		add_int(&A, 2*i);

	printf("\n\nBefore:\nA=[");
	for (i=0; i<A.size; i++)
		printf(" %d", A.array[i]);
	printf("]");

	printf("\n\nPrint reversed:\nA=[");
	for (i=A.size-1; i>=0; i--)
		printf(" %d", A.array[i]);
	printf("]");

	printf("\n\nRemoving reversed.");
	n=A.size-1;
	for (i=n; i>=0; i--)
		remove_int(&A, i);



	printf("\n\nAfter:\nA=[");
	for (i=0; i<A.size; i++)
		printf(" %d", A.array[i]);
	printf("]");

	printf("\n\nReinitializing:\nA=[");
	for (i=0; i<4; i++)
		add_int(&A, i);

	for (i=0; i<A.size; i++)
		printf(" %d", A.array[i]);
	printf("]");

	printf("\n\nRemoving items if even");
	n=A.size-1;
	for (i=n; i>=0; i--)
		if (A.array[i]%2==0)
		{
			printf("\nRemoving: %d", A.array[i]);
			remove_int(&A, i);
		}
	
	printf("\n\nAfter:\nA=[");
	for (i=0; i<A.size; i++)
		printf(" %d", A.array[i]);
	printf("]");

}


/*
 * \fn: void unittest_Household_mortgage_loan_application()
 * \brief: Unit test for: Household_mortgage_loan_application.
 * Status: NOT Tested
 */
void unittest_Household_mortgage_loan_application_1()
{

    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;
    FLAME_environment_variable_print_debug_mortgage_hh = 1;
    FLAME_environment_variable_print_debug_mortgage_hh_unittest = 1;

	ID=1;

	MORTGAGE_SAMPLING_MEANS[0] = -1.7149;
	MORTGAGE_SAMPLING_MEANS[1] = -0.3952;
	MORTGAGE_SAMPLING_MEANS[2] = 3.0422;

    MORTGAGE_SAMPLING_COVAR[0] = 0.5412804112;
    MORTGAGE_SAMPLING_COVAR[1] = 0.0809859624;
    MORTGAGE_SAMPLING_COVAR[2] = 0.0329746046;
    MORTGAGE_SAMPLING_COVAR[3] = 0.0809859624;
    MORTGAGE_SAMPLING_COVAR[4] = 0.2892559524;
    MORTGAGE_SAMPLING_COVAR[5] = 0.0823459858;
    MORTGAGE_SAMPLING_COVAR[6] = 0.0329746046;
    MORTGAGE_SAMPLING_COVAR[7] = 0.0823459858;
    MORTGAGE_SAMPLING_COVAR[8] = 0.2043532423;

	MORTGAGE_SAMPLING_FLOOR_CAPS[0] = 0.05;
	MORTGAGE_SAMPLING_FLOOR_CAPS[1] = 0.2;
	MORTGAGE_SAMPLING_FLOOR_CAPS[2] = 10;
	MORTGAGE_SAMPLING_FLOOR_CAPS[3] = 0.60;
	MORTGAGE_SAMPLING_FLOOR_CAPS[4] = 1.2;
	MORTGAGE_SAMPLING_FLOOR_CAPS[5] = 40;


	MORTGAGE_SAMPLING_DRAW.dsti = 0.0;
	MORTGAGE_SAMPLING_DRAW.loan_to_value = 1.0;
	MORTGAGE_SAMPLING_DRAW.maturity = 30.0;


	MEAN_NET_INCOME  = 1e8;

 	TOTAL_GROSS_ANNUAL_INCOME =  1e9;

	PAYMENT_ACCOUNT = 1e6;

    /***** Messages: initialize message boards **********************************/
    MB_Create(&b_mortgage_request, sizeof(m_mortgage_request));
 
    /***** Messages: pre-conditions **********************************/
    //add_mortgage_request_message();
            
    /***** Message: Adding message iterators ***************************************/
    //add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
    //add_mortgage_request_message(ID, BANK_ID, mortgage);
    //MB_Iterator_Create(b_mortgage_request, &i_mortgage_request);

    /***** Function evaluation ***************************************/
    Household_mortgage_loan_application();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_request, &i_mortgage_request);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
	//add_mortgage_request_message(ID, BANK_ID, mortgage);

    START_MORTGAGE_REQUEST_MESSAGE_LOOP
         CU_ASSERT_EQUAL(mortgage_request_message->household_id, ID);
         CU_ASSERT_DOUBLE_EQUAL(mortgage_request_message->mortgage.sampled_loan_to_value, 1.0, 1e-3);
         CU_ASSERT_DOUBLE_EQUAL(mortgage_request_message->mortgage.sampled_maturity, 20.0, 1e-3);
    FINISH_MORTGAGE_REQUEST_MESSAGE_LOOP
  
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_mortgage_loan_application()
 * \brief: Unit test for: Household_mortgage_loan_application.
 * \brief: Numerical examples, see: ABM_at_ECB/docs/discussions/notes_implementation/mortgage loan application unit tests
 * Status: NOT Tested
 */
void unittest_Household_mortgage_loan_application_2()
{

    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;
    FLAME_environment_variable_print_debug_mortgage_hh = 1;
    FLAME_environment_variable_print_debug_mortgage_hh_unittest = 1;

	ID=1;

	MORTGAGE_SAMPLING_MEANS[0] = -1.7149;
	MORTGAGE_SAMPLING_MEANS[1] = -0.3952;
	MORTGAGE_SAMPLING_MEANS[2] = 3.0422;

    MORTGAGE_SAMPLING_COVAR[0] = 0.5412804112;
    MORTGAGE_SAMPLING_COVAR[1] = 0.0809859624;
    MORTGAGE_SAMPLING_COVAR[2] = 0.0329746046;
    MORTGAGE_SAMPLING_COVAR[3] = 0.0809859624;
    MORTGAGE_SAMPLING_COVAR[4] = 0.2892559524;
    MORTGAGE_SAMPLING_COVAR[5] = 0.0823459858;
    MORTGAGE_SAMPLING_COVAR[6] = 0.0329746046;
    MORTGAGE_SAMPLING_COVAR[7] = 0.0823459858;
    MORTGAGE_SAMPLING_COVAR[8] = 0.2043532423;

	MORTGAGE_SAMPLING_FLOOR_CAPS[0] = 0.05;
	MORTGAGE_SAMPLING_FLOOR_CAPS[1] = 0.2;
	MORTGAGE_SAMPLING_FLOOR_CAPS[2] = 10;
	MORTGAGE_SAMPLING_FLOOR_CAPS[3] = 0.60;
	MORTGAGE_SAMPLING_FLOOR_CAPS[4] = 1.2;
	MORTGAGE_SAMPLING_FLOOR_CAPS[5] = 40;

	//Here we enter some numerical values from the spreadsheet provided by our ECB colleagues
	MORTGAGE_SAMPLING_DRAW.dsti = 0.05;
	MORTGAGE_SAMPLING_DRAW.loan_to_value = 0.2;
	MORTGAGE_SAMPLING_DRAW.maturity = 25.0;

	MEAN_NET_INCOME  = 3000.0;
 	TOTAL_GROSS_ANNUAL_INCOME =  12*MEAN_NET_INCOME*(1+0.00);
	TOTAL_GROSS_QUARTERLY_INCOME = TOTAL_GROSS_ANNUAL_INCOME/4;
	PAYMENT_ACCOUNT = 0.0;

	//Other inputs:
	/*
	pd_qtr = 0.003;
	lgd_qtr = 0.40;
	funding_costs=0.01;
	profit_margin = 0.01;
	*/

	//Outputs:
	/*
	pd_lifetime = 0.221;
	interest = 0.027;

	mortgage.sampled_loan_to_value = 0.20;
	mortgage.sampled_maturity = 25.0;

	mortgage.annuity = 150.0;

	print statement:
	value_at_acquisition = 163869.0;
	initial_principal = 32774.0;
	own_contribution = 131095.0
	*/


    /***** Messages: initialize message boards **********************************/
    MB_Create(&b_mortgage_request, sizeof(m_mortgage_request));
 
    /***** Messages: pre-conditions **********************************/
    //add_mortgage_request_message();
            
    /***** Message: Adding message iterators ***************************************/
    //add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
    //add_mortgage_request_message(ID, BANK_ID, mortgage);
    //MB_Iterator_Create(b_mortgage_request, &i_mortgage_request);

    /***** Function evaluation ***************************************/
    Household_mortgage_loan_application();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_request, &i_mortgage_request);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
	//add_mortgage_request_message(ID, BANK_ID, mortgage);

    START_MORTGAGE_REQUEST_MESSAGE_LOOP
        CU_ASSERT_EQUAL(mortgage_request_message->household_id, ID);
        CU_ASSERT_DOUBLE_EQUAL(mortgage_request_message->mortgage.sampled_loan_to_value, 0.2, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(mortgage_request_message->mortgage.sampled_maturity, 25.0, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(mortgage_request_message->mortgage.annuity, 150.0, 1e-3);		
    FINISH_MORTGAGE_REQUEST_MESSAGE_LOOP
  
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_receive_mortgage_loan_1()
 * \brief: Unit test for: unittest_Household_receive_mortgage_loan.
 * Status: PASS 16.1.2017
 */
void unittest_Household_receive_mortgage_loan_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;
	FLAME_environment_variable_const_init_pd_mortgage = 0.5;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=0;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;

	PLAN_BUY_HMR=1;
	PLAN_BUY_OP=0;

	PAYMENT_ACCOUNT = 0.0;

	mortgage_contract_adt mortgage_offer;
	init_mortgage_contract_adt(&mortgage_offer);

	// Initialize mortgage
	mortgage_offer.household_id 			= 1;
	mortgage_offer.bank_id 					= 2;
	mortgage_offer.initial_duration 		= 36;
	mortgage_offer.outstanding_principal		= 10.0;


	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_contract, sizeof(m_mortgage_contract));

	/***** Messages: pre-conditions **********************************/
	//add_mortgage_contract_message(bank_id, household_id, mortgage_contract_adt mortgage);
	add_mortgage_contract_message(2, 1, mortgage_offer);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_contract, &i_mortgage_contract);

    /***** Function evaluation ***************************************/
	Household_receive_mortgage_loan();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status, 0);

	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 10.0, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_mortgage_contract_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(mortgage_contract_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(mortgage_contract_message->var, value, 1e-3);
	FINISH_mortgage_contract_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_receive_mortgage_loan_2()
 * \brief: Unit test for: unittest_Household_receive_mortgage_loan.
 * Status: PASS 16.1.2017
 */
void unittest_Household_receive_mortgage_loan_2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_mortgage_info = 1;

    ID=1;
    DAY=1;
    HAVE_MORTGAGE=0;
    MORTGAGE_PAY_DAY=1;
    HMR_MORTGAGE_STATUS=0;
    OP_MORTGAGE_STATUS=0;

	PLAN_BUY_HMR=0;
	PLAN_BUY_OP=1;

	PAYMENT_ACCOUNT = 0.0;

	mortgage_contract_adt mortgage_offer;

	init_mortgage_contract_adt(&mortgage_offer);

	// Initialize mortgage
	mortgage_offer.household_id 			= 1;
	mortgage_offer.bank_id 					= 2;
	mortgage_offer.initial_duration 		= 36;
	mortgage_offer.outstanding_principal	= 10.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_contract, sizeof(m_mortgage_contract));

	/***** Messages: pre-conditions **********************************/
	//add_mortgage_contract_message(bank_id, household_id, mortgage_contract_adt mortgage);
	add_mortgage_contract_message(2, 1, mortgage_offer);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_contract, &i_mortgage_contract);

    /***** Function evaluation ***************************************/
	Household_receive_mortgage_loan();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.outstanding_principal, 10.0, 1e-3);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.arrears_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.npl_counter, 0);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].mortgage.status, 0);

	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 10.0, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_mortgage_contract_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(mortgage_contract_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(mortgage_contract_message->var, value, 1e-3);
	FINISH_mortgage_contract_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}



