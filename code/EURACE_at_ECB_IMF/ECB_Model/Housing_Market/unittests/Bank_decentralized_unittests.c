#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Bank_agent_header.h"
#include "../../../my_library_header.h"

/*
Functions in code file: decentralized housing auction

Bank_sell_housing_units: creates house agents
Bank_receive_housing_transaction
*/

/************Bank Role: Housing Market********************************/
/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * \brief: - input mortgage_info_message(household_id, bank_id, mortgage_contract_adt mortgage)
 * \brief: Status: NOT Tested or PASS date
 */
//void unittest_Bank_()
//{

	
    /************* At start of unit test, add one agent **************/
	//unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
    
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_default, sizeof(m_mortgage_default));
	
	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_default, &i_mortgage_default);

	//HH_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_payment_message(int household_id, int bank_id, double principal, double interest)

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
 * \fn: void unittest_Bank_resell_seized_housing_units()
 * \brief: Unit test for: Bank_resell_seized_housing_units.
 * Status: NOT Tested
 */
void unittest_Bank_sell_housing_units()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();

	FLAME_environment_variable_print_debug_housing = 1;

    /***** Variables: Memory pre-conditions **************************/
	
 	house_adt house; //seller_id, buyer_id, value, price
			
	house.seller_id = 1;
	house.buyer_id  = 2;
	house.value = 10.0;
	house.price = 0.0;

	add_house_adt(&SEIZED_COLLATERAL);
    
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_housing_price_ask, sizeof(m_housing_price_ask));
	
	/***** Messages: pre-conditions **********************************/


    /***** Function evaluation ***************************************/
	Bank_sell_housing_units();
    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_housing_price_ask, &i_housing_price_ask);

    START_HOUSING_PRICE_ASK_MESSAGE_LOOP
	     CU_ASSERT_DOUBLE_EQUAL(housing_price_ask_message->price, 10.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(housing_price_ask_message->ask.price, 10.0, 1e-3);
	FINISH_HOUSING_PRICE_ASK_MESSAGE_LOOP

    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}



/*
 * \fn: void unittest_Bank_receive_housing_transaction()
 * \brief: Unit test for: Bank_receive_housing_transaction.
 * \brief: - input mortgage_info_message(household_id, bank_id, mortgage_contract_adt mortgage)
 * \brief: Status: NOT Tested or PASS date
 */
void unittest_Bank_receive_housing_transaction()
{

	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	//FLAME_environment_variable_print_debug_mortgage = 1;
    
	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_default, sizeof(m_mortgage_default));
	
	/***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_default, &i_mortgage_default);

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/**** Messages: pre-conditions *********************************/
    //add_mortgage_payment_message(int household_id, int bank_id, double principal, double interest)

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_receive_housing_transaction();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);


    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
