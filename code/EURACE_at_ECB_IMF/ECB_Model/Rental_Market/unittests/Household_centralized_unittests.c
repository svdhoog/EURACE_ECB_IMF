#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"


/*
Unittests in this file (all in common with decentralized auction):

Household_read_rent_matching_info
Household_check_rental_state
Household_offer_rental_units
Household_send_rental_bid
Household_receive_rental_matching_as_landlord
Household_receive_rental_matching_as_tenant
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
	//MB_Create(&b_<message>, sizeof(m_<message>));

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();

    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Function evaluation ***************************************/
//	Household_function();
    
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
//	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}



/*
 * \fn: void unittest_Household_read_rent_matching_info_1()
 * \brief: Unit test for: Household_read_rent_matching_info.
 * Status: PASS 12.1.17
 */
void unittest_Household_read_rent_matching_info_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=3;

	IS_OWNER=1;
	//Set by function:
	//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id = hmr_matching_info_message->object_id;

	IS_TENANT=0;
	//Set by function:
	//RENTAL_UNIT.monthly_rent = rent_matching_info_message->rental_unit.monthly_rent;

	IS_LANDLORD=0;
	//Set by function:
	//add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));

	/***** Messages: pre-conditions **********************************/
   	
	//Tenant: 1 (Landlord: 5)
   	//add_rent_matching_info_message(INIT_LANDLORDS.array[i], INIT_TENANTS.array[n].id, INIT_TENANTS.array[n].monthly_rent);
  	rental_adt rental_object;
			
	rental_object.object_id = 100;
	rental_object.tenant_id  = 1;
	rental_object.landlord_id = 5;
	rental_object.monthly_rent = 10.0;

	add_rent_matching_info_message(1,5,rental_object);	

	//Owner: 3
	//add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
   	add_hmr_matching_info_message(3, 10);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);

    /***** Function evaluation ***************************************/
	Household_read_rent_matching_info();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, 10);

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
 * \fn: void unittest_Household_read_rent_matching_info_2()
 * \brief: Unit test for: Household_read_rent_matching_info.
 * Status: PASS 12.1.17
 */
void unittest_Household_read_rent_matching_info_2()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=2;

	IS_OWNER=0;
	//Set by function:
	//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id = hmr_matching_info_message->object_id;

	IS_TENANT=1;
	//Set by function:
	//RENTAL_UNIT.monthly_rent = rent_matching_info_message->rental_unit.monthly_rent;

	IS_LANDLORD=0;
	//Set by function:
	//add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));
	MB_Create(&b_house_price_index, sizeof(m_house_price_index));

	/***** Messages: pre-conditions **********************************/
   	
	//Tenants: 1,2 (Landlords: 5,6)
	//add_rent_matching_info_message(INIT_TENANTS.array[i].id, INIT_LANDLORDS.array[j], rental_object);
   	
	rental_adt rental_object;
			
	rental_object.object_id = 100;
	rental_object.tenant_id  = 1;
	rental_object.landlord_id = 5;
	rental_object.monthly_rent = 10.0;

	add_rent_matching_info_message(1,5, rental_object);
	add_rent_matching_info_message(2,6, rental_object);

	//Owners: 3,4
	//add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
   	add_hmr_matching_info_message(3, 10);
	add_hmr_matching_info_message(4, 20);

	//PRICE INDEX
	price_index_adt HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX;

    HMR_PRICE_INDEX.mean = 1.0;
    HMR_PRICE_INDEX.sd = 0.1;

    OP_PRICE_INDEX.mean = 2.0;
    OP_PRICE_INDEX.sd = 0.2;

    RENT_PRICE_INDEX.mean = 3.0;
    RENT_PRICE_INDEX.sd = 0.2;

	add_house_price_index_message(HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);
	MB_Iterator_Create(b_house_price_index, &i_house_price_index);

    /***** Function evaluation ***************************************/
	Household_read_rent_matching_info();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(RENTAL_UNIT.monthly_rent, 10.0, 1e-3);

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
 * \fn: void unittest_Household_read_rent_matching_info3()
 * \brief: Unit test for: Household_read_rent_matching_info.
 * Status: PASS 12.1.17
 */
