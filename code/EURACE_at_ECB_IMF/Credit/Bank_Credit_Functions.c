#include <gsl/gsl_math.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_statistics.h>

#include "../header.h"
#include "../Bank_agent_header.h"
#include "../my_library_header.h"
#include "../ECB_Model/Credit_Risk_Management/Bank_aux_headers.h"
#include "../Statistical_Office/balance_sheet_operations.h"
#include "../Sanity_Checks/sanity_checks_aux_headers.h"
#include "Bank_credit_aux_headers.h"

/** \fn Bank_communicate_identity()
 * \brief Function to let all agents in the economy know which IDs are banks.
 */
int Bank_communicate_identity()
{
    if (ACTIVE)
        add_bank_identity_message(ID);

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_CREDIT)
    {
        printf("\n\n Bank_communicate_identity ID: %d",ID);
    }      
    #endif


	return 0;   
}

/** \fn Bank_send_loan_portfolio_data()
 * \brief Function to send bank's complete loan portfolio (loan_adts) to the NSA agent for network analysis.
 */
int Bank_send_loan_portfolio_data()
{
  // Declare, init financial_data
  financial_data_adt financial_data;
  
  financial_data.equity		= EQUITY;
  financial_data.total_assets	= TOTAL_ASSETS;
  financial_data.total_debt	= ECB_DEBT;
  financial_data.equity_asset_ratio = EQUITY_ASSET_RATIO;
  financial_data.debt_equity_ratio = DEBT_EQUITY_RATIO;

  // Send financial_data
  add_bank_loan_portfolio_message(ID, financial_data);

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_NSA)
    {
        printf("\n\n Bank_send_loan_portfolio_data ID: %d", ID);
    }      
    #endif
    
 return 0;   
}



/** \fn Bank_send_dividend_payment()
 * \brief Payment of dividends to the clearinghouse.
 */
int Bank_send_dividend_payment()
{
    /* total dividends are send to clearinghouse, to be distributed to households */
    add_dividend_info_message(ID, TOTAL_DIVIDEND_PAYMENT);

    asset_liability_subtract(&RESERVES, &EQUITY, TOTAL_DIVIDEND_PAYMENT);
    log_outflow("bank", "reserves", "equity", "dividends", ID, TOTAL_DIVIDEND_PAYMENT);
    
    BANK_OUTFLOWS_CALENDAR.dividend_payment += TOTAL_DIVIDEND_PAYMENT;

    DIVIDEND_EXPENSE_CALENDAR_MONTH += TOTAL_DIVIDEND_PAYMENT;
     
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_send_dividend_payment()", DAY, ID);
		printf("\nFlows:");
		printf("\n Dividends %2.2f (RESERVES-,EQUITY-)", TOTAL_DIVIDEND_PAYMENT);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
   return 0;
}

/**
 * \fn Bank_send_account_interest()
 * \brief Send interest_info message and pay interest on deposits. Daily, at start of iteration.
 */   
