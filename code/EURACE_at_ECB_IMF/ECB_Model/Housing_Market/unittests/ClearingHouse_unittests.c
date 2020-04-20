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

ClearingHouse_send_unprocessed_bids
ClearingHouse_receive_real_estate_bids
ClearingHouse_run_real_estate_auction
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
 * \fn: void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_0()
 * \brief: Unit test for: ClearingHouse_recycle_unprocessed_real_estate_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_0()
{
    int rc;
    double price;

    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    FLAME_environment_variable_print_debug_housing = 1;

    /***** Variables: Memory pre-conditions **************************/
    //void add_bid_adt(bid_adt_array * array,  int id,  double price)
    add_bid_adt(&HOUSE_AUCTION.unprocessed.bids, 0, 0.01);


    //price = -0/0; //"Floating point exception"
    //price = -0.0/0; // -nan <== MOST LIKELY CAUSE OF ERRORS
    //price = -0.0/0.0; // -inf
    price = (-0.0/0.0)/(0.0/0.0); // -inf/inf = nan
    //price = sqrt(-1) //-nan
    
    //void add_ask_adt(ask_adt_array * array,  int object_id,  int id,  double price)
    add_ask_adt(&HOUSE_AUCTION.unprocessed.asks, 0, 1, price);

    /***** Messages: initialize message boards **********************************/
    MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));
    MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));

    /***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message(price, ask_adt ask);
    //add_real_estate_price_bid_message(price, bid_adt bid);
            
    /***** Function evaluation ***************************************/
    ClearingHouse_recycle_unprocessed_real_estate_bids();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);
    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);

    /***** Messages: Message post-conditions *****/
    //start a reading loop

    START_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_bid_message->bid.id, 0);
         //CU_ASSERT_DOUBLE_EQUAL(real_estate_price_bid_message->bid.price, 0.0, 1e-3);
    FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP

    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_ask_message->ask.id, 1);
         //CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->ask.price, 1.0, 1e-3);
    FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
  

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_1()
 * \brief: Unit test for: ClearingHouse_recycle_unprocessed_real_estate_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_1()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    FLAME_environment_variable_print_debug_housing = 1;

    /***** Variables: Memory pre-conditions **************************/
    //void add_bid_adt(bid_adt_array * array,  int id,  double price)
    add_bid_adt(&HOUSE_AUCTION.unprocessed.bids, 0, 0.0);

    //void add_ask_adt(ask_adt_array * array,  int id,  double price)
    add_ask_adt(&HOUSE_AUCTION.unprocessed.asks, 0, 1, 1.0);

    /***** Messages: initialize message boards **********************************/
    MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));
    MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));

    /***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message(price, ask_adt ask);
    //add_real_estate_price_bid_message(price, bid_adt bid);
            
    /***** Function evaluation ***************************************/
    ClearingHouse_recycle_unprocessed_real_estate_bids();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);
    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);

    /***** Messages: Message post-conditions *****/
    //start a reading loop

    START_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_bid_message->bid.id, 0);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_price_bid_message->bid.price, 0.0, 1e-3);
    FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP

    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_ask_message->ask.id, 1);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->ask.price, 1.0, 1e-3);
    FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
  

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_receive_real_estate_bids_0()
 * \brief: Unit test for: ClearingHouse_receive_real_estate_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_receive_real_estate_bids_0()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));
    rc = MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));

    //MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    rc = MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    
    /***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message(double value, ask_adt ask);
    ask_adt ask;

    ask.id = 0;
    
    //ask.price = -0/0; //"Floating point exception"
    //ask.price = -0.0/0; // -nan <== MOST LIKELY CAUSE OF ERRORS
    //ask.price = -0.0/0.0; // -inf
    ask.price = (-0.0/0.0)/(0.0/0.0); // -inf/inf = nan
    //ask.price = sqrt(-1) //-nan
    
    add_real_estate_price_ask_message(2.0, ask);


    //add_real_estate_price_bid_message(double value, bid_adt bid);    
    bid_adt bid;

    bid.id = 0;
    
    bid.price = 0.01;
    add_real_estate_price_bid_message(1.0, bid);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);
    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Function evaluation ***************************************/
    ClearingHouse_receive_real_estate_bids();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[0].price, 0.0, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[0].price, 0.0, 1e-3);

    /***** Message: Adding message iterators ***************************************/
    //MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
