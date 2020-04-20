#include <math.h>
#include "../header.h"     
#include "../CentralBank_agent_header.h"
#include "../my_library_header.h"
#include "../Sanity_Checks/sanity_checks_aux_headers.h"

/************Central_Bank Role: manage accounts for Governments and commercial banks********************************/
/** \fn Central_Bank_reset_variables()
 * \brief 
 * \brief currentState:	start_Central_Bank
 * \brief nextState:	CB_reset
 */
int Central_Bank_reset_variables()
{
	//Monthly resetting of variables:
	ECB_STOCKS_CALENDAR.equity_day_1 = ECB_STOCKS_CALENDAR.equity;
	
	ECB_OUTFLOWS_CALENDAR.gov_bond_purchase = 0.0;
	ECB_OUTFLOWS_CALENDAR.bank_fiat_money = 0.0;
	ECB_OUTFLOWS_CALENDAR.bank_deposits = 0.0;
	ECB_OUTFLOWS_CALENDAR.gov_deposits = 0.0;
	ECB_OUTFLOWS_CALENDAR.dividend_payment = 0.0;
	ECB_OUTFLOWS_CALENDAR.total_expenses = 0.0;
	
	ECB_INFLOWS_CALENDAR.bank_interest = 0.0;
	ECB_INFLOWS_CALENDAR.bank_debt_installment = 0.0;
	ECB_INFLOWS_CALENDAR.gov_interest = 0.0;
	ECB_INFLOWS_CALENDAR.bank_deposits = 0.0;
	ECB_INFLOWS_CALENDAR.gov_deposits = 0.0;
	ECB_INFLOWS_CALENDAR.total_income = 0.0;
	ECB_INFLOWS_CALENDAR.net_inflow = 0.0;

	return 0;
}    
    

/** \fn Central_Bank_read_bank_recapitalization_requests()
 * \brief Function to implement recapitalization of bankrupt bank
 * \brief Runs every period, but only during initial transient period, in order to make the burn-in phase easier.
 * \brief currentState:	CB_financial_operations_end
 * \brief nextState:	CB_recap_00
 * \brief UNITTEST: TESTED OK ON: 2.11.2016
 */
int Central_Bank_read_bank_recapitalization_requests()
{
	int bank_id;
	double funds;
	double total_funds=0.0;

  //Only run this function during the initial transient phase
  // NOTE: there exist 2 different constants for transient periods:
  //       INIT_TRANSIENT_PERIOD = CONST_INIT_TRANSIENT_PERIOD: used for initialization of mortgages, set to 6002
  //       TRANSITION_PHASE_RECAP: used for recapitalization, set to 3000
  if (DAY < TRANSITION_PHASE_RECAP)
  {	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_BANKRUPTCY)
	{
		printf("\n\n Central_Bank_read_recapitalization_requests: Check if function condition is valid: DAY=%d, INIT_TRANSIENT_PERIOD=%d, (DAY< INIT_TRANSIENT_PERIOD)=%d\n", DAY, INIT_TRANSIENT_PERIOD, (DAY< INIT_TRANSIENT_PERIOD));
		printf("\n\t DURING transition period: IT %d ID %d\n", DAY, ID);
	}
	#endif                    
	
	total_funds = 0.0;
	
	START_BANK_RECAPITALIZATION_REQUEST_MESSAGE_LOOP
		bank_id 	= bank_recapitalization_request_message->bank_id;
		funds 		= bank_recapitalization_request_message->funds;
		
		total_funds += funds;
		
		add_bank_recapitalization_message(bank_id, funds);

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_BANKRUPTCY)
		{
			printf("\n\t CB RECEIVED bank_recapitalization_request mesg:");
			printf("\n\t data read: %2.3f",  bank_recapitalization_request_message->funds);
			printf("\n\t funds send: %2.3f\n", funds);
		}
		#endif
	FINISH_BANK_RECAPITALIZATION_REQUEST_MESSAGE_LOOP
		
	//Accounting: add total_funds to fiat_money
	FIAT_MONEY_BANKS += total_funds;
	FIAT_MONEY += total_funds;

	ECB_STOCKS_CALENDAR.perpetuities_banks += total_funds;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_BANKRUPTCY)
	{
		if(total_funds>0.0)
		{
			printf("\n\t CB: total new funds created for Banks: %2.3f", total_funds);
		}
	}
	#endif

    } //DAY < TRANSITION_PHASE_RECAP	
    
    return 0;
}

