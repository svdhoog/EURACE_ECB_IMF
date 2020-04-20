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

/* Household functions */
void unittest_Household_read_rent_matching_info_1();
void unittest_Household_read_rent_matching_info_2();
void unittest_Household_read_rent_matching_info_3();

void unittest_Household_check_rental_state_1();
void unittest_Household_check_rental_state_2();
void unittest_Household_check_rental_state_3();
void unittest_Household_check_rental_state_4();

void unittest_Household_offer_rental_units_1();
void unittest_Household_offer_rental_units_2();

//(De)centralized
void unittest_Household_send_rental_bid();
void unittest_Household_receive_rental_matching_as_landlord();
void unittest_Household_receive_rental_matching_as_tenant();

//Decentralized only
/*
void unittest_Household_receive_rental_availability();
void unittest_Household_receive_rental_offers();
void unittest_Household_send_rental_acceptance();
*/

/* House functions */
//Decentralized only
/*
void unittest_House_send_rental_availability();
void unittest_House_receive_rental_bids();
void unittest_House_send_rental_offers();
void unittest_House_receive_rental_acceptances();
void unittest_House_send_rental_matching();
*/

/* ClearingHouse functions */
//Centralized only
void unittest_ClearingHouse_init_rent_matching_1();
void unittest_ClearingHouse_init_rent_matching_2();
void unittest_ClearingHouse_init_rent_matching_3();

void unittest_ClearingHouse_recycle_unprocessed_rental_bids();
void unittest_ClearingHouse_receive_rental_bids();

void unittest_ClearingHouse_run_rental_auction_0();
void unittest_ClearingHouse_run_rental_auction_1();
void unittest_ClearingHouse_run_rental_auction_2();
void unittest_ClearingHouse_run_rental_auction_3();
void unittest_ClearingHouse_run_rental_auction_4();
void unittest_ClearingHouse_run_rental_auction_5();
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
    pSuite = CU_add_suite("Suite_Unittests_Housing_Market", init_suite1, clean_suite1);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* add extra tests using || */
    
    if(
 
    //(De)centralized (all in common with decentralized auction):
	//CU_add_test(pSuite, "Household_read_rent_matching_info_1", unittest_Household_read_rent_matching_info_1) == NULL ||
	//CU_add_test(pSuite, "Household_read_rent_matching_info_2", unittest_Household_read_rent_matching_info_2) == NULL //||
	//CU_add_test(pSuite, "Household_read_rent_matching_info_3", unittest_Household_read_rent_matching_info_3) == NULL //||
/*
	CU_add_test(pSuite, "Household_check_rental_state_1", unittest_Household_check_rental_state_1) == NULL ||
	CU_add_test(pSuite, "Household_check_rental_state_2", unittest_Household_check_rental_state_2) == NULL ||
	CU_add_test(pSuite, "Household_check_rental_state_3", unittest_Household_check_rental_state_3) == NULL ||
	CU_add_test(pSuite, "Household_check_rental_state_4", unittest_Household_check_rental_state_4) == NULL ||

	CU_add_test(pSuite, "Household_offer_rental_units_1", unittest_Household_offer_rental_units_1) == NULL ||
	CU_add_test(pSuite, "Household_offer_rental_units_2", unittest_Household_offer_rental_units_2) == NULL ||

	CU_add_test(pSuite, "Household_send_rental_bid", unittest_Household_send_rental_bid) == NULL ||

	CU_add_test(pSuite, "Household_receive_rental_matching_as_landlord", unittest_Household_receive_rental_matching_as_landlord) == NULL ||
	CU_add_test(pSuite, "Household_receive_rental_matching_as_tenant", unittest_Household_receive_rental_matching_as_tenant) == NULL //||
*/
/*
	CU_add_test(pSuite, "ClearingHouse_init_rent_matching_1", unittest_ClearingHouse_init_rent_matching_1) == NULL ||
	CU_add_test(pSuite, "ClearingHouse_init_rent_matching_2", unittest_ClearingHouse_init_rent_matching_2) == NULL ||
*/
    CU_add_test(pSuite, "ClearingHouse_init_rent_matching_3", unittest_ClearingHouse_init_rent_matching_3) == NULL //||
/*
	CU_add_test(pSuite, "ClearingHouse_recycle_unprocessed_rental_bids", unittest_ClearingHouse_recycle_unprocessed_rental_bids) == NULL ||
	CU_add_test(pSuite, "ClearingHouse_receive_rental_bids", unittest_ClearingHouse_receive_rental_bids) == NULL ||

    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 0", unittest_ClearingHouse_run_rental_auction_0) == NULL ||
    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 1", unittest_ClearingHouse_run_rental_auction_1) == NULL ||
    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 2", unittest_ClearingHouse_run_rental_auction_2) == NULL ||
    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 3", unittest_ClearingHouse_run_rental_auction_3) == NULL ||
    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 4", unittest_ClearingHouse_run_rental_auction_4) == NULL ||
    CU_add_test(pSuite, "ClearingHouse_run_rental_auction 5", unittest_ClearingHouse_run_rental_auction_5) == NULL ) //||
*/
/* 
 //Decentralized only
 CU_add_test(pSuite, "Household_receive_rental_availability", unittest_Household_receive_rental_availability) == NULL ||
 CU_add_test(pSuite, "Household_receive_rental_offers", unittest_Household_receive_rental_offers) == NULL ||
 CU_add_test(pSuite, "Household_send_rental_acceptance", unittest_Household_send_rental_acceptance) == NULL ||
*/
/*
 CU_add_test(pSuite, "House_send_rental_availability", unittest_House_send_rental_availability) == NULL ||
 CU_add_test(pSuite, "House_receive_rental_bids", unittest_House_receive_rental_bids) == NULL ||
 CU_add_test(pSuite, "House_send_rental_offers", unittest_House_send_rental_offers) == NULL ||
 CU_add_test(pSuite, "House_receive_rental_acceptances", unittest_House_receive_rental_acceptances) == NULL ||
 CU_add_test(pSuite, "House_send_rental_matching", unittest_House_send_rental_matching) == NULL ||
*/
    )
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