void unittest_Household_read_rent_matching_info_3()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

    FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=1;

	IS_OWNER=0;
	//Set by function:
	//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id = hmr_matching_info_message->object_id;

	IS_TENANT=0;
	//Set by function:
	//RENTAL_UNIT.monthly_rent = rent_matching_info_message->rental_unit.monthly_rent;

	IS_LANDLORD=1;
	//Set by function:
	//add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rent_matching_info, sizeof(m_rent_matching_info));
	MB_Create(&b_hmr_matching_info, sizeof(m_hmr_matching_info));
	MB_Create(&b_house_price_index, sizeof(m_house_price_index));

	/***** Messages: pre-conditions **********************************/
   	
	//Tenant: 1 (Landlord: 5)
	//add_rent_matching_info_message(INIT_TENANTS.array[i].id, INIT_LANDLORDS.array[j], rental_object);
   	
	rental_adt rental_object;
			
	rental_object.object_id = 100;
	rental_object.tenant_id  = 1;
	rental_object.landlord_id = 5;
	rental_object.monthly_rent = 10.0;
	rental_object.value = 1.0;

	add_rent_matching_info_message(1,5, rental_object);

	//Owner: 3
	//add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
   	add_hmr_matching_info_message(3, 10);

	//PRICE INDEX
	price_index_adt HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX;

    HMR_PRICE_INDEX.mean = 1.0;
    HMR_PRICE_INDEX.sd = 0.1;

    OP_PRICE_INDEX.mean = 2.0;
    OP_PRICE_INDEX.sd = 0.2;

    RENT_PRICE_INDEX.mean = 3.0;
    RENT_PRICE_INDEX.sd = 0.2;

	add_house_price_index_message(HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX);


    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_matching_info, &i_rent_matching_info);
	MB_Iterator_Create(b_hmr_matching_info, &i_hmr_matching_info);
	MB_Iterator_Create(b_house_price_index, &i_house_price_index);

    /***** Function evaluation ***************************************/
	Household_read_rent_matching_info();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, 1);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].object_id, 100);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].tenant_id, 1);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].landlord_id, 5);
	CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].monthly_rent, 10.0, 1e-3);

	//House priceindex related
	int i;
	int n = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;
	printf("\nRental size: %d\n", n);

	for (i=0; i<n; i++)
		printf("\nRental unit value[%d]=%f\n", i, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value);

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
 * \fn: void unittest_Household_check_rental_state_1()
 * \brief: Unit test for: Household_check_rental_state.
 * Status: PASS 12.1.17
 */
void unittest_Household_check_rental_state_1()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=1;
	
	RENTAL_UNIT.object_id=1;
	RENTAL_UNIT.landlord_id=0;
	RENTAL_UNIT.tenant_id=ID;
	RENTAL_UNIT.monthly_rent = 1.0;

	//Case 1: After: RENTAL_UNIT.size==0
	IS_OWNER = 1;
	IS_TENANT = 0;
	JUST_PURCHASED_HMR = 1;
	PLAN_STOP_RENT_HMR = 1;

	JUST_SOLD_OP  = 0;
	PLAN_STOP_LET_OP = 0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_cancel_rental_contract, sizeof(m_cancel_rental_contract));
	MB_Create(&b_rent_payment, sizeof(m_rent_payment));
	MB_Create(&b_seized_liquidity_recompensation, sizeof(m_seized_liquidity_recompensation));

	/***** Messages: pre-conditions **********************************/
	//Output: add_cancel_rental_contract_message(ID, landlord_id, object_id);
	//Output: add_rent_payment_message(ID, landlord_id, monthly_rent);
	
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_seized_liquidity_recompensation, &i_seized_liquidity_recompensation);

    /***** Function evaluation ***************************************/
	Household_check_rental_state();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_cancel_rental_contract, &i_cancel_rental_contract);

    /***** Messages: Message post-conditions *****/
    START_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(cancel_rental_contract_message->tenant_id, 1);
	FINISH_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_check_rental_state_2()
 * \brief: Unit test for: Household_check_rental_state.
 * Status: PASS 12.1.17
 */
