#include "../header.h"
#include "../CentralBank_agent_header.h"
#include "../my_library_header.h"


/** \fn Central_Bank_initialization()
 * \brief 
 */
int Central_Bank_initialization()
{
	// Set these in order to enable usage of constants in XML function conditions!
	INIT_TRANSIENT_PERIOD 		= CONST_INIT_TRANSIENT_PERIOD;
	SWITCH_MACROPRU_POLICY 		= CONST_SWITCH_MACROPRU_POLICY;

	// Set these to propagate from global constants to agent-specific variables
	REGULATORY_CAPITAL_BUFFER 	= CONST_REGULATORY_CAPITAL_BUFFER;
	REGULATORY_LTV_CAP 			= CONST_REGULATORY_LTV_CAP;
	REGULATORY_DSTI_CAP 		= CONST_REGULATORY_DSTI_CAP;

	//Initializations
	TRANSITION_PHASE_POLICY = CONST_TRANSITION_PHASE_POLICY;
	SWITCH_MACROPRU_POLICY = CONST_SWITCH_MACROPRU_POLICY;

    #ifdef _DEBUG_MODE    
    if (PRINT_MACROPRU_INFO)
    {
        printf("\n\tSWITCH_MACROPRU_POLICY: %d", SWITCH_MACROPRU_POLICY);
		printf("\n\tCONST_SWITCH_MACROPRU_POLICY: %d", CONST_SWITCH_MACROPRU_POLICY);
        printf("\n\tTRANSITION_PHASE_POLICY: %d", TRANSITION_PHASE_POLICY);
		printf("\n\tCONST_TRANSITION_PHASE_POLICY: %d", CONST_TRANSITION_PHASE_POLICY);
		printf("\n");
    }
    #endif

	return 0;
}