/** \fn Central_Bank_read_firm_recapitalization_requests()
 * \brief Function to implement recapitalization of bankrupt firms
 * \brief Runs every period, but only during initial transient period, in order to make the burn-in phase easier.
 * \brief currentState:	CB_recap_00
 * \brief nextState:	Central_Bank_Start_End_of_Month_Loop
 * \brief UNITTEST: TESTED OK ON: 2.11.2016
 */
int Central_Bank_read_firm_recapitalization_requests()
{
	int bank_id, firm_id;
	double funds;
	double total_funds=0.0;

  //Only run this function during the initial transient phase
  // NOTE: there exist 2 different constants for transient periods:
  //       INIT_TRANSIENT_PERIOD = CONST_INIT_TRANSIENT_PERIOD: used for initialization of mortgages, set to 6002
  //       TRANSITION_PHASE_RECAP: used for firm recapitalization, set to 3000
  if (DAY < TRANSITION_PHASE_RECAP)
  {	
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_BANKRUPTCY)
		{
			printf("\n\n Central_Bank_read_recapitalization_requests: Check if function condition is valid: DAY=%d, INIT_TRANSIENT_PERIOD=%d, (DAY< INIT_TRANSIENT_PERIOD)=%d\n", DAY, INIT_TRANSIENT_PERIOD, (DAY< INIT_TRANSIENT_PERIOD));
			printf("\n\t DURING transition period: IT %d ID %d\n", DAY, ID);
		}
		#endif                    

		//Read the firm_recapitalization requests from bankrupt firms, assume ECB is 100% accomodating using fiat money
		START_FIRM_RECAPITALIZATION_REQUEST_MESSAGE_LOOP
			firm_id 	= firm_recapitalization_request_message->firm_id;
			bank_id 	= firm_recapitalization_request_message->bank_id;
			funds 		= firm_recapitalization_request_message->funds;

			total_funds += funds;

			add_firm_recapitalization_message(firm_id, bank_id, funds);

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_BANKRUPTCY)
			{
				printf("\n\t CB RECEIVED firm_recapitalization_request mesg:");
				printf("\n\t data read: %2.3f", firm_recapitalization_request_message->funds);
				printf("\n\t funds send: %2.3f\n", funds);
			}
			#endif
		FINISH_FIRM_RECAPITALIZATION_REQUEST_MESSAGE_LOOP

		//Accounting: add total_funds to fiat_money
		FIAT_MONEY_FIRMS += total_funds;
		FIAT_MONEY += total_funds;

		ECB_STOCKS_CALENDAR.perpetuities_firms += total_funds;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_BANKRUPTCY)
		{
			if(total_funds>0.0)
			{
				printf("\n\t CB: total new funds created for Firms: %2.3f", total_funds);
			}
		}
		#endif
	

    } //DAY < TRANSITION_PHASE_RECAP	
    
    return 0;
}

/** \fn Central_Bank_read_fiat_money_requests()
 * \brief Function to read fiat money requests from banks and Governments.
 * \brief Runs only on *last* day of month
 * \brief currentState:	
 * \brief nextState:	
 */
int Central_Bank_read_fiat_money_requests()
{
    //Monetization of debt
    //Read the fiat money request from governments, assume ECB is 100% accomodating
    START_REQUEST_FIAT_MONEY_MESSAGE_LOOP
       FIAT_MONEY_GOVS += request_fiat_money_message->nominal_value;
       FIAT_MONEY      += request_fiat_money_message->nominal_value;
       ECB_STOCKS_CALENDAR.perpetuities_govs += request_fiat_money_message->nominal_value;

       if (request_fiat_money_message->nominal_value >0.0)
       		log_inflow("central_bank", "credit", "deposits", "new_gov_debt", ID, request_fiat_money_message->nominal_value);
       else
       		log_outflow("central_bank", "credit", "deposits", "repaid_gov_debt", ID, -request_fiat_money_message->nominal_value);

    FINISH_REQUEST_FIAT_MONEY_MESSAGE_LOOP

    return 0;
}


/** \fn Central_Bank_read_bank_account_update()
 * \brief Frequency: daily
 */
