#include "../header.h"
#include "../ClearingHouse_agent_header.h"
#include "../my_library_header.h"

/************************************ Clearinghouse agent functions ************************************/

/* \fn: int ClearingHouse_initialization()
 * \brief Function to initialize. Note: this overwrites pop settings for the NEIGHBORING_REGION_IDS.
 */
int ClearingHouse_initialization()
{   
	//ADD HERE SOME FUNCTIONS THAT SHOULD BE RUN ONCE AT START OF SIMULATION
	
	// Set these in order to enable usage of constants in XML function conditions!
	INIT_TRANSIENT_PERIOD 		= CONST_INIT_TRANSIENT_PERIOD;
	
	return 0;
}

/** \fn Clearinghouse_send_data_to_eurostat()
 * \brief Function for ClearingHouse to send price means / indices for rental and housing markets
 * \brief Freq: Monthly, phase 0 (on last day of month)
 * \brief Message input: 
 * \brief Message output: real_estate_market
 * \brief 
 */
int Clearinghouse_send_data_to_Eurostat()
{
	add_clearinghouse_to_eurostat_data_message(RENTAL_MARKET_PRICE_MEAN, HOUSING_MARKET_PRICE_MEAN, HOUSING_MARKET_PRICE_INDEX, RENTAL_MARKET_PRICE_INDEX);

	return 0;
}