void unittest_Household_check_rental_state_2()
{
	int object_id, landlord_id, tenant_id;
	int sold;
	double monthly_rent, value;

    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=1;

	//Fill property [0]:
	object_id = 1;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 10.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list: SHOULD BE UPDATED WITH ADDITION ATTRIBUTES!! (31.10.17)
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [1]:
	object_id = 2;
	tenant_id = 2;
	landlord_id = ID;
	monthly_rent = 20.0;
	sold = 1;
	value = 0;

	//Add new rental_property to list: SHOULD BE UPDATED WITH ADDITION ATTRIBUTES!! (31.10.17)
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Case 2:
	IS_OWNER = 1;
	IS_TENANT = 0;
	JUST_SOLD_OP  = 1;
	PLAN_STOP_LET_OP = 1;

	JUST_PURCHASED_HMR = 0;
	PLAN_STOP_RENT_HMR = 0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_cancel_rental_contract, sizeof(m_cancel_rental_contract));
	MB_Create(&b_rent_payment, sizeof(m_rent_payment));
	MB_Create(&b_seized_liquidity_recompensation, sizeof(m_seized_liquidity_recompensation));

	/***** Messages: pre-conditions **********************************/
	//Output: add_cancel_rental_contract_message(ID, landlord_id, object_id);
	//Output: add_rent_payment_message(ID, landlord_id, monthly_rent);
	
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_seized_liquidity_recompensation, &i_seized_liquidity_recompensation);

    /***** Function evaluation ***************************************/
	Household_check_rental_state();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_cancel_rental_contract, &i_cancel_rental_contract);

    /***** Messages: Message post-conditions *****/
	//add_cancel_rental_contract_message(tenant_id, ID, object_id);
    START_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP
		CU_ASSERT_EQUAL(cancel_rental_contract_message->landlord_id, 1);
    	CU_ASSERT_EQUAL(cancel_rental_contract_message->tenant_id, 2);
		CU_ASSERT_EQUAL(cancel_rental_contract_message->object_id, 2);
	FINISH_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP

	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, 1);

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_check_rental_state_3()
 * \brief: Unit test for: Household_check_rental_state.
 * Status: PASS 12.1.17
 */
void unittest_Household_check_rental_state_3()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=1;
	
	RENTAL_UNIT.object_id=1;
	RENTAL_UNIT.landlord_id=2;
	RENTAL_UNIT.tenant_id=ID;
	RENTAL_UNIT.monthly_rent = 1.0;

	//Case 3:
	IS_OWNER = 0;
	IS_TENANT = 1;
	PAYMENT_ACCOUNT = 10.0; //After: 9.0

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_cancel_rental_contract, sizeof(m_cancel_rental_contract));
	MB_Create(&b_rent_payment, sizeof(m_rent_payment));
	MB_Create(&b_seized_liquidity_recompensation, sizeof(m_seized_liquidity_recompensation));

	/***** Messages: pre-conditions **********************************/
	//Output: add_cancel_rental_contract_message(ID, landlord_id, object_id);
	//Output: add_rent_payment_message(ID, landlord_id, monthly_rent);
	
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_seized_liquidity_recompensation, &i_seized_liquidity_recompensation);

    /***** Function evaluation ***************************************/
	Household_check_rental_state();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 9.0, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rent_payment, &i_rent_payment);

    /***** Messages: Message post-conditions *****/
	//add_rent_payment_message(ID, landlord_id, monthly_rent);
    START_RENT_PAYMENT_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(rent_payment_message->tenant_id, 1);
	     CU_ASSERT_EQUAL(rent_payment_message->landlord_id, 2);
	     CU_ASSERT_DOUBLE_EQUAL(rent_payment_message->monthly_rent, 1.0, 1e-3);
	FINISH_RENT_PAYMENT_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_check_rental_state_4()
 * \brief: Unit test for: Household_check_rental_state.
 * Status: PASS 12.1.17
 */
