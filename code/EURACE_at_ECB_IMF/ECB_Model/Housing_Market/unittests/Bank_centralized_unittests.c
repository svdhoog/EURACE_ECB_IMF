#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Bank_agent_header.h"
#include "../../../my_library_header.h"
#include "../../../Statistical_Office/balance_sheet_operations.h"

/*
Functions in code file: centralized housing auction

Bank_sell_real_estate
Bank_receive_real_estate_transaction
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
 * \fn: void unittest_Bank_resell_seized_real_estate()
 * \brief: Unit test for: Bank_resell_seized_real_estate.
 * Status: PASS 5.1.17, Updated: 7.6.17 PASS
 */
void unittest_Bank_sell_real_estate()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();

	FLAME_environment_variable_print_debug_housing = 1;

    /***** Variables: Memory pre-conditions **************************/
	
 	foreclosure_adt foreclosed_house; //id, seller_id, buyer_id, value, price
	
	foreclosed_house.object_id = 10;
	foreclosed_house.former_owner_id  = 2;
	foreclosed_house.bank_id = 1;
	foreclosed_house.value = 10.0;
	foreclosed_house.claim = 0.0;
	foreclosed_house.seized_liquidity  = 0.0;
	foreclosed_house.housing_market_price_index_base = 100.0;
	foreclosed_house.seizure_day = 1;

	//Init to zeros
	init_mortgage_contract_adt(&foreclosed_house.mortgage);

	//add_foreclosure_adt(&VAR, object_id, former_owner_id, bank_id, value, claim, seized_liquidity, housing_market_price_index_base, seizure_day, &mortgage)
	add_foreclosure_adt(&SEIZED_COLLATERAL, foreclosed_house.object_id, foreclosed_house.former_owner_id, foreclosed_house.bank_id, foreclosed_house.value, foreclosed_house.claim, foreclosed_house.seized_liquidity,
												foreclosed_house.housing_market_price_index_base, foreclosed_house.seizure_day, &foreclosed_house.mortgage);
    
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_price_ask, sizeof(m_real_estate_price_ask));
	MB_Create(&b_real_estate_market_data, sizeof(m_real_estate_market_data));

	/***** Messages: pre-conditions **********************************/
	double rental_market_price_mean   = 1.0;
	double housing_market_price_mean  = 1.0;
	double housing_market_price_index = 1.0;
	
	add_real_estate_market_data_message(rental_market_price_mean, housing_market_price_mean, housing_market_price_index);

	MB_Iterator_Create(b_real_estate_market_data, &i_real_estate_market_data);

	START_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP
		printf("In msg: housing_market_price_index = %f", real_estate_market_data_message->housing_market_price_index);
		CU_ASSERT_DOUBLE_EQUAL(real_estate_market_data_message->housing_market_price_index, 1.0, 1e-3);
	FINISH_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP

    /***** Function evaluation ***************************************/
	Bank_sell_real_estate();
    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_price_ask, &i_real_estate_price_ask);

    START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->price, 10.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(real_estate_price_ask_message->ask.price, 10.0, 1e-3);
	FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP

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
 * \fn: void unittest_Bank_receive_real_estate_transaction()
 * \brief: Unit test for: Bank_receive_real_estate_transaction.
 * \brief: - input real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
 * \brief: Case 1: claim=price, no seized liquidity: profit =0
 * \brief: Case 2: claim<price, no seized liquidity: profit >0
 * \brief: Case 3: claim<price, WITH seized liquidity: profit >0, REIMBURSEMENT
 * \brief: Case 4: claim>price, no seized liquidity: loan_loss >0
 * \brief: Case 5: claim>price, WITH seized liquidity: loan_loss >0, NO REIMBURSEMENT
 * \brief: Status: TODO
 */