int Bank_send_account_interest(void)
{
	int i;
	int n=0;
	double sum=0.0;
	double sum_funding_costs_expense_history = 0.0;

    double int_to_ecb=0.0;
    double int_from_ecb=0.0;
    double int_from_ecb_on_regulatory_reserves = 0.0;
	double int_from_ecb_on_excess_reserves = 0.0;

    //Pay interests to CB on CB debt of day before: overnight borrowing
    //If Bank inactive, set to zero
    //Use the ECB_INTEREST_RATE that results from the Taylor rule
    if (ACTIVE)
        int_to_ecb  = ECB_DEBT*ECB_INTEREST_RATE/240.0;

	//Interest on reserves only after transient+one quarter
	//if(DAY >= TRANSITION_PHASE_TAYLOR_RULE )
    //{
        //Interest income received on RESERVES held at CB, distinguished between regulatory and excess reserves
        //Note: includes a check if actual RESERVES are below regulatory levels
        //Note 2: at this point in the code, actual RESERVES could be negative due to deposit transfers, and other mutations
        //        if this is the case assume 0 interest is paid by ECB on this negative reserves.
        if(SWITCH_REMUNERATION_REGULATORY_RESERVES)
        	int_from_ecb_on_regulatory_reserves  = min(max(0.0, RESERVES), MIN_RESERVE_RATIO * DEPOSITS)*ECB_INTEREST_RATE/240.0;
        else
            int_from_ecb_on_regulatory_reserves  = min(max(0.0, RESERVES), MIN_RESERVE_RATIO * DEPOSITS)*CONST_INTEREST_RATE_FOR_REGULATORY_RESERVES/240.0;

        if(SWITCH_MARKDOWN_EXCESS_RESERVE_REMUNERATION)
        {
            //OPTION 1: Interest on excess reserves is a mark-down on the policy rate
            //COULD BE NEGATIVE if: (ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES)<0
            int_from_ecb_on_excess_reserves = max(0.0, max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES))/240.0;
        }
        else
        {
            //OPTION 2: Interest on excess reserves is a constant percentage
            int_from_ecb_on_excess_reserves = max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*CONST_INTEREST_RATE_FOR_EXCESS_RESERVES / 240.0;
        }       
        int_from_ecb  = int_from_ecb_on_regulatory_reserves + int_from_ecb_on_excess_reserves;

        //Component-wise checks
        if (int_from_ecb_on_regulatory_reserves < -EPSILON)
        {
            fprintf(stderr, "\n\nint_from_ecb_on_regulatory_reserves < -EPSILON, int_from_ecb_on_regulatory_reserves = %.16f", int_from_ecb_on_regulatory_reserves);
            fprintf(stderr, "\n\tRESERVES = %.16f", RESERVES);
            fprintf(stderr, "\n\tMIN_RESERVE_RATIO * DEPOSITS = %.16f", MIN_RESERVE_RATIO * DEPOSITS);
            fprintf(stderr, "\n\tECB_INTEREST_RATE = %.16f", ECB_INTEREST_RATE);
            fprintf(stderr, "\n\t(MIN_RESERVE_RATIO * DEPOSITS)*ECB_INTEREST_RATE = %.16f", (MIN_RESERVE_RATIO * DEPOSITS)*ECB_INTEREST_RATE);
            fprintf(stderr, "\n\tmin(RESERVES, MIN_RESERVE_RATIO * DEPOSITS) = %.16f", min(RESERVES, MIN_RESERVE_RATIO * DEPOSITS));
        }
        
        if (int_from_ecb_on_excess_reserves < -EPSILON)
        {
            fprintf(stderr, "\n\nint_from_ecb_on_excess_reserves < -EPSILON, int_from_ecb_on_excess_reserves = %.16f", int_from_ecb_on_excess_reserves);
            fprintf(stderr, "\n\tRESERVES - MIN_RESERVE_RATIO*DEPOSITS = %.16f", RESERVES - MIN_RESERVE_RATIO*DEPOSITS);
            fprintf(stderr, "\n\tECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES = %.16f", ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES);
            fprintf(stderr, "\n\t(RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES) = %.16f", (RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES));
            fprintf(stderr, "\n\tmax(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS) = %.16f", max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS));
            fprintf(stderr, "\n\tmax(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES) = %.16f", max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES));
            fprintf(stderr, "\n\tmax(0.0, max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES))/240.0 = %.16f", max(0.0, max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS)*(ECB_INTEREST_RATE - CONST_ECB_INTEREST_RATE_MARKDOWN_FOR_EXCESS_RESERVES))/240.0);
        }        
            
        if (int_from_ecb < -EPSILON)
        {
            fprintf(stderr, "\n\nint_from_ecb = %.16f", int_from_ecb);        
            fprintf(stderr, "\n\nint_from_ecb_on_regulatory_reserves = %.16f", int_from_ecb_on_regulatory_reserves);
            fprintf(stderr, "\n\tint_from_ecb_on_excess_reserves = %.16f", int_from_ecb_on_excess_reserves);
        }
        
        // Asserts 
        assert(int_from_ecb_on_regulatory_reserves > -EPSILON);
        assert(int_from_ecb_on_excess_reserves > -EPSILON);        
        assert(int_from_ecb > -EPSILON);
    //} //if(DAY >= TRANSITION_PHASE_TAYLOR_RULE )
    
    if (ACTIVE)
    {
	    //subtract interest paid to ECB on ECB debt
	     PROFITS[0] -= int_to_ecb; 
		 asset_liability_subtract(&RESERVES, &EQUITY, int_to_ecb);
		 log_outflow("bank", "reserves", "equity", "int_to_ecb", ID, int_to_ecb);
    }

	//Flow accounting
    BANK_OUTFLOWS_CALENDAR.cb_net_interest_payment += int_to_ecb - int_from_ecb;

    /* ALL RESERVES: Trailing sum of last 60 days of daily income flows (array length 60)
     * Initialize on todays interest, add last 59 days 
     * Note: we loop over i<size-1, so last item will be [size-2+1]=[size-1] which is OK.
     * Array: update the array, and add new item to end
     */
	INTEREST_INCOME_RESERVES = int_from_ecb; //today's interest income flow

	for (i=0; i<INTEREST_INCOME_RESERVES_DAILY_HISTORY.size-1; i++)
	{
		INTEREST_INCOME_RESERVES_DAILY_HISTORY.array[i] = INTEREST_INCOME_RESERVES_DAILY_HISTORY.array[i+1];
		INTEREST_INCOME_RESERVES += INTEREST_INCOME_RESERVES_DAILY_HISTORY.array[i+1];
	}
	INTEREST_INCOME_RESERVES_DAILY_HISTORY.array[INTEREST_INCOME_RESERVES_DAILY_HISTORY.size-1] = int_from_ecb;

	//Annualise
	INTEREST_INCOME_RESERVES = 4.0*INTEREST_INCOME_RESERVES;

	assert(INTEREST_INCOME_RESERVES>-EPSILON);


   /* REGULATORY_RESERVES: Trailing sum of last 60 days of daily income flows (array length 60)
     * Initialize on todays interest, add last 59 days
     * Note: we loop over i<size-1, so last item will be [size-2+1]=[size-1] which is OK.
     * Array: update the array, and add new item to end
     */
	INTEREST_INCOME_REGULATORY_RESERVES = int_from_ecb_on_regulatory_reserves; //today's interest income flow

	for (i=0; i<INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.size-1; i++)
	{
		INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.array[i] = INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.array[i+1];
		INTEREST_INCOME_REGULATORY_RESERVES += INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.array[i+1];
	}
	INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.array[INTEREST_INCOME_REGULATORY_RESERVES_DAILY_HISTORY.size-1] = int_from_ecb_on_regulatory_reserves;

	//Annualise
	INTEREST_INCOME_REGULATORY_RESERVES = 4.0*INTEREST_INCOME_REGULATORY_RESERVES;

	assert(INTEREST_INCOME_REGULATORY_RESERVES>-EPSILON);


   /* EXCESS_RESERVES: Trailing sum of last 60 days of daily income flows (array length 60)
     * Initialize on todays interest, add last 59 days
     * Note: we loop over i<size-1, so last item will be [size-2+1]=[size-1] which is OK.
     * Array: update the array, and add new item to end
     */
	INTEREST_INCOME_EXCESS_RESERVES = int_from_ecb_on_excess_reserves; //today's interest income flow

	for (i=0; i<INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.size-1; i++)
	{
		INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.array[i] = INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.array[i+1];
		INTEREST_INCOME_EXCESS_RESERVES += INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.array[i+1];
	}
	INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.array[INTEREST_INCOME_EXCESS_RESERVES_DAILY_HISTORY.size-1] = int_from_ecb_on_excess_reserves;

	//Annualise
	INTEREST_INCOME_EXCESS_RESERVES = 4.0*INTEREST_INCOME_EXCESS_RESERVES;

	assert(INTEREST_INCOME_EXCESS_RESERVES>-EPSILON);

 
    /* NOTE: The CB_NET_INTEREST_PAYMENT is the net interest expense flow to CB.     
     * CB_NET_INTEREST_PAYMENT < 0: the bank receives interest from ECB
     * CB_NET_INTEREST_PAYMENT > 0: the bank pays interest to ECB
     */

    /* Daily (NET) interest payment flows to CB, over a 60 day rolling window
     * Trailing sum of last 60 days3 months of daily flows.
	 */
    CB_NET_INTEREST_PAYMENT = int_to_ecb - int_from_ecb;
    for (i=0; i<CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.size-1; i++)
	{
		CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.array[i] = CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.array[i+1];
		CB_NET_INTEREST_PAYMENT += CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.array[i+1];
	}
	CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.array[CB_NET_INTEREST_PAYMENT_DAILY_HISTORY.size-1] = int_to_ecb - int_from_ecb;

	//Annualise
	CB_NET_INTEREST_PAYMENT = 4.0*CB_NET_INTEREST_PAYMENT;

	//add interest received from ECB on deposits
    PROFITS[0] += int_from_ecb; 
    asset_liability_add(&RESERVES, &EQUITY, int_from_ecb);
    log_inflow("bank", "reserves", "equity", "int_from_ecb", ID, int_from_ecb);

    add_bank_interest_payment_message(int_to_ecb - int_from_ecb);

    CB_INTEREST_EXPENSE_CALENDAR_MONTH += int_to_ecb;
    CB_INTEREST_INCOME_CALENDAR_MONTH += int_from_ecb;

    /* CB funding expense
     * Trailing sum of last 60 days of daily  CB funding expense flows
     * Initialize on todays cb_funding_expense, add last 59 days
     * Array: update the array, and add new item to end
     */
	CB_FUNDING_EXPENSE = int_to_ecb; 
	for (i=0; i<CB_FUNDING_EXPENSE_DAILY_HISTORY.size-1; i++)
	{
		CB_FUNDING_EXPENSE_DAILY_HISTORY.array[i] = CB_FUNDING_EXPENSE_DAILY_HISTORY.array[i+1];
		CB_FUNDING_EXPENSE += CB_FUNDING_EXPENSE_DAILY_HISTORY.array[i+1];
	}
	CB_FUNDING_EXPENSE_DAILY_HISTORY.array[CB_FUNDING_EXPENSE_DAILY_HISTORY.size-1] = int_to_ecb;

	//Annualise
	CB_FUNDING_EXPENSE = 4.0*CB_FUNDING_EXPENSE;

    /* Pay deposit interest on household and firm accounts
     * In the bank there is no separation of accounts, so all interest is added to total deposits.
     */
    double interest = 0.0;
    if (ACTIVE)
    {
		interest = (1/240.0)*DEPOSIT_INTEREST_RATE*DEPOSITS;
		PROFITS[0] -= interest;

		BANK_OUTFLOWS_CALENDAR.deposit_interest_payment += interest;

		liability_transformation(&EQUITY, &DEPOSITS, interest);
		log_transformation("bank", "equity", "deposits", "pa_interest", ID, interest);

	    add_account_interest_message(ID, DEPOSIT_INTEREST_RATE);

        DEPOSIT_EXPENSE_CALENDAR_MONTH += interest;

    
        
        #ifdef _DEBUG_MODE    
        if (PRINT_BANK_INFO)
        {
            printf("\n\nIT %d ID: %d Bank_send_account_interest() check deposit interest", DAY, ID);
            printf("\n\tDEPOSIT_INTEREST_RATE %f DEPOSITS %f interest %f", DEPOSIT_INTEREST_RATE, DEPOSITS, interest);
        }
        #endif
        
    }
    else
    {
    	// don't pay interest if inactive
    	add_account_interest_message(ID, 0.0);
    }

    /* Deposit funding expense
     * Trailing sum of last 60 days of daily deposit expense flows (array length 60)
     * Initialize on todays interest, add last 59 days
     * Array: update the array, and add new item to end
     */
	DEPOSIT_EXPENSE = interest; //today's interest expense flow
	for (i=0; i<DEPOSIT_EXPENSE_DAILY_HISTORY.size-1; i++)
	{
		DEPOSIT_EXPENSE_DAILY_HISTORY.array[i] = DEPOSIT_EXPENSE_DAILY_HISTORY.array[i+1];
		DEPOSIT_EXPENSE += DEPOSIT_EXPENSE_DAILY_HISTORY.array[i+1];
	}
	DEPOSIT_EXPENSE_DAILY_HISTORY.array[DEPOSIT_EXPENSE_DAILY_HISTORY.size-1] = interest;

	//Annualise
	DEPOSIT_EXPENSE = 4.0*DEPOSIT_EXPENSE;

    /* Dividend funding expense
     * Trailing sum of last 3 months of monthly dividend expense flows
     * Initialize on "todays" dividend_expense (1/20th of monthly dividends), add last 59 days
     * Array: update the array, and add new item to end
     */
	DIVIDEND_EXPENSE = TOTAL_DIVIDEND_PAYMENT/20.0;
	for (i=0; i<DIVIDEND_EXPENSE_DAILY_HISTORY.size-1; i++)
	{
		DIVIDEND_EXPENSE_DAILY_HISTORY.array[i] = DIVIDEND_EXPENSE_DAILY_HISTORY.array[i+1];
		DIVIDEND_EXPENSE += DIVIDEND_EXPENSE_DAILY_HISTORY.array[i+1];
	}
	DIVIDEND_EXPENSE_DAILY_HISTORY.array[DIVIDEND_EXPENSE_DAILY_HISTORY.size-1] = TOTAL_DIVIDEND_PAYMENT/20.0;

	//Annualise
	DIVIDEND_EXPENSE = 4.0*DIVIDEND_EXPENSE;

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_send_account_interest()", DAY, ID);
		printf("\nFlows:");
		printf("\nDeposit interest\t%f\t(DEPOSITS+,EQUITY-)", interest);
		printf("\nCB debt interest\t%f\t(RESERVES-,EQUITY-)", int_to_ecb);
		printf("\nCB reserve interest\t%f\t(RESERVES+,EQUITY+)", int_from_ecb);
		printf("\n60-day trailing sums (monthly avg):");
        printf("\nDeposit expense\t\t%f", DEPOSIT_EXPENSE);
        printf("\nCB funding expense\t%f", CB_FUNDING_EXPENSE);
		printf("\nDividend expense\t%f", DIVIDEND_EXPENSE);
        printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
    if (DAY%MONTH==0)
	{
		//loans history array, size  3, of last 3 months of monthly total loans (firm credit + mortgage credit)
		add_double(&LOANS_HISTORY, BANK_STOCKS_CALENDAR.firm_credit + BANK_STOCKS_CALENDAR.total_mortgages);
		remove_double(&LOANS_HISTORY, 0);

		//total_assets_history array, size  3, of last 3 months of monthly total assets
		add_double(&TOTAL_ASSETS_HISTORY, TOTAL_ASSETS);
		remove_double(&TOTAL_ASSETS_HISTORY, 0);		

	    //Yearly expenses: expenses are 60-day trailing sums over 3 months, annualized by *4
        TOTAL_EXPENSE = CB_FUNDING_EXPENSE + DEPOSIT_EXPENSE + DIVIDEND_EXPENSE;

		//Add yearly total expenses to 3-array
		add_double(&TOTAL_EXPENSE_HISTORY, DEPOSIT_EXPENSE + CB_FUNDING_EXPENSE + DIVIDEND_EXPENSE);
		remove_double(&TOTAL_EXPENSE_HISTORY, 0);

		//Weighted Average Costs of Funding (WACF)
		//deposit + CB funding expenses + dividend expense (w/o CB interest income) divided by total assets, first per 3-month sum, then averaged		
		//1st term: total expense flow for deposits		
		n=0;
		for (i=0; i<TOTAL_EXPENSE_HISTORY.size; i++)
		{
			if (TOTAL_ASSETS_HISTORY.array[i]>EPSILON)	
			{
				sum += TOTAL_EXPENSE_HISTORY.array[i]/TOTAL_ASSETS_HISTORY.array[i];
				n++;
			}
		}

		//Eqn (31)
		//WACF = (1.0/n)*SUM(TOTAL_EXPENSE[i]/TOTAL_ASSETS[i]);
		if (n>0)
			WACF = (1.0/n)*sum;
		else
			WACF = 0.0;

		//Funding_costs
		//deposit + CB funding expenses (w/o dividends or CB interest income) divided by total assets, first per 3-month sum, then averaged		
		//1st term: interest expense flow for deposits
		//2nd term: CB_FUNDING_EXPENSE = int_to_ecb; only the interest payment flow by bank to CB
		add_double(&FUNDING_COSTS_EXPENSE_HISTORY, DEPOSIT_EXPENSE + CB_FUNDING_EXPENSE);
		remove_double(&FUNDING_COSTS_EXPENSE_HISTORY, 0);

		n=0;
        sum=0.0;
		sum_funding_costs_expense_history = 0.0;
		for (i=0; i<FUNDING_COSTS_EXPENSE_HISTORY.size; i++)
		{
			sum_funding_costs_expense_history += FUNDING_COSTS_EXPENSE_HISTORY.array[i];

			if (TOTAL_ASSETS_HISTORY.array[i]>EPSILON)	
			{
				sum += FUNDING_COSTS_EXPENSE_HISTORY.array[i]/TOTAL_ASSETS_HISTORY.array[i];
				n++;
			}
		}

		//Catch n==0
		assert__(n>0)
		{
			fprintf(stderr, "\n\tWARNING %s():%d: n=%d sum= %f\n", __FUNCTION__, __LINE__, n, sum);
			fprintf(stderr, "\n\t TOTAL_ASSETS_HISTORY.array = [");
			for (i=0; i<TOTAL_ASSETS_HISTORY.size; i++)
				fprintf(stderr, " %f ", TOTAL_ASSETS_HISTORY.array[i] );
			fprintf(stderr, "]");

			fprintf(stderr, "\n\t FUNDING_COSTS_EXPENSE_HISTORY.array = [");
			for (i=0; i<FUNDING_COSTS_EXPENSE_HISTORY.size; i++)
				fprintf(stderr, "%f ", FUNDING_COSTS_EXPENSE_HISTORY.array[i] );
			fprintf(stderr, "]");
		}

		//Eqn (32)
		if (n>0)
			FUNDING_COSTS = (1.0/n)*sum;
		else
			FUNDING_COSTS = 0.0;

		
		//Catch FUNDING_COSTS==0
		if(FUNDING_COSTS < EPSILON)
		{
		    #ifdef _DEBUG_MODE    
		    if (PRINT_BANK_INFO)
		    {		
	   		    fprintf(stderr, "\n\tWARNING %s():%d: FUNDING_COSTS %f n=%d sum= %f\nReason: sum_funding_costs_expense_history=%f\n", __FUNCTION__, __LINE__, FUNDING_COSTS, n, sum, sum_funding_costs_expense_history);

	   		    fprintf(stderr, "\n\t LOANS_HISTORY.array = [");
	   		    for (i=0; i<LOANS_HISTORY.size; i++)
	   		    	fprintf(stderr, "%f ", LOANS_HISTORY.array[i] );
	   		    fprintf(stderr, "]");				

	   		    fprintf(stderr, "\n\t FUNDING_COSTS_EXPENSE_HISTORY.array = [");
	   		    for (i=0; i<FUNDING_COSTS_EXPENSE_HISTORY.size; i++)
	   		    	fprintf(stderr, "%f ", FUNDING_COSTS_EXPENSE_HISTORY.array[i] );
	   		    fprintf(stderr, "]");				

	   		    fprintf(stderr, "\n\t TOTAL_ASSETS_HISTORY.array = [");
	   		    for (i=0; i<TOTAL_ASSETS_HISTORY.size; i++)
	   		    	fprintf(stderr, "%f ", TOTAL_ASSETS_HISTORY.array[i] );
	   		    fprintf(stderr, "]");
			}      
		    #endif
		}

		FUNDING_COSTS_RAW = FUNDING_COSTS;

		FUNDING_COSTS = max(0.0, FUNDING_COSTS);
	}

	// Cap funding costs
	if(FUNDING_COSTS > CONST_CAP_FUNDING_COSTS)
		FUNDING_COSTS = CONST_CAP_FUNDING_COSTS;

	SPREAD_FUNDING_COSTS_TO_ECB_INTEREST_RATE = FUNDING_COSTS - ECB_INTEREST_RATE;


    BANK_OUTFLOWS_CALENDAR.deposit_interest_payment += interest;
	
   return 0;
}


