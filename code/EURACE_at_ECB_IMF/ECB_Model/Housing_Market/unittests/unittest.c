/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../../header.h"
#include <CUnit/Basic.h>
#include <gsl/gsl_rng.h>

//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************/

/* Library function */
void unittest_quicksort_double_1();

/* Household functions */
void unittest_Household_check_housing_state_1();
void unittest_Household_check_housing_state_2();

void unittest_Household_sell_real_estate_1();
void unittest_Household_sell_real_estate_2();

//(De)centralized
void unittest_Household_buy_real_estate();

void unittest_Household_receive_real_estate_transaction_as_seller_1();
void unittest_Household_receive_real_estate_transaction_as_seller_2();
void unittest_Household_receive_real_estate_transaction_as_buyer_1();
void unittest_Household_receive_real_estate_transaction_as_buyer_2();

//Decentralized only
void unittest_Household_receive_real_estate_availability();
void unittest_Household_receive_real_estate_offers();
void unittest_Household_send_real_estate_acceptance();

/* Bank functions */
void unittest_Bank_sell_real_estate();
void unittest_Bank_receive_real_estate_transaction();

/* House functions */
//Decentralized only
void unittest_House_send_real_estate_availability();
void unittest_House_receive_real_estate_bids();
void unittest_House_send_real_estate_offers();
void unittest_House_receive_real_estate_acceptances();
void unittest_House_send_real_estate_transaction();

/* ClearingHouse functions */
//Centralized only
void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_0();
void unittest_ClearingHouse_receive_real_estate_bids_0();

void unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_1();
void unittest_ClearingHouse_receive_real_estate_bids_1();

void unittest_ClearingHouse_receive_real_estate_bids_2();

