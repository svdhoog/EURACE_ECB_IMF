#include "../../header.h"
#include "../../Bank_agent_header.h"
#include "../../my_library_header.h"
#include "../Credit_Risk_Management/Bank_aux_headers.h"
#include "../../Statistical_Office/balance_sheet_operations.h"
#include "../../Sanity_Checks/sanity_checks_aux_headers.h"
#include "../../Credit/Bank_credit_aux_headers.h"

/* GLOBAL VARIABLES */
FILE *file1;
char *filename;



/***************** TEMPLATE ****************/
/** \fn Bank_TEMPLATE()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
int Bank_TEMPLATE()
{
  	//Message input:

	//Message output:

    #ifdef _DEBUG_MODE    
    if (PRINT_BANK_MORTGAGE_INFO)
    {
        printf("\n\n Bank_ ID: %d", ID);
    }      
    #endif
 
 return 0;   
}

/***************** END OF TEMPLATE ****************/

/** \fn Bank_set_pd_lgd_mortgage_loans()
 * \brief Function to compute at monthly frequency the PD and LGD
 * \brief See auxiliary functions:
 * \brief - Bank_set_pd_lgd_monthly_household_loans
 * \brief - Bank_set_pd_lgd_quarterly_household_loans();
 * \brief - Bank_set_pd_lgd_yearly_household_loans
 * \Brief Frequency: monthly, on DAY 2
 * \Brief After Function:
 * \Brief Called in Function:
 * \Brief Before Function: 
 * \brief Message input: 
 * \brief - mortgage_arrears_message(ID, bank_id, loan_loss, loan_default, arrears_counter);
 * \brief - mortgage_npl_message(ID, bank_id, loan_loss, loan_default, npl_counter);
 * \brief - mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default);*
 * \brief Message output: 
 * \brief - none
 * \brief UNITTEST: 
 */
int Bank_set_pd_lgd_mortgage_loans()
{

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d Bank_set_pd_lgd_mortgage_loans", DAY, ID);		
	}
	#endif

	//RUN YEARLY: only on day 2 of year
	//if (DAY%(MONTH*12)==2)
	//RUN QUARTERLY: only on day 2of quarter
	//if (DAY%(MONTH*3)==2)
	//RUN MONTHLY: only on day 2 of MONTH
	//if (DAY%MONTH==2)


	//Runs monthly: at end of DAY 2
	//LL and LD: Loan Loss and Loan Default flows (daily flows)
	Bank_set_pd_lgd_monthly_household_loans();

	//PD and LGD (monthly sums)
	if (DAY%MONTH==2)
		Bank_set_pd_lgd_quarterly_household_loans();
	
	//Regulatory PD and LGD (5-year MAs, but updated every month)
	if (DAY%MONTH==2)
		Bank_set_pd_lgd_reg_household_loans();

	return 0;   
}


/** \fn Bank_receive_mortgage_payments()
 * \brief Function to
 * \brief Message input: 
 * \brief - add_mortgage_payment_message(ID, bank_id, monthly_principal, monthly_interest)
 * \brief Message output: None
 * \brief UNITTEST: PASS 15.1.2017
 */
int Bank_receive_mortgage_payments()
{
	int id=0;
	int paid_off=0;
	int object_id=0;
	double sum_interest_flow=0.0;
	double sum_principal_flow=0.0;
	

	//XML: Filter on bank_id
	//XML: Sort on key household_id
	int total_msgs=0;
	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		total_msgs++;
		id = mortgage_payment_message->household_id;
		object_id = mortgage_payment_message->mortgage.object_id;

		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d MORTGAGE PAYMENT MESSAGE RECEIVED: CUSTOMER: %4d OBJECT: %4d", DAY, ID, id, object_id);		
		}
		#endif
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_read_mortgage_payments(): Before MORTGAGE_PAYMENT_MESSAGE_LOOP", DAY, ID);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");

		printf("=== BEFORE Bank_receive_mortgage_payments ===\n");
		
		print_bank_balance_sheet();
	}
	#endif
	double sum_principal_stock=0.0;
	int count=0;
	START_MORTGAGE_PAYMENT_MESSAGE_LOOP

		assert(mortgage_payment_message->mortgage.active);

		BANK_MORTGAGES_PAYING++;

		count++;
		id = mortgage_payment_message->household_id;
		object_id = mortgage_payment_message->mortgage.object_id;

	  	//Accounting
	  	BANK_INFLOWS_CALENDAR.mortgage_payments += mortgage_payment_message->mortgage.monthly_principal + mortgage_payment_message->mortgage.monthly_interest;

		//NEW STYLE: add mortgage from mesg to portfolio
	  	//Check for full repayment: only if not yet repaid, add mortgage to portfolio
	  	paid_off=0;
		if (mortgage_payment_message->mortgage.current_duration==0)
		{
			paid_off = 1;
			BANK_MORTGAGES_PAID_OFF++;
		}

		sum_principal_flow += mortgage_payment_message->mortgage.monthly_principal;
		sum_interest_flow += mortgage_payment_message->mortgage.monthly_interest;

		log_outflow("bank", "deposits", "mortgages", "mortgage_principal", ID, mortgage_payment_message->mortgage.monthly_principal);
		log_transformation("bank", "deposits", "equity", "mortgage_interest", ID, mortgage_payment_message->mortgage.monthly_interest);

		MORTGAGE_INCOME_CALENDAR_MONTH += mortgage_payment_message->mortgage.monthly_interest;

		sum_principal_stock += mortgage_payment_message->mortgage.outstanding_principal;

		// check consistency
		#ifdef _DEBUG_MODE 
		int x=0;
		int found=0;
		for(x=0;x<MORTGAGE_PORTFOLIO.size;x++)
		{
			if(MORTGAGE_PORTFOLIO.array[x].object_id==object_id)
			{
				found=1;
				break;
			}
		}

		if(!found)
			printf("BUG: received mortgage payment, but cannot find mortgage (ITS: %d bank ID: %d, object_id: %d\n", DAY, ID, object_id);
		else
		{
			if(fabs(MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_payment_message->mortgage.monthly_principal-mortgage_payment_message->mortgage.outstanding_principal)>1e-3)
			{
				printf("BUG: household reported inconsistent values in mortgage payment (ITS: %d bank ID: %d, object_id: %d)\n", DAY, ID, object_id);
				printf("\tprincipal in books: %f\n", MORTGAGE_PORTFOLIO.array[x].outstanding_principal);
				printf("\tprincipal payment: %f\n", mortgage_payment_message->mortgage.monthly_principal);
				printf("\tself-reported new principal: %f\n ",mortgage_payment_message->mortgage.outstanding_principal);
				printf("\tdiff: %f (%.2f%%)\n", MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_payment_message->mortgage.monthly_principal-mortgage_payment_message->mortgage.outstanding_principal, fabs(MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_payment_message->mortgage.monthly_principal-mortgage_payment_message->mortgage.outstanding_principal)/mortgage_payment_message->mortgage.outstanding_principal*100);
			}
		}
		#endif
		

	  	#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_TABLES)
		{
			if(count==1)
			{
				printf("\nIT %d ID: %d Bank_receive_mortgage_payments", DAY, ID);
                printf("\n\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
                printf("\n\tNo\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 
            }

			printf("\n\t%2d/%2d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
				count,
				total_msgs,
				mortgage_payment_message->household_id,
				mortgage_payment_message->mortgage.object_id,
				mortgage_payment_message->mortgage.monthly_principal,
				mortgage_payment_message->mortgage.monthly_interest,
				mortgage_payment_message->mortgage.outstanding_principal,
				mortgage_payment_message->mortgage.outstanding_interest,
				mortgage_payment_message->mortgage.monthly_total_repayment,
				mortgage_payment_message->mortgage.initial_duration,
				mortgage_payment_message->mortgage.current_duration,
				mortgage_payment_message->mortgage.status,			
				mortgage_payment_message->mortgage.arrears_counter,
				mortgage_payment_message->mortgage.npl_counter,
				paid_off);
		}      
	  	#endif
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP


	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_TABLES)
		if(count>0) printf("\n");
	#endif
		
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE PAYMENT MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	//Process the flows
	asset_liability_subtract(&PL_MORTGAGE_LOANS, &DEPOSITS, sum_principal_flow);
	//PL_MORTGAGE_LOANS -= sum_principal_flow;
	//DEPOSITS 		  -= sum_principal_flow;
	
	PROFITS[0] 		  += sum_interest_flow;
	
	liability_transformation(&DEPOSITS, &EQUITY, sum_interest_flow);

	/* Trailing sum of 3 months of daily income flows (array length 3)
     * Initialize on todays interest flow, add last 2 months
     * Array: update the array, and add new item to end
     * See below at full mortgege repayments.
     */
	INTEREST_INCOME_MORTGAGES = sum_interest_flow; //today's interest income flow
	
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_read_mortgage_payments(): After MORTGAGE_PAYMENT_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\nPrincipal payment flow\t%f\t(PL MORTGAGES-,DEPOSITS-)", sum_principal_flow);
		printf("\nInterest payment flow\t%f\t(DEPOSITS-,EQUITY+)", sum_interest_flow);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");

		printf("=== AFTER Bank_receive_mortgage_payments ===\n");
		
		print_bank_balance_sheet();
	}
	#endif

	enforce_bank_balanced_balance_sheet("Bank_receive_mortgage_payments");

	return 0;   
}

