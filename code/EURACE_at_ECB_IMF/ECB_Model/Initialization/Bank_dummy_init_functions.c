#include "../../header.h"
#include "../../Bank_agent_header.h"
#include "../../my_library_header.h"
#include "../Credit_Risk_Management/Bank_aux_headers.h"

/***************** TEMPLATE ****************/
/** \fn Bank_TEMPLATE()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
/*
int Bank_TEMPLATE()
{
  	//Message input:

	//Message output:

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MORTGAGE)
    {
        printf("\n\n Bank_ ID: %d", ID);
    }      
    #endif
 
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

/** \fn Bank_init_mortgages()
 * \brief Function to initialize the Banks mortgages.portfolio from data send by households.
 * \brief Content: Note: this function runs only once, when iteration is init_transient_period.
 * \brief - Message input: mortgage_info_message(household_id, bank_id, mortgage_contract_adt mortgage)
 * \brief - Message output: None
 * \brief currentState:	Bank_init_after_transient
 * \brief nextState:	Bank_update_policy_rate
 * \brief UNITTEST: PASS 3.11.16
 */
int Bank_init_mortgages()
{
	return 0;
}


