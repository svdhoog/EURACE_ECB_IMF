#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"


/*
Unittests in this file (all in common with decentralized auction):

Household_check_housing_state
Household_sell_real_estate
Household_buy_real_estate
Household_receive_real_estate_transaction_as_seller
Household_receive_real_estate_transaction_as_buyer
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
//void unittest_Household_function()
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
//	Household_function();
    
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
 * \fn: void unittest_Household_check_housing_state_1()
 * \brief: Unit test for: Household_check_housing_state.
 * Status: NOT Tested
 */
void unittest_Household_check_housing_state_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	HOUSING_MARKET_PRICE_INDEX = 100.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_market_data, sizeof(m_real_estate_market_data));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
   	double rental_market_price_mean   = 1.0;
	double housing_market_price_mean  = 1.0;
	double housing_market_price_index = 1.0;
	
	add_real_estate_market_data_message(rental_market_price_mean, housing_market_price_mean, housing_market_price_index);

	MB_Iterator_Create(b_real_estate_market_data, &i_real_estate_market_data);

	START_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP
		printf("In msg: housing_market_price_index = %f", real_estate_market_data_message->housing_market_price_index);
		CU_ASSERT_DOUBLE_EQUAL(real_estate_market_data_message->housing_market_price_index, 1.0, 1e-3);
	FINISH_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_market_data, &i_real_estate_market_data);

    /***** Function evaluation ***************************************/
	Household_check_housing_state();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(HOUSING_MARKET_PRICE_INDEX, 1.0, 1e-3);

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
 * \fn: void unittest_Household_check_housing_state_2()
 * \brief: Unit test for: Household_check_housing_state.
 * Status: NOT Tested
 */
void unittest_Household_check_housing_state_2()
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
	Household_check_housing_state();
    
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
 * \fn: void unittest_Household_sell_real_estate_1()
 * \brief: Unit test for: Household_sell_real_estate.
 * Case 1: PLAN_SELL_HMR =1
 * Status: PASS 5.1.17
 */
void unittest_Household_sell_real_estate_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
	FLAME_environment_variable_print_debug_housing = 1;
    
    /***** Variables: Memory pre-conditions **************************/
	PLAN_SELL_HMR =1;
	PLAN_SELL_OP=0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = 10.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));

	/***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message();
   	//add_real_estate_price_ask_message(double value, ask_adt house);


    /***** Function evaluation ***************************************/
	Household_sell_real_estate();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->price, 10.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->ask.price, 10.0, 1e-3);
	FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_sell_real_estate_2()
 * \brief: Unit test for: Household_sell_real_estate.
 * Case 2: PLAN_SELL_OP=1
 * Status: PASS 5.1.17
 */
void unittest_Household_sell_real_estate_2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_housing = 1;
	
    /***** Variables: Memory pre-conditions **************************/
	PLAN_SELL_HMR =0;
	PLAN_SELL_OP=1;

	rental_adt house;
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, house.object_id, house.tenant_id, house.landlord_id, house.monthly_rent, house.price_index_base, house.rent_price_index_base, house.sold, house.value, house.hpi_base, house.purchase_day);

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].object_id = 101;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].value = 11.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));

	/***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message();
   	//add_real_estate_price_ask_message(double value, ask_adt house);


    /***** Function evaluation ***************************************/
	Household_sell_real_estate();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->price, 11.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->ask.price, 11.0, 1e-3);
	FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_buy_real_estate()
 * \brief: Unit test for: Household_buy_real_estate.
 * Status: PASS 5.1.17
 */
void unittest_Household_buy_real_estate()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	HOUSING_BID_PRICE = 1.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));

	/***** Messages: pre-conditions **********************************/
    //add_real_estate_price_bid_message();
    	    
    /***** Function evaluation ***************************************/
	Household_buy_real_estate();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(real_estate_price_bid_message->price, 1.0);
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_bid_message->bid.price, 1.0, 1e-3);
	FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_real_estate_transaction_as_seller_1()
 * \brief: Unit test for: Household_receive_real_estate_transaction_as_seller.
 * Case 1: PLAN_SELL_HMR =1 
 * Status: PASS 5.1.17
 */
void unittest_Household_receive_real_estate_transaction_as_seller_1()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_housing = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=1;
	PLAN_SELL_HMR =1;
	PLAN_SELL_OP=0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = 10.0;

	PAYMENT_ACCOUNT=0.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_transaction, sizeof(m_real_estate_transaction));

	/***** Messages: pre-conditions **********************************/
    
 	house_adt transaction;
			
	transaction.seller_id = 1;
	transaction.buyer_id  = 2;
	transaction.price = 20.0;

	add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Function evaluation ***************************************/
	Household_receive_real_estate_transaction_as_seller();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 20.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_real_estate_transaction_as_seller_2()
 * \brief: Unit test for: Household_receive_real_estate_transaction_as_seller.
 * Case 2: PLAN_SELL_OP =1 
 * Status: PASS 5.1.17
 */
void unittest_Household_receive_real_estate_transaction_as_seller_2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_housing = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=1;
	PLAN_SELL_HMR =0;
	PLAN_SELL_OP=1;

	rental_adt house;
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, house.object_id, house.tenant_id, house.landlord_id, house.monthly_rent, house.price_index_base, house.rent_price_index_base, house.sold, house.value, house.hpi_base, house.purchase_day);

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = 10.0;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].value = 11.0;

	PAYMENT_ACCOUNT=0.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_transaction, sizeof(m_real_estate_transaction));

	/***** Messages: pre-conditions **********************************/
    
 	house_adt transaction;
	transaction.object_id = 101;
	transaction.seller_id = 1;
	transaction.buyer_id  = 2;
	transaction.price = 20.0;

	add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Function evaluation ***************************************/
	Household_receive_real_estate_transaction_as_seller();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 20.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_real_estate_transaction_as_buyer_1()
 * \brief: Unit test for: Household_receive_real_estate_transaction_as_buyer.
 * Case 1: PLAN_BUY_HMR =1 
 * Status: PASS 5.1.17
 */
void unittest_Household_receive_real_estate_transaction_as_buyer_1()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_housing = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=2;
	PLAN_BUY_HMR =1;
	PLAN_BUY_OP=0;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = 0.0;

	PAYMENT_ACCOUNT=20.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_transaction, sizeof(m_real_estate_transaction));

	/***** Messages: pre-conditions **********************************/
    
 	house_adt transaction;
			
	transaction.seller_id = 1;
	transaction.buyer_id  = 2;
	transaction.price = 20.0;

	add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Function evaluation ***************************************/
	Household_receive_real_estate_transaction_as_buyer();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value, 20.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_real_estate_transaction_as_buyer_2()
 * \brief: Unit test for: Household_receive_real_estate_transaction_as_buyer.
 * Case 2: PLAN_BUY_OP =1 
 * Status: PASS 5.1.17
 */
void unittest_Household_receive_real_estate_transaction_as_buyer_2()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_housing = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=2;
	PLAN_BUY_HMR =0;
	PLAN_BUY_OP=1;

	PAYMENT_ACCOUNT=20.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_transaction, sizeof(m_real_estate_transaction));

	/***** Messages: pre-conditions **********************************/
    
 	house_adt transaction;

	transaction.object_id = 101;
	transaction.seller_id = 1;
	transaction.buyer_id  = 2;
	transaction.price = 20.0;

	add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Function evaluation ***************************************/
	Household_receive_real_estate_transaction_as_buyer();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].value, 20.0, 1e-3);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].object_id, 101);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