void unittest_Bank_receive_real_estate_transaction()
{
	int i;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();

	FLAME_environment_variable_print_debug_housing = 0;
	FLAME_environment_variable_print_housing_info = 1;
	FLAME_environment_variable_print_bank_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    ID = 1;

		CASH               =      0.00;    DEPOSITS               =  4711.23;
		RESERVES           =    105.40;    ECB_DEBT               =     0.00;
		FIRM_CREDIT       =     -0.00;    LLR_FIRM_LOANS         =     0.00;
		PL_MORTGAGE_LOANS  =      0.00;    LLR_PL_MORTGAGE_LOANS  =     0.00;
		NPL_MORTGAGE_LOANS =   6673.70;    LLR_NPL_MORTGAGE_LOANS =  3336.85;
  SEIZED_COLLATERAL_VALUE  =   1776.20;    EQUITY                 =   507.22; 



	//Local var
 	foreclosure_adt foreclosed_house; //id, seller_id, buyer_id, value, price

	//For this test:	
	foreclosed_house.object_id = 10;
	foreclosed_house.former_owner_id  = 2;
	foreclosed_house.bank_id = 1;
	foreclosed_house.value = 10.0;
	foreclosed_house.claim = 9.0; //9.0 //11.0
	foreclosed_house.seized_liquidity  = 1.0;
	foreclosed_house.housing_market_price_index_base = 100.0;
	foreclosed_house.seizure_day = 1;

	//add_foreclosure_adt(&VAR, object_id, former_owner_id, bank_id, value, claim, seized_liquidity, housing_market_price_index_base, seizure_day, &mortgage)
	add_foreclosure_adt(&SEIZED_COLLATERAL, foreclosed_house.object_id, foreclosed_house.former_owner_id, foreclosed_house.bank_id, foreclosed_house.value, foreclosed_house.claim, foreclosed_house.seized_liquidity,
												foreclosed_house.housing_market_price_index_base, foreclosed_house.seizure_day, &foreclosed_house.mortgage);

	//To init SEIZED_COLLATERAL =   1776.20
	foreclosed_house.object_id = 1;
	foreclosed_house.former_owner_id  = 2;
	foreclosed_house.bank_id = 1;
	foreclosed_house.value = 1766.20;
	foreclosed_house.claim = 0.0; //9.0 //11.0
	foreclosed_house.seized_liquidity  = 0.0;
	foreclosed_house.housing_market_price_index_base = 100.0;
	foreclosed_house.seizure_day = 1;


	//add_foreclosure_adt(&VAR, object_id, former_owner_id, bank_id, value, claim, seized_liquidity, housing_market_price_index_base, seizure_day, &mortgage)
	add_foreclosure_adt(&SEIZED_COLLATERAL, foreclosed_house.object_id, foreclosed_house.former_owner_id, foreclosed_house.bank_id, foreclosed_house.value, foreclosed_house.claim, foreclosed_house.seized_liquidity,
												foreclosed_house.housing_market_price_index_base, foreclosed_house.seizure_day, &foreclosed_house.mortgage);


	SEIZED_COLLATERAL_VALUE=0.0;
	MORTGAGE_CLAIM=0.0;

	for (i=0; i<SEIZED_COLLATERAL.size; i++)
	{
	     MORTGAGE_CLAIM += SEIZED_COLLATERAL.array[i].claim;
	 	SEIZED_COLLATERAL_VALUE += SEIZED_COLLATERAL.array[i].value;
	}

    CU_ASSERT_EQUAL(SEIZED_COLLATERAL.size, 2);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_real_estate_transaction, sizeof(m_real_estate_transaction));


	/***** Messages: pre-conditions **********************************/
 	house_adt transaction;
			
	transaction.object_id = 10;
	transaction.seller_id = 1;
	transaction.buyer_id  = 3;
	transaction.price = 10.0;

	add_real_estate_transaction_message(transaction.seller_id, transaction.buyer_id, transaction);
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_real_estate_transaction, &i_real_estate_transaction);

    /***** Function evaluation ***************************************/
	

	Bank_receive_real_estate_transaction();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(CASH, 0.0, 1e-3);

	CU_ASSERT_EQUAL(SEIZED_COLLATERAL.size, 1);

	//printf("\nclaim %f price %f seized liquidity %f profit %f loan_loss %f", house.claim, transaction.price, house.seized_liquidity, profit, loan_loss );

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();

    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
