#include <CUnit/Basic.h>
#include "../../header.h"
#include "../../Bank_agent_header.h"
#include "../../my_library_header.h"

/*
 * 1) initialize agent
 * 2) add memory pre-conditions
 * 3) initialise MBs
 * 4) add message pre-conditions
 * 5) evaluate function to be tested
 * 6) create MB iterators
 * 7) assertions on memory post-conditions
 * 8) assertions on message post-conditions
 * 9) free agent
 * 10) free MBs
 */

/*
 * \fn: void asset_liability_add(double * asset, double * liability, double value)
 * \brief: Unit test for: 
 * Status: NOT Tested
 */
void unittest_asset_liability_add()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	RESERVES = 10.0;
	EQUITY = 11.0;

	/***** Messages: initialize message boards **********************************/
//    rc = MB_Create(&b_<message>, sizeof(m_<message>));
    	    
	/***** Messages: pre-conditions **********************************/
//    add_<message>_message();

    /***** Function evaluation ***************************************/
	asset_liability_add(&RESERVES, &EQUITY, 1.0);
    	    
    /***** Message: Adding message iterators ***************************************/
//	rc = MB_Iterator_Create(b_<message>, &i_<message>);
			
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(RESERVES, 11.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(EQUITY, 12.0, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
	
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void asset_liability_add(double * asset, double * liability, double value)
 * \brief: Unit test for: 
 * Status: NOT Tested
 */
void unittest_asset_liability_subtract()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	RESERVES = 10.0;
	EQUITY = 11.0;
	TOTAL_DIVIDEND_PAYMENT = 1.0;

	/***** Messages: initialize message boards **********************************/
//    rc = MB_Create(&b_<message>, sizeof(m_<message>));
    	    
	/***** Messages: pre-conditions **********************************/
//    add_<message>_message();

    /***** Function evaluation ***************************************/
	asset_liability_subtract(&RESERVES, &EQUITY, TOTAL_DIVIDEND_PAYMENT);
    	    
    /***** Message: Adding message iterators ***************************************/
//	rc = MB_Iterator_Create(b_<message>, &i_<message>);
			
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(RESERVES, 9.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(EQUITY, 10.0, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
	
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void liability_transformation(double * from, double * to, double value)
 * \brief: Unit test for: 
 * Status: NOT Tested
 */
void unittest_liability_transformation()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	EQUITY = 11.0;
	LOAN_LOSS_RESERVE_FIRM_LOANS = 0.0;

	/***** Messages: initialize message boards **********************************/
//    rc = MB_Create(&b_<message>, sizeof(m_<message>));
    	    
	/***** Messages: pre-conditions **********************************/
//    add_<message>_message();

    /***** Function evaluation ***************************************/
	liability_transformation(&EQUITY, &LOAN_LOSS_RESERVE_FIRM_LOANS, 1.0);
    	    
    /***** Message: Adding message iterators ***************************************/
//	rc = MB_Iterator_Create(b_<message>, &i_<message>);
			
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(EQUITY, 10.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(LOAN_LOSS_RESERVE_FIRM_LOANS, 1.0, 1e-3);

	/***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
	
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void asset_transformation(double * from, double * to, double value)
 * \brief: Unit test for: Bank_function.
 * Status: NOT Tested
 */
void unittest_asset_transformation()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	SEIZED_COLLATERAL = 10.0;
	RESERVES = 10.0;
	
	/***** Messages: initialize message boards **********************************/
//    rc = MB_Create(&b_<message>, sizeof(m_<message>));
    	    
	/***** Messages: pre-conditions **********************************/
//    add_<message>_message();

    /***** Function evaluation ***************************************/
	asset_transformation(&SEIZED_COLLATERAL, &RESERVES, 1.0);
    	    
    /***** Message: Adding message iterators ***************************************/
//	rc = MB_Iterator_Create(b_<message>, &i_<message>);
			
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(SEIZED_COLLATERAL, 9.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(RESERVES, 11.0, 1e-3);
	
    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
	START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
	
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
