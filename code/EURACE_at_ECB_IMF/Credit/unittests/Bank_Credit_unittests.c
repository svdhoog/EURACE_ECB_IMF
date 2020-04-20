#include <CUnit/Basic.h>
#include "../../header.h"
#include "../../Bank_agent_header.h"
//#include "my_library_header.h"

/************Bank Role: Credit Market********************************/

/************ Unit tests ********************************/
/*
 * \fn: void unittest_Bank_decide_credit_conditions()
 * \brief: Unit test for: Bank_decide_credit_conditions.
 * \brief: Test if ranking (sorting in ascending order) of messages works correctly.
 * \brief: <sort><key>loan_rea</key><order>ascend</order></sort>
 * Status: NOT Tested
 */

void unittest_Bank_decide_credit_conditions()
{
//	    /************* At start of unit test, add one agent **************/
		unittest_init_Bank_agent();

		FLAME_environment_variable_print_debug_credit = 1;
		FLAME_environment_variable_print_debug_credit_unittest = 1;

//		/***** Variables: Memory pre-conditions **************************/
        ID = 1;
        ECB_DEBT = 0;
        EXCESS_RISK_EXPOSURE = 10.0;
		EXCESS_LIQUIDITY = 100.0;
        printf("\n\t In unittest: EXCESS_RISK_EXPOSURE %f EXCESS_LIQUIDITY %f", EXCESS_RISK_EXPOSURE,EXCESS_LIQUIDITY);
        
        //Ensure: (loan_rea <= EXCESS_RISK_EXPOSURE)
		//then: credit_allowed = credit_request;
		//Ensure: (EXCESS_LIQUIDITY>credit_allowed)
		//then: credit granted = credit_allowed

        EQUITY = 30;
//        MIN_INTEREST = 2;

		/***** Messages: initialize message boards **********************************/
        MB_Create(&b_loan_request_ranked, sizeof(m_loan_request_ranked));
		MB_Create(&b_loan_conditions, sizeof(m_loan_conditions));        

//      /***** Messages: pre-conditions **********************************/
		//add_loan_request_ranked_message(firm_id, bank_id, equity, debt, credit_requested, loan_rea, minsky_state);
		add_loan_request_ranked_message(1, ID, 2.0, 2.0, 2.0, 2.0, 2);
		add_loan_request_ranked_message(2, ID, 1.0, 1.0, 1.0, 1.0, 1);

	    /***** Message: Adding message iterators ***************************************/
		MB_Iterator_Create(b_loan_request_ranked, &i_loan_request_ranked);

			
//	    /***** Function evaluation ***************************************/
        Bank_decide_credit_conditions();
//	    
//	    /***** Variables: Memory post-conditions *************************/
//		CU_ASSERT_DOUBLE_EQUAL(<var_name1>, <value>, 1e-3);
//		CU_ASSERT_EQUAL(<var_name2>, <value>);
//		

	    /***** Message: Adding message iterators ***************************************/
		MB_Iterator_Create(b_loan_conditions, &i_loan_conditions);

//     /***** Messages: post-conditions **********************************/
	 	int count=0;
	 	START_LOAN_CONDITIONS_MESSAGE_LOOP
	 		printf("Mesg %d: firm_id=%d", count, loan_conditions_message->firm_id);
			count++;
		FINISH_LOAN_CONDITIONS_MESSAGE_LOOP
//	
	    	//CU_ASSERT_DOUBLE_EQUAL(loan_conditions_message->amount_offered_credit, 1.2959, 1e-5);
			//CU_ASSERT_EQUAL(<msg_name>_message-><var_name>, <value>);

//	    /************* At end of unit test, free the agent **************/
	    //free_agent();

		unittest_free_Bank_agent();
//     /************* At end of unit tests, free all Messages **********/
	    free_messages();
}


/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: NOT Tested
 */