int Central_Bank_read_bank_account_update()
{
	int i;
	double a,b;
	double temp_deposit = 0;
	double interest = 0.0;

    #ifdef _DEBUG_MODE
        int bank_mesg_count=0;
    #endif
    
    ECB_DEPOSITS=0.0;
	BANK_RESERVES=0.0;

    //Recomputing fiat_money_banks, adding it later to fiat_money
    FIAT_MONEY -= FIAT_MONEY_BANKS;
    FIAT_MONEY_BANKS = 0.0;

    //record previous value
    a = ECB_STOCKS_CALENDAR.fiat_money_banks;
    ECB_STOCKS_CALENDAR.fiat_money_banks =0.0;

    //Recomputing fiat_money_banks from messages
    START_BANK_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
        ECB_DEPOSITS += bank_to_central_bank_account_update_message->payment_account;
        
		//3.10.2016, Flow accounting
		FIAT_MONEY_BANKS += bank_to_central_bank_account_update_message->ecb_debt;
		FIAT_MONEY +=  bank_to_central_bank_account_update_message->ecb_debt;
        
		ECB_STOCKS_CALENDAR.fiat_money_banks +=  bank_to_central_bank_account_update_message->ecb_debt;

        //Search the correct account and update the value    
        for (i=0;i<ACCOUNTS_BANKS.size;i++)
        {       
            if(ACCOUNTS_BANKS.array[i].id == bank_to_central_bank_account_update_message->id)
            {
            	temp_deposit = ACCOUNTS_BANKS.array[i].payment_account;
            
                ACCOUNTS_BANKS.array[i].payment_account = 
                bank_to_central_bank_account_update_message->payment_account;

                BANK_RESERVES += ACCOUNTS_BANKS.array[i].payment_account;
                
                if(temp_deposit >= ACCOUNTS_BANKS.array[i].payment_account)
                {
                 	ECB_OUTFLOWS_CALENDAR.bank_deposits +=
                 	(temp_deposit - ACCOUNTS_BANKS.array[i].payment_account);
                 }
                 else
                 {
                 	ECB_INFLOWS_CALENDAR.bank_deposits +=
                 	(ACCOUNTS_BANKS.array[i].payment_account - temp_deposit);
                 
                 }
            }
        }
        
        #ifdef _DEBUG_MODE
        	if(PRINT_DEBUG_CB)
            	bank_mesg_count++;        
        #endif
    FINISH_BANK_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP

    //Check difference between new and old ECB debt to banks: either a net outflow (new loans) or inflow (debt installment)
	b = ECB_STOCKS_CALENDAR.fiat_money_banks - a;
	if(b>0.0)
	{
	    ECB_OUTFLOWS_CALENDAR.bank_fiat_money = b;
	    ECB_INFLOWS_CALENDAR.bank_debt_installment = 0.0;

	    log_inflow("central_bank", "credit", "deposits", "new_bank_debt", ID, b);	    
	}
	else
	{
	    ECB_OUTFLOWS_CALENDAR.bank_fiat_money = 0.0;
	    ECB_INFLOWS_CALENDAR.bank_debt_installment = b;

	    log_outflow("central_bank", "credit", "deposits", "repaid_bank_debt", ID, -b);	    
	}

    //Interest from bank to ECB: net between interest on ECB debt and interest on Bank reserves held at ECB
    //bank_interest_amount = int_to_ecb - int_from_ecb
    interest = 0.0;
    START_BANK_INTEREST_PAYMENT_MESSAGE_LOOP
        interest += bank_interest_payment_message->bank_interest_amount;

        if (bank_interest_payment_message->bank_interest_amount>0.0)
        	log_transformation("central_bank", "deposits", "equity", "cb_interest_income", ID, bank_interest_payment_message->bank_interest_amount);
       	else
       		log_transformation("central_bank", "equity", "deposits", "cb_interest_expense", ID, -bank_interest_payment_message->bank_interest_amount);

    FINISH_BANK_INTEREST_PAYMENT_MESSAGE_LOOP

	EQUITY += interest;

    ECB_INFLOWS_CALENDAR.bank_interest += interest;

    #ifdef _DEBUG_MODE
        if(PRINT_DEBUG_CB)
        {                        
            if(bank_mesg_count!=ACCOUNTS_BANKS.size)
            {    
                printf("\nERROR in %s/%s(), line %d: nr of mesg not equal to size of bank account array.\n", __FILE__, __FUNCTION__, __LINE__);
                printf("\n Nr of Bank_mesg=%d, size of bank account array=%d\n", bank_mesg_count, ACCOUNTS_BANKS.size);
            }    
            else
                printf("\n Nr of mesg equal to size of bank account array.\n");
        }
    #endif
      

	return 0;
}

   
/** \fn Central_Bank_send_dividend()
 * \brief In case of positive ECB equity, the ECB makes a profit, and 
 * \brief should distribute this evenly among governments
 */
