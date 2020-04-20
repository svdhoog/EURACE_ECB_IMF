#include "../header.h"
#include "../Bank_agent_header.h"
#include "../my_library_header.h"
#include "sanity_checks_aux_headers.h"
#include "../Statistical_Office/balance_sheet_operations.h"
#include <assert.h>

int Bank_sanity_checks()
{
    int i;
    
    double precision = 1e-3;
    
	double total_liabilities = 0.0;
	double total_assets = 0.0;

    #ifdef _DEBUG_MODE        
    if (PRINT_CREDIT_INFO)
    {
        //Semi-annual print out of funding costs
        if(DAY%120==0)
        {
            int n = DEPOSIT_EXPENSE_DAILY_HISTORY.size;
            
            printf("\n\nSemi-annual Bank balance sheet:");
            
            if (ACTIVE)
            {
                printf("\n\n[%d]\t%f\t%f", ID, RESERVES, FIRM_CREDIT);
                printf("|\t%f\t%f\t%f\t%f\t", DEPOSITS, ECB_DEBT, LLR_FIRM_LOANS, EQUITY);
                printf("|\t%f\t%f\t%f\t%f\t%f\t%f\n", DEPOSIT_INTEREST_RATE, FUNDING_COSTS, FIRM_INTEREST_PAYMENTS, DEPOSIT_EXPENSE_DAILY_HISTORY.array[n-1], DIVIDEND_EXPENSE, PROFITS_FLOW);
            }
            else
            {
                printf("\n\n[%d]\t%f\t%f", ID, RESERVES, FIRM_CREDIT);
                printf("|\t%f\t%f\t%f\t%f\t", DEPOSITS, ECB_DEBT, LLR_FIRM_LOANS, EQUITY);
                printf("|\t%f\t%f\t%f\t%f\t%f\t%f-inactive-\n", DEPOSIT_INTEREST_RATE, FUNDING_COSTS, FIRM_INTEREST_PAYMENTS, DEPOSIT_EXPENSE_DAILY_HISTORY.array[n-1], DIVIDEND_EXPENSE, PROFITS_FLOW);
            }
        }

        //Quarterly print out of firm loan portfolio
        if(DAY%60==0)
        {
            if (FIRM_LOAN_PORTFOLIO.size>0)
            {
                printf("\n\nQuarterly Firm loan portfolio:");
                printf("\n\tID\t Firm ID\t Loan ID\t principal\t installment\t periods");
            }
            for(i=0; i<FIRM_LOAN_PORTFOLIO.size; i++)
                printf("\n\t%4d\t%8d\t%8d\t%8.2f\t%8.2f\t%4d", FIRM_LOAN_PORTFOLIO.array[i].bank_id, FIRM_LOAN_PORTFOLIO.array[i].firm_id, FIRM_LOAN_PORTFOLIO.array[i].loan_id, FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal, FIRM_LOAN_PORTFOLIO.array[i].installment_amount, FIRM_LOAN_PORTFOLIO.array[i].nr_periods_before_repayment);
        }
    }
    #endif
    
	if (ENABLE_SANITY_CHECKS)
	{		
		//balance sheet differential
		total_liabilities = DEPOSITS + ECB_DEBT 
							+ LLR_FIRM_LOANS 
							+ LLR_PL_MORTGAGE_LOANS 
							+ LLR_NPL_MORTGAGE_LOANS 
							+ CAPITAL_ADJ_PL_MORTGAGE_LOANS
							+ CAPITAL_ADJ_NPL_MORTGAGE_LOANS	
							+ CAPITAL_ADJ_PL_FIRM_LOANS 
							+ CAPITAL_ADJ_NPL_FIRM_LOANS 
							+ EQUITY;

		total_assets = RESERVES + FIRM_CREDIT + PL_MORTGAGE_LOANS + NPL_MORTGAGE_LOANS;
		
        // Relative deviation wrt total asset value
        assert__(fabs(total_assets - total_liabilities)/total_assets < precision)
        {
            fprintf(stderr, "\nIT %d ID %d ABORTED: Bank Balance sheet not balanced! In %s/%s(): Relative diff too large: fabs(total_assets - total_liabilities)=%f [total_assets %f; %f percent] (precision %f)\n",
                    DAY, ID, __FILE__, __FUNCTION__, fabs(total_assets - total_liabilities), total_assets, fabs(total_assets - total_liabilities)/total_assets, precision);
        }
        
        // Absolute deviation 
        double precision_abs = 1e-2;

        assert__(fabs(total_assets - total_liabilities) < precision_abs)
        {
            fprintf(stderr, "\nIT %d ID %d ABORTED: Bank Balance sheet not balanced! In %s/%s(): Abolute diff too large: fabs(total_assets - total_liabilities)=%f [total_assets %f; %f percent] (precision %f)\n",
                    DAY, ID, __FILE__, __FUNCTION__, fabs(total_assets - total_liabilities), total_assets, fabs(total_assets - total_liabilities)/total_assets, precision_abs);
        }
      
		BALANCE_SHEET_DELTA = total_assets - total_liabilities;
		BALANCE_SHEET_DELTA_ABS = fabs(total_assets - total_liabilities);
	}
	
	return 0;
}