/** \fn Bank_rank_credit_requests()
 * \brief Function to rank_credit requests of firms according to a financial indicator.
 */
int Bank_rank_credit_requests()
{
	int firm_id;
    double e=0.0;
	double c=0.0;
	double d=0.0;
	double credit_requested=0.0;
	double loan_rea=0.0;
	double multiplicator = 0.0;
	double bankruptcy_prob = 0.0;
    int minsky_state=0;
    
    #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_CREDIT)
    {
		printf("\n\n Bank_rank_credit_requests bank_id: %d ",ID);
		printf("\n\n LOAN_REQUEST_MESSAGE_LOOP");
    }  
    #endif              

    START_LOAN_REQUEST_MESSAGE_LOOP

		if(GRANT_LOANS_DEFINITELY || (GRANT_LOANS_MAYBE && random_unif()<CONST_BANK_LOAN_GRANTING_PROB) )
		{

            firm_id = loan_request_message->firm_id;
            e = loan_request_message->equity;
            d = loan_request_message->total_debt;
            c = loan_request_message->external_financial_needs;
            credit_requested = c;
            minsky_state = loan_request_message->minsky_state;

            #ifdef _DEBUG_MODE        
            if (PRINT_CREDIT_INFO)
            {
            	printf("\nIT %d ID %d LOAN REQUEST MESSAGE RECEIVED:", DAY, ID);
            	printf("Firm %4d credit requested: %f", firm_id, credit_requested);
            }
            #endif

            if (e<0)
            	printf("\n Error ! The equity of the firm is negative: %f",e); 

			// 19.01.2017, SH: New-style PD, LGD computations using actual default rates of firm loans, FIRM_PD_PL_YEAR, FIRM_LGD_PL_YEAR)
            if (SWITCH_ENDOGENOUS_PD)
            {
            	Bank_set_risk_weight_firm_loans();

            	loan_rea = BASEL_RISK_WEIGHT_FIRM_LOANS_PL * credit_requested;
            }
            else
            {
				// 14.5.2013, SH: Old-style probability of default: based on the firm's debt/equity ratio incl. new loan: (d+c)/e
            	multiplicator = 0.10;
            	bankruptcy_prob = 1-exp(-multiplicator*(d+c)/e);
            	loan_rea = bankruptcy_prob*credit_requested;
            }


    		  //Bank adds message to be sorted/ranked on ascending loan_rea: low to high risk
            if(SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS==0 && SWITCH_CREDIT_RATIONING_PONZI_FIRMS==0)	
            {
                #ifdef _DEBUG_MODE
                    if (PRINT_DEBUG_CREDIT_RATIONING)
                        printf("\n\n Case 1: Firm %d SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS==%d SWITCH_CREDIT_RATIONING_PONZI_FIRMS==%d\n", firm_id, SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS, SWITCH_CREDIT_RATIONING_PONZI_FIRMS);
                #endif

            	add_loan_request_ranked_message(firm_id, ID,e,d,c,loan_rea, minsky_state);
            }
            if(SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS ==1)
            {
                #ifdef _DEBUG_MODE
                    if (PRINT_DEBUG_CREDIT_RATIONING)
                        printf("\n\n Case 2: Firm %d SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS==%d SWITCH_CREDIT_RATIONING_PONZI_FIRMS==%d\n", firm_id, SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS, SWITCH_CREDIT_RATIONING_PONZI_FIRMS);
                #endif

    		    //Unsound firms: Minsky_state = 3 or 4 (speculative or ponzi finance):
    		    //Credit rationing occurs automatically if (credit_allowed < 1e-6)
            	if(minsky_state==3 || minsky_state==4)
            		add_loan_request_ranked_message(firm_id, ID,e,d,1e-10,loan_rea, minsky_state);
            	else
            		add_loan_request_ranked_message(firm_id, ID,e,d,c,loan_rea, minsky_state);
            }		

            if(SWITCH_CREDIT_RATIONING_PONZI_FIRMS ==1)
            {
                #ifdef _DEBUG_MODE
                    if (PRINT_DEBUG_CREDIT_RATIONING)
                        printf("\n\n Case 3: Firm %d SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS==%d SWITCH_CREDIT_RATIONING_PONZI_FIRMS==%d\n", firm_id, SWITCH_CREDIT_RATIONING_UNSOUND_FIRMS, SWITCH_CREDIT_RATIONING_PONZI_FIRMS);
                #endif

    		    //Ponzi firms: Minsky_state = 4 (ponzi finance):
            	if(minsky_state==4)
            		add_loan_request_ranked_message(firm_id, ID,e,d,1e-10,loan_rea, minsky_state);
            	else
            		add_loan_request_ranked_message(firm_id, ID,e,d,c,loan_rea, minsky_state);
            }		


        	#ifdef _DEBUG_MODE
            if (PRINT_DEBUG_CREDIT)
            {
            	printf("\n\n\t firm_id: %d firm EQUITY=%f TOTAL_DEBT=%f bankruptcy_prob =%f loan_rea =%f", firm_id, e, d,bankruptcy_prob, loan_rea);
            }  
        	#endif
        	
		}//if(GRANT_LOANS_DEFINITELY || (GRANT_LOANS_MAYBE && random_unif()<CONST_BANK_LOAN_GRANTING_PROB) )

    FINISH_LOAN_REQUEST_MESSAGE_LOOP

    return 0;
}

/** \fn Bank_decide_credit_conditions()
 * \brief Function sets credit allowed for a firm.
 */
