#include <CUnit/Basic.h>

#include "../../header.h"
#include "../../CentralBank_agent_header.h"
#include "../../my_library_header.h"


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
 * \fn: void unittest_Central_Bank_read_recapitalization_requests()
 * \brief: Unit test for: Central_Bank_read_recapitalization_requests.
 * Status: NOT Tested
 */
void unittest_Central_Bank_monetary_policy()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    
    FLAME_environment_variable_print_debug_cb=1;
    FLAME_environment_variable_policy_taylor_rule=1;

    FLAME_environment_variable_const_ecb_interest_rate = 0.02;
    FLAME_environment_variable_const_mon_pol_1 = 0.5;
    FLAME_environment_variable_const_mon_pol_2 = 1.5;
	FLAME_environment_variable_target_real_interest_rate = 0.04;
	FLAME_environment_variable_target_inflation_rate = 0.0;

    /***** Variables: Memory pre-conditions **************************/

	//(DAY < TRANSITION_PHASE_RECAP)
	iteration_loop=1; //DAY=1;

	/***** Messages: initialize message boards **********************************/
	//eurostat_send_macrodata_message
	MB_Create(&b_eurostat_send_macrodata, sizeof(m_eurostat_send_macrodata));
	
	/***** Messages: pre-conditions **********************************/

	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_eurostat_send_macrodata, &i_eurostat_send_macrodata);

	/***** Messages: initialize message boards **********************************/
	//policy_rate_message
	MB_Create(&b_policy_rate, sizeof(m_policy_rate));
	
	/***** Messages: pre-conditions **********************************/
	//add_policy_rate_message(1.0);
	
	/***** Function evaluation ***************************************/
	Central_Bank_monetary_policy();
    
	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_policy_rate, &i_policy_rate);
	
	/***** Messages: Message post-conditions *****/
	//start reading loops

	START_POLICY_RATE_MESSAGE_LOOP
		printf("\n\t UNITTEST: policy_rate_message->policy_rate_value = %2.3f\n", policy_rate_message->policy_rate_value);
	FINISH_POLICY_RATE_MESSAGE_LOOP
	
	
	/***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(FIAT_MONEY, 330.0, 1e-3);

	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Central_Bank_read_recapitalization_requests()
 * \brief: Unit test for: Central_Bank_read_recapitalization_requests.
 * Status: NOT Tested
 */
void unittest_Central_Bank_read_recapitalization_requests()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_CentralBank_agent();
	
    /************* Setting environment variables **************/
    FLAME_environment_variable_print_debug_bankruptcy=1;
    
    /***** Variables: Memory pre-conditions **************************/

	//(DAY < TRANSITION_PHASE_RECAP)
	iteration_loop=1; //DAY=1;
	FLAME_environment_variable_transition_phase_recap = 200;

	FIAT_MONEY_BANKS = 0.0;
	FIAT_MONEY_FIRMS = 0.0;
	FIAT_MONEY = 0.0;

	/***** Messages: initialize message boards **********************************/
	//firm_recapitalization_request_message
	MB_Create(&b_firm_recapitalization_request, sizeof(m_firm_recapitalization_request));
	
	/***** Messages: pre-conditions **********************************/
	//add_firm_recapitalization_request_message(int firm_id, int bank_id, double funds)
	add_firm_recapitalization_request_message(1,3,100.0);
	add_firm_recapitalization_request_message(2,4,200.0);

	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_recapitalization_request, &i_firm_recapitalization_request);

	/***** Messages: initialize message boards **********************************/
	//bank_recapitalization_request_message
	MB_Create(&b_bank_recapitalization_request, sizeof(m_bank_recapitalization_request));
	
	/***** Messages: pre-conditions **********************************/
        //add_bank_recapitalization_request_message(int bank_id, double funds)
        add_bank_recapitalization_request_message(3,10.0);
	add_bank_recapitalization_request_message(4,20.0);

	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_bank_recapitalization_request, &i_bank_recapitalization_request);

	/***** Messages: initialize message boards **********************************/
	//firm_recapitalization_message
	MB_Create(&b_firm_recapitalization, sizeof(m_firm_recapitalization));
	
	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_recapitalization, &i_firm_recapitalization);

	/***** Messages: initialize message boards **********************************/
	//bank_recapitalization_message
	MB_Create(&b_bank_recapitalization, sizeof(m_bank_recapitalization));
	
	/***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_bank_recapitalization, &i_bank_recapitalization);
	
	/***** Function evaluation ***************************************/
	Central_Bank_read_recapitalization_requests();
    
	
	/***** Messages: Message post-conditions *****/
	//start reading loops

	START_FIRM_RECAPITALIZATION_MESSAGE_LOOP
		printf("\n\t firm_recapitalization_message->funds = %2.3f\n", firm_recapitalization_message->funds);
	
	if(firm_recapitalization_message->firm_id == 1)
	     CU_ASSERT_DOUBLE_EQUAL(firm_recapitalization_message->funds, 100.0, 1e-3);

	if(firm_recapitalization_message->firm_id == 2)
	     CU_ASSERT_DOUBLE_EQUAL(firm_recapitalization_message->funds, 200.0, 1e-3);
	FINISH_FIRM_RECAPITALIZATION_MESSAGE_LOOP
	
	
	START_BANK_RECAPITALIZATION_MESSAGE_LOOP
	printf("\n\t bank_recapitalization_message->funds = %2.3f\n", bank_recapitalization_message->funds);

	if(bank_recapitalization_message->bank_id == 3)
	     CU_ASSERT_DOUBLE_EQUAL(bank_recapitalization_message->funds, 10.0, 1e-3);

	if(bank_recapitalization_message->bank_id == 4)
	     CU_ASSERT_DOUBLE_EQUAL(bank_recapitalization_message->funds, 20.0, 1e-3);
	FINISH_BANK_RECAPITALIZATION_MESSAGE_LOOP

	
	/***** Variables: Memory post-conditions *****/
	printf("\n\t FIAT_MONEY_FIRMS=%2.3f\n", FIAT_MONEY_FIRMS);
	CU_ASSERT_DOUBLE_EQUAL(FIAT_MONEY_FIRMS, 300.0, 1e-3);
	
	printf("\n\t FIAT_MONEY_BANKS=%2.3f\n", FIAT_MONEY_FIRMS);
	CU_ASSERT_DOUBLE_EQUAL(FIAT_MONEY_BANKS, 30.0, 1e-3);
	
	printf("\n\t FIAT_MONEY=%2.3f\n", FIAT_MONEY_FIRMS);
	CU_ASSERT_DOUBLE_EQUAL(FIAT_MONEY, 330.0, 1e-3);

	
	/************* At end of unit test, free the agent **************/
	unittest_free_CentralBank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
