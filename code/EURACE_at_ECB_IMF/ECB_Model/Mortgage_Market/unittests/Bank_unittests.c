#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Bank_agent_header.h"
#include "../../../my_library_header.h"
#include "../../Credit_Risk_Management/Bank_aux_headers.h"
#include "../../../Statistical_Office/balance_sheet_operations.h"

/************Bank Role: Mortgage Market********************************/
/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: NOT Tested
 */
//void unittest_Bank_()
//{

	
    /************* At start of unit test, add one agent **************/
	//unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
    
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_writeoff, sizeof(m_mortgage_writeoff));
	
	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);

	//HH_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_payment_message(object_id, int household_id, int bank_id, double principal, double interest)

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	//Bank_();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
//	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}

    
/************ Unit tests ********************************/
/*
 * \fn: void unittest_Bank_init_mortgages()
 * \brief: Unit test for: Bank_init_mortgages.
 * \brief: - input mortgage_info_message(household_id, bank_id, mortgage_contract_adt mortgage)
 * Status: PASS 3.11.16
 */
void unittest_Bank_init_mortgages()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_mortgage = 1;

	//This bank ID
	ID = 2;
	
	//Local var
	mortgage_contract_adt mortgage;
	
	//Init to zeros
	init_mortgage_contract_adt(&mortgage);
	
	mortgage.household_id = 1;
	mortgage.bank_id = 2;
	mortgage.initial_duration = 1;
	mortgage.interest_rate_annual = 0.0;
	mortgage.current_duration = 1;
	mortgage.outstanding_principal = 10.0;
	mortgage.monthly_total_repayment = 0.0;
	mortgage.monthly_principal = 10.0;
	mortgage.monthly_interest = 0.0;
	mortgage.status = 2;
	mortgage.arrears_counter = 0;
	mortgage.npl_counter = 1;
	

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_info, sizeof(m_mortgage_info));
	
	/***** Messages: pre-conditions **********************************/
	add_mortgage_info_message(1, 2, mortgage);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_info, &i_mortgage_info);

    /***** Function evaluation ***************************************/
	Bank_init_mortgages();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);
	printf("\n\tMORTGAGE_PORTFOLIO.array[0].household_id=%d\n", MORTGAGE_PORTFOLIO.array[0].household_id);
	CU_ASSERT_EQUAL(MORTGAGE_PORTFOLIO.array[0].household_id, 1);
	
	printf("\n\tMORTGAGE_PORTFOLIO.array[0].outstanding_principal=%2.3f\n", MORTGAGE_PORTFOLIO.array[0].outstanding_principal);
    	CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_PORTFOLIO.array[0].outstanding_principal, 10.0, 1e-3);
    	

	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].household_id);
	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].bank_id);
	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].initial_duration);
	printf("\n\t %2.3f", MORTGAGE_PORTFOLIO.array[0].interest_rate_annual);
	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].current_duration);
	printf("\n\t %2.3f", MORTGAGE_PORTFOLIO.array[0].outstanding_principal);
	printf("\n\t %2.3f", MORTGAGE_PORTFOLIO.array[0].monthly_total_repayment);
	printf("\n\t %2.3f", MORTGAGE_PORTFOLIO.array[0].monthly_principal);
	printf("\n\t %2.3f", MORTGAGE_PORTFOLIO.array[0].monthly_interest);
	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].status);
	printf("\n\t %d", MORTGAGE_PORTFOLIO.array[0].arrears_counter);
	printf("\n\t %d\n", MORTGAGE_PORTFOLIO.array[0].npl_counter);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Bank_read_mortgage_messages()
 * \brief: Unit test for: Bank_read_mortgage_messages.
 * \brief: See unittests in Bank_aux_unittests.c
 * Status: NOT Tested
 */