int Bank_decide_credit_conditions()
{
	int firm_id;
	
	double e,d;
	double credit_request=0.0;
	double credit_allowed=0.0;
	double loan_rea=0.0;
	double loan_llp=0.0;
	double interest=0.0;

	double f __attribute__((unused))  = 0.0;
	double pr __attribute__((unused)) = 0.0;

	int n;
	int max_loan_id;

    //Old rule, before 13.03.2020
    //double pd = 1-pow(1-FIRM_PD_PL_YEAR_MA,1/12.0);
	//HYPOTHETICAL_INTEREST_RATE_FIRM_LOANS = max(0.00001,12*((1 - pd*((1 - FIRM_WD_FACTOR_MA) - (FUNDING_COSTS+BANK_PROFIT_MARGIN)/12))/(1 - pd) - 1) + (FUNDING_COSTS+BANK_PROFIT_MARGIN));

    //New rule, 13.03.2020
    double pd = 1-pow(1-FIRM_PD_PL_YEAR_MA,1/12.0);  //monthly PD measure, based on annual PD MA measure
    double lgd = FIRM_WD_FACTOR_MA;                  //LGD measure, based on annual LGD MA measure

    HYPOTHETICAL_INTEREST_RATE_FIRM_LOANS = max(0.00001, (FUNDING_COSTS+BANK_PROFIT_MARGIN) + 12.0 * pd * lgd / (1-pd) );

	/* Regulatory Capital Adequacy Requirement:
	 * REGULATORY_CAPITAL_BUFFER*REA <= EQUITY <-> EQUITY - REGULATORY_CAPITAL_BUFFER*REA >= 0
	 * REGULATORY_CAPITAL_BUFFER is a percentage, consisting of 3 components:
	 * 1. Base regulation: Basel I: 2.5% Basel II: 4.5%, Basel III 8.5%
	 * 2. Counter-cyclical capital buffer (CCyB): 0 - 2.5% (top-up rights, discretionary by ECB)
	 * 3. Capital conservation buffer (CCoB): 0 - 2% (G-SIBs and SIFIs, discretionary by national authority)
	 */
	EXCESS_RISK_EXPOSURE = max(0.0, EQUITY - REGULATORY_CAPITAL_BUFFER*REA);
	EXCESS_LIQUIDITY = max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS);

	  #ifdef _DEBUG_MODE    
      if (PRINT_DEBUG_CREDIT)
	  {
		printf("\nIn Bank_decide_credit_conditions: EXCESS_RISK_EXPOSURE: REGULATORY_CAPITAL_BUFFER: %f", REGULATORY_CAPITAL_BUFFER);
	  }
	  #endif



	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT)
	{
		printf("\n\n Bank_decide_credit_conditions bank_id: %d ",ID);
		printf("\n\n LOAN_REQUEST_RANKED_MESSAGE_LOOP");
		printf("\n\t Bank status before loan offer: EXCESS_RISK_EXPOSURE %f EXCESS_LIQUIDITY %f", EXCESS_RISK_EXPOSURE,EXCESS_LIQUIDITY);

		if(EXCESS_RISK_EXPOSURE<0.0 || EXCESS_LIQUIDITY<0.0)
				printf("\n\t Bank negative values: EXCESS_RISK_EXPOSURE %f EXCESS_LIQUIDITY %f", EXCESS_RISK_EXPOSURE,EXCESS_LIQUIDITY);
	}
	#endif
	
	//Start of ranked msgs
	START_LOAN_REQUEST_RANKED_MESSAGE_LOOP
		firm_id 				= loan_request_ranked_message->firm_id;
		e 						= loan_request_ranked_message->equity;
		d 						= loan_request_ranked_message->total_debt;

		credit_request 			= loan_request_ranked_message->credit_request;
		loan_rea				= loan_request_ranked_message->loan_rea;

        #ifdef _DEBUG_MODE        
        if (PRINT_CREDIT_INFO)
        {
			printf("\nIT %d ID %d LOAN REQUEST RANKED MESSAGE RECEIVED:", DAY, ID);
            printf("Firm %4d credit request: %f loan rea %f", firm_id, credit_request, loan_rea);
        }
        #endif

		// Start rationing routines
		EXCESS_RISK_EXPOSURE = max(0.0, EQUITY - REGULATORY_CAPITAL_BUFFER*REA);
		EXCESS_LIQUIDITY = max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS);

		// Step 1. Set credit_allowed according to RISK_EXPOSURE constraint
		// EXCESS_RISK_EXPOSURE is the max. risk exposure amount still possible, before the bank would violate its regulatory capital buffer
		// Constrain credit offer to the "excess risk exposure" budget if this constraint is binding
		if ( loan_rea < EXCESS_RISK_EXPOSURE )
		{
			// Risk_exposure of requested loan is within available budget of bank
			credit_allowed = credit_request;
		}
		else 
		{
			// Risk_exposure of requested loan exceeds available budget of bank			
			// Credit_allowed is derived from REA constraint
			credit_allowed = credit_request;
			
			// Partial rationing: fill up to constraint
			// Note: 0<= (EXCESS_RISK_EXPOSURE/loan_rea) <= 1 due to (loan_rea > EXCESS_RISK_EXPOSURE)
			if(SWITCH_BANK_CAPITAL_FULL_RATIONING==0)
				credit_allowed = max(0.0, credit_request * EXCESS_RISK_EXPOSURE/loan_rea);  
				
			// Full rationing: No credit allowed
			if(SWITCH_BANK_CAPITAL_FULL_RATIONING==1)
				credit_allowed = 0.0;
		}
		
		// Step 2. Set credit_allowed according to LIQUIDITY constraint
		// EXCESS_LIQUIDITY is the liquidity still available, before the bank would violate its regulatory minimimal reserve requirement
		// Constrain credit offer to the "excess liquidity" budget if this constraint is binding
		if (EXCESS_LIQUIDITY > credit_allowed)
		{
			// Reduce liquidity budget by credit_allowed
			EXCESS_LIQUIDITY -= credit_allowed;					
		}
		else
		{
			credit_allowed = credit_request;
			
			// Partial rationing: fill up to liquidity constraint
			if(SWITCH_BANK_LIQUIDITY_FULL_RATIONING==0)
				credit_allowed = EXCESS_LIQUIDITY;
				
			// Full rationing: no credit allowed
			if(SWITCH_BANK_LIQUIDITY_FULL_RATIONING==1)
				credit_allowed = 0.0;
					
			EXCESS_LIQUIDITY -= credit_allowed;					
		}
		
		// Step 3. Set credit_allowed according to LOAN_LOSS_PROVISIONING constraint
		// EXCESS_LLP is the max. loan loss provisioning still possible, before the bank would violate its regulatory capital buffer
		// Constrain credit offer to the "excess llp" budget if this constraint is binding
		loan_llp = FIRM_PD_PL_YEAR_MA*FIRM_WD_FACTOR_MA * credit_request;

		if ( loan_llp < EXCESS_RISK_EXPOSURE )
		{
		    credit_allowed = credit_request;
		}
		else 
		{
		    // Loan loss provisioning of requested loan exceeds available LLP budget of bank            
		    // Credit_allowed is derived from MAX_LLR constraint
		    credit_allowed = credit_request;

		    // Partial rationing: fill up to constraint
		    // Note: expected loss of new loan should exactly equal the excess LLP still available
		        // EL = EXCESS_LLP -> PD*LGD*L= EXCESS_LLP -> L = EXCESS_LLP/PD*LGD
		    if(SWITCH_BANK_CAPITAL_FULL_RATIONING==0)
		        credit_allowed = max(0.0,  EXCESS_RISK_EXPOSURE/FIRM_PD_PL_YEAR_MA*FIRM_WD_FACTOR_MA);  

		    // Full rationing: No credit allowed
		    if(SWITCH_BANK_CAPITAL_FULL_RATIONING==1)
		        credit_allowed = 0.0;
		}

		// Set annual interest rate
		//Interest rate rule based on funding costs (see Mortgage market code, Bank_process_mortgage_requests() )
		f = FUNDING_COSTS;

		// Currently fixed margin
		pr = BANK_PROFIT_MARGIN;

        //Old rule, before 13.03.2020
		//interest =  max(0.00001,12*((1 - pd*((1 - FIRM_WD_FACTOR_MA) - (FUNDING_COSTS+pr)/12))/(1 - pd) - 1) + (FUNDING_COSTS+pr)); //interest in decimal form

        //New rule, 13.03.2020
        interest = max(0.00001, (f + pr) + 12.0 * pd * lgd / (1-pd) );

		if (pd > 1.0-EPSILON)
			printf("\n\n\t WARNING IT %d ID %d FIRM_LGD_PL_YEAR %f FIRM_PD_PL_YEAR %f close to 1: causes DIV/0", DAY, ID, FIRM_LGD_PL_YEAR, FIRM_PD_PL_YEAR);

		assert(pd<1.0-EPSILON);

        #ifdef _DEBUG_MODE        
        if (PRINT_DEBUG_FLOORS_AND_CEILINGS)
        {
            if (interest > CONST_CAP_FIRM_LOAN_INTEREST_RATE)
                fprintf(stderr, "\nIT %d ID %d WARNING: Firm loan interest %f exceeds upper bound %f, setting interest equal to cap %f", DAY, ID, interest, CONST_CAP_FIRM_LOAN_INTEREST_RATE, CONST_CAP_FIRM_LOAN_INTEREST_RATE);
        }              
        #endif
        
        // CAP ALWAYS ON:
        /*
        if (interest > CONST_CAP_FIRM_LOAN_INTEREST_RATE)
            interest =  CONST_CAP_FIRM_LOAN_INTEREST_RATE;
        */
        
        // Only make loan offers for positive credit request
		if (credit_allowed > 1e-6)
		{
			// Actual loan specific REA depends on final credit
			loan_rea = loan_rea*(credit_allowed/credit_request);
			
			// Reduce excess exposure units with risk-exposure of actual loan offer				
			EXCESS_RISK_EXPOSURE -= loan_rea;
			
			n = FIRM_LOAN_PORTFOLIO.size;
			max_loan_id = FIRM_LOAN_PORTFOLIO.array[n-1].loan_id + 1;

			// Send response to firm with credit conditions
			add_loan_conditions_message(firm_id, ID, max_loan_id, interest, credit_allowed);				
		}
		
		#ifdef _DEBUG_MODE
		if (PRINT_DEBUG_CREDIT)
		{
			printf("\n\n\t firm_id: %d firm EQUITY=%f TOTAL_DEBT=%f loan_rea =%f", firm_id, e, d,loan_rea);
			printf("\n\t firm_id: %d credit_requested: %f credit_allowed: %f", firm_id, credit_request, credit_allowed);
			printf("\n\t ECB_INTEREST_RATE: %f interest: %f",ECB_INTEREST_RATE,interest);
			printf("\n\t Bank status after loan offer: EXCESS_RISK_EXPOSURE %f EXCESS_LIQUIDITY %f", EXCESS_RISK_EXPOSURE,EXCESS_LIQUIDITY);
		}  
		#endif              
		
		assert(e<1e+12);
	FINISH_LOAN_REQUEST_RANKED_MESSAGE_LOOP
    
    return 0;
}


/** \fn Bank_issue_loans()
 * \brief Function to issue new loans.
 */
int Bank_issue_loans()
{
	int transfer_bank_id;

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_CREDIT)
    {
        printf("\n\n Bank_issue_loans ID: %d",ID); 
        printf("\n\t FIRM_CREDIT before: %f",FIRM_CREDIT);
    }        
    #endif
            
	double llp_pl_firm_loans=0.0;

    START_LOAN_ACCEPTANCE_MESSAGE_LOOP
        if(loan_acceptance_message->bank_id==ID)
        {
        	//Add firm loan to portfolio
            add_firm_loan_adt(&FIRM_LOAN_PORTFOLIO, loan_acceptance_message->firm_id, loan_acceptance_message->loan.bank_id, loan_acceptance_message->loan.loan_id, loan_acceptance_message->loan.outstanding_principal, loan_acceptance_message->loan.interest_rate, loan_acceptance_message->loan.installment_amount, loan_acceptance_message->loan.bad_debt, loan_acceptance_message->loan.nr_periods_before_repayment);

			//Stock accounting
			FIRM_CREDIT 	+= loan_acceptance_message->loan.outstanding_principal;
						
			//Add loan amount to temporary deposit account
			DEPOSITS += loan_acceptance_message->loan.outstanding_principal;

			log_inflow("bank", "loans", "deposits", "loan_issued", ID, loan_acceptance_message->loan.outstanding_principal);

			//If loan originating bank is not depositor bank:
			transfer_bank_id = loan_acceptance_message->transfer_bank_id;
			if(ID != transfer_bank_id)
			{
				//Transfer loan amount out of temp account
				DEPOSITS -= loan_acceptance_message->loan.outstanding_principal;

				//Decrease reserves of originating bank
				RESERVES -= loan_acceptance_message->loan.outstanding_principal;

				log_outflow("bank", "deposits", "reserves", "loan_issued", ID, loan_acceptance_message->loan.outstanding_principal);

				//Transfer reserves to depositor bank
				add_reserve_transfer_message(ID, transfer_bank_id, loan_acceptance_message->loan.outstanding_principal);
			}

            //Flow accounting 
           	BANK_NO_FIRM_LOANS_NEW++;

            BANK_FIRM_LOANS_NEW_BUSINESS	+= loan_acceptance_message->loan.outstanding_principal; 
            
            BANK_OUTFLOWS_CALENDAR.firm_loan_issues +=  loan_acceptance_message->loan.outstanding_principal; 

			SUM_INTEREST_RATES_NEW_FIRM_LOANS += loan_acceptance_message->loan.interest_rate;
			
			WEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS += loan_acceptance_message->loan.interest_rate * loan_acceptance_message->loan.outstanding_principal;

            WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS += ( loan_acceptance_message->loan.interest_rate - FUNDING_COSTS ) * loan_acceptance_message->loan.outstanding_principal;

            WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE += ( loan_acceptance_message->loan.interest_rate - ECB_INTEREST_RATE ) * loan_acceptance_message->loan.outstanding_principal;

            WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF += ( loan_acceptance_message->loan.interest_rate - WACF ) * loan_acceptance_message->loan.outstanding_principal;
            
	        #ifdef _DEBUG_MODE        
	        if (PRINT_CREDIT_INFO)
	        {
				printf("\nIT %d ID %d LOAN ACCEPTANCE MESSAGE RECEIVED:", DAY, ID);
	            printf("Firm %4d credit taken: %f interest_rate: %f", loan_acceptance_message->loan.firm_id, loan_acceptance_message->loan.outstanding_principal, loan_acceptance_message->loan.interest_rate);
	        }
	        #endif

            #ifdef _DEBUG_MODE
			if (PRINT_DEBUG_CREDIT) 
				printf("\n\t loan outstanding_principal: %f",loan_acceptance_message->loan.outstanding_principal);     
            #endif
        }
    FINISH_LOAN_ACCEPTANCE_MESSAGE_LOOP
    
   	//Mean interest rates for the new and for all firm loans; compute interest rate spreads
	compute_mean_interest_rates_all_firm_loans();

	//Save old LLR:
	double old_llr_firm_loans;
	old_llr_firm_loans = LLR_FIRM_LOANS;

	//Recompute LLR for firm loans, given the newly issued loans:
	Bank_update_LLR_firm_loans();
	
	llp_pl_firm_loans = LLR_FIRM_LOANS - old_llr_firm_loans;
    
    //Process flows
    PROFITS[0] 		 -= llp_pl_firm_loans;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_issue_loans()", DAY, ID);
		printf("\nFlows:");
		printf("\nNew loans\t\t%f\t(FIRM_CREDIT+,DEPOSITS+/RESERVES-)", BANK_FIRM_LOANS_NEW_BUSINESS);
		printf("\nLLP PL FIRM LOANS\t%f\t(LLR_FIRM_LOANS+,EQUITY-)", llp_pl_firm_loans);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
	enforce_bank_balanced_balance_sheet("Bank_issue_loans");

    return 0;
}


/** \fn Bank_receive_installment()
 * \brief Read installment msg: debt repayment and interest.
 * Also read bankruptcy msg of firms.
 * Compute daily, quarterly, yearly PD and LGD
 */