/** \fn Bank_read_mortgage_arrears_messages()
 * \brief Function to
 * \brief Message input: 
 * \brief - add_mortgage_arrears_message(ID, bank_id, loan_loss, loan_default, arrears_counter);
 * \brief Message output: 
 * \brief - none
 * \brief UNITTEST: PASS 2.11.2016
 */
int Bank_read_mortgage_arrears_messages()
{	

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE RESET: size %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	double sum_pricipal_stock=0.0;
	
	START_MORTGAGE_ARREARS_MESSAGE_LOOP
			assert(mortgage_arrears_message->mortgage.active);

			BANK_MORTGAGES_ARREARS++;
            
            //sum_principal_flow += mortgage_arrears_message->mortgage.outstanding_principal;

            sum_pricipal_stock += mortgage_arrears_message->mortgage.outstanding_principal;
            
			#ifdef _DEBUG_MODE    
			if (PRINT_BANK_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d MORTGAGE ARREARS MESSAGE RECEIVED: CUSTOMER: %4d OBJECT %4d COUNTER: %d", DAY, ID, mortgage_arrears_message->mortgage.household_id, mortgage_arrears_message->mortgage.object_id, mortgage_arrears_message->mortgage.arrears_counter);		
			}
			#endif

			#ifdef _DEBUG_MODE 
		int x=0;
		int found=0;
		for(x=0;x<MORTGAGE_PORTFOLIO.size;x++)
		{
			if(MORTGAGE_PORTFOLIO.array[x].object_id==mortgage_arrears_message->mortgage.object_id)
			{
				found=1;
				break;
			}
		}

		if(!found)
			printf("BUG: received mortgage arrears message, but cannot find mortgage (ITS: %d bank ID: %d, object_id: %d\n", DAY, ID, mortgage_arrears_message->mortgage.object_id);
		else
		{
			if(fabs(MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_arrears_message->mortgage.outstanding_principal)>1e-3)
			{
				printf("BUG: household reported inconsistent values for mortgage in arrears (ITS: %d bank ID: %d, object_id: %d)\n", DAY, ID, mortgage_arrears_message->mortgage.object_id);
				printf("\tprincipal in books: %f\n", MORTGAGE_PORTFOLIO.array[x].outstanding_principal);
				printf("\tself-reported new principal: %f\n ",mortgage_arrears_message->mortgage.outstanding_principal);
				printf("\tdiff: %f (%.2f%%)\n", MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_arrears_message->mortgage.outstanding_principal, fabs(MORTGAGE_PORTFOLIO.array[x].outstanding_principal-mortgage_arrears_message->mortgage.outstanding_principal)/mortgage_arrears_message->mortgage.outstanding_principal*100);
			}
		}
		#endif

	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP    

	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE ARREARS MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	enforce_bank_balanced_balance_sheet("Bank_read_mortgage_arrears_messages");
	
	return 0;   
}	

/** \fn Bank_read_mortgage_npl_messages()
 * \brief Function to
 * \brief See auxiliary functions:
 * \brief - recompute_mortgage_stocks
 * \brief Message input: 
 * \brief - add_mortgage_npl_message(ID, bank_id, loan_loss, loan_default, npl_counter);
 * \brief Message output: 
 * \brief - none
 * \brief UNITTEST: PASS 2.11.2016
 */
