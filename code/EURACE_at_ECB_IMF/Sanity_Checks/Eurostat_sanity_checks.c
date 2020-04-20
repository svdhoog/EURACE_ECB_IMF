#include "../header.h"
#include "../Eurostat_agent_header.h"
#include "../my_library_header.h"
#include "sanity_checks_aux_headers.h"

/* \fn: int Eurostat_sanity_checks()
 * \brief Sanity checks for Household data: 
 * \brief - household_send_data_message(ID, household_info);
 * \brief -	tenant_info_message, homeless_info_message, owner_info_message, landlord_info_message
 * \brief Sanity checks for Bank data: 
 * \brief -	bank_send_data_message->financial_data
 */
int Eurostat_sanity_checks()
{
	if (ENABLE_SANITY_CHECKS)
	{
	    //Sanity checks for Bank data: on day 2, the bank mortgage portfolio is collected.
	    if (BANK_NO_MORTGAGES != HH_NO_MORTGAGES)
	        fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: BANK_NO_MORTGAGES %d HH_NO_MORTGAGES %d not equal!", DAY, BANK_NO_MORTGAGES, HH_NO_MORTGAGES);

	    assert(BANK_NO_MORTGAGES == HH_NO_MORTGAGES);

/*
		// These checks fail because function Eurostat_compute_house_price_index only runs once 
		// Sanity checks for Household data:
		if (HH_IS_HOMELESS != COUNT_HOMELESS)
		    fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: HH_IS_HOMELESS %d COUNT_HOMELESS %d not equal!", DAY, HH_IS_HOMELESS, COUNT_HOMELESS);

		if (HH_IS_TENANT != COUNT_TENANTS)
		    fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: HH_IS_TENANT %d COUNT_TENANTS %d not equal!", DAY, HH_IS_TENANT, COUNT_TENANTS);    

		if (HH_IS_OWNER != COUNT_OWNERS)
		    fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: HH_IS_OWNER %d COUNT_OWNERS %d not equal!", DAY, HH_IS_OWNER, COUNT_OWNERS);

		if (HH_IS_LANDLORD != COUNT_LANDLORDS)
		    fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: HH_IS_LANDLORD %d COUNT_LANDLORDS %d not equal!", DAY, HH_IS_LANDLORD, COUNT_LANDLORDS);    

		if (HH_IS_HOMELESS + HH_IS_TENANT + HH_IS_OWNER != NUM_HOUSEHOLDS)
		    fprintf(stderr, "\nIT %d ERROR in Eurostat sanity checks: HH_IS_HOMELESS + HH_IS_TENANT + HH_IS_OWNER %d NUM_HOUSEHOLDS %d not equal!", DAY, HH_IS_HOMELESS + HH_IS_TENANT + HH_IS_OWNER, NUM_HOUSEHOLDS);
*/
	}

	return 0;
}