int Bank_receive_installment()
{
	int transfer_bank_id;
	
    FIRM_LOAN_INSTALLMENTS=0.0;
    FIRM_INTEREST_PAYMENTS=0.0;
    
    /*For validation rules*/
    if(DAY%MONTH == 1)
    {
    	FIRM_LOAN_INSTALLMENTS_IN_CALENDAR_MONTH=0.0;
    	FIRM_INTEREST_PAYMENTS_IN_CALENDAR_MONTH=0.0;
		TOTAL_BAD_DEBT_CALENDAR_MONTH=0.0;
	}

    #ifdef _DEBUG_MODE     
    if (PRINT_DEBUG_CREDIT) 
    {
        printf("\n\n IT %d ID: %d Bank_receive_installment ", DAY,ID);
        printf("\n\t PROFITS: %f FIRM_CREDIT: %f",PROFITS[0],FIRM_CREDIT);
    }
    #endif
        
    START_FIRM_INSTALLMENT_MESSAGE_LOOP
        if(firm_installment_message->bank_id==ID)
        {
        	BANK_NO_FIRM_LOANS++;

        	if (firm_installment_message->installment_amount > EPSILON)
				BANK_NO_FIRM_LOANS_PAYING++;

			PROFITS[0] 		+= firm_installment_message->interest_amount;
			EQUITY 			+= firm_installment_message->interest_amount;

            FIRM_LOAN_INCOME_CALENDAR_MONTH +=firm_installment_message->interest_amount;
            
			//Subtract installment amount from deposits: credit destruction
			FIRM_CREDIT  	-= firm_installment_message->installment_amount;

			//If loan originating bank is not the depositor bank of the borrower: need to transfer reserves from depositor bank into this bank
			transfer_bank_id = firm_installment_message->transfer_bank_id;
			if(ID != transfer_bank_id)
			{
				//Increase reserves of this bank
				RESERVES += firm_installment_message->installment_amount;
                RESERVES += firm_installment_message->interest_amount;

                log_transformation("bank", "loans", "reserves", "loan_installment", ID, firm_installment_message->installment_amount);
                log_inflow("bank", "equity", "reserves", "loan_interest", ID, firm_installment_message->interest_amount);
                
				//Transfer reserves to depositor bank
                add_reserve_transfer_message(ID, transfer_bank_id, -(firm_installment_message->installment_amount + firm_installment_message->interest_amount));
			}
            else
            {
                // Firm pays interest and installment out of deposit account at this bank
                DEPOSITS    -= firm_installment_message->interest_amount;
                DEPOSITS    -= firm_installment_message->installment_amount;

                log_outflow("bank", "loans", "deposits", "loan_installment", ID, firm_installment_message->installment_amount);
                log_transformation("bank", "deposits", "equity", "loan_interest", ID, firm_installment_message->interest_amount);
            }

            #ifdef _DEBUG_MODE        
            if (PRINT_CREDIT_INFO)
            {
				printf("\nIT %d ID %d FIRM INSTALLMENT MESSAGE RECEIVED:", DAY, ID);
                printf("Firm %4d principal %f interest %f", firm_installment_message->firm_id, firm_installment_message->installment_amount, firm_installment_message->interest_amount);
            }
            #endif

            //Flow accounting
            FIRM_INTEREST_PAYMENTS += firm_installment_message->interest_amount;
            FIRM_LOAN_INSTALLMENTS += firm_installment_message->installment_amount;
            FIRM_LOAN_INSTALLMENTS_IN_CALENDAR_MONTH += firm_installment_message->installment_amount;
			FIRM_INTEREST_PAYMENTS_IN_CALENDAR_MONTH += firm_installment_message->interest_amount;
			BANK_INFLOWS_CALENDAR.firm_interest_payments += firm_installment_message->interest_amount;
			BANK_INFLOWS_CALENDAR.firm_loan_installments += firm_installment_message->installment_amount;

			//Find correct loan_id in FIRM_LOAN_PORTFOLIO
			int found=0;
			int i=0;
			while(!found && i<FIRM_LOAN_PORTFOLIO.size)
			{
				if(FIRM_LOAN_PORTFOLIO.array[i].loan_id==firm_installment_message->loan_id && FIRM_LOAN_PORTFOLIO.array[i].firm_id==firm_installment_message->firm_id)
					found=1;
				else i++;
			}

			if(!found)
            {
				fprintf(stderr, "\n\nIT %d ID %d ERROR in Bank_receive_installment(), firm_installment_message: Loan not found! firm_id %d loan_id %d", DAY, ID, firm_installment_message->firm_id, firm_installment_message->loan_id);
                
                fprintf(stderr, "\n\n Bank ID %d FIRM_LOAN_PORTFOLIO", ID);
                fprintf(stderr, "\n\t bank_id\t firm_id\t\t loan_id\t principal\t installment\t periods");
                
                for(i=0; i<FIRM_LOAN_PORTFOLIO.size; i++)
                    fprintf(stderr, "\n\t%8d\t%8d\t%8d\t%f\t%f\t%d", FIRM_LOAN_PORTFOLIO.array[i].bank_id, FIRM_LOAN_PORTFOLIO.array[i].firm_id, FIRM_LOAN_PORTFOLIO.array[i].loan_id, FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal, FIRM_LOAN_PORTFOLIO.array[i].installment_amount, FIRM_LOAN_PORTFOLIO.array[i].nr_periods_before_repayment);
            }
			assert(found);

			//Update firm_loan_adt in FIRM_LOAN_PORTFOLIO
			FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal -= firm_installment_message->installment_amount;
			FIRM_LOAN_PORTFOLIO.array[i].nr_periods_before_repayment--;

			//Check for nr_periods_before_repayment==0, and if so, remove firm_loan_adt from FIRM_LOAN_PORTFOLIO
			if(FIRM_LOAN_PORTFOLIO.array[i].nr_periods_before_repayment==0)
				remove_firm_loan_adt(&FIRM_LOAN_PORTFOLIO, i);
        }
    FINISH_FIRM_INSTALLMENT_MESSAGE_LOOP
    
    //Check for FIRM_CREDIT == -0.000 AND RESET (full repayment of all loans)
    //if(FIRM_CREDIT<0 && fabs(FIRM_CREDIT)<1e-6)
	//	FIRM_CREDIT = 0.0;

	/* UPDATE LLR AFTER ALL FIRM PAYMENTS: EL, LLR */
	Bank_update_LLR_firm_loans();
	
    /* Trailing sum of last 60 days of daily income flows (array length 60)
     * Initialize on todays interest, add last 59 days
     * Array: update the array, and add new item to end
     */
	INTEREST_INCOME_FIRM_LOANS = FIRM_INTEREST_PAYMENTS; //today's interest income flow
	
	int i=0;
	for (i=0; i<INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.size-1; i++)
	{
		INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.array[i] = INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.array[i+1];
		INTEREST_INCOME_FIRM_LOANS += INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.array[i+1];
	}
	INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.array[INTEREST_INCOME_FIRM_LOANS_DAILY_HISTORY.size-1] = FIRM_INTEREST_PAYMENTS;

	//Annualise
	INTEREST_INCOME_FIRM_LOANS *= 4;


	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_receive_installment() after FIRM_INSTALLMENT_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\n Firm loans\t\t%f\t(FIRM_CREDIT-,DEPOSITS-)", FIRM_LOAN_INSTALLMENTS);
        printf("\n Interest\t\t%f\t(DEPOSITS-,EQUITY+)", FIRM_INTEREST_PAYMENTS);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif

	enforce_bank_balanced_balance_sheet("Bank_receive_installment -- after firm loan installments");

    double bad_debt_flow = 0.0;

    double sum_principal = 0;
	
    START_FIRM_BANKRUPTCY_MESSAGE_LOOP
    if(firm_bankruptcy_message->bank_id==ID)
    {
    	BANK_NO_FIRM_LOANS++;
		BANK_NO_FIRM_LOANS_WRITEOFF++;

		bad_debt_flow += firm_bankruptcy_message->bad_debt;
	    TOTAL_BAD_DEBT_CALENDAR_MONTH += firm_bankruptcy_message->bad_debt;

	    log_outflow("bank", "equity", "loans", "loan_writedown", ID, firm_bankruptcy_message->bad_debt);

        FIRM_LOAN_WRITEDOWN_CALENDAR_MONTH += firm_bankruptcy_message->bad_debt;

		//Find correct loan_id in FIRM_LOAN_PORTFOLIO
	    int found=0;
		int i=0;
		while(!found && i<FIRM_LOAN_PORTFOLIO.size)
		{
			if(FIRM_LOAN_PORTFOLIO.array[i].loan_id==firm_bankruptcy_message->loan_id && FIRM_LOAN_PORTFOLIO.array[i].firm_id==firm_bankruptcy_message->firm_id)				
				found=1;
			else i++;
		}
		
		if(!found)
            fprintf(stderr, "\n\nIT %d ID %d ERROR in Bank_receive_installment(), firm_bankruptcy_message: Loan_id not found! firm_id %d loan_id %d", DAY, ID, firm_bankruptcy_message->firm_id, firm_bankruptcy_message->loan_id);

	    assert(found);

	    sum_principal+=FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal;

		//Update firm_loan_adt in FIRM_LOAN_PORTFOLIO
		FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal -= firm_bankruptcy_message->bad_debt;
		FIRM_LOAN_PORTFOLIO.array[i].bad_debt += firm_bankruptcy_message->bad_debt;

		//Re-determine the installment:
		FIRM_LOAN_PORTFOLIO.array[i].installment_amount = FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal/FIRM_LOAN_PORTFOLIO.array[i].nr_periods_before_repayment;

        #ifdef _DEBUG_MODE        
        if (PRINT_CREDIT_INFO)
        {
            printf("\nIT %d ID %d FIRM BANKRUPTCY MESSAGE RECEIVED", DAY, ID);
            printf("Firm %4d bad_debt %f credit_refunded %f", firm_bankruptcy_message->firm_id, firm_bankruptcy_message->bad_debt, firm_bankruptcy_message->credit_refunded);
        }
        #endif
    }
    FINISH_FIRM_BANKRUPTCY_MESSAGE_LOOP   
	
	//Process flows
	asset_liability_subtract(&FIRM_CREDIT,&EQUITY, bad_debt_flow);
	
    //Check for FIRM_CREDIT == -0.000 AND RESET (full repayment of all loans)
    if(FIRM_CREDIT<0 && fabs(FIRM_CREDIT)<1e-6)
		FIRM_CREDIT = 0.0;

	add_double(&FIRM_BAD_DEBT_FLOW_HISTORY, bad_debt_flow);
	if(FIRM_BAD_DEBT_FLOW_HISTORY.size>240)
		remove_double(&FIRM_BAD_DEBT_FLOW_HISTORY, 0);

	add_double(&FIRM_WD_PRINCIPAL_HISTORY, sum_principal);
	if(FIRM_WD_PRINCIPAL_HISTORY.size>240)
		remove_double(&FIRM_WD_PRINCIPAL_HISTORY, 0);

	double sum_bad_debt=0;
	for (i=0;i<FIRM_BAD_DEBT_FLOW_HISTORY.size;i++)
		sum_bad_debt+=FIRM_BAD_DEBT_FLOW_HISTORY.array[i];

	double sum_wd_principal=0;
	for (i=0;i<FIRM_WD_PRINCIPAL_HISTORY.size;i++)
		sum_wd_principal+=FIRM_WD_PRINCIPAL_HISTORY.array[i];

	if (sum_wd_principal>0)
		FIRM_WD_FACTOR_MA = sum_bad_debt / sum_wd_principal;
	else
		FIRM_WD_FACTOR_MA = 0;

	/************************** PD and LGD for firm loans ***************/
	//RUN DAILY: at end of day
	Bank_set_pd_lgd_daily_firm_loans();

	//PD and LGD (monthly sums)
	if (DAY%MONTH==0)
		Bank_set_pd_lgd_quarterly_firm_loans();

	//RUN DAILY: at end of day
	//After updating the PD and LGD, re-compute FIRM_EL_PL_YEAR
	//Bank_update_LLR_firm_loans();  
	
	//For now assume all firm loans have the same PD
	FIRM_EL_PL_YEAR = FIRM_PD_PL_YEAR_MA * FIRM_WD_FACTOR_MA * FIRM_CREDIT;

	//Update LLR delta:
	double llp_pl_firm_loans = FIRM_EL_PL_YEAR - LLR_FIRM_LOANS;
	//LLR_FIRM_LOANS  += llp_pl_firm_loans;
	//EQUITY 			-= llp_pl_firm_loans;
	PROFITS[0] 			-= llp_pl_firm_loans;
	liability_transformation(&EQUITY, &LLR_FIRM_LOANS, llp_pl_firm_loans);
	if (LLR_FIRM_LOANS<0.0)
		liability_transformation(&EQUITY, &LLR_FIRM_LOANS, -LLR_FIRM_LOANS);

	LLR = LLR_FIRM_LOANS + LLR_PL_MORTGAGE_LOANS + LLR_NPL_MORTGAGE_LOANS;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_receive_installment(): After Firm BANKRUPTCY_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\n Bad debt flow\t\t%f\t(FIRM_CREDIT-,LLR_FIRM_LOANS-)", bad_debt_flow);
		printf("\n LLP Firm loans\t\t%f\t(LLR_FIRM_LOANS-,EQUITY+)", llp_pl_firm_loans);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
	enforce_bank_balanced_balance_sheet("Bank_receive_installment -- after firm banmkruptcy messages");

	//Regulatory PD and LGD (5-year MAs, but updated every month, on first day of month)
	if (DAY%MONTH==0)
		Bank_set_pd_lgd_reg_firm_loans();

	//RUN MONTHY: Update risk weights on first day of month
	if (DAY%MONTH==0)
		Bank_set_risk_weight_firm_loans();

	//Assume: all firms loans have same Basel risk weight
	REA_FIRM_LOANS = BASEL_RISK_WEIGHT_FIRM_LOANS_PL * FIRM_CREDIT;

	//Set REA
	REA = REA_FIRM_LOANS + REA_MORTGAGE_LOANS;
	
    return 0;
}

