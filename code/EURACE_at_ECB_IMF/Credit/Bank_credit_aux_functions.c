#include "../header.h"
#include "../my_library_header.h"
#include "../Bank_agent_header.h"
#include "Bank_credit_aux_headers.h"

int Bank_check_equity_requirement(void)
{
	REGULATORY_EQUITY = REA*REGULATORY_CAPITAL_BUFFER;
	
	BEHAVIORAL_EQUITY = REA*(REGULATORY_CAPITAL_BUFFER + CONST_BEHAVIORAL_CAPITAL_BUFFER);

	//Stay or become active and grant loans always if actual_capital_ratio >= behavioral_capital_req (plus some percentage above it)
	if (EQUITY > BEHAVIORAL_EQUITY - EPSILON) 
	{
		ACTIVE=1;
		
		GRANT_LOANS_DEFINITELY=1;

		GRANT_LOANS_MAYBE=0;

		return 0;
	}

	//Stay or become active and grant loans with some probability 	
	if (REGULATORY_EQUITY < EQUITY && EQUITY < BEHAVIORAL_EQUITY)
	{
		ACTIVE=1;

		GRANT_LOANS_DEFINITELY=0;

		GRANT_LOANS_MAYBE=1;

		return 0;
	}

	//Stay or become inactive and grant no loans if actual_capital_ratio <= regulatory_capital_req (plus some percentage below it)	
	if (EQUITY < REGULATORY_EQUITY + EPSILON) 
	{
		ACTIVE=0;

		GRANT_LOANS_DEFINITELY=0;

		GRANT_LOANS_MAYBE=0;

		return 0;
	}

	return 0;
}
