#include "../header.h"
#include "../Bank_agent_header.h"
#include "../my_library_header.h"

int Bank_initialization()
{
	// Set these in order to enable usage of constants in XML function conditions!
	INIT_TRANSIENT_PERIOD 		= CONST_INIT_TRANSIENT_PERIOD;
	
	// Set these to propagate from global constants to agent-specific variables
	REGULATORY_CAPITAL_BUFFER 	= CONST_REGULATORY_CAPITAL_BUFFER;
	REGULATORY_LTV_CAP 			= CONST_REGULATORY_LTV_CAP;
	REGULATORY_DSTI_CAP 		= CONST_REGULATORY_DSTI_CAP;

	//temp firm loan
	firm_loan_adt loan;
	init_firm_loan_adt(&loan);

	//Initialize loan_id's for all firm loans
    int loan_id = 0;

	#ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_CREDIT)
    {
    	printf("\n\n Bank_initialization ID: %d",ID);
        printf("\n\t bank_id\t firm_id\t\t loan_id\t principal\t installment\t periods");
    }              
	#endif

	START_FIRM_INITIAL_LOAN_MESSAGE_LOOP
	if(ID==firm_initial_loan_message->bank_id)
	{
		loan_id++;
		
	    loan.bank_id = ID;
        loan.firm_id = firm_initial_loan_message->firm_id;
	    loan.loan_id = loan_id;
	    loan.outstanding_principal = firm_initial_loan_message->outstanding_principal;
	    loan.interest_rate = firm_initial_loan_message->interest_rate;
	    loan.installment_amount = firm_initial_loan_message->installment_amount;
	    loan.bad_debt = firm_initial_loan_message->bad_debt;
	    loan.nr_periods_before_repayment = firm_initial_loan_message->nr_periods_before_repayment;

		add_firm_loan_adt(&FIRM_LOAN_PORTFOLIO, loan.firm_id, loan.bank_id, loan.loan_id, loan.outstanding_principal, loan.interest_rate, loan.installment_amount, loan.bad_debt, loan.nr_periods_before_repayment);

        add_bank_initial_loan_id_message(loan.firm_id, loan.loan_id);

		#ifdef _DEBUG_MODE        
		if (PRINT_DEBUG_CREDIT)
		{
            printf("\n\t%8d\t%8d\t%8d\t%f\t%f\t%d", ID, loan.firm_id, loan.loan_id, loan.outstanding_principal, loan.installment_amount, loan.nr_periods_before_repayment);
		}              
		#endif
	}
	FINISH_FIRM_INITIAL_LOAN_MESSAGE_LOOP

	return 0;
}

/* \fn: int Bank_reset_variables()
 * \brief: Function to reset Bank variables, 1st day of month
 */