void unittest_Household_check_rental_state_4()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	FLAME_environment_variable_print_debug_rental = 1;

    /***** Variables: Memory pre-conditions **************************/
	ID=1;
	
	//Case 4: All agents
	PAYMENT_ACCOUNT = 10.0; //After: 20.0	

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_cancel_rental_contract, sizeof(m_cancel_rental_contract));
	MB_Create(&b_rent_payment, sizeof(m_rent_payment));
	MB_Create(&b_seized_liquidity_recompensation, sizeof(m_seized_liquidity_recompensation));

	/***** Messages: pre-conditions **********************************/
	//Output: add_cancel_rental_contract_message(ID, landlord_id, object_id);
	//Output: add_rent_payment_message(ID, landlord_id, monthly_rent);
	
	//Input: add_seized_liquidity_recompensation_message(id, amount);
	add_seized_liquidity_recompensation_message(1, 10.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_seized_liquidity_recompensation, &i_seized_liquidity_recompensation);

    /***** Function evaluation ***************************************/
	Household_check_rental_state();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 20.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_offer_rental_units_1()
 * \brief: Unit test for: Household_offer_rental_units.
 * Case: PLAN_LET_OP=1;
 * Status: PASS 12.1.17
 */
void unittest_Household_offer_rental_units_1()
{
	int object_id, landlord_id, tenant_id;
	int sold;
	double monthly_rent, value;

    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
	FLAME_environment_variable_print_debug_rental = 1;
    
    /***** Variables: Memory pre-conditions **************************/
	ID=1;

	PLAN_LET_OP=1;

	//Fill property [0]:
	object_id = 1;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 10.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [1]:
	object_id = 2;
	tenant_id = 1;
	landlord_id = ID;
	monthly_rent = 20.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rental_price_ask, sizeof(m_rental_price_ask));

	/***** Messages: pre-conditions **********************************/
    //add_rental_price_ask_message();
   	//add_rental_price_ask_message(double value, ask_adt house);


    /***** Function evaluation ***************************************/
	Household_offer_rental_units();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rental_price_ask, &i_rental_price_ask);

    /***** Messages: Message post-conditions *****/

	int count=0;

    START_RENTAL_PRICE_ASK_MESSAGE_LOOP
	     CU_ASSERT_DOUBLE_EQUAL(rental_price_ask_message->price, 10.0, 1e-3);
	     CU_ASSERT_DOUBLE_EQUAL(rental_price_ask_message->ask.price, 10.0, 1e-3);
   		count++;
	FINISH_RENTAL_PRICE_ASK_MESSAGE_LOOP

	printf("\n Mesgs: %d\n", count);

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_offer_rental_units_2()
 * \brief: Unit test for: Household_offer_rental_units.
 * Case 1: PLAN_LET_OP=1;
 * Status: PASS 12.1.17
 */
void unittest_Household_offer_rental_units_2()
{
	int object_id, landlord_id, tenant_id;
	int sold;
	double monthly_rent, value;

    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
	FLAME_environment_variable_print_debug_rental = 1;
    
    /***** Variables: Memory pre-conditions **************************/
	ID=1;

	PLAN_LET_OP=1;

	//Fill property [0]:
	object_id = 1;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 10.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [1]:
	object_id = 2;
	tenant_id = 1;
	landlord_id = ID;
	monthly_rent = 20.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [2]:
	object_id = 3;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 30.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rental_price_ask, sizeof(m_rental_price_ask));

	/***** Messages: pre-conditions **********************************/
    //add_rental_price_ask_message();
   	//add_rental_price_ask_message(double value, ask_adt house);


    /***** Function evaluation ***************************************/
	Household_offer_rental_units();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rental_price_ask, &i_rental_price_ask);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

	int count=0;
    START_RENTAL_PRICE_ASK_MESSAGE_LOOP
		count++;
	FINISH_RENTAL_PRICE_ASK_MESSAGE_LOOP
	
	printf("\n Mesgs: %d\n", count);

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_send_rental_bid()
 * \brief: Unit test for: Household_send_rental_bid.
 * Status: PASS 12.1.17
 */
