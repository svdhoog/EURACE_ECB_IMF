#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../House_agent_header.h"
#include "../../../my_library_header.h"

/*
 * 1) initialize agent
 * 2) add memory pre-conditions
 * 3) initialise MBs
 * 4) add message pre-conditions
 * 5) evaluate function to be tested
 * 6) assertions on memory post-conditions
 * 7) create MB iterators
 * 8) assertions on message post-conditions
 * 9) free agent
 * 10) free MBs
 */

/*
Unittests in this file:

House_send_housing_availability
House_receive_housing_bids
House_send_housing_offers
House_receive_housing_acceptances
House_send_housing_transaction
*/

/*
 * \fn: void unittest_House_function()
 * \brief: Unit test for: House_function.
 * Status: NOT Tested
 */
// void unittest_House_function()
// {
// //	int rc;
	
//     /************* At start of unit test, add one agent **************/
// 	unittest_init_House_agent();
	
//     /***** Variables: Memory pre-conditions **************************/

// 	/***** Messages: initialize message boards **********************************/
// 	//MB_Create(&b_<message>, sizeof(m_<message>));

// 	/***** Messages: pre-conditions **********************************/
//     //add_<message>_message();
    	    
//     /***** Function evaluation ***************************************/
// 	House_function();
    
//     /***** Variables: Memory post-conditions *****/
// //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


//     /***** Message: Adding message iterators ***************************************/
// 	//MB_Iterator_Create(b_<message>, &i_<message>);

//     /***** Messages: Message post-conditions *****/
// 	//start a reading loop
// /*
//     START_<message>_MESSAGE_LOOP
// 	     CU_ASSERT_EQUAL(<message>_message->var, value);
// 	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
// 	FINISH_<message>_MESSAGE_LOOP
// */	
//     /************* At end of unit test, free the agent **************/
// 	unittest_free_House_agent();

//     /************* At end of unit tests, free all Messages **********/
//     free_messages();
// }

/*
 * \fn: void unittest_House_send_housing_availability()
 * \brief: Unit test for: House_send_housing_availability.
 * Status: NOT Tested
 */
void unittest_House_send_housing_availability()
{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_House_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
	House_send_housing_availability();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_House_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_receive_housing_bids()
 * \brief: Unit test for: House_receive_housing_bids.
 * Status: NOT Tested
 */
void unittest_House_receive_housing_bids()
{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_House_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
	House_receive_housing_bids();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_House_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_House_send_housing_offers()
 * \brief: Unit test for: House_send_housing_offers.
 * Status: NOT Tested
 */
void unittest_House_send_housing_offers()
{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_House_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
	House_send_housing_offers();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_House_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_House_receive_housing_acceptances()
 * \brief: Unit test for: House_receive_housing_acceptances.
 * Status: NOT Tested
 */
void unittest_House_receive_housing_acceptances()
{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_House_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
	House_receive_housing_acceptances();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_House_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_House_send_housing_transaction()
 * \brief: Unit test for: House_send_housing_transaction.
 * Status: NOT Tested
 */
void unittest_House_send_housing_transaction()
{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_House_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
	House_send_housing_transaction();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
	unittest_free_House_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