int Bank_reset_variables()
{
	BANK_STOCKS_CALENDAR.cash_day_1 = RESERVES;
	
	init_bank_outflows_adt(&BANK_OUTFLOWS_CALENDAR);
	init_bank_inflows_adt(&BANK_INFLOWS_CALENDAR);

	BANK_INFLOWS_MONTHLY_MORTGAGE_PAYMENTS = 0.0;
	BANK_OUTFLOWS_MONTHLY_MORTGAGE_ISSUES = 0.0;
	BANK_OUTFLOWS_CALENDAR.deposit_interest_payment  = 0.0;
	
	//Monthly resetting
	PROFITS[0]=0;  
	
    BANK_NO_FIRM_LOANS 			= 0;
    BANK_NO_FIRM_LOANS_NEW		= 0;
	BANK_FIRM_LOANS_NEW_BUSINESS = 0.0;

	BANK_NO_FIRM_LOANS_PAYING 	= 0;
	BANK_NO_FIRM_LOANS_WRITEOFF = 0;

	BANK_NO_MORTGAGES			= 0;
	BANK_NO_MORTGAGES_NEW 		= 0; //accepted: on signing contract
	BANK_MORTGAGES_NEW_BUSINESS = 0.0;

	BANK_MORTGAGES_REQUESTED	= 0; //requested: after HH checked LTV/DSTI/own contribution
	BANK_MORTGAGES_PAYING	= 0;
	BANK_MORTGAGES_ARREARS	= 0;
	BANK_MORTGAGES_NPL		= 0;
	BANK_MORTGAGES_WRITEOFF	= 0;
	BANK_MORTGAGES_PAID_OFF	= 0;

	//Sums: sum(r_i) 
	SUM_INTEREST_RATES_NEW_FIRM_LOANS = 0.0;
	SUM_INTEREST_RATES_NEW_MORTGAGES = 0.0;
	
	//Weighted-sums: sum(r_i*L_i) 
	WEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS = 0.0;
	WEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES = 0.0;

    WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = 0.0;
    WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = 0.0;
	WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = 0.0;

	WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = 0.0;
	WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = 0.0;
	WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = 0.0;
    
	// Reset MEAN INTEREST RATES in case of inactive banks so that this does not corrupt
	// the overall average across all banks. This is needed because inactive banks do not
	// run the function Bank_issue_loans().
	if (ACTIVE==0)
	{
		MEAN_INTEREST_RATE_NEW_FIRM_LOANS = 0.0;
		MEAN_INTEREST_RATE_NEW_MORTGAGES = 0.0;
		
		//------------
		WEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS = 0.0;
        
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = 0.0;
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = 0.0;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = 0.0;

		WEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES = 0.0;

		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = 0.0;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = 0.0;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = 0.0;

		//------------
		MEAN_INTEREST_RATE_ALL_FIRM_LOANS = 0.0;
		MEAN_INTEREST_RATE_ALL_MORTGAGES = 0.0;

		WEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS = 0.0;
		WEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES = 0.0;
	}

	SEIZED_COLLATERAL_VALUE_CALENDAR = 0.0;
	
	LLP_FIRM_LOANS = 0.0;
	LLP_PL_MORTGAGE_LOANS = 0.0;
	LLP_NPL_MORTGAGE_LOANS = 0.0;

	MORTGAGE_EXTRA_PROFIT_CALENDAR_MONTH = 0.0;
	MORTGAGE_WRITEOFF_CALENDAR_MONTH = 0.0;
	DEPOSIT_EXPENSE_CALENDAR_MONTH = 0.0;
	DIVIDEND_EXPENSE_CALENDAR_MONTH = 0.0;
	CB_INTEREST_EXPENSE_CALENDAR_MONTH = 0.0;
	CB_INTEREST_INCOME_CALENDAR_MONTH = 0.0;
	MORTGAGE_INCOME_CALENDAR_MONTH = 0.0;
	FIRM_LOAN_INCOME_CALENDAR_MONTH = 0.0;
	FIRM_LOAN_WRITEDOWN_CALENDAR_MONTH = 0.0;
	TAX_EXPENSE_CALENDAR_MONTH = 0.0;
	
	MRTG_VARIABLE_OFFERED_FLOW_COUNT = 0;
	MRTG_FIXED_OFFERED_FLOW_COUNT = 0;
	MRTG_TOTAL_OFFERED_FLOW_COUNT = 0;
	
	MRTG_VARIABLE_ACCEPTED_FLOW_COUNT = 0;
	MRTG_FIXED_ACCEPTED_FLOW_COUNT = 0;
	MRTG_TOTAL_ACCEPTED_FLOW_COUNT = 0;

	return 0;
}

/** \fn Bank_read_policy_announcements()
 * \brief Function to read the policy_announcements from Governments.
 * \brief Frequency: Runs only on 1st of the month, top of stategraph
 */