void unittest_Household_send_rental_bid()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	RENT_BID_PRICE = 1.0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rental_price_bid, sizeof(m_rental_price_bid));

	/***** Messages: pre-conditions **********************************/
    //add_rental_price_bid_message();
    	    
    /***** Function evaluation ***************************************/
	Household_send_rental_bid();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rental_price_bid, &i_rental_price_bid);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_RENTAL_PRICE_BID_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(rental_price_bid_message->price, 1.0);
	     CU_ASSERT_DOUBLE_EQUAL(rental_price_bid_message->bid.price, 1.0, 1e-3);
	FINISH_RENTAL_PRICE_BID_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Household_receive_rental_matching_as_landlord()
 * \brief: Unit test for: Household_receive_rental_matching_as_landlord.
 * Case: PLAN_LET_OP=1
 * Status: PASS 12.1.17
 */
void unittest_Household_receive_rental_matching_as_landlord()
{
	int object_id, landlord_id, tenant_id;
	int sold;
	double monthly_rent, value;

    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_rental = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=1;
    PLAN_RENT_HMR =0;
	PLAN_LET_OP=1;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = 10.0;
	
	//Fill property [0]:
	object_id = 1;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 10.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [1]:
	object_id = 2;
	tenant_id = -1;
	landlord_id = ID;
	monthly_rent = 20.0;
	sold = 0;
	value = 0;

	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	//Fill property [2]:
	object_id = 3;
	tenant_id = 2;
	landlord_id = ID;
	monthly_rent = 30.0;
	sold = 0;
	value = 0;
	
	//Add new rental_property to list:
	add_rental_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, object_id, tenant_id, landlord_id, monthly_rent, sold, value);

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rental_matching, sizeof(m_rental_matching));

	/***** Messages: pre-conditions **********************************/
 	rental_adt matching1, matching2;
			
	matching1.object_id = 1;
	matching1.landlord_id = 1;
	matching1.tenant_id  = 3;
	matching1.monthly_rent=10.0;
	matching1.sold=0;

	matching2.object_id = 2;
	matching2.landlord_id = 1;
	matching2.tenant_id  = 4;
	matching2.monthly_rent=20.0;
	matching2.sold=0;

	add_rental_matching_message(matching1.tenant_id, matching1.landlord_id, matching1);
	add_rental_matching_message(matching2.tenant_id, matching2.landlord_id, matching2);

    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Function evaluation ***************************************/
	Household_receive_rental_matching_as_landlord();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].object_id, 1);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[0].tenant_id, 3);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].object_id, 2);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[1].tenant_id, 4);
	
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[2].object_id, 3);
	CU_ASSERT_EQUAL(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[2].tenant_id, 2);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Household_receive_rental_matching_as_tenant()
 * \brief: Unit test for: Household_receive_rental_matching_as_tenant.
 * Case 1: PLAN_RENT_HMR =1 
 * Status: PASS 12.1.17
 */
void unittest_Household_receive_rental_matching_as_tenant()
{
    /************* At start of unit test, add one agent **************/
	unittest_init_Household_agent();

	FLAME_environment_variable_print_debug_rental = 1;
	
    /***** Variables: Memory pre-conditions **************************/
    ID=2;
	PLAN_RENT_HMR =1;
	PLAN_LET_OP=0;

	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_rental_matching, sizeof(m_rental_matching));

	/***** Messages: pre-conditions **********************************/
 	rental_adt matching1, matching2;
			
	matching1.object_id = 1;
	matching1.landlord_id = 1;
	matching1.tenant_id  = 2;
	matching1.monthly_rent=10.0;
	matching1.sold=0;

	matching2.object_id = 2;
	matching2.landlord_id = 1;
	matching2.tenant_id  = 4;
	matching2.monthly_rent=20.0;
	matching2.sold=0;

	add_rental_matching_message(matching1.tenant_id, matching1.landlord_id, matching1);
	add_rental_matching_message(matching2.tenant_id, matching2.landlord_id, matching2);
	
    	    
    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_rental_matching, &i_rental_matching);

    /***** Function evaluation ***************************************/
	Household_receive_rental_matching_as_tenant();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(RENTAL_UNIT.object_id, 1);
	CU_ASSERT_EQUAL(RENTAL_UNIT.landlord_id, 1);
	CU_ASSERT_DOUBLE_EQUAL(RENTAL_UNIT.monthly_rent, 10.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
