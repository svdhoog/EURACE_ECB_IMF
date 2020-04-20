#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../ClearingHouse_agent_header.h"
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

ClearingHouse_init_rent_matching
ClearingHouse_recycle_unprocessed_rental_bids
clearingHouse_receive_rental_bids
ClearingHouse_run_rental_auction
*/

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * Status: NOT Tested
 */
//void unittest_ClearingHouse_function()
//{
//	int rc;
	
    /************* At start of unit test, add one agent **************/
//	unittest_init_ClearingHouse_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Function evaluation ***************************************/
//	ClearingHouse_function();
    
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
//	unittest_free_ClearingHouse_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}

/*
 * \fn: void unittest_ClearingHouse_init_rent_matching_1()
 * \brief: Unit test for: ClearingHouse_init_rent_matching.
 * Case: No. Tenants > No. Landlord
 * Status: PASS 12.1.2017
 */
void unittest_ClearingHouse_init_rent_matching_1()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_ClearingHouse_agent();
	
    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_owner_info, sizeof(m_owner_info));
	MB_Create(&b_tenant_info, sizeof(m_tenant_info));
	MB_Create(&b_landlord_info, sizeof(m_landlord_info));
	
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));

	/***** Messages: pre-conditions **********************************/
    //add_tenant_info_message(ID, monthly_rent);
	add_tenant_info_message(1, 1.0);
	add_tenant_info_message(2, 2.0);
	add_tenant_info_message(3, 3.0);

    //add_landlord_info_message(ID, op_value);
	add_landlord_info_message(4, 0.0);
	add_landlord_info_message(5, 0.0);
    	    
    //add_owner_info_message(ID, hmr_value);
	add_owner_info_message(7, 0.0);
	add_owner_info_message(8, 0.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_owner_info, &i_owner_info);
	MB_Iterator_Create(b_tenant_info, &i_tenant_info);
	MB_Iterator_Create(b_landlord_info, &i_landlord_info);

    /***** Function evaluation ***************************************/
	ClearingHouse_init_rent_matching();
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);

    /***** Messages: Message post-conditions *****/

	//add_rent_matching_info_message(INIT_TENANTS.array[i].id, INIT_LANDLORDS.array[j], rental_object);
    START_RENT_MATCHING_INFO_MESSAGE_LOOP

		printf("\n Rent_matching Msg data: TENANT ID %d LANDLORD ID %d\n", rent_matching_info_message->tenant_id, rent_matching_info_message->landlord_id);

		printf("\n Rent_matching Msg rental_unit data: TENANT ID %d LANDLORD ID %d Rent: %.2f object_id %d\n", rent_matching_info_message->rental_unit.tenant_id, rent_matching_info_message->rental_unit.landlord_id, rent_matching_info_message->rental_unit.monthly_rent, rent_matching_info_message->rental_unit.object_id);

	FINISH_RENT_MATCHING_INFO_MESSAGE_LOOP

	//add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
    START_HMR_MATCHING_INFO_MESSAGE_LOOP

		printf("\n Msg data: OWNER ID %d object_id %d\n", hmr_matching_info_message->owner_id, hmr_matching_info_message->object_id);
	
	FINISH_HMR_MATCHING_INFO_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
	unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_init_rent_matching_2()
 * \brief: Unit test for: ClearingHouse_init_rent_matching.
 * Case: No. Tenants < No. Landlord
 * Status: PASS 12.1.2017
 */