int Central_Bank_send_dividend()
{
	double dividend_per_gov=0.0;
	int no_govs;

	if(EQUITY>0.0)
	{
		no_govs = TOTAL_REGIONS/NO_REGIONS_PER_GOV;

		//Step 1 : ECB pays out all EQUITY as dividends
		dividend_per_gov = EQUITY/no_govs;

		//Step 2 : ECB adds dividends to Governments' payment accounts
/*
		for (j=0; j<ACCOUNTS_GOVS.size; j++)	
			ACCOUNTS_GOVS.array[j].payment_account += dividend_per_gov;
*/
		//Step 3 : ECB sends ecb_dividend_payment_msg, Governments adjust payment accounts
		add_ecb_dividend_message(dividend_per_gov);

		log_transformation("central_bank", "equity", "deposits", "cb_dividends", ID, dividend_per_gov);

        #ifdef _DEBUG_MODE        
        if(PRINT_DEBUG_CB)
            printf("\nIT %d ID %d ecb_dividend=%f\n", DAY, ID, dividend_per_gov);
        #endif

		//Recompute stocks
		ECB_OUTFLOWS_CALENDAR.dividend_payment = EQUITY;
		
		EQUITY = 0.0;
	}

	return 0;
}

/** \fn Central_Bank_read_gov_account_update()
 * \brief Frequency: daily
 */
int Central_Bank_read_gov_account_update()
{
	int i;
	double temp_deposit;

    #ifdef _DEBUG_MODE
        int gov_mesg_count=0;
    #endif

    ECB_STOCKS_CALENDAR.payment_account_govs = 0.0;
	
	GOV_DEPOSITS = 0.0;

    START_GOV_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
        for (i=0;i<ACCOUNTS_GOVS.size;i++)
        {       
            //Search the correct account and update the value
            if(ACCOUNTS_GOVS.array[i].id == gov_to_central_bank_account_update_message->id)
            {
            	temp_deposit = ACCOUNTS_GOVS.array[i].payment_account;
                               
                ACCOUNTS_GOVS.array[i].payment_account = 
                gov_to_central_bank_account_update_message->payment_account;
                                
                if(temp_deposit >= ACCOUNTS_GOVS.array[i].payment_account)
                {
                 	ECB_OUTFLOWS_CALENDAR.gov_deposits +=
                 	(temp_deposit - ACCOUNTS_GOVS.array[i].payment_account);
                 }
                 else
				{
                 	ECB_INFLOWS_CALENDAR.gov_deposits +=
                 	(ACCOUNTS_GOVS.array[i].payment_account - temp_deposit);
                }
                
            }
    		
    		//Total Gov deposits at ECB
    		ECB_STOCKS_CALENDAR.payment_account_govs += gov_to_central_bank_account_update_message->payment_account;

            //Total deposits at ECB
            ECB_DEPOSITS += gov_to_central_bank_account_update_message->payment_account;
            GOV_DEPOSITS += gov_to_central_bank_account_update_message->payment_account;
        }
        
        #ifdef _DEBUG_MODE        
        if(PRINT_DEBUG_CB)
            gov_mesg_count++;    
        #endif
        
    FINISH_GOV_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP

    #ifdef _DEBUG_MODE
        if(PRINT_DEBUG_CB)
        {                        
            if(gov_mesg_count!=ACCOUNTS_GOVS.size)
            {
                printf("\nERROR in %s/%s(), line %d: nr of mesg not equal to size of gov account array.\n", __FILE__, __FUNCTION__, __LINE__);
                printf("\n Nr of Gov_mesg=%d, size of gov account array=%d\n", gov_mesg_count, ACCOUNTS_GOVS.size);
            }
            else
                printf("\n Nr of mesg equal to size of gov account array.\n");
        }
    #endif      

	return 0;
}

/** \fn Central_Bank_accounting()
 * \brief Frequency: daily
 */