void unittest_ClearingHouse_run_real_estate_auction_0();
void unittest_ClearingHouse_run_real_estate_auction_1();
void unittest_ClearingHouse_run_real_estate_auction_2();
void unittest_ClearingHouse_run_real_estate_auction_3();
void unittest_ClearingHouse_run_real_estate_auction_4();
void unittest_ClearingHouse_run_real_estate_auction_5();
void unittest_ClearingHouse_run_real_estate_auction_6();
/*************************** end prototypes ***************************/

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{
	/******* GSL INIT */
	const gsl_rng_type * T;

	gsl_rng_env_setup();

	//returns a pointer to a newly-created instance of gsl_rng
	T = gsl_rng_default;

	FLAME_GSL_RNG = gsl_rng_alloc (T);

	//Init on time
	unsigned long int gsl_seed = (unsigned long int)time(NULL); 
		
	gsl_rng_set(FLAME_GSL_RNG, gsl_seed);
	/******* END GSL INIT */

    CU_pSuite pSuite = NULL;
    
    /* Init FLAME */
    initialise_unit_testing();
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_Unittests_Real_estate_Market", init_suite1, clean_suite1);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* add extra tests using || */
    
    if(
        //CU_add_test(pSuite, "quicksort_double_1", unittest_quicksort_double_1) == NULL) // ||        
        
        //(De)centralized (all in common with decentralized auction):
        //CU_add_test(pSuite, "Household_check_housing_state_1", unittest_Household_check_housing_state_1) == NULL) // ||
        //CU_add_test(pSuite, "Household_check_housing_state_2", unittest_Household_check_housing_state_2) == NULL) // ||

/*
        CU_add_test(pSuite, "Household_sell_real_estate_1", unittest_Household_sell_real_estate_1) == NULL) // ||
        CU_add_test(pSuite, "Household_sell_real_estate_2", unittest_Household_sell_real_estate_2) == NULL) // ||

        CU_add_test(pSuite, "Household_buy_real_estate", unittest_Household_buy_real_estate) == NULL) // ||

        CU_add_test(pSuite, "Household_receive_real_estate_transaction_as_seller_1", unittest_Household_receive_real_estate_transaction_as_seller_1) == NULL) // ||
        CU_add_test(pSuite, "Household_receive_real_estate_transaction_as_seller_2", unittest_Household_receive_real_estate_transaction_as_seller_2) == NULL) // ||
        CU_add_test(pSuite, "Household_receive_real_estate_transaction_as_buyer_1", unittest_Household_receive_real_estate_transaction_as_buyer_1) == NULL) // ||
        CU_add_test(pSuite, "Household_receive_real_estate_transaction_as_buyer_2", unittest_Household_receive_real_estate_transaction_as_buyer_2) == NULL //||
        */
        //Odd values: bid.price=0.0 ask.price=1.0/0.0=-nan
        //CU_add_test(pSuite, "ClearingHouse_recycle_unprocessed_real_estate_bids 0", unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_0) == NULL) // ||
        //CU_add_test(pSuite, "ClearingHouse_receive_real_estate_bids 0", unittest_ClearingHouse_receive_real_estate_bids_0) == NULL //||

        //Normal values: 
        //CU_add_test(pSuite, "ClearingHouse_recycle_unprocessed_real_estate_bids 1", unittest_ClearingHouse_recycle_unprocessed_real_estate_bids_1) == NULL) // ||
        //CU_add_test(pSuite, "ClearingHouse_receive_real_estate_bids 1", unittest_ClearingHouse_receive_real_estate_bids_1) == NULL) // ||

        //Test: order of bids and asks
        //CU_add_test(pSuite, "ClearingHouse_receive_real_estate_bids 2", unittest_ClearingHouse_receive_real_estate_bids_2) == NULL) // ||        

        /*
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 0", unittest_ClearingHouse_run_real_estate_auction_0) == NULL) // ||
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 1", unittest_ClearingHouse_run_real_estate_auction_1) == NULL) // ||
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 2", unittest_ClearingHouse_run_real_estate_auction_2) == NULL) // ||
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 3", unittest_ClearingHouse_run_real_estate_auction_3) == NULL) // ||
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 4", unittest_ClearingHouse_run_real_estate_auction_4) == NULL) // ||
        CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 5", unittest_ClearingHouse_run_real_estate_auction_5) == NULL //||
        */
        //Test: order of bids and asks
        //CU_add_test(pSuite, "ClearingHouse_run_real_estate_auction 6", unittest_ClearingHouse_run_real_estate_auction_6) == NULL //||

        //CU_add_test(pSuite, "Bank_sell_real_estate", unittest_Bank_sell_real_estate) == NULL) //||
        CU_add_test(pSuite, "Bank_receive_real_estate_transaction", unittest_Bank_receive_real_estate_transaction) == NULL )//||
        
        /* 
         //Decentralized only
         CU_add_test(pSuite, "Household_receive_real_estate_availability", unittest_Household_receive_real_estate_availability) == NULL) // ||
         CU_add_test(pSuite, "Household_receive_real_estate_offers", unittest_Household_receive_real_estate_offers) == NULL) // ||
         CU_add_test(pSuite, "Household_send_real_estate_acceptance", unittest_Household_send_real_estate_acceptance) == NULL) // ||
        */
        /*
         CU_add_test(pSuite, "House_send_real_estate_availability", unittest_House_send_real_estate_availability) == NULL) // ||
         CU_add_test(pSuite, "House_receive_real_estate_bids", unittest_House_receive_real_estate_bids) == NULL) // ||
         CU_add_test(pSuite, "House_send_real_estate_offers", unittest_House_send_real_estate_offers) == NULL) // ||
         CU_add_test(pSuite, "House_receive_real_estate_acceptances", unittest_House_receive_real_estate_acceptances) == NULL) // ||
         CU_add_test(pSuite, "House_send_real_estate_transaction", unittest_House_send_real_estate_transaction) == NULL) // ||
        */
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    /* Free FLAME */
    clean_up(0);
    
    return CU_get_error();
}
