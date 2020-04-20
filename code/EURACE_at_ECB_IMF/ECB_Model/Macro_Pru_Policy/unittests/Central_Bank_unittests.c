#include <CUnit/Basic.h>

#include "../../../header.h"     
#include "../../../CentralBank_agent_header.h"
#include "../../../my_library_header.h"
#include "../Central_Bank_aux_header.h"


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
 * \fn: void unittest_CentralBank_function()
 * \brief: Unit test for: CentralBank_function.
 * Status: NOT Tested
 */
/*
void unittest_CentralBank_function()
{
	int rc;
*/	
    /************* At start of unit test, add one agent **************/
//	unittest_init_CentralBank_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
/*
	rc = MB_Create(&b_<message>, sizeof(m_<message>));
    	    #ifdef ERRCHECK
    	    if (rc != MB_SUCCESS)
    	    {
    	       fprintf(stderr, "ERROR: Could not create '<message>' board\n");
    	       switch(rc) {
    	           case MB_ERR_INVALID:
    	               fprintf(stderr, "\t reason: Invalid message size\n");
    	               break;
    	           case MB_ERR_MEMALLOC:
    	               fprintf(stderr, "\t reason: out of memory\n");
    	               break;
    	           case MB_ERR_INTERNAL:
    	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
    	               break;
    	       }
    	    }
    	    #endif
*/	
	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Message: Adding message iterators (IF function reads messages) ***************************************/
	//rc = MB_Iterator_Create(b_<message>, &i_<message>);
/*			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for '<message>'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: '<message>' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: '<message>' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}
*/
    /***** Function evaluation ***************************************/
//	CentralBank_function();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators (IF function writes messages) ***************************************/
	//rc = MB_Iterator_Create(b_<message>, &i_<message>);
/*			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for '<message>'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: '<message>' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: '<message>' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}
*/

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
//	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}



/*
 * \fn: void unittest_policy_response_1()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 1: input signal > max input (x_ceiling), output is max output (y_ceiling)
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=1;
	ind.signal = 11.0;
	double y_floor = 0.001;
	double y_ceiling = 2.5;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 0.0
	ind.slope = 0.0;     //should: 0.25
	ind.response = 0.0;  //should: 2.5

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.2f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 2.5, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 0.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, 0.25, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_policy_response_2()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 2: input signal < min input (x_floor), output is min output (y_floor)
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=1;
	ind.signal = -1.0;
	double y_floor = 0.001;
	double y_ceiling = 2.5;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 0.0
	ind.slope = 0.0;     //should: 0.25
	ind.response = 0.0;  //should: 2.5

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.2f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 0.001, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 0.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, 0.25, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_policy_response_3()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 3: input signal in (x_floor,x_ceiling), output is on response function
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_3()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=1;
	ind.signal = 8.0;
	double y_floor = 0.001;
	double y_ceiling = 2.5;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 0.0
	ind.slope = 0.0;     //should: 0.25
	ind.response = 0.0;  //should: 2.0

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.2f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 2.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 0.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, 0.25, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_policy_response_4()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 4: input signal < min input (x_floor), output is max output (y_ceiling)
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_4()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=0;
	ind.signal = -1.0;
	double y_floor = 0.6;
	double y_ceiling = 1.0;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 1.0
	ind.slope = 0.0;     //should: -0.04
	ind.response = 0.0;  //should: 1.0

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.5f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 1.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 1.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, -0.04, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_policy_response_5()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 5: input signal > max input (x_ceiling), output is min output (y_floor)
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_5()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=0;
	ind.signal = 11.0;
	double y_floor = 0.6;
	double y_ceiling = 1.0;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 1.0
	ind.slope = 0.0;     //should: -0.04
	ind.response = 0.0;  //should: 0.6

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.2f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 0.6, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 1.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, -0.04, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_policy_response_6()
 * \brief: Unit test for: unittest_policy_response.
 * \brief: Case 6: input signal in (x_floor,x_ceiling), output is on response function
 * Status: OK PASS 11.10.17, 17.06.18, 10.09.18
 */
void unittest_policy_response_6()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	indicator_adt ind;

	ind.positively_sloped=0;
	ind.signal = 5.0;
	double y_floor = 0.6;
	double y_ceiling = 1.0;
	ind.x_floor = 0.0;
	ind.x_ceiling = 10.0;
	ind.intercept = 0.0; //should: 1.0
	ind.slope = 0.0;     //should: -0.04
	ind.response = 0.0;  //should: 0.8

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	policy_response(&ind, y_floor, y_ceiling);
    
	printf("\n\n ind.response %2.2f", ind.response);
	printf("\n ind.intercept %2.2f", ind.intercept);
	printf("\n ind.slope %2.2f", ind.slope);

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.response, 0.8, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.intercept, 1.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(ind.slope, -0.04, 1e-3);
	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Central_Bank_read_macro_indicators()
 * \brief: Unit test for: unittest_Central_Bank_read_macro_indicators.
 * Status: OK PASS 17.06.18, 10.09.18
 */