void unittest_ClearingHouse_init_rent_matching_2()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_ClearingHouse_agent();
	
    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_owner_info, sizeof(m_owner_info));
	MB_Create(&b_tenant_info, sizeof(m_tenant_info));
	MB_Create(&b_landlord_info, sizeof(m_landlord_info));
	
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));

	/***** Messages: pre-conditions **********************************/
    //add_tenant_info_message(ID, monthly_rent);
	add_tenant_info_message(1, 1.0);

    //add_landlord_info_message(ID, op_value);
	add_landlord_info_message(4, 0.0);
	add_landlord_info_message(5, 0.0);
    	    
    //add_owner_info_message(ID, hmr_value);
	add_owner_info_message(7, 0.0);
	add_owner_info_message(8, 0.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_owner_info, &i_owner_info);
	MB_Iterator_Create(b_tenant_info, &i_tenant_info);
	MB_Iterator_Create(b_landlord_info, &i_landlord_info);

    /***** Function evaluation ***************************************/
	ClearingHouse_init_rent_matching();
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);

    /***** Messages: Message post-conditions *****/

	//add_rent_matching_info_message(INIT_TENANTS.array[i].id, INIT_LANDLORDS.array[j], rental_object);
    START_RENT_MATCHING_INFO_MESSAGE_LOOP

		printf("\n Rent_matching Msg data: TENANT ID %d LANDLORD ID %d\n", rent_matching_info_message->tenant_id, rent_matching_info_message->landlord_id);

		printf("\n Rent_matching Msg rental_unit data: TENANT ID %d LANDLORD ID %d Rent: %.2f object_id %d\n", rent_matching_info_message->rental_unit.tenant_id, rent_matching_info_message->rental_unit.landlord_id, rent_matching_info_message->rental_unit.monthly_rent, rent_matching_info_message->rental_unit.object_id);

	FINISH_RENT_MATCHING_INFO_MESSAGE_LOOP

	//add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
    START_HMR_MATCHING_INFO_MESSAGE_LOOP

		printf("\n Msg data: OWNER ID %d object_id %d\n", hmr_matching_info_message->owner_id, hmr_matching_info_message->object_id);
	
	FINISH_HMR_MATCHING_INFO_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
	unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_ClearingHouse_init_rent_matching_3()
 * \brief: Unit test for: ClearingHouse_init_rent_matching.
 * Case: All Tenants are Landlords; test self-tenant vs. non self-tenant case
 * Status: PASS 
 */