/** \fn Bank_receive_reserve_transfers()
 * \brief Read reserve transfer messages.
 */
int Bank_receive_reserve_transfers()
{
	double delta=0.0;

	START_RESERVE_TRANSFER_MESSAGE_LOOP
        if(ID==reserve_transfer_message->transfer_bank_id)
        {
			delta    += reserve_transfer_message->reserves_delta;
        }
	FINISH_RESERVE_TRANSFER_MESSAGE_LOOP

	asset_liability_add(&RESERVES, &DEPOSITS, delta);

	if (delta > 0)
		log_inflow("bank", "reserves", "deposits", "reserve_transfer", ID, delta);
	else
		log_outflow("bank", "reserves", "deposits", "reserve_transfer", ID, -delta);
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_receive_reserve_transfers()", DAY, ID);
		printf("\nFlows:");
		printf("\n Reserve transfers %2.2f (RESERVES+,DEPOSITS+)", delta);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)
	{
		printf("\n\n IT %d ID: %d Bank_receive_reserve_transfers",DAY,ID);
		printf("\n\t ECB_DEBT: %f DEPOSITS: %f EQUITY: %f",ECB_DEBT,DEPOSITS,EQUITY);
		printf("\n\t FIRM_CREDIT: %f RESERVES: %f",FIRM_CREDIT,RESERVES);
	}
	#endif   
	
	
	#ifdef _DEBUG_MODE       
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)
	{
		if(RESERVES/DEPOSITS < MIN_RESERVE_RATIO)
		{
			printf("\n\t FAILED Min. reserve req");
			printf("\n\n IT %d ID: %d Bank_receive_reserve_transfers: Checking loan/deposits ratio after update", DAY, ID);
			printf("\n\t RESERVES: %f DEPOSITS=%f , RESERVES/DEPOSITS=%f MIN_RESERVE_RATIO=%f", RESERVES, DEPOSITS, RESERVES/DEPOSITS, MIN_RESERVE_RATIO);
		}
	}
	#endif
	
	enforce_bank_balanced_balance_sheet("Bank_receive_reserve_transfers");

	return 0;
}

/** \fn Bank_account_update_deposits()
 * \brief Update the bank accounts of households and firms.
 */
int Bank_account_update_deposits()
{
	double mutation=0.0;
	double sum=0.0;    
    double eps=1e-3;
	
	START_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP 
	if (bank_account_update_message->bank_id==ID)
	{
		sum += bank_account_update_message->payment_account;

		// ABORT if improper payment account detected! (SERIOUS ERROR!!)
		if (bank_account_update_message->payment_account < -eps)
		{
			fprintf(stderr, "\nWARNING: bank received negative payment account! (iteration: %d, bank_id: %d, sender_id: %d, payment_account: %f)", DAY, ID, bank_account_update_message->id, bank_account_update_message->payment_account);
			assert(bank_account_update_message->payment_account > -eps);
		}

		if (bank_account_update_message->payment_account != bank_account_update_message->payment_account)
		{
			fprintf(stderr, "\nABORTED: bank received NaN payment account! (iteration: %d, bank_id: %d, sender_id: %d, payment_account: %f)", DAY, ID, bank_account_update_message->id, bank_account_update_message->payment_account);
            assert(bank_account_update_message->payment_account == bank_account_update_message->payment_account);
        }

		if (bank_account_update_message->payment_account == INFINITY || bank_account_update_message->payment_account == -INFINITY)
		{
			fprintf(stderr, "\nABORTED: bank received infinity payment account! (iteration: %d, bank_id: %d, sender_id: %d, payment_account: %f)", DAY, ID, bank_account_update_message->id, bank_account_update_message->payment_account);
            assert(bank_account_update_message->payment_account < INFINITY && bank_account_update_message->payment_account > -INFINITY);
        }
	}
	FINISH_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP

	//Store the deposit status before the intra-day deposit mutations are accounted for
	DEPOSITS_PREVIOUS = DEPOSITS;
	
	//Set current deposit status
	DEPOSITS = sum;

    //Intra-day deposit mutation:
	//This is a proxy for the payment settlement system due to withdrawals/deposits on the deposit accounts
	mutation = (DEPOSITS -  DEPOSITS_PREVIOUS);
	
    //Add intra-day deposit mutation (inflow or outflow) to reserves:
	RESERVES += mutation;

	if (mutation > 0)
		log_inflow("bank", "reserves", "deposits", "deposits_transfer", ID, mutation);
	else
		log_outflow("bank", "reserves", "deposits", "deposits_transfer", ID, -mutation);
	
	#ifdef  _DEBUG_MODE   
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_account_update_deposits()", DAY, ID);
		printf("\nFlows:");
		printf("\n Deposit mutations %2.2f (RESERVES+,DEPOSITS+)", mutation);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
	
	if(mutation >= 0)
	{
		BANK_INFLOWS_CALENDAR.deposit_inflow += mutation;
	}
	else
	{
		BANK_OUTFLOWS_CALENDAR.deposit_outflow += -mutation;
	}

	
    return 0;
}


/** \fn Bank_accounting()
 * \brief Function to compute the profits, set dividend payments, pay taxes.
 */
int Bank_accounting()
{

	TOTAL_CREDIT = PL_MORTGAGE_LOANS + NPL_MORTGAGE_LOANS + FIRM_CREDIT;

	NO_HOUSES_FOR_SALE = SEIZED_COLLATERAL.size;
	
	//Set bounds on inputs of the bank dividend rate rule:
	double x,l,u;
	
	x = (EQUITY/TOTAL_ASSETS);
	l = CONST_EQUITY_TOTAL_ASSET_RATIO_LBND; //0.0
	u = CONST_EQUITY_TOTAL_ASSET_RATIO_UBND; //0.2
	double equity_total_asset_ratio = min(max(l, x), u);

	x = (REA/TOTAL_CREDIT);
	l = CONST_REA_TOTAL_CREDIT_RATIO_LBND; //0.0
	u = CONST_REA_TOTAL_CREDIT_RATIO_UBND; //1.0
	double rea_total_credit_ratio = min(max(l, x), u);

	//ECB specs Eqn (29)
	BANK_DIVIDEND_RATE = 0.0;

    // No dividends if inactive OR during first 3000 its
    if (ACTIVE)
		BANK_DIVIDEND_RATE = min(max(0.0, BANK_EXPENSE_COEFFS.a_div + BANK_EXPENSE_COEFFS.b_div * equity_total_asset_ratio + BANK_EXPENSE_COEFFS.c_div * pow(equity_total_asset_ratio, 2) + BANK_EXPENSE_COEFFS.d_div * rea_total_credit_ratio ), 1.0);

	// Cap dividend rate (for testing purposes)
	if (BANK_DIVIDEND_RATE > CONST_CAP_BANK_DIVIDEND_RATE)
		BANK_DIVIDEND_RATE = CONST_CAP_BANK_DIVIDEND_RATE;
	
	TAXES = 0.0;
	TOTAL_DIVIDEND_PAYMENT = 0.0;

    // Tax and dividend payment
    // If-condition of Eq (30): PROFITS[0] > 0.0
    if (PROFITS[0] > 0.0)
    {
         TAXES = TAX_RATE_CORPORATE*PROFITS[0];

         PROFITS[0] -= TAXES;

         EQUITY -= TAXES;  
         RESERVES -= TAXES;

         log_outflow("bank", "equity", "reserves", "taxes", ID, TAXES);
	 
         add_tax_payment_message(GOV_ID, TAXES);

         TAX_EXPENSE_CALENDAR_MONTH += TAXES;

         BANK_OUTFLOWS_CALENDAR.tax_payment += TAXES;
	 
		//21.8.2017: ECB specs Eqn (30), BANK_DIVIDEND_RATE comes from data estimation (annual horizon), and is converted to a monthly horizon by dividing by 12.
		TOTAL_DIVIDEND_PAYMENT = min(max(0.0, (1/12.0)*BANK_DIVIDEND_RATE*EQUITY), PROFITS[0]);

		//Check for BEHAVIORAL equity requirement violation (was: regulatory equity requirement)
		if (EQUITY - TOTAL_DIVIDEND_PAYMENT < BEHAVIORAL_EQUITY)
		{
			BANK_DIVIDEND_RATE = 0.0;
			TOTAL_DIVIDEND_PAYMENT = 0.0;
		}


        DIVIDEND_PER_SHARE = TOTAL_DIVIDEND_PAYMENT/CURRENT_SHARES_OUTSTANDING; 
         
         //The dividend msg is send in the function Bank_send_dividend_payment (see below), monthly, 1st of month      
         //add_dividend_per_share_message(ID, DIVIDEND_PER_SHARE);                  
    }

    //Growth rate of profits

    if (PROFITS[1] > 0.0)
		PROFIT_GROWTH_RATE_MONTHLY = 100 * (PROFITS[0]/PROFITS[1] - 1);
	else
		PROFIT_GROWTH_RATE_MONTHLY = 0.0;

	//Delay current month's profit and overwrite profit value of last month
	PROFITS[1] = PROFITS[0];
	PROFITS_FLOW = PROFITS[0];

      #ifdef _DEBUG_MODE    
      if (PRINT_BANK_INFO)
	  {
		  printf("\n\nIT %d ID: %d Bank_accounting()", DAY, ID);
		  printf("\nFlows:");
		  printf("\n Taxes %2.2f (RESERVES-,EQUITY-)", TAXES);
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  printf("\n");
		  
		  print_bank_balance_sheet();
	  }
	  #endif
	  
      
      //27.3.2017 SvdH
	//Bank checks solvency condition. THIS is the default place where we are checking at end of the calendar month.
    //See Mortgage_Market/Bank_Functions.c, Bank_read_mortgage_messages() for other place.
	Bank_check_equity_requirement();

      #ifdef _DEBUG_MODE    
      if (PRINT_DEBUG_CREDIT)
	  {
		printf("\nIn Bank_accounting: REGULATORY_CAPITAL_BUFFER: %f CONST_BEHAVIORAL_CAPITAL_BUFFER %f", REGULATORY_CAPITAL_BUFFER, CONST_BEHAVIORAL_CAPITAL_BUFFER);
	  }
	  #endif


    EARNINGS_CALENDAR_MONTH = MORTGAGE_EXTRA_PROFIT_CALENDAR_MONTH + MORTGAGE_INCOME_CALENDAR_MONTH + FIRM_LOAN_INCOME_CALENDAR_MONTH + CB_INTEREST_INCOME_CALENDAR_MONTH -
    MORTGAGE_WRITEOFF_CALENDAR_MONTH - DEPOSIT_EXPENSE_CALENDAR_MONTH - DIVIDEND_EXPENSE_CALENDAR_MONTH - CB_INTEREST_EXPENSE_CALENDAR_MONTH - FIRM_LOAN_WRITEDOWN_CALENDAR_MONTH - TAX_EXPENSE_CALENDAR_MONTH;

    RELATIVE_EARNINGS_CALENDAR_MONTH = EARNINGS_CALENDAR_MONTH / EQUITY;
	
	return 0;
}