void unittest_Bank_read_mortgage_messages_1()
{

	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
	ID=1;
    
    //set some values
	int 	object_id = 0;
	int 	household_id = 1;
	int 	bank_id = 2;
	int 	initial_duration = 2;
	double 	interest_rate_annual = 0.0;
	int		current_duration = 2;
	double 	outstanding_principal = 20.0;
	double 	monthly_total_repayment = 11.0;
	double 	monthly_principal = 10.0;
	double 	monthly_interest = 1.0;
	
	int arrears_counter = 0;
	int npl_counter = 1;
	int status = 2;

	double loan_loss	= outstanding_principal;
	double loan_default	= outstanding_principal;

	double seized_liquidity=0.0;
	foreclosure_adt house;
	init_foreclosure_adt(&house);

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_writeoff, sizeof(m_mortgage_writeoff));
	MB_Create(&b_mortgage_arrears, sizeof(m_mortgage_arrears));
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	MB_Create(&b_mortgage_npl, sizeof(m_mortgage_npl));
	MB_Create(&b_real_estate_market_data, sizeof(m_real_estate_market_data));


	/***** Messages: pre-conditions **********************************/
	add_mortgage_arrears_message(object_id, ID, bank_id, loan_loss, loan_default, arrears_counter);
	add_mortgage_payment_message(object_id, ID, bank_id, loan_loss, loan_default, monthly_principal, monthly_interest);
	add_mortgage_npl_message(object_id, ID, bank_id, loan_loss, loan_default, npl_counter);
	add_mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default, house);

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);
	MB_Iterator_Create(b_mortgage_arrears, &i_mortgage_arrears);
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);
	MB_Iterator_Create(b_mortgage_npl, &i_mortgage_npl);
	MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);
	MB_Iterator_Create(b_real_estate_market_data, &i_real_estate_market_data);

	
	//HH_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_payment_message(object_id, int household_id, int bank_id, double principal, double interest)

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_read_mortgage_messages();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
	int count_arrears=0;
	START_MORTGAGE_ARREARS_MESSAGE_LOOP
		count_arrears++;
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP
	printf("\ncount_arrears=%d\n", count_arrears);
	
	int count_pay=0;
	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		count_pay++;
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	printf("\ncount_pay=%d\n", count_pay);
	
	int count_npl=0;
	START_MORTGAGE_NPL_MESSAGE_LOOP
		count_npl++;
	FINISH_MORTGAGE_NPL_MESSAGE_LOOP
	printf("\ncount_npl=%d\n", count_npl);

	int count_default=0;
	START_MORTGAGE_WRITEOFF_MESSAGE_LOOP
		count_default++;
	FINISH_MORTGAGE_WRITEOFF_MESSAGE_LOOP
	printf("\ncount_default=%d\n", count_default);

    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Bank_read_mortgage_messages()
 * \brief: Unit test for: Bank_read_mortgage_messages.
 * \brief: Test if the content of foreclosed_house.mortgage  is correctly copied into the array element SEIZED_COLLATERAL.array[i].mortgage
 * Status: NOT Tested
 */