int Bank_read_mortgage_npl_messages()
{

	double npl_flow=0.0;
	
	double llp_pl_mortgages = 0.0;
	double llp_npl_mortgages = 0.0;

	START_MORTGAGE_NPL_MESSAGE_LOOP	
		assert(mortgage_npl_message->mortgage.active);

		BANK_MORTGAGES_NPL++;

		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d MORTGAGE NPL MESSAGE RECEIVED: CUSTOMER: %4d OBJECT %4d COUNTER: %d", DAY, ID, mortgage_npl_message->mortgage.household_id, mortgage_npl_message->mortgage.object_id, mortgage_npl_message->mortgage.npl_counter);
		}      
		#endif

	FINISH_MORTGAGE_NPL_MESSAGE_LOOP
	
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE NPL MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	// PL->NPL in balance sheet is done when portfolio is reconstructed!!!


	/*	Balance sheet operations:
	 *	asset_liability_add(&asset, &liability, value);
	 *	asset_liability_subtract(&asset, &liability, value);
	 *	asset_transformation(&from, &to, value);
	 *	liability_transformation(&from, &to, value);
	 */

	//NPL flow shifts from PL stock to NPL stock
	//asset_transformation(&PL_MORTGAGE_LOANS, &NPL_MORTGAGE_LOANS, npl_flow);
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_read_mortgage_npl_messages(): After MORTGAGE_NPL_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\nNPL MORTGAGE flow\t%f\t(NLP+,PL-)", npl_flow);
		printf("\nPL MORTGAGE flow\t%f\t(NLP+,PL-)", -npl_flow);
		printf("\nLLP NPL Mortgages\t%f\t(LLR+,EQUITY-)", llp_npl_mortgages);
		printf("\nLLP PL Mortgages\t%f\t(LLR+,EQUITY-)", llp_pl_mortgages);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");

		printf("=== AFTER Bank_read_mortgage_npl_messages ===\n");
		
		print_bank_balance_sheet();
	}
	#endif

	enforce_bank_balanced_balance_sheet("Bank_read_mortgage_npl_messages");

	
	return 0;   
}

/** \fn Bank_read_mortgage_writeoff_messages()
 * \brief Function to
 * \brief See auxiliary functions:
 * \brief - 
 * \brief Message input: 
 * \brief - mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default);
 * \brief Message output: 
 * \brief - none
 * \brief UNITTEST: PASS 2.11.2016
 */
int Bank_read_mortgage_writeoff_messages()
{


	/******************* SECTION: mortgage_writeoff ******************************/
	double npl_writeoff_flow=0.0;
	double ll_total = 0.0;
	double llp_npl_mortgages = 0.0;
	
	foreclosure_adt foreclosed_house;
	init_foreclosure_adt(&foreclosed_house);
	
	double seized_collateral=0.0;
	double seized_liquidity=0.0;
	
	double loan_loss=0.0;
	double sum_loan_loss=0.0;
	double reimbursement = 0.0;
	double sum_reimbursement=0.0;
	
	//Read mortgage writeoffs
	START_MORTGAGE_WRITEOFF_MESSAGE_LOOP
	if (ID==mortgage_writeoff_message->bank_id)
	{		
		assert(mortgage_writeoff_message->house.seized_house.mortgage.active);
		
		BANK_MORTGAGES_WRITEOFF++;

		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d MORTGAGE WRITEOFF MESSAGE RECEIVED: CUSTOMER: %4d OBJECT %4d seized liquidity: %f", DAY, ID, mortgage_writeoff_message->household_id, mortgage_writeoff_message->house.seized_house.object_id, mortgage_writeoff_message->house.seized_liquidity);
		}      
		#endif

		//Update total loan default and loan loss values for later update of loan loss reserve
		npl_writeoff_flow += mortgage_writeoff_message->house.seized_house.mortgage.outstanding_principal;	//Use outstanding principal directly
		
		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d SEIZING OBJECT %d", DAY, ID, mortgage_writeoff_message->house.seized_house.object_id);
		}      
		#endif
		
		copy_foreclosure_adt(&(mortgage_writeoff_message->house), &foreclosed_house);

		//Add attributes for re-evaluating the value of the collateral:
		foreclosed_house.seizure_day = DAY;

		//add_foreclosure_adt(&VAR, object_id, former_owner_id, bank_id, value, claim, seized_liquidity, housing_market_price_index_base, seizure_day, mortgage)
		add_foreclosure_adt(&SEIZED_COLLATERAL, foreclosed_house.seized_liquidity, foreclosed_house.seizure_day, &(foreclosed_house.seized_house));
		
		log_transformation("bank", "mortgages", "mortgages_defaulted", "foreclosure", ID, foreclosed_house.seized_house.mortgage.outstanding_principal);

		seized_collateral 	+= foreclosed_house.seized_house.purchase_price;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_HOUSING_INFO)
		{
			printf("\n\nInputs:\nvalue\t\t%.2f\nclaim\t\t%.2f\nseized liq.\t%.2f", foreclosed_house.seized_house.purchase_price, 0.0, foreclosed_house.seized_liquidity);
			printf("\n\nOutputs:\nloan_loss\t%.2f\nreimbursement\t%.2f", loan_loss, reimbursement);
			
		}      
		#endif
	}
	FINISH_MORTGAGE_WRITEOFF_MESSAGE_LOOP
	
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE WRITEOFF MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	/*	Balance sheet operations:
	 *	asset_liability_add(&asset, &liability, value);
	 *	asset_liability_subtract(&asset, &liability, value);
	 *	asset_transformation(&from, &to, value);
	 *	liability_transformation(&from, &to, value);
	 */
	
	//Process seized_collateral
	SEIZED_COLLATERAL_VALUE_CALENDAR += seized_collateral;

	
	//Set plan to sell:
	if (SEIZED_COLLATERAL.size>0)  PLAN_SELL_REAL_ESTATE=1;


	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_INFO)
	{
		if(ll_total>0)
		{
			printf("\n\n Updating the loan loss reserve for mortgage loans due to loan losses (defaults)");
			printf("\n\t LLR_NPL_MORTGAGE_LOANS=%f", LLR_NPL_MORTGAGE_LOANS); 
		}
	}      
	#endif

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_read_mortgage_writeoff_messages(): After MORTGAGE_WRITEOFF_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\nSeized collateral\t%f\t(Coll+,EQUITY+)", seized_collateral);
		printf("\nSeized liquidity\t%f\t(EQUITY+,DEPOSITS-)", seized_liquidity);
		printf("\nNPL MORTGAGE flow\t%f\t(NLP+,EQUITY+)", -npl_writeoff_flow);
		printf("\nLLP NPL Mortgages\t%f\t(LLR+,EQUITY-)", llp_npl_mortgages);
		//printf("\nMortgage claims\t\t%f\t(MORTGAGE_CLAIM+)", bad_debt_flow);
		printf("\nLoan losses\t\t%f\t(--)", sum_loan_loss);
		printf("\nReimbursements\t\t%f\t(DEPOSITS+,EQUITY-)", sum_reimbursement);
		printf("\n");
		printf("\n");

		printf("=== AFTER Bank_read_mortgage_writeoff_messages ===\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
	//Free
	free_foreclosure_adt(&foreclosed_house);

	enforce_bank_balanced_balance_sheet("Bank_read_mortgage_writeoff_messages");

	
	return 0;   
}