int Bank_idle()
{
    return 0;
}


/** \fn Bank_update_ecb_account()
 * \brief Increase or reduce the bank's ECB debt, add bank_to_central_bank_account_update message for overnight deposits.
 */
int Bank_update_ecb_account()
{   
	double a,b;
	
	REGULATORY_RESERVES = MIN_RESERVE_RATIO*DEPOSITS;

	EXCESS_RESERVES = max(0.0, RESERVES - REGULATORY_RESERVES);

	// Procedure to add ECB debt daily
	EXCESS_LIQUIDITY = RESERVES - REGULATORY_RESERVES;
	
	if (EXCESS_LIQUIDITY<0.0)
	{
		log_inflow("bank", "ecb_debt", "reserves", "new_cb_debt", ID, fabs(EXCESS_LIQUIDITY));

		//Monetary base is increased  
		ECB_DEBT  	+=  fabs(EXCESS_LIQUIDITY);
		RESERVES	+=  fabs(EXCESS_LIQUIDITY);
		BANK_INFLOWS_CALENDAR.new_ecb_debt += fabs(EXCESS_LIQUIDITY);
		EXCESS_LIQUIDITY = 0.0;

		
	}
	// Procedure to reduce ECB debt
	else if ( (ECB_DEBT>0.0)&& (EXCESS_LIQUIDITY>0.0) ) 
	{
		#ifdef _DEBUG_MODE       
		if (PRINT_DEBUG_CREDIT)
		{
			fprintf(stderr, "\n\t Before: RESERVES=%f ECB_DEBT=%f EXCESS_LIQUIDITY %f", RESERVES, ECB_DEBT, EXCESS_LIQUIDITY);
		}
		#endif
		
		//Case 1: Sufficient excess liquidity to fully repay ECB debt
		//a: fraction of ECB debt that is repaid
		a=1.0;
		if(EXCESS_LIQUIDITY > ECB_DEBT)
		{
			log_outflow("bank", "ecb_debt", "reserves", "repaid_cb_debt", ID, a*ECB_DEBT);

			RESERVES -= a*ECB_DEBT;
			ECB_DEBT -= a*ECB_DEBT;
			BANK_OUTFLOWS_CALENDAR.debt_installment_to_ecb += a*ECB_DEBT;
			
			//EXCESS_LIQUIDITY = RESERVES - MIN_RESERVE_RATIO*DEPOSITS;			
			EXCESS_LIQUIDITY -= a*ECB_DEBT;

			//If a==1.0, then ECB_DEBT=0.0 should now hold
			assert__(ECB_DEBT<EPSILON)
			{
				fprintf(stderr, "\n\t In %s:%s(), line %d: ECB_DEBT=%f", __FILE__, __FUNCTION__, __LINE__, ECB_DEBT);
			}

			#ifdef _DEBUG_MODE       
			if (PRINT_DEBUG_CREDIT)
			{
				fprintf(stderr, "\n\t Using reserves to fully pay off ECB_DEBT (EXCESS_LIQUIDITY>=ECB_DEBT):");
				fprintf(stderr, "\n\t After: RESERVES=%f ECB_DEBT=%f EXCESS_LIQUIDITY=%f", RESERVES,ECB_DEBT,EXCESS_LIQUIDITY);
			}
			#endif
		}
		
		//Case 2: Insufficient excess liquidity to fully repay: repay partly, deplete the excess liquidity
		//b: fraction of EXCESS_LIQUIDITY that is used to repay ECB debt, (1-b) remains for giving loans next day.
		b=1.0;
		if(EXCESS_LIQUIDITY<ECB_DEBT)
		{
			log_outflow("bank", "ecb_debt", "reserves", "repaid_cb_debt", ID, b*EXCESS_LIQUIDITY);

			RESERVES 	-= b*EXCESS_LIQUIDITY;
			ECB_DEBT	-= b*EXCESS_LIQUIDITY;
			BANK_OUTFLOWS_CALENDAR.debt_installment_to_ecb += b*EXCESS_LIQUIDITY;

			//EXCESS_LIQUIDITY = RESERVES - MIN_RESERVE_RATIO*DEPOSITS;
			EXCESS_LIQUIDITY -= b*EXCESS_LIQUIDITY;

			//If b==1.0, then EXCESS_LIQUIDITY=0.0 should now hold
			assert__(EXCESS_LIQUIDITY<EPSILON)
			{
				fprintf(stderr, "\n\t In %s:%s(), line %d: EXCESS_LIQUIDITY=%f", __FILE__, __FUNCTION__, __LINE__, EXCESS_LIQUIDITY);
			}

			#ifdef _DEBUG_MODE       
			if (PRINT_DEBUG_CREDIT)
			{
				fprintf(stderr, "\n\t Using reserves to partially pay off ECB_DEBT (EXCESS_LIQUIDITY<ECB_DEBT):");
				fprintf(stderr, "\n\t After: RESERVES=%f ECB_DEBT=%f EXCESS_LIQUIDITY=%f", RESERVES,ECB_DEBT,EXCESS_LIQUIDITY);
			}
			#endif
		}
	}
	//End of procedure
	
	
	//Inform ECB about bank's payment account
	add_bank_to_central_bank_account_update_message(ID, RESERVES, ECB_DEBT);	
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_update_ecb_account()", DAY, ID);
		printf("\nFlows:");
		if (EXCESS_LIQUIDITY<0.0) printf("\n Excess liquidity %2.2f (RESERVES+,ECB_DEBT+)", EXCESS_LIQUIDITY); //Obtain additional ECB_DEBT and RESERVES
		if (EXCESS_LIQUIDITY>0.0) printf("\n Excess liquidity %2.2f (RESERVES-,ECB_DEBT-)", EXCESS_LIQUIDITY); //Use reserves to fully or partially pay off ECB_DEBT
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
    return 0;
}

/** \fn Bank_recapitalization_request()
 * \brief Function to let Banks send recapitalization requests.
 */
int Bank_recapitalization_request()
{
	//Attempt bank recapitalization during initial transition phase
	if (ACTIVE==0 && DAY<TRANSITION_PHASE_RECAP)
	{
		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_INFO)
		{
			printf("\n\n IT %d Bank_read_mortgage_writeoffs: Bank ID %d sends bank_recapitalization_request msg", DAY, ID);
		}      
		#endif
		
		//CONST_BANK_RECAP_FACTOR = 2
		add_bank_recapitalization_request_message(ID, CONST_BANK_RECAP_FACTOR * fabs(EQUITY-REGULATORY_EQUITY)); 
	}

	return 0;   
}

/*
 * \fn Bank_check_recapitalization()
 * \brief Function to read the recapitalization message from ECB (if any)
 * \brief Content: read recapitalization msg from ECB, and add funds received to the reserves and equity.
 * \brief currentState:	check_recapitalization
 * \brief nextState:	mortgage_00
 * \brief Conditon: only runs during the initial transient phase: if (DAY < TRANSITION_PHASE_RECAP)
 * \brief UNITTEST: PASS 2.11.2016
 */
int Bank_check_recapitalization()
{
  if (DAY < TRANSITION_PHASE_RECAP)
  {	
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_BANKRUPTCY)
		{
			printf("\n\n Bank_check_recapitalization: Check function condition: DAY=%d, TRANSITION_PHASE_RECAP=%d, (DAY< TRANSITION_PHASE_RECAP)=%d\n", DAY, TRANSITION_PHASE_RECAP, (DAY< TRANSITION_PHASE_RECAP));
			printf("\n\t IT %d ID %d Bank status before recapitalization messages", DAY, ID);
			printf("\n\t EQUITY: %2.3f RESERVES: %2.3f\n", EQUITY, RESERVES);
		}
		#endif                    

		START_BANK_RECAPITALIZATION_MESSAGE_LOOP	
		if(ID==bank_recapitalization_message->bank_id)
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_BANKRUPTCY)
			{
				printf("\n\t RECEIVED bank_recapitalization message:");
				printf("\n\t funds: %2.3f", bank_recapitalization_message->funds);
			}
			#endif

			//Recapitalize by adding funds to asset and liabilities
			RESERVES += bank_recapitalization_message->funds;
			EQUITY += bank_recapitalization_message->funds;

			//Set active flag to 1 if recapitalization was successful
			//Bank checks solvency condition after recapitalization. This is the 3rd place where we are checking.
			//See above for 2nd check in Bank_set_risk_exposure_amounts()
			//See Credit/Bank_Credit_Functions.c, Bank_accounting() for default check at end of month.
			Bank_check_equity_requirement();
		}
		FINISH_BANK_RECAPITALIZATION_MESSAGE_LOOP	

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_BANKRUPTCY)
		{
			printf("\n\t Bank status after ALL recapitalization messages:");
			printf("\n\t EQUITY: %2.3f RESERVES: %2.3f\n", EQUITY, RESERVES);
		}
		#endif
		
		
		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_INFO)
		{
			printf("\n\nBank_check_recapitalization(): After BANK_RECAPITALIZATION_MESSAGE_LOOP");
			print_bank_balance_sheet();
		}
		#endif
		
    } //DAY< TRANSITION_PHASE_RECAP	
	
    return 0;
}


/** \fn Bank_send_data_to_Eurostat()
 * \brief Function to send data to Eurostat.
 */