void unittest_Bank_read_mortgage_messages_2()
{

	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
	ID=1;
    
    //set some values
	int 	object_id = 10;
	int 	household_id = 2;
	int 	bank_id = 1;
	int 	initial_duration = 2;
	double 	interest_rate_annual = 0.0;
	int		current_duration = 2;
	double 	outstanding_principal = 20.0;
	double 	monthly_total_repayment = 11.0;
	double 	monthly_principal = 10.0;
	double 	monthly_interest = 1.0;
	
	int arrears_counter = 0;
	int npl_counter = 1;
	int status = 2;

	double loan_loss	= outstanding_principal;
	double loan_default	= outstanding_principal;

	double seized_liquidity=0.0;
	
	foreclosure_adt foreclosed_house;
	init_foreclosure_adt(&foreclosed_house);

	foreclosed_house.object_id = 10;
	foreclosed_house.former_owner_id  = 2;
	foreclosed_house.bank_id = 1;
	foreclosed_house.value = 10.0;
	foreclosed_house.claim = 0.0;
	foreclosed_house.seized_liquidity  = 0.0;
	foreclosed_house.housing_market_price_index_base = 100.0;
	foreclosed_house.seizure_day = 1;

	//Init to zeros
	init_mortgage_contract_adt(&foreclosed_house.mortgage);
	foreclosed_house.mortgage.initial_duration = 1;
	foreclosed_house.mortgage.interest_rate_annual = 0.02;
	foreclosed_house.mortgage.current_duration = 1;
	foreclosed_house.mortgage.outstanding_principal = 2.0;
	foreclosed_house.mortgage.outstanding_interest = 0.01;
	foreclosed_house.mortgage.monthly_total_repayment = 1500.0;

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_writeoff, sizeof(m_mortgage_writeoff));
	MB_Create(&b_mortgage_arrears, sizeof(m_mortgage_arrears));
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	MB_Create(&b_mortgage_npl, sizeof(m_mortgage_npl));
	MB_Create(&b_mortgage_writeoff, sizeof(m_mortgage_writeoff));
	MB_Create(&b_real_estate_market_data, sizeof(m_real_estate_market_data));


	/***** Messages: pre-conditions **********************************/
	//add_mortgage_arrears_message(object_id, ID, bank_id, loan_loss, loan_default, arrears_counter);
	//add_mortgage_payment_message(object_id, ID, bank_id, loan_loss, loan_default, monthly_principal, monthly_interest);
	//add_mortgage_npl_message(object_id, ID, bank_id, loan_loss, loan_default, npl_counter);
	add_mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default, foreclosed_house);

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);
	MB_Iterator_Create(b_mortgage_arrears, &i_mortgage_arrears);
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);
	MB_Iterator_Create(b_mortgage_npl, &i_mortgage_npl);
	MB_Iterator_Create(b_mortgage_writeoff, &i_mortgage_writeoff);
	MB_Iterator_Create(b_real_estate_market_data, &i_real_estate_market_data);
	
	//HH_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_payment_message(object_id, int household_id, int bank_id, double principal, double interest)

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_read_mortgage_messages();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
	START_MORTGAGE_WRITEOFF_MESSAGE_LOOP
		printf("\nIn msg loop:\n\t mortgage_writeoff_message->house.object_id %d\n\t mortgage_writeoff_message->house.mortgage.monthly_total_repayment %f",
		 mortgage_writeoff_message->house.object_id, mortgage_writeoff_message->house.mortgage.monthly_total_repayment);
	FINISH_MORTGAGE_WRITEOFF_MESSAGE_LOOP

	printf("\nAfter msg loop: \nSEIZED_COLLATERAL.array[0].object_id %d\nSEIZED_COLLATERAL.array[0].mortgage.monthly_total_repayment %f",
	 SEIZED_COLLATERAL.array[0].object_id, SEIZED_COLLATERAL.array[0].mortgage.monthly_total_repayment);


	CU_ASSERT_EQUAL(SEIZED_COLLATERAL.array[0].object_id, foreclosed_house.object_id);

	CU_ASSERT_DOUBLE_EQUAL(SEIZED_COLLATERAL.array[0].mortgage.monthly_total_repayment, foreclosed_house.mortgage.monthly_total_repayment, 1e-3);
    	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Bank_receive_mortgage_payments_1()
 * \brief: Unit test for: Bank_receive_mortgage_payments.
 * \brief: Case 1: Full repayment of last installment
 * Status: PASS 15.1.2017
 */
void unittest_Bank_receive_mortgage_payments_1()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_mortgage = 1;

	ID=1;

	mortgage_contract_adt mortgage;

	mortgage.object_id = 0;
	mortgage.household_id	=2;
	mortgage.bank_id		=1;
	mortgage.current_duration = 1;
	mortgage.outstanding_principal=10.0;

	//Add ADT to dynamic array mortgage_portfolio
	add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
							mortgage.object_id,
							mortgage.household_id,
							mortgage.bank_id,
							mortgage.initial_duration,
							mortgage.interest_rate_annual,
							mortgage.current_duration,
							mortgage.outstanding_principal,
							mortgage.outstanding_interest,
							mortgage.monthly_total_repayment,
							mortgage.monthly_principal,
							mortgage.monthly_interest,
							mortgage.status,			
							mortgage.arrears_counter,
							mortgage.npl_counter);

	double loan_loss = 0.0;
	double loan_default = 0.0;
    double principal = 10.0;
    double interest = 1.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/
    //add_mortgage_payment_message(object_id, int household_id, int bank_id, double principal, double interest)
	add_mortgage_payment_message(mortgage.object_id, mortgage.household_id, ID, loan_loss, loan_default, principal, interest);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_receive_mortgage_payments();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    	CU_ASSERT_EQUAL(MORTGAGE_PORTFOLIO.size, 0);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Bank_receive_mortgage_payments_2()
 * \brief: Unit test for: Bank_receive_mortgage_payments.
 * \brief: Case 2: Repayment of before-last installment
 * Status: PASS 15.1.2017
 */