void unittest_ClearingHouse_init_rent_matching_3()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_ClearingHouse_agent();
	
    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_owner_info, sizeof(m_owner_info));
	MB_Create(&b_tenant_info, sizeof(m_tenant_info));
	MB_Create(&b_landlord_info, sizeof(m_landlord_info));
	
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));

	/***** Messages: pre-conditions **********************************/
    //add_tenant_info_message(ID, monthly_rent);
	add_tenant_info_message(1, 1.0);
	add_tenant_info_message(2, 1.0);
	add_tenant_info_message(3, 1.0);

    //add_landlord_info_message(ID, op_value);
	add_landlord_info_message(1, 0.0);
	add_landlord_info_message(2, 0.0);
	add_landlord_info_message(3, 0.0);
    	    
    //add_owner_info_message(ID, hmr_value);
	//add_owner_info_message(7, 0.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_owner_info, &i_owner_info);
	MB_Iterator_Create(b_tenant_info, &i_tenant_info);
	MB_Iterator_Create(b_landlord_info, &i_landlord_info);

    /***** Function evaluation ***************************************/
	ClearingHouse_init_rent_matching();
    
    /***** Variables: Memory post-conditions *****/
	//CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);

    /***** Messages: Message post-conditions *****/

	//add_rent_matching_info_message(INIT_TENANTS.array[i].id, INIT_LANDLORDS.array[j], rental_object);
    START_RENT_MATCHING_INFO_MESSAGE_LOOP

		printf("\n Rent_matching Msg data: TENANT ID %d LANDLORD ID %d\n", rent_matching_info_message->tenant_id, rent_matching_info_message->landlord_id);

		//printf("\n Rent_matching Msg rental_unit data: TENANT ID %d LANDLORD ID %d Rent: %.2f object_id %d\n", rent_matching_info_message->rental_unit.tenant_id, rent_matching_info_message->rental_unit.landlord_id, rent_matching_info_message->rental_unit.monthly_rent, rent_matching_info_message->rental_unit.object_id);

	FINISH_RENT_MATCHING_INFO_MESSAGE_LOOP


    /************* At end of unit test, free the agent **************/
	unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_recycle_unprocessed_rental_bids()
 * \brief: Unit test for: ClearingHouse_recycle_unprocessed_rental_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_recycle_unprocessed_rental_bids()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
    //void add_bid_adt(bid_adt_array * array,  int id,  double price)
    add_bid_adt(&RENTAL_AUCTION.unprocessed.bids, 0, 0.0);

    //void add_ask_adt(ask_adt_array * array,  int id,  double price)
    add_ask_adt(&RENTAL_AUCTION.unprocessed.asks, 0, 1, 1.0);

    /***** Messages: initialize message boards **********************************/
    MB_Create(&b_rental_price_bid, sizeof(m_rental_price_bid));
    MB_Create(&b_rental_price_ask, sizeof(m_rental_price_ask));

    /***** Messages: pre-conditions **********************************/
    //add_rental_price_ask_message(price, ask_adt ask);
    //add_rental_price_bid_message(price, bid_adt bid);
            
    /***** Function evaluation ***************************************/
    ClearingHouse_recycle_unprocessed_rental_bids();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_price_bid, &i_rental_price_bid);
    MB_Iterator_Create(b_rental_price_ask, &i_rental_price_ask);

    /***** Messages: Message post-conditions *****/
    //start a reading loop

    START_RENTAL_PRICE_BID_MESSAGE_LOOP
         CU_ASSERT_EQUAL(rental_price_bid_message->bid.id, 0);
         CU_ASSERT_DOUBLE_EQUAL(rental_price_bid_message->bid.price, 0.0, 1e-3);
    FINISH_RENTAL_PRICE_BID_MESSAGE_LOOP

    START_RENTAL_PRICE_ASK_MESSAGE_LOOP
         CU_ASSERT_EQUAL(rental_price_ask_message->ask.id, 1);
         CU_ASSERT_DOUBLE_EQUAL(rental_price_ask_message->ask.price, 1.0, 1e-3);
    FINISH_RENTAL_PRICE_ASK_MESSAGE_LOOP
  

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_receive_rental_bids()
 * \brief: Unit test for: ClearingHouse_receive_rental_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_receive_rental_bids()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_rental_price_bid, sizeof(m_rental_price_bid));
    rc = MB_Create(&b_rental_price_bid, sizeof(m_rental_price_bid));

    //MB_Create(&b_rental_price_ask, sizeof(m_rental_price_ask));
    rc = MB_Create(&b_rental_price_ask, sizeof(m_rental_price_ask));
    
    /***** Messages: pre-conditions **********************************/
    //add_rental_price_ask_message(double value, ask_adt ask);
    ask_adt ask;

    ask.id = 0;
    
    ask.price = 2.0;
    add_rental_price_ask_message(2.0, ask);

    ask.price = 3.0;
    add_rental_price_ask_message(3.0, ask);

    ask.price = 5.0;
    add_rental_price_ask_message(5.0, ask);

    ask.price = 7.0;
    add_rental_price_ask_message(7.0, ask);


    //add_rental_price_bid_message(double value, bid_adt bid);    
    bid_adt bid;

    bid.id = 0;
    
    bid.price = 1.0;
    add_rental_price_bid_message(1.0, bid);

    bid.price = 4.0;
    add_rental_price_bid_message(4.0, bid);

    bid.price = 6.0;
    add_rental_price_bid_message(6.0, bid);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_price_ask, &i_rental_price_ask);
    MB_Iterator_Create(b_rental_price_bid, &i_rental_price_bid);

    /***** Function evaluation ***************************************/
    ClearingHouse_receive_rental_bids();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.bids.array[0].price, 1.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.bids.array[1].price, 4.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.bids.array[2].price, 6.0, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.asks.array[0].price, 2.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.asks.array[1].price, 3.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.asks.array[2].price, 5.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.pending.asks.array[3].price, 7.0, 1e-3);

    /***** Message: Adding message iterators ***************************************/
    //MB_Iterator_Create(b_rental_price_bid, &i_rental_price_bid);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
/*
    START_rental_price_bid_MESSAGE_LOOP
         CU_ASSERT_EQUAL(rental_price_bid_message->var, value);
         CU_ASSERT_DOUBLE_EQUAL(rental_price_bid_message->var, value, 1e-3);
    FINISH_RENTAL_PRICE_BID_MESSAGE_LOOP
*/  


    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: Empty lists 
 * Status: PASS 5.1.16
 */
