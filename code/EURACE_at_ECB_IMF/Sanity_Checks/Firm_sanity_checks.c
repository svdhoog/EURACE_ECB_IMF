#include "../header.h"
#include "../Firm_agent_header.h"
#include "../my_library_header.h"
#include "sanity_checks_aux_headers.h"

int Firm_sanity_checks()
{
	double eps=1e-4;

	if (ENABLE_SANITY_CHECKS)
	{
		//Check non-NaN condition:
	    assert__(EQUITY==EQUITY)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: EQUITY %f", __FUNCTION__, __LINE__, EQUITY);
    	}

		assert__(TOTAL_VALUE_CAPITAL_STOCK==TOTAL_VALUE_CAPITAL_STOCK)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_VALUE_CAPITAL_STOCK %f", __FUNCTION__, __LINE__, TOTAL_VALUE_CAPITAL_STOCK);
    	}

		assert__(TOTAL_VALUE_LOCAL_INVENTORY==TOTAL_VALUE_LOCAL_INVENTORY)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_VALUE_LOCAL_INVENTORY %f", __FUNCTION__, __LINE__, TOTAL_VALUE_LOCAL_INVENTORY);
    	}

		assert__(TOTAL_DEBT==TOTAL_DEBT)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_DEBT %f", __FUNCTION__, __LINE__, TOTAL_DEBT);
    	}

		assert__(PAYMENT_ACCOUNT==PAYMENT_ACCOUNT)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: PAYMENT_ACCOUNT %f", __FUNCTION__, __LINE__, PAYMENT_ACCOUNT);
    	}


		//Check positivity condition:
	    assert__(PAYMENT_ACCOUNT > -eps)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: PAYMENT_ACCOUNT %f", __FUNCTION__, __LINE__, PAYMENT_ACCOUNT);
    	}

	    assert__(TOTAL_VALUE_CAPITAL_STOCK > -eps)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_VALUE_CAPITAL_STOCK %f", __FUNCTION__, __LINE__, TOTAL_VALUE_CAPITAL_STOCK);
    	}

	    assert__(TOTAL_VALUE_LOCAL_INVENTORY > -eps)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_VALUE_LOCAL_INVENTORY %f", __FUNCTION__, __LINE__, TOTAL_VALUE_LOCAL_INVENTORY);
    	}	

	    assert__(TOTAL_DEBT > -eps)
	    {
    	    fprintf(stderr, "\n\tWARNING %s():%d: TOTAL_DEBT %f", __FUNCTION__, __LINE__, TOTAL_DEBT);
    	}

	}

	return 0;
}