void unittest_Bank_receive_mortgage_payments_2()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_mortgage = 1;

	ID=1;

	mortgage_contract_adt mortgage;

	mortgage.object_id 		= 0;
	mortgage.household_id	=2;
	mortgage.bank_id		=1;
	mortgage.current_duration =2;
	mortgage.outstanding_principal=20.0;

	//Add ADT to dynamic array mortgage_portfolio
	add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
							mortgage.object_id,
							mortgage.household_id,
							mortgage.bank_id,
							mortgage.initial_duration,
							mortgage.interest_rate_annual,
							mortgage.current_duration,
							mortgage.outstanding_principal,
							mortgage.outstanding_interest,
							mortgage.monthly_total_repayment,
							mortgage.monthly_principal,
							mortgage.monthly_interest,
							mortgage.status,			
							mortgage.arrears_counter,
							mortgage.npl_counter);

	double loan_loss = 0.0;
	double loan_default = 0.0;

    double principal = 10.0;
    double interest = 1.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/
    //add_mortgage_payment_message(object_id, int household_id, int bank_id, double principal, double interest)
	add_mortgage_payment_message(mortgage.object_id, mortgage.household_id, ID, loan_loss, loan_default, principal, interest);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_receive_mortgage_payments();
    
    /***** Variables: Memory post-conditions *****/
    	CU_ASSERT_EQUAL(MORTGAGE_PORTFOLIO.size, 1);
		CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_PORTFOLIO.array[0].outstanding_principal, 10.0, 1e-3);
    
    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Bank_process_mortgage_requests_1()
 * \brief: Unit test for: Bank_process_mortgage_requests.
 * \brief Message input:
 * \brief - add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
 * \brief Message output:
 * \brief - add_mortgage_contract_message(bank_id, household_id, mortgage_contract_adt mortgage);
 * \brief UNITTEST: NOT TESTED
 * Status: NOT Tested
 */
void unittest_Bank_process_mortgage_requests_1()
{

	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();

	FLAME_environment_variable_print_debug_mortgage = 1;
    /***** Variables: Memory pre-conditions **************************/

   	mortgage_application_adt mortgage_request;
	init_mortgage_application_adt(&mortgage_request);

	//Other inputs in Bank_Functions.c, line 700, 703: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	/*
		funding_costs: f=0.01;
		profit_margin: pr = 0.01;
	*/

	
	// RESULT: 24.5.17 PASS
	//Case: profit_margin pr = 0.00, DSTI=0.05, LTV=0.20, annuity=150
	printf("\n\n Unittest 1");
	//Inputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	HH_PD_PL_YEAR 								= 0.0025;
	HH_LGD_PL_YEAR 								= 0.40;
	mortgage_request.annuity 				= 150.0;
	mortgage_request.sampled_loan_to_value	= 0.20;
	mortgage_request.sampled_maturity		= 25;

	//Outputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	double pd_lifetime_loan = 0.221443;
	double i = 0.015710;
	double initial_principal = 37194.908222;
	double value_at_acquisition = 185974.541;
	double own_contribution = 148779.633;
	

	/*
	// RESULT: 24.5.17 PASS
	//Case: profit_margin pr = 0.01, DSTI=0.05, LTV=0.20, annuity=150
	printf("\n\n Unittest 2");
	//Inputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	HH_PD_PL_YEAR 								= 0.0025;
	HH_LGD_PL_YEAR 								= 0.40;
	mortgage_request.annuity 				= 150.0;
	mortgage_request.sampled_loan_to_value	= 0.20;
	mortgage_request.sampled_maturity		= 25;

	//Outputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	double pd_lifetime_loan = 0.221443;
	double i = 0.026790;
	double initial_principal = 32773.818;
	double value_at_acquisition = 163869.088;
	double own_contribution = 131095.271;
	*/	

	/*
	// RESULT: 24.5.17 PASS
	//Case: profit_margin pr = 0.00, DSTI=0.60, LTV=1.00, annuity=1800
	printf("\n\n Unittest 3");
	//Inputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	HH_PD_PL_YEAR 								= 0.0025;
	HH_LGD_PL_YEAR 								= 0.40;
	mortgage_request.annuity 				= 1800.0;
	mortgage_request.sampled_loan_to_value	= 1.00;
	mortgage_request.sampled_maturity		= 25;

	//Outputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	double pd_lifetime_loan = 0.221443;
	double i = 0.015710;
	double initial_principal = 446338.899;
	double value_at_acquisition = 446338.899;
	double own_contribution = 0.0;
	*/

	/*
	// RESULT: 24.5.17 PASS
	//Case: profit_margin pr = 0.00, DSTI=0.60, LTV=1.20, annuity=1800
	printf("\n\n Unittest 4");
	//Inputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	HH_PD_PL_YEAR 								= 0.0025;
	HH_LGD_PL_YEAR 								= 0.40;
	mortgage_request.annuity 				= 1800.0;
	mortgage_request.sampled_loan_to_value	= 1.20;
	mortgage_request.sampled_maturity		= 25;

	//Outputs: TAKEN FROM NUMERICAL EXAMPLES PROVIDED BY ECB COLLEAGUES
	double pd_lifetime_loan = 0.221443;
	double i = 0.015710;
	double initial_principal = 446338.899;
	double value_at_acquisition = 371949.082;
	double own_contribution = 0.0;
	*/	

	printf("\n\nUnittest HH_PD_PL_YEAR: %f", HH_PD_PL_YEAR);
	printf("\nUnittest HH_LGD_PL_YEAR: %f", HH_LGD_PL_YEAR);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_request, sizeof(m_mortgage_request));
	MB_Create(&b_mortgage_contract, sizeof(m_mortgage_contract));
	
	/***** Messages: pre-conditions **********************************/
	//add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
	add_mortgage_request_message(1, 2, mortgage_request);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_request, &i_mortgage_request);

    /***** Function evaluation ***************************************/
	Bank_process_mortgage_requests();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_contract, &i_mortgage_contract);

    /***** Messages: Message post-conditions *****/
	mortgage_contract_adt mortgage_offer;

	START_MORTGAGE_CONTRACT_MESSAGE_LOOP
		copy_mortgage_contract_adt(&(mortgage_contract_message->mortgage), &mortgage_offer);
	FINISH_MORTGAGE_CONTRACT_MESSAGE_LOOP

	//TESTS:
	CU_ASSERT_EQUAL(		mortgage_offer.initial_duration 		, mortgage_request.sampled_maturity*12);
	CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.interest_rate_annual 		, i, 1e-3);												//Eq (9)
	CU_ASSERT_EQUAL(		mortgage_offer.current_duration 		, mortgage_request.sampled_maturity*12);
	//CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.outstanding_principal 	, initial_principal, 1e-3);
	//CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.outstanding_interest 		, mortgage_offer.initial_total_interest, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.monthly_total_repayment 	, mortgage_request.annuity, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.monthly_principal 		, initial_principal/(mortgage_request.sampled_maturity*12), 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(mortgage_offer.monthly_interest 			, (i/12.0)*initial_principal, 1e-3);

    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Bank_check_recapitalization()
 * \brief: Unit test for: Bank_check_recapitalization.
 * Status: PASS 2.11.16
 */
