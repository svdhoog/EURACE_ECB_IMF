#include "../header.h"
#include "../CentralBank_agent_header.h"
#include "../my_library_header.h"
#include "sanity_checks_aux_headers.h"
#include <assert.h>

int CentralBank_sanity_checks()
{
	//double precision = 1e-16;
	//double precision = 1e-6;
	double precision = 1e-2;

	if (ENABLE_SANITY_CHECKS)
	{		
		BALANCE_SHEET_DELTA_ABS = fabs(ECB_STOCKS_CALENDAR.equity - EQUITY);

        if(PRINT_DEBUG_CB)
        {                        
            if(BALANCE_SHEET_DELTA_ABS > precision)
                fprintf(stderr, "\nIT %d ID %d ABORTED: CB Balance sheet not balanced!: fabs(ECB_STOCKS_CALENDAR.equity - EQUITY)=%f ECB_STOCKS_CALENDAR.equity %f EQUITY %f (precision %f) (%f %% of EQUITY) (%f %% of ECB_STOCKS_CALENDAR.equity)\n",
                        DAY, ID, BALANCE_SHEET_DELTA_ABS, ECB_STOCKS_CALENDAR.equity, EQUITY, precision, (BALANCE_SHEET_DELTA_ABS/fabs(EQUITY)-1)*100.0, (BALANCE_SHEET_DELTA_ABS/fabs(ECB_STOCKS_CALENDAR.equity)-1)*100.0);
        }

		if (DAY==1)
			GOLD = ECB_DEPOSITS + EQUITY - FIAT_MONEY;

		assert__(fabs(FIAT_MONEY_BANKS+FIAT_MONEY_GOVS+GOLD-ECB_DEPOSITS-EQUITY)<precision)
		{
					fprintf(stderr, "\nIT %d ID %d ABORTED: CB Balance sheet not balanced!", DAY, ID);
					fprintf(stderr, "\n\t In %s:%s(), line %d: fabs(FIAT_MONEY_BANKS+FIAT_MONEY_GOVS+GOLD-ECB_DEPOSITS-EQUITY) = %f\n", __FILE__, __FUNCTION__, __LINE__, fabs(FIAT_MONEY_BANKS+FIAT_MONEY_GOVS+GOLD-ECB_DEPOSITS-EQUITY));
		}
    }

	//assert(BALANCE_SHEET_DELTA_ABS < precision);
	
	return 0;
}