/*
    START_real_estate_price_bid_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_bid_message->var, value);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_price_bid_message->var, value, 1e-3);
    FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
*/  


    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_ClearingHouse_receive_real_estate_bids_1()
 * \brief: Unit test for: ClearingHouse_receive_real_estate_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_receive_real_estate_bids_1()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));
    rc = MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));

    //MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    rc = MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    
    /***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message(double value, ask_adt ask);
    ask_adt ask;

    ask.id = 0;
    
    ask.price = 2.0;
    add_real_estate_price_ask_message(2.0, ask);

    ask.price = 3.0;
    add_real_estate_price_ask_message(3.0, ask);

    ask.price = 5.0;
    add_real_estate_price_ask_message(5.0, ask);

    ask.price = 7.0;
    add_real_estate_price_ask_message(7.0, ask);


    //add_real_estate_price_bid_message(double value, bid_adt bid);    
    bid_adt bid;

    bid.id = 0;
    
    bid.price = 1.0;
    add_real_estate_price_bid_message(1.0, bid);

    bid.price = 4.0;
    add_real_estate_price_bid_message(4.0, bid);

    bid.price = 6.0;
    add_real_estate_price_bid_message(6.0, bid);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);
    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Function evaluation ***************************************/
    ClearingHouse_receive_real_estate_bids();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[0].price, 1.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[1].price, 4.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[2].price, 6.0, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[0].price, 2.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[1].price, 3.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[2].price, 5.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[3].price, 7.0, 1e-3);

    /***** Message: Adding message iterators ***************************************/
    //MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
/*
    START_real_estate_price_bid_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_price_bid_message->var, value);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_price_bid_message->var, value, 1e-3);
    FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
*/  


    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_ClearingHouse_receive_real_estate_bids_2()
 * \brief: Unit test for: ClearingHouse_receive_real_estate_bids.
 * Status: PASS 4.1.16
 */
void unittest_ClearingHouse_receive_real_estate_bids_2()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));
    rc = MB_Create(&b_real_estate_price_bid, sizeof(m_real_estate_price_bid));

    //MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    rc = MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
    
    /***** Messages: pre-conditions **********************************/
    //add_real_estate_price_ask_message(double value, ask_adt ask);
    ask_adt ask;

    ask.id = 0;
    

    ask.price = 3.0;
    add_real_estate_price_ask_message(3.0, ask);

    ask.price = 2.0;
    add_real_estate_price_ask_message(2.0, ask);

    ask.price = 5.0;
    add_real_estate_price_ask_message(5.0, ask);

    ask.price = 7.0;
    add_real_estate_price_ask_message(7.0, ask);

    //add_real_estate_price_bid_message(double value, bid_adt bid);    
    bid_adt bid;

    bid.id = 0;
    
    bid.price = 4.0;
    add_real_estate_price_bid_message(4.0, bid);

    bid.price = 1.0;
    add_real_estate_price_bid_message(1.0, bid);

    bid.price = 6.0;
    add_real_estate_price_bid_message(6.0, bid);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);
    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);

    /***** Function evaluation ***************************************/
    ClearingHouse_receive_real_estate_bids();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[0].price, 1.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[1].price, 4.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.bids.array[2].price, 6.0, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[0].price, 2.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[1].price, 3.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[2].price, 5.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.pending.asks.array[3].price, 7.0, 1e-3);


    /***** Message: Adding message iterators ***************************************/
//    MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);
//    MB_Iterator_Create(b_real_estate_price_bid, &i_real_estate_price_bid);
    
    /***** Messages: Message post-conditions *****/
    //start a reading loop

    printf("\n\n Bids:");
    START_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
        printf(" %2.2f ", real_estate_price_bid_message->bid.price);
    FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
  
    printf("\n\n Asks:");
    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
        printf(" %2.2f ", real_estate_price_ask_message->ask.price);
    FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
    printf("\n");

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
void unittest_ClearingHouse_run_real_estate_auction_0()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    FLAME_environment_variable_print_debug_housing = 1;

    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);

    //Msg list empty, so no printf:
    START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
         printf("real_estate_transaction_message->seller_id  %d", real_estate_transaction_message->seller_id);
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

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
void unittest_ClearingHouse_run_real_estate_auction_1()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_housing = 1;


	add_bid_adt(&HOUSE_AUCTION.pending.bids, 0, 1.0);
	add_bid_adt(&HOUSE_AUCTION.pending.bids, 0, 4.0);
	add_bid_adt(&HOUSE_AUCTION.pending.bids, 0, 6.0); 
	                                  
	add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 0, 2.0);
	add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 0, 3.0);
	add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 0, 5.0);
	add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 0, 7.0);

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 0);

    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 2);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.unprocessed.asks.array[0].price, 7.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSE_AUCTION.unprocessed.asks.array[1].price, 2.0, 1e-3);

    printf("\n\n HOUSE_AUCTION.pending.bids.size=%d", HOUSE_AUCTION.pending.bids.size);
    printf("\n HOUSE_AUCTION.pending.asks.size=%d", HOUSE_AUCTION.pending.asks.size);

    printf("\n HOUSE_AUCTION.unprocessed.bids.size=%d", HOUSE_AUCTION.unprocessed.bids.size);
    printf("\n HOUSE_AUCTION.unprocessed.asks.size=%d", HOUSE_AUCTION.unprocessed.asks.size);
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
void unittest_ClearingHouse_run_real_estate_auction_2()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_housing = 1;

    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 1, 5.0);

    add_bid_adt(&HOUSE_AUCTION.pending.bids, 2, 6.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);

    START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_transaction_message->seller_id, 1);
         CU_ASSERT_EQUAL(real_estate_transaction_message->buyer_id, 2);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_transaction_message->house.price, 6.0, 1e-3);
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

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
void unittest_ClearingHouse_run_real_estate_auction_3()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_housing = 1;

    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 1, 5.0);

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 1);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);

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
void unittest_ClearingHouse_run_real_estate_auction_4()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_housing = 1;

    add_bid_adt(&HOUSE_AUCTION.pending.bids, 2, 6.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 1);
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);

    START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
         CU_ASSERT_EQUAL(real_estate_transaction_message->seller_id, 1);
         CU_ASSERT_EQUAL(real_estate_transaction_message->buyer_id, 2);
         CU_ASSERT_DOUBLE_EQUAL(real_estate_transaction_message->house.price, 6.0, 1e-3);
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

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
void unittest_ClearingHouse_run_real_estate_auction_5()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_housing = 1;

    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 1, 5.0);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 2, 5.0);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 3, 5.0);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 4, 5.0);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 0, 5, 5.0);

    add_bid_adt(&HOUSE_AUCTION.pending.bids, 6, 5.0); 
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 7, 5.0); 
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 8, 5.0); 
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 9, 5.0); 
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 10, 5.0); 

    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 0);
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 0);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);

    START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
        printf("\n B=%d S=%d\n", real_estate_transaction_message->buyer_id, real_estate_transaction_message->seller_id);
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_ClearingHouse_function()
 * \brief: Unit test for: ClearingHouse_function.
 * \brief: 7 bids, 7 asks.
 * Status: TEST 7.3.17
 * Result: Msgs with equal bid price: processed last-in-first-out (LIFO, since MB works as a stack, so no randomization of ties)
 */