void unittest_Bank_check_recapitalization()
{
	
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
	
	//(DAY < TRANSITION_PHASE)
	iteration_loop=1; //DAY=1;
	FLAME_environment_variable_transition_phase_recap = 200;
	
	ID = 2;
	RESERVES = 100.0;
	EQUITY = 100.0;
	ACTIVE = 0;
	INACTIVE = 1;
    
	/***** Messages: initialize message boards **********************************/
	//firm_recapitalization_message
	MB_Create(&b_firm_recapitalization, sizeof(m_firm_recapitalization));
	
	
	/***** Messages: pre-conditions **********************************/
	//add_firm_recapitalization_message(int firm_id, int bank_id, double funds)
	add_firm_recapitalization_message(1,2,100.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_recapitalization, &i_firm_recapitalization);

	/***** Messages: initialize message boards **********************************/
	//bank_recapitalization_message
	MB_Create(&b_bank_recapitalization, sizeof(m_bank_recapitalization));
	
	/***** Messages: pre-conditions **********************************/
        //add_bank_recapitalization_message(int bank_id, double funds)
        add_bank_recapitalization_message(2,200.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_bank_recapitalization, &i_bank_recapitalization);

    /***** Function evaluation ***************************************/
	Bank_check_recapitalization();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);

    //added: 100 for firm, 200 for bank
    	CU_ASSERT_DOUBLE_EQUAL(RESERVES, 400.0, 1e-3);
    	CU_ASSERT_DOUBLE_EQUAL(EQUITY, 400.0, 1e-3);
	CU_ASSERT_EQUAL(ACTIVE, 1);
	CU_ASSERT_EQUAL(INACTIVE, 0);
		
    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