void unittest_Bank_receive_installment()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	ID = 1;
	EXCESS_RISK_EXPOSURE=0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_firm_installment, sizeof(m_firm_installment));
	MB_Create(&b_firm_bankruptcy, sizeof(m_firm_bankruptcy));

	/***** Messages: pre-conditions **********************************/
    //add_firm_installment_message(bank_id, transfer_bank_id, installment_amount, interest_amount, rea_per_installment, rea)
    add_firm_installment_message(1, 0, 20.0, 1.0, 1, 2);
	add_firm_installment_message(2, 0, 40.0, 2.0, 2, 4);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_installment, &i_firm_installment);
	MB_Iterator_Create(b_firm_bankruptcy, &i_firm_bankruptcy);

    /***** Function evaluation ***************************************/
	Bank_receive_installment();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_FIRM_INSTALLMENT_MESSAGE_LOOP
	if(firm_installment_message->bank_id==1)
	{
	     CU_ASSERT_DOUBLE_EQUAL(firm_installment_message->rea_per_installment, 1.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(firm_installment_message->rea, 2.0, 1e-3);
	}
	FINISH_FIRM_INSTALLMENT_MESSAGE_LOOP
*/
	CU_ASSERT_DOUBLE_EQUAL(EXCESS_RISK_EXPOSURE, 1.0, 1e-3);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: PASS 17.5.17 
 */
void unittest_Bank_accounting_1()
{

    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
	FLAME_environment_variable_switch_bank_inactive_on_reg_equity=1;

    /***** Variables: Memory pre-conditions **************************/
	ID = 1;
	EQUITY = 1.0;
	REGULATORY_EQUITY = 2.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/

	switch(SWITCH_BANK_INACTIVE_ON_REG_EQUITY)
	{
		case 0:
			(EQUITY > 0.0) ? (ACTIVE=1) : (ACTIVE=0);
			break;
		case 1:
			(EQUITY > REGULATORY_EQUITY) ? (ACTIVE=1) : (ACTIVE=0);		
			break;
	}
	
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
	CU_ASSERT_EQUAL(ACTIVE, 0);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_firm_installment_message_LOOP
	FINISH_firm_installment_message_LOOP
*/
//	CU_ASSERT_DOUBLE_EQUAL(EXCESS_RISK_EXPOSURE, 1.0, 1e-3);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: PASS 17.5.17 
 */
void unittest_Bank_accounting_2()
{

    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
	FLAME_environment_variable_switch_bank_inactive_on_reg_equity=0;

    /***** Variables: Memory pre-conditions **************************/
	ID = 1;
	EQUITY = 1.0;
	REGULATORY_EQUITY = 2.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/

	switch(SWITCH_BANK_INACTIVE_ON_REG_EQUITY)
	{
		case 0:
			(EQUITY > 0.0) ? (ACTIVE=1) : (ACTIVE=0);
			break;
		case 1:
			(EQUITY > REGULATORY_EQUITY) ? (ACTIVE=1) : (ACTIVE=0);		
			break;
	}
	
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
	CU_ASSERT_EQUAL(ACTIVE, 1);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_firm_installment_message_LOOP
	FINISH_firm_installment_message_LOOP
*/
//	CU_ASSERT_DOUBLE_EQUAL(EXCESS_RISK_EXPOSURE, 1.0, 1e-3);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: PASS 17.5.17 Should yield a failed test
 */
void unittest_Bank_accounting_3()
{

    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
	FLAME_environment_variable_switch_bank_inactive_on_reg_equity=1;

    /***** Variables: Memory pre-conditions **************************/
	ID = 1;
	EQUITY = 1.0;
	REGULATORY_EQUITY = 2.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/

	switch(SWITCH_BANK_INACTIVE_ON_REG_EQUITY)
	{
		case 0:
			(EQUITY > 0.0) ? (ACTIVE=1) : (ACTIVE=0);
			break;
		case 1:
			(EQUITY > REGULATORY_EQUITY) ? (ACTIVE=1) : (ACTIVE=0);		
			break;
	}
	
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
	CU_ASSERT_EQUAL(ACTIVE, 1);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_firm_installment_message_LOOP
	FINISH_firm_installment_message_LOOP
*/
//	CU_ASSERT_DOUBLE_EQUAL(EXCESS_RISK_EXPOSURE, 1.0, 1e-3);
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}



/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: 
 */
void unittest_Bank_send_data_to_Eurostat()
{

    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	ID=0;
	
    double equity = 1.0;
    double total_assets = 1.0;
    double ecb_debt = 1.0;
    double equity_asset_ratio = 1.0;
    double debt_equity_ratio = 1.0;
    double rea = 1.0;
    double pl_mortgage_loans = 1.0;
    double npl_mortgage_loans = 1.0;
    double total_credit = 1.0;
    double total_bad_debt_calendar_month = 1.0;

    financial_data_adt financial_data;
    
    //fixture 1:
    financial_data.equity   = equity;
    financial_data.total_assets = total_assets;
    financial_data.total_debt = ecb_debt;
    financial_data.equity_asset_ratio = equity_asset_ratio;
    financial_data.debt_equity_ratio = debt_equity_ratio;
    financial_data.rea = rea;
    financial_data.pl_mortgage_loans = pl_mortgage_loans;
    financial_data.npl_mortgage_loans = npl_mortgage_loans;
    financial_data.pl_firm_loans = total_credit;
    financial_data.npl_firm_loans = total_bad_debt_calendar_month;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_bank_send_data, sizeof(m_bank_send_data));

	/***** Messages: pre-conditions **********************************/
    //add_firm_installment_message(bank_id, transfer_bank_id, installment_amount, interest_amount, rea_per_installment, rea)

//	  add_bank_send_data_message(ID, financial_data);


    /***** Function evaluation ***************************************/
	Bank_send_data_to_Eurostat();
    

    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_bank_send_data, &i_bank_send_data);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	printf("\nOut loop\n");
	
	START_BANK_SEND_DATA_MESSAGE_LOOP
		printf("\nIn loop\n");

		printf("\nbank_send_data_message->financial_data.equity %2.2f\n", bank_send_data_message->financial_data.equity);
		
		CU_ASSERT_DOUBLE_EQUAL(bank_send_data_message->financial_data.equity, 0.0, 1e-3);

	FINISH_BANK_SEND_DATA_MESSAGE_LOOP

	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