/** \fn Bank_reconstruct_mortgage_portfolio()
 * \brief Function to reconstruct the Bank mortgage portfolio from all mortgage messages (payment, arears, npl)
 * \brief Message input: 
 * \brief - mortgage_payment, mortgage_arears, mortgage_npl
 * \brief Message output: 
 * \brief - none
 */
int Bank_reconstruct_mortgage_portfolio()
{
	
	// Local mortgage var
	mortgage_contract_adt mortgage;

	// Initialize all attributes to zero
	init_mortgage_contract_adt(&mortgage);

	// Local mortgage portfolio var
	mortgage_contract_adt_array mortgage_portfolio;

	// Initialize: allocate memory for mortgage_portfolio array
	init_mortgage_contract_adt_array(&mortgage_portfolio);

	// Copy current mortgage_portfolio array into local var (to store a copy of the before state)
	copy_mortgage_contract_adt_array(&MORTGAGE_PORTFOLIO, &mortgage_portfolio);	

	// Reset the mortgage portfolio
	// Sets size=0, but does not remove adts explicitly:
	reset_mortgage_contract_adt_array(&MORTGAGE_PORTFOLIO);


	//NEW STYLE: add mortgage from mesg to portfolio
	START_MORTGAGE_PAYMENT_MESSAGE_LOOP
		assert(mortgage_payment_message->mortgage.bank_id==ID);

	  	//Check for full repayment: only if not yet repaid, add mortgage to portfolio
		if (mortgage_payment_message->mortgage.current_duration > 0)
		{						
			//Extract ADT from message (pointer to struct in mesg, and copy to local var using flame inbuild function)
			copy_mortgage_contract_adt( &(mortgage_payment_message->mortgage), &mortgage);
					
			//Add ADT to dynamic array mortgage_portfolio
			add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
				mortgage.type,
				mortgage.active,
				mortgage.is_variable_rate,
				mortgage.object_id,				
				mortgage.household_id,
				mortgage.bank_id,
				mortgage.initial_duration,
				mortgage.base_rate,
				mortgage.markup_on_base_rate,
				mortgage.interest_rate_annual,
				mortgage.current_duration,
				mortgage.outstanding_principal,
				mortgage.outstanding_interest,
				mortgage.monthly_total_repayment,
				mortgage.monthly_principal,
				mortgage.monthly_interest,
				mortgage.status,			
				mortgage.arrears_counter,
				mortgage.npl_counter,
				mortgage.collateral_value,
				mortgage.loan_default,
				mortgage.loan_loss);
		}
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP

	START_MORTGAGE_ARREARS_MESSAGE_LOOP		
		assert(mortgage_arrears_message->mortgage.bank_id==ID);

		//Extract ADT from message (pointer to struct in mesg, and copy to local var using flame inbuild function)
		copy_mortgage_contract_adt( &(mortgage_arrears_message->mortgage), &mortgage);
				
		//Add ADT to dynamic array mortgage_portfolio
		add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
			mortgage.type,			
			mortgage.active,
			mortgage.is_variable_rate,
			mortgage.object_id,				
			mortgage.household_id,
			mortgage.bank_id,
			mortgage.initial_duration,
			mortgage.base_rate,
			mortgage.markup_on_base_rate,
			mortgage.interest_rate_annual,
			mortgage.current_duration,
			mortgage.outstanding_principal,
			mortgage.outstanding_interest,
			mortgage.monthly_total_repayment,
			mortgage.monthly_principal,
			mortgage.monthly_interest,
			mortgage.status,			
			mortgage.arrears_counter,
			mortgage.npl_counter,
			mortgage.collateral_value,
			mortgage.loan_default,
			mortgage.loan_loss);
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP


	START_MORTGAGE_NPL_MESSAGE_LOOP		
		assert(mortgage_npl_message->mortgage.bank_id==ID);

		//Extract ADT from message (pointer to struct in mesg, and copy to local var using flame inbuild function)
		copy_mortgage_contract_adt( &(mortgage_npl_message->mortgage), &mortgage);
				
		//Add ADT to dynamic array mortgage_portfolio
		add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
			mortgage.type,			
			mortgage.active,
			mortgage.is_variable_rate,
			mortgage.object_id,				
			mortgage.household_id,
			mortgage.bank_id,
			mortgage.initial_duration,
			mortgage.base_rate,
			mortgage.markup_on_base_rate,
			mortgage.interest_rate_annual,
			mortgage.current_duration,
			mortgage.outstanding_principal,
			mortgage.outstanding_interest,
			mortgage.monthly_total_repayment,
			mortgage.monthly_principal,
			mortgage.monthly_interest,
			mortgage.status,			
			mortgage.arrears_counter,
			mortgage.npl_counter,
			mortgage.collateral_value,
			mortgage.loan_default,
			mortgage.loan_loss);
	FINISH_MORTGAGE_NPL_MESSAGE_LOOP

	//Free allocated memory
	free_mortgage_contract_adt(&mortgage);

	//Compute PL_MORTGAGE_LOANS and NPL_MORTGAGE_LOANS from Bank Portfolio
	recompute_mortgage_stocks();

	//Eq (19): After updating the PD and LGD, re-compute the new Expected Loss and adjust LLR for all PL and NPL mortgage loans in portfolio of the bank
	Bank_update_LLR_mortgage_loans();


	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_reconstruct_mortgage_portfolio(): After all MORTGAGE_MESSAGE_LOOPS", DAY, ID);
		printf("\nFlows:");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");

		printf("=== AFTER Bank_reconstruct_mortgage_portfolio ===\n");
		
		print_bank_balance_sheet();
	}
	#endif


	#ifdef _DEBUG_MODE        
	if (PRINT_DEBUG_FILES_BANKS)
	{                       
		// Sanity check: compare mortgage_portfolio before and after reconstruction
		// This is a diff between the mortgage arrays mortgage_portfolio and MORTGAGE_PORTFOLIO
		mortgage_portfolio_diff(&mortgage_portfolio, &MORTGAGE_PORTFOLIO);
	}
	#endif

	// Free the allocated memory
	free_mortgage_contract_adt_array(&mortgage_portfolio);

	enforce_bank_balanced_balance_sheet("Bank_reconstruct_mortgage_portfolio");

	return 0;
}