int Bank_send_data_to_Eurostat()
{
	// Declare, init financial_data
	financial_data_adt financial_data;

    financial_data.active = ACTIVE;
    
	financial_data.equity		= EQUITY;
	financial_data.total_assets	= TOTAL_ASSETS;
	financial_data.total_debt	= ECB_DEBT;
    financial_data.llr    = LLR;

    financial_data.risk_weighted_capital_ratio = RISK_WEIGHTED_CAPITAL_RATIO;
    
	financial_data.equity_asset_ratio = EQUITY_ASSET_RATIO;
	financial_data.debt_equity_ratio = DEBT_EQUITY_RATIO;
	financial_data.rea = REA;

	financial_data.total_mortgage_loans = TOTAL_MORTGAGE_CREDIT;

	financial_data.pl_mortgage_loans = PL_MORTGAGE_LOANS;
	financial_data.npl_mortgage_loans = NPL_MORTGAGE_LOANS;

	financial_data.pl_mortgage_loans_op		= PL_MORTGAGE_LOANS_OP;
	financial_data.npl_mortgage_loans_op	= NPL_MORTGAGE_LOANS_OP;
	financial_data.pl_mortgage_loans_hmr	= PL_MORTGAGE_LOANS_HMR;
	financial_data.npl_mortgage_loans_hmr	= NPL_MORTGAGE_LOANS_HMR;

	financial_data.total_firm_loans = FIRM_CREDIT;
	financial_data.pl_firm_loans = FIRM_CREDIT;
	financial_data.npl_firm_loans = TOTAL_BAD_DEBT_CALENDAR_MONTH;

	financial_data.bank_no_mortgages = MORTGAGE_PORTFOLIO.size;
	financial_data.bank_no_mortgages_new	= BANK_NO_MORTGAGES_NEW;
	financial_data.bank_mortgages_new_business	= BANK_MORTGAGES_NEW_BUSINESS;
	
	financial_data.weighted_sum_interest_rates_new_mortgages = WEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES;				//Note: this is sum i_j*L_j (not the weighted mean: sum i_j*L_j/sum L_j)
	financial_data.weighted_sum_interest_rates_all_mortgages = WEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES;				//Note: this is sum i_j*L_j (not the weighted mean: sum i_j*L_j/sum L_j)

    financial_data.weighted_sum_interest_rate_spreads_new_mortgages_funding_costs = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS; 
    financial_data.weighted_sum_interest_rate_spreads_new_mortgages_policy_rate = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE; 
    financial_data.weighted_sum_interest_rate_spreads_new_mortgages_wacf = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF; 

	financial_data.bank_mortgages_paying		= BANK_MORTGAGES_PAYING;
	financial_data.bank_mortgages_arrears		= BANK_MORTGAGES_ARREARS;
	financial_data.bank_mortgages_npl			= BANK_MORTGAGES_NPL;
	financial_data.bank_mortgages_writeoff	= BANK_MORTGAGES_WRITEOFF;
	financial_data.bank_mortgages_paid_off	= BANK_MORTGAGES_PAID_OFF;
	financial_data.bank_mortgages_requested   = BANK_MORTGAGES_REQUESTED;

	financial_data.bank_no_firm_loans 			= BANK_NO_FIRM_LOANS;
	financial_data.bank_no_firm_loans_new 		= BANK_NO_FIRM_LOANS_NEW;
	financial_data.bank_firm_loans_new_business	= BANK_FIRM_LOANS_NEW_BUSINESS;
    
    financial_data.weighted_sum_interest_rates_new_firm_loans = WEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS;               //Note: this is sum i_j*L_j (not the weighted mean: sum i_j*L_j/sum L_j)
	financial_data.weighted_sum_interest_rates_all_firm_loans = WEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS;               //Note: this is sum i_j*L_j (not the weighted mean: sum i_j*L_j/sum L_j)

    financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_funding_costs = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS; 
    financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_policy_rate = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE; 
    financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_wacf = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF; 

	financial_data.bank_no_firm_loans_paying		= BANK_NO_FIRM_LOANS_PAYING;
	financial_data.bank_no_firm_loans_writeoff	= BANK_NO_FIRM_LOANS_WRITEOFF;
	financial_data.wacf 							= WACF;
	financial_data.funding_costs					= FUNDING_COSTS;
	financial_data.deposits 						= DEPOSITS;
    financial_data.deposit_interest_rate            = DEPOSIT_INTEREST_RATE;

	// Send financial_data
	add_bank_send_data_message(ID, financial_data);

	#ifdef _DEBUG_MODE       
  	if (PRINT_BANK_MORTGAGE_INFO && DAY >= CONST_INIT_TRANSIENT_PERIOD)
  	{
  		printf("\nIT %d ID %d Bank_send_data_to_Eurostat: financial_data.bank_no_mortgages %d", DAY, ID, financial_data.bank_no_mortgages);
  	}
	#endif

	free_financial_data_adt(&financial_data);

	return 0;
}

/** \fn Bank_stocks_and_flows()
 * \brief Updating stocks and flows
 */
int Bank_stocks_and_flows()
{   
	int i;
	double total_outstanding_mortgage_principal = 0.0;

	BANK_STOCKS_CALENDAR.deposits = DEPOSITS;
	
	BANK_STOCKS_CALENDAR.ecb_debt = ECB_DEBT;

	if(EQUITY>EPSILON) LLR_EQUITY_RATIO = (LLR_FIRM_LOANS + LLR_PL_MORTGAGE_LOANS + LLR_NPL_MORTGAGE_LOANS)/EQUITY;
	else LLR_EQUITY_RATIO = 0.0;

	if(REA>0.1) RISK_WEIGHTED_CAPITAL_RATIO = EQUITY/REA;
	else RISK_WEIGHTED_CAPITAL_RATIO = 0.0;

	if (TOTAL_CREDIT>0.0)
		EQUITY_CREDIT_RATIO = EQUITY / TOTAL_CREDIT;
	else
		EQUITY_CREDIT_RATIO = 0.0;

	//Total mortgage loans: outstanding principle (interest not included)
	for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
		total_outstanding_mortgage_principal  += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

	//For positive base money, total deposits need to exceed total outstanding loans
	DEPOSITS_MINUS_LOANS = DEPOSITS - (FIRM_CREDIT + total_outstanding_mortgage_principal);
	
	DEPOSITS_TO_LOANS_RATIO = 0.0;
	if((FIRM_CREDIT + total_outstanding_mortgage_principal)>EPSILON)
		DEPOSITS_TO_LOANS_RATIO = DEPOSITS / (FIRM_CREDIT + total_outstanding_mortgage_principal);

	BANK_STOCKS_CALENDAR.equity=EQUITY;
	
	BANK_STOCKS_CALENDAR.total_liabilities = DEPOSITS 
												+ ECB_DEBT 
												+ LLR_FIRM_LOANS 
												+ LLR_PL_MORTGAGE_LOANS 
												+ LLR_NPL_MORTGAGE_LOANS 
												+ CAPITAL_ADJ_PL_MORTGAGE_LOANS
												+ CAPITAL_ADJ_NPL_MORTGAGE_LOANS	
												+ CAPITAL_ADJ_PL_FIRM_LOANS 
												+ CAPITAL_ADJ_NPL_FIRM_LOANS 
												+ EQUITY;

	BANK_STOCKS_CALENDAR.reserves= RESERVES;

	BANK_STOCKS_CALENDAR.regulatory_reserves= REGULATORY_RESERVES;

	BANK_STOCKS_CALENDAR.firm_credit = FIRM_CREDIT;

	BANK_STOCKS_CALENDAR.total_mortgages = PL_MORTGAGE_LOANS 
										  + NPL_MORTGAGE_LOANS;
	

	BANK_STOCKS_CALENDAR.total_assets = RESERVES 
										+ FIRM_CREDIT 
										+ PL_MORTGAGE_LOANS 
										+ NPL_MORTGAGE_LOANS;

	BANK_STOCKS_CALENDAR.current_shares_outstanding = CURRENT_SHARES_OUTSTANDING;

	BANK_STOCKS_CALENDAR.cash_day_20 = RESERVES;

   	//For plotting:
	BANK_INFLOWS_MONTHLY_MORTGAGE_PAYMENTS = BANK_INFLOWS_CALENDAR.mortgage_payments;
	
	BANK_OUTFLOWS_MONTHLY_MORTGAGE_ISSUES = BANK_OUTFLOWS_CALENDAR.mortgage_loan_issues;

	NO_MORTGAGES_OUTSTANDING	= MORTGAGE_PORTFOLIO.size;

	BANK_OUTFLOWS_CALENDAR.total_expenses =	
	BANK_OUTFLOWS_CALENDAR.firm_loan_issues +
	BANK_OUTFLOWS_CALENDAR.debt_installment_to_ecb+
	BANK_OUTFLOWS_CALENDAR.cb_net_interest_payment +
	BANK_OUTFLOWS_CALENDAR.deposit_interest_payment +
	BANK_OUTFLOWS_CALENDAR.dividend_payment +
	BANK_OUTFLOWS_CALENDAR.deposit_outflow +
	BANK_OUTFLOWS_CALENDAR.tax_payment +
	BANK_OUTFLOWS_CALENDAR.mortgage_loan_issues;
	
	BANK_INFLOWS_CALENDAR.total_income=
	BANK_INFLOWS_CALENDAR.firm_loan_installments +
	BANK_INFLOWS_CALENDAR.new_ecb_debt +
	BANK_INFLOWS_CALENDAR.deposit_inflow +
	BANK_INFLOWS_CALENDAR.firm_interest_payments +
	BANK_INFLOWS_CALENDAR.mortgage_payments;
	
	BANK_INFLOWS_CALENDAR.net_inflow =
	BANK_INFLOWS_CALENDAR.total_income -
	BANK_OUTFLOWS_CALENDAR.total_expenses;
	
	BANK_INFLOWS_CALENDAR.net_deposit_inflow =
	BANK_INFLOWS_CALENDAR.deposit_inflow -
	BANK_OUTFLOWS_CALENDAR.deposit_outflow;
	
	//Bank leverage ratio
	if (DEPOSITS>EPSILON)
	{
		LEVERAGE_RATIO = FIRM_CREDIT/DEPOSITS;
		RESERVE_DEPOSIT_RATIO  = (RESERVES)/DEPOSITS;
	}

	TOTAL_ASSETS = BANK_STOCKS_CALENDAR.total_assets;

	if(TOTAL_ASSETS > EPSILON)
		EQUITY_ASSET_RATIO = EQUITY/TOTAL_ASSETS;
	else
		EQUITY_ASSET_RATIO=0.0;

	if(TOTAL_CREDIT > EPSILON)
		REA_TOTAL_CREDIT_RATIO = REA/TOTAL_CREDIT;
	else
		REA_TOTAL_CREDIT_RATIO=0.0;

	//Update excess risk exposure and excess liquidity (previously only used in the loan conditions)
	//Now these values could have changed at end of the month
	EXCESS_RISK_EXPOSURE = max(0.0, EQUITY - REGULATORY_CAPITAL_BUFFER*REA);
	EXCESS_LIQUIDITY = max(0.0, RESERVES - MIN_RESERVE_RATIO*DEPOSITS);
	
	  #ifdef _DEBUG_MODE    
      if (PRINT_DEBUG_CREDIT)
	  {
		printf("\nIn Bank_stocks_and_flows EXCESS_RISK_EXPOSURE: REGULATORY_CAPITAL_BUFFER: %f", REGULATORY_CAPITAL_BUFFER);
	  }
	  #endif


	// liquidity_coverage_ratio
	if ( BANK_OUTFLOWS_CALENDAR.deposit_outflow > EPSILON )
		LIQUIDITY_COVERAGE_RATIO = BANK_STOCKS_CALENDAR.cash_day_20 / BANK_OUTFLOWS_CALENDAR.deposit_outflow;
	else LIQUIDITY_COVERAGE_RATIO = 0.0;


	double seized = 0.0;
	int z;
	for(z=0; z<SEIZED_COLLATERAL.size; z++)
		seized += SEIZED_COLLATERAL.array[z].seized_house.mortgage.outstanding_principal;

	log_stock("bank", "mortgages_defaulted", ID, seized);
	log_stock("bank", "mortgages", ID, TOTAL_MORTGAGE_CREDIT-seized);
	log_stock("bank", "loans", ID, FIRM_CREDIT);
	log_stock("bank", "reserves", ID, RESERVES);
	log_stock("bank", "deposits", ID, DEPOSITS);
	log_stock("bank", "ecb_debt", ID, ECB_DEBT);
	log_stock("bank", "equity", ID, EQUITY + LLR);

    return 0;
}