void unittest_ClearingHouse_run_real_estate_auction_6()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_ClearingHouse_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_print_debug_housing = 1;

    add_bid_adt(&HOUSE_AUCTION.pending.bids, 1249, 6.44);
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 1122, 1.99); 

    add_bid_adt(&HOUSE_AUCTION.pending.bids,  375, 6.95); 
    add_bid_adt(&HOUSE_AUCTION.pending.bids,  879, 6.60);

    add_bid_adt(&HOUSE_AUCTION.pending.bids,  955, 8.49);
    add_bid_adt(&HOUSE_AUCTION.pending.bids, 1533, 8.38);

    add_bid_adt(&HOUSE_AUCTION.pending.bids,  185, 9.65); 

    printf("\n Before\n");
    for (i=0; i<HOUSE_AUCTION.pending.bids.size; i++)
        printf("\n %2.2f\n", HOUSE_AUCTION.pending.bids.array[i]);


    add_ask_adt(&HOUSE_AUCTION.pending.asks, 1, 1133, 0.01);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 2,  472, 1.39); 
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 3,  476, 2.60); 
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 4, 1548, 4.37);
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 5,  577, 5.03); 
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 6,  133, 8.56); 
    add_ask_adt(&HOUSE_AUCTION.pending.asks, 7, 1298, 9.63);


    /***** Messages: initialize message boards **********************************/
    //MB_Create(&b_<message>, sizeof(m_<message>));

    /***** Messages: pre-conditions **********************************/
    //add_<message>_message();
            
    /***** Function evaluation ***************************************/
    ClearingHouse_run_real_estate_auction();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.bids.size, 1);
    CU_ASSERT_EQUAL(HOUSE_AUCTION.unprocessed.asks.size, 1);

    /***** Message: Adding message iterators ***************************************/
    MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Messages: Message post-conditions *****/
    //start a reading loop
    //add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);


    START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
        printf("\n B=%d S=%d \n", real_estate_transaction_message->buyer_id, real_estate_transaction_message->seller_id);
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

    printf("\n After:\n");
    for (i=0; i<HOUSE_AUCTION.pending.bids.size;i++)
        printf("\n %2.2f\n", HOUSE_AUCTION.pending.bids.array[i]);


    /************* At end of unit test, free the agent **************/
    unittest_free_ClearingHouse_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_quicksort_double_1()
 * \brief: Unit test for: quicksort_double().
 * Status: TEST
 */
void unittest_quicksort_double_1()
{
    int i;
    double_array bids;
    int res_bids;

    init_double_array(&bids);

    add_double(&bids, 2.0);
    add_double(&bids, 1.0);
    add_double(&bids, 4.0);
    add_double(&bids, 3.0);

    printf("\n Before\n");
    for (i=0; i<bids.size;i++)
        printf("\n %2.2f\n", bids.array[i]);


    //Testing if qsort function are working:
    //NOTE!!: This only sorts the prices, it does not carry along the IDs!!
    //res_bids = quicksort_double(&bids.array, bids.size);
    quicksort_double(bids.array, bids.size);

    if (res_bids==0)
        fprintf(stderr, "\nCorrect quicksort_double() sorting bids in ascending order");
    
    if (res_bids==1)
        fprintf(stderr, "\nFailure in quicksort_double() sorting bids in ascending order");

    printf("\n After:\n");
    for (i=0; i<bids.size;i++)
        printf("\n %2.2f\n", bids.array[i]);

}