/** \fn Bank_set_risk_exposure_amounts()
 * \brief Function to
 * \brief See auxiliary functions:
 * \brief - Bank_set_risk_weight_household_loans()
 * \brief - recompute_mortgage_stocks()
 * \brief Message input: 
 * \brief - none
 * \brief Message output: 
 * \brief - bank_recapitalization_request_message(bank_id, equity)
 * \brief UNITTEST: PASS 2.11.2016
 */
int Bank_set_risk_exposure_amounts()
{

	/******************** SECTION: update regulatory risk weights and REA **************/
    //1.6.2017 SvdH
	//RUN MONTHY: update regulatory risk weights on day 2 of month
	if (DAY%MONTH==2)
		Bank_set_risk_weight_household_loans();



		
	//Set REA for mortgage loans
	REA_MORTGAGE_LOANS_PL = BASEL_RISK_WEIGHT_MORTGAGE_LOANS_PL * PL_MORTGAGE_LOANS;
	REA_MORTGAGE_LOANS_NPL = BASEL_RISK_WEIGHT_MORTGAGE_LOANS_NPL * NPL_MORTGAGE_LOANS; 
		
	//Eq 22: REA total
	REA_MORTGAGE_LOANS 	 = REA_MORTGAGE_LOANS_PL + REA_MORTGAGE_LOANS_NPL;
	REA = REA_FIRM_LOANS + REA_MORTGAGE_LOANS;
	
    //27.3.2017 SvdH
	//Bank checks solvency condition. This is the 2nd place where we are checking.
	//See Credit/Bank_Credit_Functions.c, Bank_accounting() for default check at end of month.
	Bank_check_equity_requirement();

	return 0;   
}

/** \fn Bank_receive_full_mortgage_payments()
 * \brief Function to process full mortgage repayments by households that sell their HMR or OPs.
 * \brief Message input: 
 * \brief - add_mortgage_full_payment_message(ID, bank_id, monthly_principal, monthly_interest)
 * \brief Message output: None
 * \brief UNITTEST: PASS 15.1.2017
 */
int Bank_receive_full_mortgage_payments()
{

	int i=0;
	int id=0;
	int object_id=0;
	double interest=0.0;
	double principal=0.0;
	double sum_interest_flow=0.0;
	double sum_principal_flow=0.0;

	
	
	//XML: Filter on bank_id
	//XML: Sort on key household_id
	int count=0;
	START_FULL_MORTGAGE_PAYMENT_MESSAGE_LOOP
	  count++;

	  id 		= full_mortgage_payment_message->household_id;
	  object_id = full_mortgage_payment_message->mortgage.object_id;
	  principal = full_mortgage_payment_message->mortgage.outstanding_principal;

	  if (!FREE_EARLY_REPAYMENT)
	  	interest = full_mortgage_payment_message->mortgage.outstanding_interest;
	  else
	  	interest = 0.0;
	  
	  //Accounting
	  BANK_MORTGAGES_PAID_OFF++;

	  sum_principal_flow += principal;
	  sum_interest_flow	 += interest;

	  log_outflow("bank", "mortgages", "deposits", "mortgage_principal", ID, principal);
	  log_transformation("bank", "deposits", "equity", "mortgage_interest", ID, interest);

	  MORTGAGE_INCOME_CALENDAR_MONTH += interest;

	  //search for correct household and object_id.
	  int found=0;
	  for(i=0; i<MORTGAGE_PORTFOLIO.size; i++)
	  {
		  if(MORTGAGE_PORTFOLIO.array[i].household_id==id 
		  	 && MORTGAGE_PORTFOLIO.array[i].object_id==object_id)
		  {
		  	found=1;
		  	break;
		  }
	  }

	 	assert(found);
		remove_mortgage_contract_adt(&MORTGAGE_PORTFOLIO, i);
	  
	  #ifdef _DEBUG_MODE    
	  if (PRINT_BANK_MORTGAGE_TABLES)
	  {
		  if(count==1)
		  {
			printf("\nID: %d Bank_receive_full_mortgage_payments", ID);
			printf("\n\tNo\t\tHH ID\t\tprincipal\t\tinterest\t\tpaid off"); 
		  }
		  printf("\n\t%2d\t\t%4d\t\t%f\t\t%f\t\t%d", count, id, principal, interest, (MORTGAGE_PORTFOLIO.array[i].current_duration==0));
	  }      
	  #endif
	FINISH_FULL_MORTGAGE_PAYMENT_MESSAGE_LOOP

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_TABLES)
		if(count>0) printf("\n");
	#endif
	
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER FULL MORTGAGE PAYMENT MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

		
	//Process flows
	asset_liability_subtract(&PL_MORTGAGE_LOANS,&DEPOSITS,sum_principal_flow);
	liability_transformation(&DEPOSITS,&EQUITY,sum_interest_flow);
	PROFITS[0] += sum_interest_flow;
	

	/* CONTINUATION FROM ABOVE, line  : sum of mortgage interest income flows
	 * Trailing sum of last 3 months of mortgage interest income flows (array length 3)
     * Initialize on current month's interest flow, add last 2 months
     * Array: update the array, and add new item to end
     */
	INTEREST_INCOME_MORTGAGES += sum_interest_flow; //today's interest income flow
	for (i=0; i<INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.size-1; i++)
	{
		INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.array[i] = INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.array[i+1];
		INTEREST_INCOME_MORTGAGES += INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.array[i+1];
	}
	INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.array[INTEREST_INCOME_MORTGAGES_MONTHLY_HISTORY.size-1] = sum_interest_flow;

	//Annualise
	INTEREST_INCOME_MORTGAGES *= 4;

	recompute_mortgage_stocks();

	Bank_update_LLR_mortgage_loans();
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nBank_receive_full_mortgage_payments(): After FULL_MORTGAGE_PAYMENT_MESSAGE_LOOP");
		printf("\nFlows:");
		printf("\nPrincipal payment flow\t%f\t(PL MORTGAGES-,DEPOSITS-)", sum_principal_flow);
		printf("\nInterest payment flow\t%f\t(DEPOSITS-,EQUITY+)", sum_interest_flow);
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

