#include "../../header.h"
#include "../../Bank_agent_header.h"
#include "../../my_library_header.h"
#include "../Credit_Risk_Management/Bank_aux_headers.h"
#include "../../Statistical_Office/balance_sheet_operations.h"

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
    if (PRINT_BANK_MORTGAGE_TABLES)
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
	int i;
	double sum;
	mortgage_contract_adt mortgage;

	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_init_mortgages(): Before one-time dividend payment to distribute new mortgages", DAY, ID);
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
		
		print_bank_balance_sheet();
	}
	#endif
	
	
	// Read mortgage message and save own mortgages to portfolio
	// mortgage_info_message(household_id, bank_id, mortgage_contract_adt mortgage);
	int count=0;
	double sum_principal=0.0;
	START_MORTGAGE_INFO_MESSAGE_LOOP
	if (ID==mortgage_info_message->bank_id)
	{
		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_TABLES)
		{
			if(count==0)
			{
				printf("\n\n ID: %d Bank_init_mortgages", ID);
				printf("\n\t No\t\tID\tobject_id\tduration\tprincipal\tstatus");
			}
		}      
		#endif
			
		if (mortgage_info_message->mortgage.object_id==0)
			fprintf(stderr, "\nIT %d ID %d Bank_init_mortgages(), inside MORTGAGE_INFO_MESSAGE_LOOP: mortgage_info_message->mortgage.object_id=%d", DAY, ID, mortgage_info_message->mortgage.object_id);
			
		//re-initialize local var to zero
		init_mortgage_contract_adt(&mortgage);
			
		//Extract ADT from message (pointer to struct in mesg, and copy to local var using flame inbuild function)
		copy_mortgage_contract_adt( &(mortgage_info_message->mortgage), &mortgage);


		//Add ADT to dynamic array mortgage_portfolio
		add_mortgage_contract_adt(&MORTGAGE_PORTFOLIO,
							mortgage.type,
							mortgage.is_variable_rate,
							mortgage.active,
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

		count++;
		sum_principal += mortgage.outstanding_principal;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_BANK_MORTGAGE_TABLES)
		{
			printf("\n\t%2d\t\t%4d\t\t%4d\t\t%3d\t\t%4.2f\t\t%d", count, mortgage.household_id, mortgage.object_id, mortgage.current_duration, mortgage.outstanding_principal, mortgage.status);
		}      
		#endif
	}
	FINISH_MORTGAGE_INFO_MESSAGE_LOOP

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_MORTGAGE_TABLES)
	{
		printf("\n\t \t \t\t \t \t\t \t \t\t sum principle \t%4.2f\t\t \t", sum_principal);
	}      
	#endif
	
	//free 
	free_mortgage_contract_adt(&mortgage);
	
	for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
		if (MORTGAGE_PORTFOLIO.array[i].object_id==0)
			fprintf(stderr, "\nIT %d ID %d Bank_init_mortgages(), after MORTGAGE_INFO_MESSAGE_LOOP: MORTGAGE_PORTFOLIO.array[%d].object_id=%d", DAY, ID, i, MORTGAGE_PORTFOLIO.array[i].object_id);
		
	
	/******************** At mortgage initialization: Update Bank equity and LLR *************/
	
	//26.5.17: Code as in Bank_read_mortgage_messages()
	//1. Compute sum of outstanding_loans and add to equity
	//2. Compute HH_EL_PL_YEAR and shift from equity to LLR


	sum=0.0;
	for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
		sum  += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

	//Reset and initialize HH_PL_MONTHLY to hold 3 MONTHLY values equal to sum of outstanding mortgages:
	reset_double_array(&HH_PL_MONTHLY); //sets HH_PL_MONTHLY.size=0, but does not clear previous values
	for (i=0; i<3; i++)
		add_double(&HH_PL_MONTHLY, sum);

	//Init PL_MORTGAGE_LOANS
	PL_MORTGAGE_LOANS = sum;
	
	//Recompute HH_EL_PL_YEAR from scratch and subtract initial LLR from equity
	Bank_update_LLR_mortgage_loans();
	
	double dividend_payment = 0.0;

	//TODO: How to distribute the initial PL Mortgage stock?
	int	one_time_dividend_payment = 1;

	if (one_time_dividend_payment)
	{
		//Banks receive as equity: (1-pct) of total amount of initial MORTGAGES plus the amount needed to build-up their LLR:

        //Catch 0 because this will not work (DEPOSITS decrease, dividend is negative)
        // bug?
        //if(CONST_ONE_TIME_DIVIDEND_PCT < EPSILON)
        //   CONST_ONE_TIME_DIVIDEND_PCT = 1.0;
        
        //Banks receive as equity: (1-pct) of total amount of initial MORTGAGES plus the amount needed to build-up their LLR:

        // LLR stuff removed, built-up later, banks potentially go bankrupt
		EQUITY += (1-CONST_ONE_TIME_DIVIDEND_PCT)*sum;
		
		// Households receive the remainder as deposits via dividend
	 	// Percentage that HHs receive on their deposit account (one-time) from the sum amount of initial MORTGAGES
		DEPOSITS += CONST_ONE_TIME_DIVIDEND_PCT*sum;
		dividend_payment = CONST_ONE_TIME_DIVIDEND_PCT*sum;

		add_dividend_info_message(ID, dividend_payment);                  
	}
	else
	{
			//Add total amount for the LLP to equity:
			EQUITY += sum;
	}

	//Eq (19): Initialize the LLR with the expected loss HH_EL_PL_YEAR:
	//EQUITY  			 	-= HH_EL_PL_YEAR;
	//LLR_PL_MORTGAGE_LOANS	 = HH_EL_PL_YEAR;		

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\n IT %d ID: %d Bank_init_mortgages: ALLOCATE EQUITY AND LLR BASED ON HH_EL_PL_YEAR", DAY,ID);
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\t Outstanding mortgage loans: %f HH_EL_PL_YEAR: %f EQUITY: %f LLR MORTGAGES: %f delta: xxx", sum, HH_EL_PL_YEAR, EQUITY, LLR_PL_MORTGAGE_LOANS);
	#endif   
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_init_mortgages(): After one-time dividend payment to distribute new mortgages", DAY, ID);
		printf("\nFlows:");
		printf("\nTotal Mortgage assets\t%8.2f", sum);
		printf("\n--> Equity\t\t%8.2f\t(EQUITY+)", sum-dividend_payment);
		printf("\n--> Dividend\t\t%8.2f\t(DEPOSITS+)", dividend_payment);
		printf("\nLLP PL Mortgages\t%8.2f\t(LLR+,EQUITY-)", HH_EL_PL_YEAR);
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