void unittest_Central_Bank_read_macro_indicators()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    //FLAME_environment_variable_const_macroprupol_1 = 0.5;

    /***** Variables: Memory pre-conditions **************************/
	//Add macro indicators to adt
	macro_indicators_adt macro_indicators;

	macro_indicators.credit_to_gdp_ratio  							= 1.0;
	macro_indicators.credit_minus_gdp_growth 						= 200.0;
    macro_indicators.credit_growth                                  = 300.0;
    
	macro_indicators.mortgage_credit_growth 						= 400.0;
    macro_indicators.housing_market_price_growth                    = 500.0;
    macro_indicators.house_price_minus_rent_price_growth            = 600.0;
	macro_indicators.mortgage_interest_rate_spread_policy_rate      = 7.0;

    macro_indicators.investment_gdp_ratio                           = 8.0;
    macro_indicators.firm_credit_growth 							= 900.0;
	macro_indicators.new_mortgage_to_housing_value_ratio		 	= 10.0;
	macro_indicators.new_firm_loans_to_investment_ratio 			= 11.0;
	macro_indicators.rental_market_price_growth 					= 1200.0;

	/***** Messages: initialize message boards **********************************/
	//eurostat_to_ecb_macro_indicators_message
	MB_Create(&b_eurostat_to_ecb_macro_indicators, sizeof(m_eurostat_to_ecb_macro_indicators));
	
	/***** Messages: pre-conditions **********************************/
	add_eurostat_to_ecb_macro_indicators_message(macro_indicators);

	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_eurostat_to_ecb_macro_indicators, &i_eurostat_to_ecb_macro_indicators);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	Central_Bank_read_macro_indicators();
    

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(FIAT_MONEY, 330.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(CREDIT_TO_GDP_RATIO_INDICATOR.signal, 1.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal, 2.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(CREDIT_GROWTH_INDICATOR.signal, 3.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_CREDIT_GROWTH_INDICATOR.signal, 4.0, 1e-3);	
	CU_ASSERT_DOUBLE_EQUAL(HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal, 5.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal, 6.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal, 7.0, 1e-3);

	CU_ASSERT_DOUBLE_EQUAL(INVESTMENT_GDP_RATIO_INDICATOR.signal, 8.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(FIRM_CREDIT_GROWTH_INDICATOR.signal, 9.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal, 10.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.signal , 11.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(RENTAL_MARKET_PRICE_GROWTH_INDICATOR.signal, 12.0, 1e-3);

	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Central_Bank_activate_policy_responses()
 * \brief: Unit test for: Central_Bank_activate_policy_responses.
 * Status: OK PASS 10.09.18
 */
void unittest_Central_Bank_activate_policy_responses()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/

    FLAME_environment_variable_print_macropru_info=1; //print debug info
    
	FLAME_environment_variable_const_switch_macropru_policy=1;
    FLAME_environment_variable_switch_macropru_policy_case=1; // Capital buffer
    FLAME_environment_variable_switch_macropru_policy_indicator = 4; // Indicator: MORTGAGE_CREDIT_GROWTH_INDICATOR
	

    /***** Variables: Memory pre-conditions **************************/
	 FLAME_environment_variable_const_regulatory_capital_buffer = 0.10;
	 COUNTER_CYCLICAL_CAPITAL_BUFFER = 0.0;
	 CAPITAL_CONSERVATION_BUFFER = 0.0;

	MORTGAGE_CREDIT_GROWTH_INDICATOR.positively_sloped=0;
	MORTGAGE_CREDIT_GROWTH_INDICATOR.signal = 0.075;
	/*y_floor = 0.001;	  */	FLAME_environment_variable_const_macropru_response_lbnd_1 = 0.001;
	/*y_ceiling = 0.025; */		FLAME_environment_variable_const_macropru_response_ubnd_1 = 0.025;
	/*MORTGAGE_CREDIT_GROWTH_INDICATOR.x_floor = 0.0;     */    FLAME_environment_variable_const_macropru_indicator_lbnd_4 = 0.0;
	/*MORTGAGE_CREDIT_GROWTH_INDICATOR.x_ceiling = 0.15;  */    FLAME_environment_variable_const_macropru_indicator_ubnd_4 = 0.15;
	MORTGAGE_CREDIT_GROWTH_INDICATOR.intercept = 0.0; //should be: 0.001 (0.1%)
	MORTGAGE_CREDIT_GROWTH_INDICATOR.slope = 0.0;     //should be: 0.16  (16%)
	MORTGAGE_CREDIT_GROWTH_INDICATOR.response = 0.0;  //should be: 0.013 (1.3%, half-way along the policy response function)

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	//MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	Central_Bank_activate_policy_responses();
    

	/***** Messages: Message post-conditions *****/
	//start reading loops


	/***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(MORTGAGE_CREDIT_GROWTH_INDICATOR.positively_sloped, 1);
	CU_ASSERT_DOUBLE_EQUAL(COUNTER_CYCLICAL_CAPITAL_BUFFER, 0.0125, 1e-3);

	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