int Bank_send_max_expenditure_info()
{

	mortgage_application_adt mortgage_request;
	int id=0, m=0;
	double f=0.0, pr=0.0, i=0.0, derived_principal=0.0, max_expenditure=0.0, a=0.0;

    //New rule, 13.03.2020
	double pd = 1-pow(1-HH_PD_PL_YEAR_MA,1/12.0);   //monthly PD, based on annual PD MA measure
	double lgd = HH_LGD_PL_YEAR_MA;  				//LGD, based on annual LGD MA measure

	//Set funding costs
	f = FUNDING_COSTS;

	//Set profit_margin pr:  TODO SECTION 6 FROM MODEL DESCRIPTION
	pr = BANK_PROFIT_MARGIN;

	START_MORTGAGE_MAX_EXPENDITURE_REQUEST_MESSAGE_LOOP
		if (ID==mortgage_max_expenditure_request_message->bank_id)
		{
			init_mortgage_application_adt(&mortgage_request);
			copy_mortgage_application_adt( &(mortgage_max_expenditure_request_message->mortgage), &mortgage_request);

			id = mortgage_max_expenditure_request_message->household_id;
			
			//New rule, 13.03.2020
			i = max(0.00001, (f + pr) + 12.0 * pd * lgd / (1-pd) );

			//Set cap on mortgage interest rate
			if(i>CONST_CAP_MORTGAGE_INTEREST_RATE)
				i = CONST_CAP_MORTGAGE_INTEREST_RATE;

			assert(i> -1e-7);

			m = mortgage_request.sampled_maturity;

			a = mortgage_request.annuity;

			derived_principal = a * (12/i) * (pow(1 + i/12, m)-1)/pow(1 + i/12, m);

			max_expenditure = derived_principal / mortgage_request.sampled_loan_to_value;

			add_mortgage_max_expenditure_response_message(id, max_expenditure);

			//printf("----- max_expenditure: %f\n",max_expenditure);
		}
	FINISH_MORTGAGE_MAX_EXPENDITURE_REQUEST_MESSAGE_LOOP

	free_mortgage_application_adt(&mortgage_request);

	return 0;
}

/** \fn Bank_process_mortgage_requests()
 * \brief Function to compute data for mortgage Bank_process_mortgage_requests
 * \brief REF: Model Specifications, 1 August 2016.
 * \brief Message input: 
 * \brief - mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage, avg_annual_gross_income, total_quarterly_gross_income, dsti, cash);
 * \brief Message output:
 * \brief - add_mortgage_offer_message(bank_id, household_id, mortgage_contract_adt mortgage);
 * \brief UNITTEST: TODO
 */