void enforce_bank_balanced_balance_sheet(char string[])
{
	int i;
    double precision = 1e-3;
    double precision_abs = 1e-2;

	//balance sheet differential
	double total_liabilities = 	DEPOSITS + ECB_DEBT 
								+ LLR_FIRM_LOANS 
								+ LLR_PL_MORTGAGE_LOANS 
								+ LLR_NPL_MORTGAGE_LOANS 
								+ CAPITAL_ADJ_PL_MORTGAGE_LOANS
								+ CAPITAL_ADJ_NPL_MORTGAGE_LOANS	
								+ CAPITAL_ADJ_PL_FIRM_LOANS 
								+ CAPITAL_ADJ_NPL_FIRM_LOANS 
								+ EQUITY;
		
	double total_assets = RESERVES + FIRM_CREDIT + PL_MORTGAGE_LOANS + NPL_MORTGAGE_LOANS;


	//Check consistency between FIRM_CREDIT and contents of FIRM_LOAN_PORTFOLIO
	double total_firm_loan_portfolio=0.0;

	for(i=0; i<FIRM_LOAN_PORTFOLIO.size; i++)
		total_firm_loan_portfolio += FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal;

    assert__(fabs(FIRM_CREDIT - total_firm_loan_portfolio) < precision_abs) 
    {
        fprintf(stderr, "\n\tIT %d ID %d ERROR in %s abolute diff too large: inconsistency between FIRM_CREDIT and contents of FIRM_LOAN_PORTFOLIO: fabs(FIRM_CREDIT - total_firm_loan_portfolio)=%f", DAY, ID, string, fabs(FIRM_CREDIT - total_firm_loan_portfolio));
    }

	// Relative deviation wrt total asset value
	assert__(fabs(total_assets - total_liabilities)/total_assets < precision)
	{
        fprintf(stderr, "\nIT %d ID %d ABORTED: Bank Balance sheet not balanced! (%s) Relative diff too large: fabs(total_assets - total_liabilities)=%f [total_assets %f; %f percent] (precision %f)\n",
                DAY, ID, string, fabs(total_assets - total_liabilities), total_assets, fabs(total_assets - total_liabilities)/total_assets, precision);
	}
	
	// Absolute deviation 
	assert__(fabs(total_assets - total_liabilities) < precision_abs)
    {
        fprintf(stderr, "\nIT %d ID %d ABORTED: Bank Balance sheet not balanced! (%s) Abolute diff too large: fabs(total_assets - total_liabilities)=%f [total_assets %f; %f percent] (precision %f)\n",
                DAY, ID, string, fabs(total_assets - total_liabilities), total_assets, fabs(total_assets - total_liabilities)/total_assets, precision_abs);
    }

}