int Bank_read_policy_announcements()
{

	//4.2.2010: ECB_INTEREST_RATE is a constant
	ECB_INTEREST_RATE = CONST_ECB_INTEREST_RATE;

	//4.5.2017: ECB_INTEREST_RATE is set by ECB using endogenous monetary policy rule:
	//see: Central_Bank_Functions.c, Central_Bank_monetary_policy()
    START_POLICY_RATE_MESSAGE_LOOP
           ECB_INTEREST_RATE = policy_rate_message->policy_rate_value;      
		
		    #ifdef _DEBUG_MODE        
		        if (PRINT_DEBUG_CREDIT)
		        {
		            printf("\n\n Bank_read_policy_rate ID: %d",ID); 
		            printf("\n\t policy_rate_value: %f ECB_INTEREST_RATE: %f", policy_rate_message->policy_rate_value, ECB_INTEREST_RATE);
		        }              
		    #endif
    FINISH_POLICY_RATE_MESSAGE_LOOP
      
	//Set bounds on inputs of the deposit interest rate rule:
	double x,l,u;

	x = (EQUITY/TOTAL_ASSETS);
	l = CONST_EQUITY_TOTAL_ASSET_RATIO_LBND; //0.0
	u = CONST_EQUITY_TOTAL_ASSET_RATIO_UBND; //0.2
	double equity_total_asset_ratio = min(max(l, x), u);

	x = (REA/TOTAL_CREDIT);
	l = CONST_REA_TOTAL_CREDIT_RATIO_LBND; //0.0
	u = CONST_REA_TOTAL_CREDIT_RATIO_UBND; //1.0
	double rea_total_credit_ratio = min(max(l, x), u);

	//DEPOSIT_RATE set according to ECB model specs, Eqn (24) regression
	// No deposit interest if inactive OR during first 6000 its	
    if (TOTAL_ASSETS>EPSILON && EQUITY>EPSILON && ACTIVE)
        DEPOSIT_INTEREST_RATE = max(0.0, BANK_EXPENSE_COEFFS.a_dep + BANK_EXPENSE_COEFFS.b_dep * equity_total_asset_ratio  + BANK_EXPENSE_COEFFS.c_dep * pow(equity_total_asset_ratio, 2)  + BANK_EXPENSE_COEFFS.d_dep * rea_total_credit_ratio + BANK_EXPENSE_COEFFS.e_dep * ECB_INTEREST_RATE);
	else
		DEPOSIT_INTEREST_RATE = 0.0;

	// CAP only during INIT_TRANSIENT_PERIOD:
	//if (DAY < INIT_TRANSIENT_PERIOD && DEPOSIT_INTEREST_RATE > CONST_CAP_DEPOSIT_INTEREST_RATE)
    // CAP ALWAYS ON:
    if (DEPOSIT_INTEREST_RATE > CONST_CAP_DEPOSIT_INTEREST_RATE)            
    {
        #ifdef _DEBUG_MODE        
        if (PRINT_DEBUG_FLOORS_AND_CEILINGS)
        {
            fprintf(stderr, "\nIT %d ID %d WARNING: DEPOSIT_INTEREST_RATE %f exceeds upper bound %f, setting DEPOSIT_INTEREST_RATE equal to cap %f", DAY, ID, DEPOSIT_INTEREST_RATE, CONST_CAP_DEPOSIT_INTEREST_RATE, CONST_CAP_DEPOSIT_INTEREST_RATE);
        }              
        #endif
		
        /*
		fprintf(stderr, "\nDeposit_interest_rate set via structural estimation of empirical data: bank_expense_coeffs");
		fprintf(stderr, "\n ID\t a\t b\t c\t d\t e\t EQUITY \t TA \t EQUITY/TA\t (EQ/TA)^2 \t REA \t REA/TA \t ECB_int \t RHS \t LHS");
		fprintf(stderr, "\n %4d", ID);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.a_dep);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.b_dep);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.c_dep);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.d_dep);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.e_dep);
		fprintf(stderr, "\t %8.2f", EQUITY);
		fprintf(stderr, "\t %8.2f", TOTAL_ASSETS);
		fprintf(stderr, "\t %.2f", EQUITY/TOTAL_ASSETS);
		fprintf(stderr, "\t %.2f", pow(EQUITY/TOTAL_ASSETS, 2));
		fprintf(stderr, "\t %8.2f", REA);
		fprintf(stderr, "\t %8.2f", (REA/TOTAL_ASSETS));
		fprintf(stderr, "\t %.2f", ECB_INTEREST_RATE);
		fprintf(stderr, "\t %.2f", BANK_EXPENSE_COEFFS.a_dep + BANK_EXPENSE_COEFFS.b_dep * (EQUITY/TOTAL_ASSETS)  + BANK_EXPENSE_COEFFS.c_dep * pow(EQUITY/TOTAL_ASSETS, 2)  + BANK_EXPENSE_COEFFS.d_dep * (REA/TOTAL_ASSETS) + BANK_EXPENSE_COEFFS.e_dep * ECB_INTEREST_RATE);
		fprintf(stderr, "\t %.2f", DEPOSIT_INTEREST_RATE);

		//Newline: coeff*value
		fprintf(stderr, "\n r_dep =");		
		fprintf(stderr, "%.2f", BANK_EXPENSE_COEFFS.a_dep);
		fprintf(stderr, "\t\t\t\t");
		fprintf(stderr, "\t%+.2f", BANK_EXPENSE_COEFFS.b_dep*(EQUITY/TOTAL_ASSETS) );
		fprintf(stderr, "\t%+.2f", BANK_EXPENSE_COEFFS.c_dep*pow(EQUITY/TOTAL_ASSETS, 2));
		fprintf(stderr, "\t%+.2f", BANK_EXPENSE_COEFFS.d_dep*(REA/TOTAL_ASSETS));
		fprintf(stderr, "\t%+.2f", BANK_EXPENSE_COEFFS.e_dep*ECB_INTEREST_RATE);
		*/
		DEPOSIT_INTEREST_RATE = CONST_CAP_DEPOSIT_INTEREST_RATE;
	}


    #ifdef _DEBUG_MODE        
    if (PRINT_CREDIT_INFO)
    {
        //printf("\n\t DEPOSIT_INTEREST_RATE: %f ECB_INTEREST_RATE_MARKDOWN: %f", DEPOSIT_INTEREST_RATE, ECB_INTEREST_RATE_MARKDOWN);
		
		//printf("\nID,a,b,c,d,e,%s,%s,%s,%s,%s,%s,%s,%s,%s", "E", "TA", "E/A", "(E/A)^2", "REA", "REA/TA", "i^cb", "r(eq24)", "r(set)");

		printf("\nDeposit_interest_rate set via structural estimation of empirical data: bank_expense_coeffs");
		printf("\n ID\t a\t b\t c\t d\t e\t EQUITY \t TA \t EQUITY/TA\t (EQ/TA)^2 \t REA \t REA/TA \t ECB_int \t RHS \t LHS");
		printf("\n %4d", ID);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.a_dep);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.b_dep);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.c_dep);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.d_dep);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.e_dep);
		printf("\t %8.2f", EQUITY);
		printf("\t %8.2f", TOTAL_ASSETS);
		printf("\t %.2f", EQUITY/TOTAL_ASSETS);
		printf("\t %.2f", pow(EQUITY/TOTAL_ASSETS, 2));
		printf("\t %8.2f", REA);
		printf("\t %8.2f", (REA/TOTAL_ASSETS));
		printf("\t %.2f", ECB_INTEREST_RATE);
		printf("\t %.2f", BANK_EXPENSE_COEFFS.a_dep + BANK_EXPENSE_COEFFS.b_dep * (EQUITY/TOTAL_ASSETS)  + BANK_EXPENSE_COEFFS.c_dep * pow(EQUITY/TOTAL_ASSETS, 2)  + BANK_EXPENSE_COEFFS.d_dep * (REA/TOTAL_ASSETS) + BANK_EXPENSE_COEFFS.e_dep * ECB_INTEREST_RATE);
		printf("\t %.2f", DEPOSIT_INTEREST_RATE);

		//Newline: coeff*value
		printf( "\n r_dep =");		
		printf( "%.2f", BANK_EXPENSE_COEFFS.a_dep);
		printf( "\t\t\t\t");
		printf( "\t%+.2f", BANK_EXPENSE_COEFFS.b_dep*(EQUITY/TOTAL_ASSETS) );
		printf( "\t%+.2f", BANK_EXPENSE_COEFFS.c_dep*pow(EQUITY/TOTAL_ASSETS, 2));
		printf( "\t%+.2f", BANK_EXPENSE_COEFFS.d_dep*(REA/TOTAL_ASSETS));
		printf( "\t%+.2f", BANK_EXPENSE_COEFFS.e_dep*ECB_INTEREST_RATE);
    }              
    #endif


    //Message send by Government:
    START_POLICY_ANNOUNCEMENT_MESSAGE_LOOP          
        //Filter: 
        if(policy_announcement_message->gov_id == GOV_ID)
        {
            TAX_RATE_CORPORATE = policy_announcement_message->tax_rate_corporate;
        }
    FINISH_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_CREDIT)
    {
        printf("\n Bank_read_policy_announcements ID: %d",ID);
        printf("\n\tTAX_RATE_CORPORATE: %f", TAX_RATE_CORPORATE);
    }
    #endif

    // Central Bank Policy Stance announcements (Cap, LTV cap, DSTI cap)
    // Set these to propagate from ECB values to agent-specific variables
	START_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

		REGULATORY_CAPITAL_BUFFER 	= ecb_policy_announcement_message->regulatory_capital_buffer;
		REGULATORY_LTV_CAP 			= ecb_policy_announcement_message->regulatory_ltv_cap;
		REGULATORY_DSTI_CAP 		= ecb_policy_announcement_message->regulatory_dsti_cap;

	    #ifdef _DEBUG_MODE    
	    if (PRINT_DEBUG_MACROPRU)
	    {
	        printf("\nIT %d ID %d BANK RECEIVED ECB POLICY ANNOUNCEMENT:", DAY, ID);
            printf("\n\tREGULATORY_CAPITAL_BUFFER: %f (%f %%)\n\tREGULATORY_LTV_CAP: %f (%f %%)\n\tREGULATORY_DSTI_CAP: %f (%f %%)", REGULATORY_CAPITAL_BUFFER, 100.0*REGULATORY_CAPITAL_BUFFER, REGULATORY_LTV_CAP, 100.0*REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP, 100*REGULATORY_DSTI_CAP);
	    }
	    #endif
	FINISH_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

    // Central Bank Macroprudential Policy announcements
	START_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
		REGULATORY_CAPITAL_BUFFER	= ecb_macroprudential_policy_announcement_message->regulatory_capital_buffer;
	    REGULATORY_LTV_CAP 			= ecb_macroprudential_policy_announcement_message->regulatory_ltv_cap;
	    REGULATORY_DSTI_CAP 		= ecb_macroprudential_policy_announcement_message->regulatory_dsti_cap;

	    #ifdef _DEBUG_MODE    
	    if (PRINT_DEBUG_MACROPRU)
	    {
	        printf("\nIT %d ID %d BANK RECEIVED ECB MACROPRUDENTIAL POLICY ANNOUNCEMENT:", DAY, ID);
            printf("\n\tREGULATORY_CAPITAL_BUFFER: %f (%f %%)\n\tREGULATORY_LTV_CAP: %f (%f %%)\n\tREGULATORY_DSTI_CAP: %f (%f %%)", REGULATORY_CAPITAL_BUFFER, 100.0*REGULATORY_CAPITAL_BUFFER, REGULATORY_LTV_CAP, 100.0*REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP, 100*REGULATORY_DSTI_CAP);
	    }
	    #endif
    FINISH_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
        


    return 0;
}
