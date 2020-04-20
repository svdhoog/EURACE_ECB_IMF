#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"

/*
Functions in centralized unittest code file:

Household_check_housing_state
Household_sell_housing_units
Household_send_housing_bid
Household_receive_housing_transaction_as_seller
Household_receive_housing_transaction_as_buyer

Unittests in this file:

Household_receive_housing_availability
Household_receive_housing_offers
Household_send_housing_acceptance
*/

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
// void unittest_Household_()
// {
	
//     /************* At start of unit test, add one agent **************/
// 	unittest_init_Household_agent();
	
//     /***** Variables: Memory pre-conditions **************************/

// 	/***** Messages: initialize message boards **********************************/
// /*
// 	MB_Create(&b_<message>, sizeof(m_<message>));
// */	
// 	/***** Messages: pre-conditions **********************************/
//     //add_<message>_message();
    	    
//     /***** Message: Adding message iterators ***************************************/
// 	//MB_Iterator_Create(b_<message>, &i_<message>);

//     **** Function evaluation **************************************
// 	Household_function();
    
//     /***** Variables: Memory post-conditions *****/
// //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

//     /***** Messages: Message post-conditions *****/
// 	//start a reading loop
// /*
//     START_<message>_MESSAGE_LOOP
// 	     CU_ASSERT_EQUAL(<message>_message->var, value);
// 	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
// 	FINISH_<message>_MESSAGE_LOOP
// */	
//     /************* At end of unit test, free the agent **************/
// 	unittest_free_Household_agent();
//     /************* At end of unit tests, free all Messages **********/
//     free_messages();
// }




/*
 * \fn: void unittest_Household_receive_housing_availability()
 * \brief: Unit test for: Household_receive_housing_availability.
 * Status: NOT Tested
 */
void unittest_Household_receive_housing_availability()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
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
	Household_receive_housing_availability();
    
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
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_housing_offers()
 * \brief: Unit test for: Household_receive_housing_offers.
 * Status: NOT Tested
 */
void unittest_Household_receive_housing_offers()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
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
	Household_receive_housing_offers();
    
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
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_send_housing_acceptance()
 * \brief: Unit test for: Household_send_housing_acceptance.
 * Status: NOT Tested
 */
void unittest_Household_send_housing_acceptance()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
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
	Household_send_housing_acceptance();
    
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
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