int Bank_process_mortgage_requests()
{
	int id, count;
	double m, f, pr;

	double a = 0.0;
	double i = 0.0;
	double spread = 0.0;
	double loan_to_value, home_value, own_contribution;

	double total_interest_owed;

	double derived_principal;

	mortgage_application_adt mortgage_request;
	init_mortgage_application_adt(&mortgage_request);

	mortgage_contract_adt mortgage_offer;
	init_mortgage_contract_adt(&mortgage_offer);

	double pd = 1-pow(1-HH_PD_PL_YEAR_MA,1/12.0);  	//monthly PD measure, based on annual PD MA measure
	double lgd = HH_LGD_PL_YEAR_MA;  				//LGD measure, based on annual LGD MA measure

	//Set funding costs
	f = FUNDING_COSTS;

	//Set profit_margin pr:  TODO SECTION 6 FROM MODEL DESCRIPTION
	pr = BANK_PROFIT_MARGIN;

	HYPOTHETICAL_INTEREST_RATE_MORTGAGES = max(0.00001, (f + pr) + 12.0 * pd * lgd / (1-pd));
	
	count=0;
	START_MORTGAGE_REQUEST_MESSAGE_LOOP

		if(GRANT_LOANS_DEFINITELY || (GRANT_LOANS_MAYBE && random_unif()<CONST_BANK_LOAN_GRANTING_PROB) )
		{

			BANK_MORTGAGES_REQUESTED++;

			count++;

			id = mortgage_request_message->household_id;

			copy_mortgage_application_adt( &(mortgage_request_message->mortgage), &mortgage_request);

						//Set sampled values:
			m = mortgage_request.sampled_maturity; //MONTHLY

			//Monthly annuity: debt service payment (interest+principal)
			a = mortgage_request.annuity;

			//Annualized interest rate
			i = max(0.00001, (f + pr) + 12.0 * pd * lgd / (1-pd) );

			assert(m>0);
			assert(a>0);

			//Set cap on mortgage interest rate
			if(i>CONST_CAP_MORTGAGE_INTEREST_RATE)
				i = CONST_CAP_MORTGAGE_INTEREST_RATE;

			if(i < -1e-7)
			{
				fprintf(stderr, "\nERROR in Bank_process_mortgage_requests(): i < -1e-7");
				fprintf(stderr, "\n\t i %f f %f pr %f HH_LGD_PL_YEAR %f HH_PD_PL_YEAR %f", i, f, pr, HH_LGD_PL_YEAR, HH_PD_PL_YEAR);
				fprintf(stderr, "\n\t (1+f+pr) %f", (1+f+pr) );
				fprintf(stderr, "\n\t (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR %f", (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR );
				fprintf(stderr, "\n\t ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) %f", ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) );
				fprintf(stderr, "\n\t ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) / (1 - HH_PD_PL_YEAR) %f", ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) / (1 - HH_PD_PL_YEAR) );
				fprintf(stderr, "\n\t ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) / (1 - HH_PD_PL_YEAR) -1 %f", ( (1+f+pr) - (1 - HH_LGD_PL_YEAR)*HH_PD_PL_YEAR) / (1 - HH_PD_PL_YEAR) -1 );
			}

			assert(i > -1e-7);

			// Eq(11): Loan value at origination, depends on annuity, duration of loan at origination (from mortgage_request mesg)
			derived_principal = a * (12/i) * (pow(1 + i/12, m)-1)/pow(1 + i/12, m);


			if(derived_principal < -1e-7 || derived_principal != derived_principal)
			{
				fprintf(stderr, "\nERROR in %s(), line %d: derived_principal %f", __FUNCTION__, __LINE__, derived_principal);
				fprintf(stderr, "\n\t annuity %f interest %f maturity %f derived_principal %f", a, i, m, derived_principal);
				fprintf(stderr, "\n\t derived_principal = a * (12/i) * (pow(1 + i/12, m)-1)/pow(1 + i/12, m)");
				fprintf(stderr, "\n\t (12/i) = %f", (12/i));
				fprintf(stderr, "\n\t pow(1 + i/12, m) = %f", pow(1 + i/12, m));
				fprintf(stderr, "\n\t pow(1 + i/12, m)-1 = %f", pow(1 + i/12, m)-1);
				fprintf(stderr, "\n\t (pow(1 + i/12, m)-1)/pow(1 + i/12, m) = %f", (12/i) * (pow(1 + i/12, m)-1)/pow(1 + i/12, m) );
			}

			//assert(derived_principal > -1e-7);
			assert__(derived_principal > -1e-7 && derived_principal == derived_principal)
			{
			    fprintf(stderr, "\n\tWARNING %s(), line %d: derived_principal %f\n", __FUNCTION__, __LINE__, derived_principal);
			}

			
			//Eqn (12): total interest due over life time of loan (time-invariant), given initial principal, initial duration
			total_interest_owed = ((i/12) * derived_principal * m)/(1-pow(1+i/12, -m)) - derived_principal;

			//Eqn (13-15) are in auxiliary functions, relevenat only during path of repayment schedule

			//Sampled LTV for (16)
			loan_to_value = mortgage_request.sampled_loan_to_value;

			// Eq(16): Value of HMR at origination
			home_value = derived_principal/loan_to_value;

			// Eq(17): Own contribution
			own_contribution = max(0.0, home_value - derived_principal);
			
			// Initialize mortgage
			mortgage_offer.household_id 			= id;
			mortgage_offer.bank_id 					= ID;
			mortgage_offer.initial_duration 		= m; //months

			mortgage_offer.is_variable_rate = 0;

			mortgage_offer.base_rate = 0.0;
			mortgage_offer.markup_on_base_rate = 0.0;
			mortgage_offer.interest_rate_annual 	= i;
	        mortgage_offer.current_duration 		= m; //months
			mortgage_offer.outstanding_principal 	= derived_principal;

			mortgage_offer.outstanding_interest 	= total_interest_owed;
			mortgage_offer.monthly_total_repayment 	= a;

			if (random_unif() < SHARE_VARIABLE_RATE_MORTGAGES+EPSILON)
			{
				mortgage_offer.is_variable_rate = 1;
				mortgage_offer.base_rate = ECB_INTEREST_RATE;
				mortgage_offer.markup_on_base_rate = mortgage_offer.interest_rate_annual - mortgage_offer.base_rate;
			}
			
			//mortgage_offer.monthly_principal 		= 0.0; //not needed
			//mortgage_offer.monthly_interest 		= 0.0; //not needed
			
			#ifdef _DEBUG_MODE    
			if (PRINT_BANK_MORTGAGE_INFO)
			{
				printf("\n\nIT %d BANK %d MORTGAGE REQUEST MESSAGE RECEIVED from HH ID: %d", DAY, ID, id);
				printf("\n\tHH_PD_PL_YEAR: %f", HH_PD_PL_YEAR);
				printf("\n\tHH_LGD_PL_YEAR: %f", HH_LGD_PL_YEAR);
				
				printf("\n\tSampled LTV: %f", mortgage_request.sampled_loan_to_value);
				printf("\n\tmaturity (months): %d", mortgage_request.sampled_maturity);
				printf("\n\tannuity: %f", mortgage_request.annuity);
				
				printf("\n\tFunding costs: %f", f);
				printf("\n\tProfit_margin: %f", pr);
				printf("\n\tSpread: %f", spread);
				
				printf("\n\tInterest rate, annualized: %f", i);
				printf("\n\tLoan value at origination (principal): %f", derived_principal);
				printf("\n\tValue at acquisition (home_value): %f", home_value);
				printf("\n\tOwn contribution: %f", own_contribution);			
			}      
			#endif
			
			//Table output:
			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				if(count==1)
				{
					printf("\n\n ID: %d Bank_process_mortgage_requests", ID);
					printf("\n\t HH ID \t init principal\t duration \t monthly total repayment"); 
				}					
				printf("\n\t %d \t %8.6f \t %8d \t %8.6f \t\t\n", 
						mortgage_offer.household_id,
						mortgage_offer.outstanding_principal,
						mortgage_offer.current_duration,
						mortgage_offer.monthly_total_repayment);
			}      
			#endif

			//Annual interest is in decimal form: i=1.0 means an annual interest rate of 100%
		  	if (i<1.0)
		  	{
				add_mortgage_contract_message(ID, id, mortgage_offer);

				MRTG_TOTAL_OFFERED_FLOW_COUNT++;
				MRTG_TOTAL_OFFERED_CUM_COUNT++;

				if (mortgage_offer.is_variable_rate)
				{
					MRTG_VARIABLE_OFFERED_FLOW_COUNT++;					
					MRTG_VARIABLE_OFFERED_CUM_COUNT++;
				}
				else
				{
					MRTG_FIXED_OFFERED_FLOW_COUNT++;
					MRTG_FIXED_OFFERED_CUM_COUNT++;
				}

				#ifdef _DEBUG_MODE    
				if (PRINT_BANK_MORTGAGE_INFO)
				{
					printf("\nIT %d BANK %d MORTGAGE OFFER:\n\t HH %d\n\t initial_duration %d\n\t interest_rate_annual %f\n\t current_duration %d\n\t outstanding_principal € %f\n\t outstanding_interest € %f\n\t monthly_total_repayment € %f", 
					DAY,
					ID, 
					mortgage_offer.household_id, 
					mortgage_offer.initial_duration, 
					mortgage_offer.interest_rate_annual, 
					mortgage_offer.current_duration, 
					mortgage_offer.outstanding_principal, 
					mortgage_offer.outstanding_interest,
					mortgage_offer.monthly_total_repayment);
				}      
				#endif
			}
			else
			{
				#ifdef _DEBUG_MODE    
				if (PRINT_BANK_MORTGAGE_INFO)
				{
					printf("\nIT %d BANK %d DISCARDING OFFER (i>1.0):\n\t HH %d\n\t initial_duration %d\n\t interest_rate_annual %f\n\t current_duration %d\n\t outstanding_principal € %f\n\t outstanding_interest € %f\n\t monthly_total_repayment € %f\n\t monthly_principal € %f\n\t monthly_interest € %f", 
					DAY,
					ID, 
					mortgage_offer.household_id, 
					mortgage_offer.initial_duration, 
					mortgage_offer.interest_rate_annual, 
					mortgage_offer.current_duration, 
					mortgage_offer.outstanding_principal, 
					mortgage_offer.outstanding_interest,
					mortgage_offer.monthly_total_repayment,
					mortgage_offer.monthly_principal,
					mortgage_offer.monthly_interest);
				}      
				#endif
			}
		}//if(GRANT_LOANS_DEFINITELY || (GRANT_LOANS_MAYBE && random_unif()<CONST_BANK_LOAN_GRANTING_PROB) )

	FINISH_MORTGAGE_REQUEST_MESSAGE_LOOP
	
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE REQUEST MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	//Free allocated memory
	free_mortgage_application_adt(&mortgage_request);
	free_mortgage_contract_adt(&mortgage_offer);

	return 0;   
}