int Central_Bank_accounting()
{
	int j;

   //Start of month: reset stock-flow accounting by setting the total flows equal to the flows of day 1
    if(DAY%MONTH == 1)
    {
		ECB_OUTFLOWS_CALENDAR.total_expenses = 
		ECB_OUTFLOWS_CALENDAR.gov_bond_purchase +
		
		ECB_OUTFLOWS_CALENDAR.bank_fiat_money +
		ECB_OUTFLOWS_CALENDAR.bank_deposits +
		ECB_OUTFLOWS_CALENDAR.gov_deposits +
		ECB_OUTFLOWS_CALENDAR.dividend_payment;

		ECB_INFLOWS_CALENDAR.total_income = 
		ECB_INFLOWS_CALENDAR.bank_interest +
		ECB_INFLOWS_CALENDAR.bank_debt_installment +
		ECB_INFLOWS_CALENDAR.gov_interest +
		ECB_INFLOWS_CALENDAR.bank_deposits +
		ECB_INFLOWS_CALENDAR.gov_deposits;
	}

   //End of month: cumulative stock-flow accounting per day
    if(DAY%MONTH == 0)
    {
		ECB_OUTFLOWS_CALENDAR.total_expenses += 
		ECB_OUTFLOWS_CALENDAR.gov_bond_purchase +
		
		ECB_OUTFLOWS_CALENDAR.bank_fiat_money +
		ECB_OUTFLOWS_CALENDAR.bank_deposits +
		ECB_OUTFLOWS_CALENDAR.gov_deposits +
		ECB_OUTFLOWS_CALENDAR.dividend_payment;
		
		ECB_INFLOWS_CALENDAR.total_income += 
		ECB_INFLOWS_CALENDAR.bank_interest +
		ECB_INFLOWS_CALENDAR.bank_debt_installment +
		ECB_INFLOWS_CALENDAR.gov_interest +
		ECB_INFLOWS_CALENDAR.bank_deposits +
		ECB_INFLOWS_CALENDAR.gov_deposits;
		
		
		ECB_INFLOWS_CALENDAR.net_inflow =
		ECB_INFLOWS_CALENDAR.total_income -
		ECB_OUTFLOWS_CALENDAR.total_expenses; 
	}//end of month loop

		//Bonds
/*
		for(j = 0; j < ECB_BOND_HOLDINGS.size; j++)
		{
		    units += ECB_BOND_HOLDINGS.array[j].quantity;
		    value += ECB_BOND_HOLDINGS.array[j].quantity*ECB_BOND_HOLDINGS.array[j].face_value;
		}
		ECB_STOCKS_CALENDAR.nr_gov_bonds = units;
		ECB_STOCKS_CALENDAR.gov_bond_holdings =value;
*/
		ECB_STOCKS_CALENDAR.nr_gov_bonds = 0;
		ECB_STOCKS_CALENDAR.gov_bond_holdings = 0.0;
		
		ECB_STOCKS_CALENDAR.total_assets =
		ECB_STOCKS_CALENDAR.fiat_money_banks +
		ECB_STOCKS_CALENDAR.gov_bond_holdings;

		//Recomputing accounts
		ECB_STOCKS_CALENDAR.payment_account_banks=0.0;
		ECB_STOCKS_CALENDAR.payment_account_govs=0.0;

		for (j=0;j<ACCOUNTS_BANKS.size;j++)
		{       
			ECB_STOCKS_CALENDAR.payment_account_banks += 
			ACCOUNTS_BANKS.array[j].payment_account;
		}
	
		for (j=0;j<ACCOUNTS_GOVS.size;j++)
		{       
			ECB_STOCKS_CALENDAR.payment_account_govs += 
			ACCOUNTS_GOVS.array[j].payment_account;
		}

		//fiat money
		ECB_STOCKS_CALENDAR.fiat_money = FIAT_MONEY;

		//Outstanding perpetuities to banks/govs	
		ECB_STOCKS_CALENDAR.fiat_money_banks = FIAT_MONEY_BANKS;
		ECB_STOCKS_CALENDAR.fiat_money_govs = FIAT_MONEY_GOVS;

		//ECB ASSETS
		ECB_STOCKS_CALENDAR.total_assets =
		ECB_STOCKS_CALENDAR.perpetuities_firms + 
		ECB_STOCKS_CALENDAR.perpetuities_banks +
		ECB_STOCKS_CALENDAR.perpetuities_govs;
		
		//LIABILITIES
		ECB_STOCKS_CALENDAR.total_liabilities = 
		ECB_STOCKS_CALENDAR.payment_account_govs +
		ECB_STOCKS_CALENDAR.payment_account_banks +
		ECB_STOCKS_CALENDAR.fiat_money;

		//EQUITY
		ECB_STOCKS_CALENDAR.equity = 
		ECB_STOCKS_CALENDAR.total_assets - ECB_STOCKS_CALENDAR.total_liabilities;

		//SANITY CHECK
		CentralBank_sanity_checks();


	log_stock("central_bank", "credit", ID, FIAT_MONEY);
	log_stock("central_bank", "deposits", ID, ECB_DEPOSITS);
	log_stock("central_bank", "equity", ID, EQUITY);

	return 0;
}