void unittest_ClearingHouse_run_rental_auction_0()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_rental_matching_message(transaction.tenant_id, transaction.landlord_id, transaction);

    //Msg list empty, so no printf:
    START_RENTAL_MATCHING_MESSAGE_LOOP
         printf("rental_matching_message->landlord_id  %d", rental_matching_message->landlord_id);
    FINISH_RENTAL_MATCHING_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: Multiple bids, asks, 2 pairs of matching
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_run_rental_auction_1()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_rental = 1;


	add_bid_adt(&RENTAL_AUCTION.pending.bids, 0, 1.0);
	add_bid_adt(&RENTAL_AUCTION.pending.bids, 0, 4.0);
	add_bid_adt(&RENTAL_AUCTION.pending.bids, 0, 6.0); 
	                                  
	add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 0, 2.0);
	add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 0, 3.0);
	add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 0, 5.0);
	add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 0, 7.0);

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.bids.size, 0);

    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.asks.size, 2);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.unprocessed.asks.array[0].price, 7.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(RENTAL_AUCTION.unprocessed.asks.array[1].price, 2.0, 1e-3);

    printf("\n\n RENTAL_AUCTION.pending.bids.size=%d", RENTAL_AUCTION.pending.bids.size);
    printf("\n RENTAL_AUCTION.pending.asks.size=%d", RENTAL_AUCTION.pending.asks.size);

    printf("\n RENTAL_AUCTION.unprocessed.bids.size=%d", RENTAL_AUCTION.unprocessed.bids.size);
    printf("\n RENTAL_AUCTION.unprocessed.asks.size=%d", RENTAL_AUCTION.unprocessed.asks.size);
    printf("\n");

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
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: 1 bid, 1 ask, 1 match 
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_run_rental_auction_2()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_rental = 1;

    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 1, 5.0);

    add_bid_adt(&RENTAL_AUCTION.pending.bids, 2, 6.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_rental_matching_message(transaction.tenant_id, transaction.landlord_id, transaction);

    START_RENTAL_MATCHING_MESSAGE_LOOP
         CU_ASSERT_EQUAL(rental_matching_message->landlord_id, 1);
         CU_ASSERT_EQUAL(rental_matching_message->tenant_id, 2);
         CU_ASSERT_EQUAL(rental_matching_message->rental_unit.landlord_id, 1);
         CU_ASSERT_EQUAL(rental_matching_message->rental_unit.tenant_id, 2);
         CU_ASSERT_DOUBLE_EQUAL(rental_matching_message->rental_unit.monthly_rent, 6.0, 1e-3);
    FINISH_RENTAL_MATCHING_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: 0 bids, 1 ask
 * Status: PASS 5.1.16
 */
void unittest_ClearingHouse_run_rental_auction_3()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_rental = 1;

    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 1, 5.0);

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.asks.size, 1);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_rental_matching_message(transaction.tenant_id, transaction.landlord_id, transaction);

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: 1 bid, 0 asks
 * Status: PASS 5.1.16
 */
void unittest_ClearingHouse_run_rental_auction_4()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_rental = 1;

    add_bid_adt(&RENTAL_AUCTION.pending.bids, 2, 6.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.bids.size, 1);
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_rental_matching_message(transaction.tenant_id, transaction.landlord_id, transaction);

    START_RENTAL_MATCHING_MESSAGE_LOOP
         CU_ASSERT_EQUAL(rental_matching_message->landlord_id, 1);
         CU_ASSERT_EQUAL(rental_matching_message->tenant_id, 2);
         CU_ASSERT_EQUAL(rental_matching_message->rental_unit.landlord_id, 1);
         CU_ASSERT_EQUAL(rental_matching_message->rental_unit.tenant_id, 2);
         CU_ASSERT_DOUBLE_EQUAL(rental_matching_message->rental_unit.monthly_rent, 6.0, 1e-3);
    FINISH_RENTAL_MATCHING_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: 5 bids, 5 asks all equal price, test if tie-breaking uses randomization or not after FLAME sorting.
 * Status: PASS 5.1.16
 * Result: Msgs with equal bid price: processed last-in-first-out (LIFO, since MB works as a stack, so no randomization of ties)
 */
void unittest_ClearingHouse_run_rental_auction_5()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_rental = 1;

    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 1, 5.0);
    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 2, 5.0);
    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 3, 5.0);
    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 4, 5.0);
    add_ask_adt(&RENTAL_AUCTION.pending.asks, 0, 5, 5.0);

    add_bid_adt(&RENTAL_AUCTION.pending.bids, 6, 5.0); 
    add_bid_adt(&RENTAL_AUCTION.pending.bids, 7, 5.0); 
    add_bid_adt(&RENTAL_AUCTION.pending.bids, 8, 5.0); 
    add_bid_adt(&RENTAL_AUCTION.pending.bids, 9, 5.0); 
    add_bid_adt(&RENTAL_AUCTION.pending.bids, 10, 5.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_rental_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(RENTAL_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_rental_matching_message(transaction.tenant_id, transaction.landlord_id, transaction);

    START_RENTAL_MATCHING_MESSAGE_LOOP
        printf("\n B=%d S=%d\n", rental_matching_message->tenant_id, rental_matching_message->landlord_id);
    FINISH_RENTAL_MATCHING_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