int Bank_process_mortgage_accepts()
{

	double outstanding_principal =0.0;
	double outstanding_interest  =0.0;
	double sum_principal_flow = 0.0;
	mortgage_contract_adt mortgage;

	START_MORTGAGE_ACCEPTED_MESSAGE_LOOP

		#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
		{
			printf("\n\nIT %d BANK %d MORTGAGE ACCEPTED MESSAGE RECEIVED from HH ID: %d", DAY, ID, mortgage_accepted_message->household_id);
		}
		#endif

		outstanding_principal = mortgage_accepted_message->mortgage.outstanding_principal;
		outstanding_interest  = mortgage_accepted_message->mortgage.outstanding_interest;
		sum_principal_flow	 += outstanding_principal;
		
		BANK_OUTFLOWS_CALENDAR.mortgage_loan_issues += outstanding_principal + outstanding_interest;

		BANK_OUTFLOWS_MONTHLY_MORTGAGE_ISSUES += outstanding_principal + outstanding_interest;
		
		BANK_NO_MORTGAGES_NEW++;
		BANK_MORTGAGES_NEW_BUSINESS += mortgage_accepted_message->mortgage.outstanding_principal;

		SUM_INTEREST_RATES_NEW_MORTGAGES += mortgage_accepted_message->mortgage.interest_rate_annual;

		WEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES += mortgage_accepted_message->mortgage.interest_rate_annual * mortgage_accepted_message->mortgage.outstanding_principal;

        WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS += ( mortgage_accepted_message->mortgage.interest_rate_annual - FUNDING_COSTS ) * mortgage_accepted_message->mortgage.outstanding_principal;

        WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE += ( mortgage_accepted_message->mortgage.interest_rate_annual - ECB_INTEREST_RATE ) * mortgage_accepted_message->mortgage.outstanding_principal;

        WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF += ( mortgage_accepted_message->mortgage.interest_rate_annual - WACF ) * mortgage_accepted_message->mortgage.outstanding_principal;

		//Add mortgage to portfolio
		
		//re-initialize all attributes to zero
		init_mortgage_contract_adt(&mortgage);
			
		//Extract ADT from message (pointer to struct in mesg, and copy to local var using flame inbuild function)
		copy_mortgage_contract_adt( &(mortgage_accepted_message->mortgage), &mortgage);
		
		//Add ADT to dynamic array mortgage_portfolio
		add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
							mortgage.type,
							mortgage.active,
							mortgage.is_variable_rate,
							mortgage.object_id,				
							mortgage.household_id,
							mortgage.bank_id,
							mortgage.initial_duration,
							mortgage.base_rate,
							mortgage.markup_on_base_rate,
							mortgage.interest_rate_annual,
							mortgage.current_duration,
							mortgage.outstanding_principal,
							mortgage.outstanding_interest,
							mortgage.monthly_total_repayment,
							mortgage.monthly_principal,
							mortgage.monthly_interest,
							mortgage.status,			
							mortgage.arrears_counter,
							mortgage.npl_counter,
							mortgage.collateral_value,
							mortgage.loan_default,
							mortgage.loan_loss);

		log_inflow("bank", "mortgages", "deposits", "mortgage_accepted", ID, outstanding_principal);

		MRTG_TOTAL_ACCEPTED_FLOW_COUNT++;
		MRTG_TOTAL_ACCEPTED_CUM_COUNT++;

		if (mortgage.is_variable_rate)
		{
			MRTG_VARIABLE_ACCEPTED_FLOW_COUNT++;
			MRTG_VARIABLE_ACCEPTED_CUM_COUNT++;
		}
		else
		{
			MRTG_FIXED_ACCEPTED_FLOW_COUNT++;
			MRTG_FIXED_ACCEPTED_CUM_COUNT++;
		}

	FINISH_MORTGAGE_ACCEPTED_MESSAGE_LOOP

	//Mean interest rates for the new and for all mortgages; compute interest rate spreads
	compute_mean_interest_rates_all_mortgages();

	//Ratios of accept/offered for variable-rate and fixed-rate mortgages
	compute_acceptance_rates_mortgage_loans();

	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d AFTER MORTGAGE ACCEPTED MESSAGES: MORTGAGE PORTFOLIO SIZE %d", DAY, ID, MORTGAGE_PORTFOLIO.size);		
	}
	#endif

	//Free allocated memory
	free_mortgage_contract_adt(&mortgage);

	/* Process newly created mortgage loan flow:
	 * Here we create a **temporary** deposit account that only exists during the current day, to store the newly created mortgage
	 * NOTE for Dirk: This is where we account for the "RESERVES" that the Buyer's Bank should transfer to the Seller's Bank.
	 * See below: Bank_account_update_deposits(), line 1086, where we account for these "temporarily added" DEPOSITS,
	 * where we compute the mutation = (DEPOSITS -  DEPOSITS_PREVIOUS);
	 */
	asset_liability_add(&PL_MORTGAGE_LOANS,&DEPOSITS, sum_principal_flow);
	
	recompute_mortgage_stocks();

	Bank_update_LLR_mortgage_loans();
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nBank_process_mortgage_accepts(): After MORTGAGE_ACCEPTED_MESSAGE_LOOP");
		printf("\nFlows:");
		printf("\nPrincipal flow\t%f\t(PL MORTGAGES+,DEPOSITS+)", sum_principal_flow);
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
	

	//Print output to file
	#ifdef _DEBUG_MODE        
	if (PRINT_DEBUG_FILES)
	{                       
		filename = malloc(40*sizeof(char));
		filename[0]=0;
		sprintf(filename, "its/Bank_balance_sheet_%d.txt", ID);
		//strcpy(filename, "its/Bank_balance_sheet_%d.txt");      
		file1 = fopen(filename,"a");
		fprintf(file1,"\nIT %d %f %f", DAY,RESERVES,DEPOSITS);
		fclose(file1);
		free(filename);
	}                
    #endif

	return 0;
}
