#include "../header.h"
#include "../Firm_agent_header.h"
#include "../my_library_header.h"



/** \fn Firm_send_initial_loan()
 * \brief Function to send Firm's initial loan to Bank.
 */
int Firm_send_initial_loan()
{
    //temp firm loan
    firm_loan_adt loan;
    init_firm_loan_adt(&loan);

    loan.bank_id = LOANS.array[0].bank_id;
    loan.loan_id = LOANS.array[0].loan_id;
    loan.outstanding_principal = LOANS.array[0].loan_value;
    loan.interest_rate = LOANS.array[0].interest_rate;
    loan.installment_amount = LOANS.array[0].installment_amount;
    loan.bad_debt = LOANS.array[0].bad_debt;
    loan.nr_periods_before_repayment = LOANS.array[0].nr_periods_before_repayment;

    add_firm_initial_loan_message(ID, loan.bank_id, loan.loan_id, loan.outstanding_principal, loan.interest_rate, loan.installment_amount, loan.bad_debt, loan.nr_periods_before_repayment);

  return 0;   
}

/** \fn Firm_read_initial_loan_id()
 * \brief Function to read initial loan_id.
 */
int Firm_read_initial_loan_id()
{
  #ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_CREDIT)
    {
      printf("\n\n Firm_read_initial_loan_id ID: %d",ID);
      printf("\n\t bank_id\t firm_id\t\t loan_id");
    }              
  #endif

  int count=0;

  START_BANK_INITIAL_LOAN_ID_MESSAGE_LOOP
  if(ID==bank_initial_loan_id_message->firm_id)  
  {
      count++;

      LOANS.array[0].loan_id = bank_initial_loan_id_message->loan_id;

      #ifdef _DEBUG_MODE        
      if (PRINT_DEBUG_CREDIT)
      {
          printf("\n\t%8d\t%8d\t%8d", LOANS.array[0].bank_id, ID, LOANS.array[0].loan_id);
      }              
      #endif
  }
  FINISH_BANK_INITIAL_LOAN_ID_MESSAGE_LOOP

  //Each firm only receives 1 message since only one initial loan exists
  assert(count==1);

  return 0;   
}

/** \fn Firm_send_loan_portfolio_data()
 * \brief Function to send Firm's complete debt portfolio (loan_adts) to the NSA agent for network analysis.
 */
int Firm_send_loan_portfolio_data()
{
  int i;

  // Declare, init financial_data
  financial_data_adt financial_data;
  
  financial_data.equity		= EQUITY;
  financial_data.total_assets	= TOTAL_ASSETS;
  financial_data.total_debt	= TOTAL_DEBT;
  financial_data.equity_asset_ratio = EQUITY_ASSET_RATIO;
  financial_data.debt_equity_ratio = DEBT_EQUITY_RATIO;

  // Send financial_data
  for(i=0;i<LOANS.size;i++)
    add_firm_loan_portfolio_message(ID, financial_data, LOANS.array[i]);

  // Firms with no loans alo send a message
  if(LOANS.size==0)
  {
  	//Create a temporary empty debt_item: loan_adt
  	debt_item loan_adt;
      
  	loan_adt.bank_id=0;
  	loan_adt.loan_value=0;
  	loan_adt.interest_rate=0;
  	loan_adt.installment_amount=0;
  	loan_adt.bad_debt=0;
  	loan_adt.nr_periods_before_repayment=0;

  	//Add empty loan_adt to message
  	add_firm_loan_portfolio_message(ID, financial_data, loan_adt);
  }

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_NSA)
    {
        printf("\n\n Firm_send_loan_portfolio_data ID: %d Sent %d loan_adts (debt_items)", ID, LOANS.size);
    }      
    #endif
      
  return 0;   
}


/*
 * \fn Firm_receive_account_interest()
 * \brief Read account_interest message from Bank. Daily, at start of iteration.
 */
int Firm_receive_account_interest()
{
    double interest =0;
    double interest_rate =0;
    
    /*For validation check*/
    if(DAY%MONTH == 1)
    {
	   RECEIVED_INTEREST_IN_CALENDAR_MONTH=0;
    }
        
    //Read message from bank, set risk-free rate.
    START_ACCOUNT_INTEREST_MESSAGE_LOOP
	if(account_interest_message->bank_id == BANK_ID)
	    interest_rate = account_interest_message->interest_rate;
    FINISH_ACCOUNT_INTEREST_MESSAGE_LOOP
    
    /*Add daily interest on deposits: (1/240) of the interest rate*/
    interest = (1/240.0)*interest_rate*PAYMENT_ACCOUNT;
    PAYMENT_ACCOUNT += interest;
    log_inflow("firm", "payment_account", "net_worth", "pa_interest", ID, interest);

    
    /*For validation check*/
    RECEIVED_INTEREST_DAILY = interest;
    RECEIVED_INTEREST_IN_CALENDAR_MONTH += interest;
    FIRM_INFLOWS_CALENDAR.deposit_interest += interest; 

    return 0;	
}

int Firm_request_loans()
{
/*
	SET_OF_LENDERS: dynamic array of POTENTIAL_LENDER
	POTENTIAL_LENDER ADT: dynamic array: (bank, contact)
	NUM_BANKS: int
	NUMBER_OF_BANKS_ASKED: int
*/
 
	int num_banks=0;
	int j=0;

    if (EXTERNAL_FINANCIAL_NEEDS>0.0)
    {
         //Delete the old set of lenders
		reset_int_array(&SET_OF_LENDERS);


         //Delete the old set of potential lenders
		reset_int_array(&POTENTIAL_LENDERS);
		
        //Search for active banks' name
         START_BANK_IDENTITY_MESSAGE_LOOP
                 add_int(&POTENTIAL_LENDERS,bank_identity_message->bank_id);
                 num_banks++;
         FINISH_BANK_IDENTITY_MESSAGE_LOOP
    
        #ifdef _DEBUG_MODE
            if (PRINT_DEBUG_CREDIT)
            {
                printf("\n\n Firm_request_loans ID: %d",ID);
                printf("\n\t Found NUMBER_OF_BANKS: %d, POTENTIAL_LENDERS.size=%d",num_banks, POTENTIAL_LENDERS.size);
				printf("\n\t POTENTIAL_LENDERS: [");
				for(j=0;j<POTENTIAL_LENDERS.size;j++) printf("%d ",POTENTIAL_LENDERS.array[j]);
				printf("]");

            }
        #endif       
         
        
        //Create bank network for this firm
		NUMBER_OF_BANKS_ASKED=0;
		
          while(POTENTIAL_LENDERS.size>0 && NUMBER_OF_BANKS_ASKED<num_banks && NUMBER_OF_BANKS_ASKED<NUMBER_OF_BANKS_TO_APPLY)
          {	
				// choose random bank id
                j= random_int(0,POTENTIAL_LENDERS.size-1) ;
                add_loan_request_message(ID, POTENTIAL_LENDERS.array[j], EQUITY, TOTAL_DEBT, (EXTERNAL_FINANCIAL_NEEDS+1e-04), MINSKY_STATE);

				//Add bank to set of lenders asked
				add_int(&SET_OF_LENDERS, POTENTIAL_LENDERS.array[j]);

				//Move bank from potential to set of lenders
				remove_int(&POTENTIAL_LENDERS, j);
				NUMBER_OF_BANKS_ASKED++;
          }
	}


        #ifdef _DEBUG_MODE
            if (PRINT_DEBUG_CREDIT)
            {
                printf("\n\t NUMBER_OF_BANKS_ASKED: %d",NUMBER_OF_BANKS_ASKED);
				printf("\n\t SET_OF_LENDERS: [");
				for(j=0;j<SET_OF_LENDERS.size;j++) printf("%d ",SET_OF_LENDERS.array[j]);
				printf("]");
				printf("\n\t POTENTIAL_LENDERS: [");
				for(j=0;j<POTENTIAL_LENDERS.size;j++) printf("%d ",POTENTIAL_LENDERS.array[j]);
				printf("]");

                printf("\n\t EQUITY: %f TOTAL_DEBT: %f EXTERNAL_FINANCIAL_NEEDS: %f",EQUITY,TOTAL_DEBT,EXTERNAL_FINANCIAL_NEEDS);
                
            }
        #endif       

        assert(EQUITY<1e+12);
        assert(EXTERNAL_FINANCIAL_NEEDS<1e+12);

        return 0;
}
     
    
int Firm_accept_loans()
{
  double credit_demand=0.0;
  double credit_offered=0.0;
  double credit_accepted=0.0;
  double total_credit_taken=0.0;

  //Create a local firm_loan_adt
  firm_loan_adt loan;
  init_firm_loan_adt(&loan);

    #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_CREDIT)
    {
        printf("\n\n\t Firm_get_loan REFACTORED");
    }  
    #endif              

    // Resetting on day 1 of calandar month: see Firm_Stat_Functions
    // TOTAL_EXTERNAL_FINANCING_OBTAINED=0.0;

    // Set LAST_INTEREST_RATE_FOR_LOANS only if (credit_accepted>1e-4)
    LAST_INTEREST_RATE_FOR_LOANS = -1e-4;

    //Travers the banks according to the order in the rate_order_array,
    //obtain a loan if credit_demand >= credit_offer
    START_LOAN_CONDITIONS_MESSAGE_LOOP

        credit_offered = loan_conditions_message->amount_offered_credit;

        //Set credit demand
        credit_demand = EXTERNAL_FINANCIAL_NEEDS - total_credit_taken;
        credit_accepted = 0.0;

        if (credit_demand < 0.0 && fabs(credit_demand)>1e-2)
            fprintf(stderr, "\n WARNING in function Firm_get_loan, line 209: credit_demand is NEGATIVE. EXTERNAL_FINANCIAL_NEEDS %f total_credit_taken %f credit_demand %f Firm sets: credit_accepted = 0.0;\n", EXTERNAL_FINANCIAL_NEEDS, total_credit_taken, credit_demand);

        if (credit_demand > 0.0)
        {			
            if (credit_demand >= credit_offered)
                credit_accepted = credit_offered;
            else
                credit_accepted = credit_demand;
        }

             
          //Fill in local firm_loan_adt
          loan.firm_id = ID;
          loan.bank_id = loan_conditions_message->bank_id;
          loan.loan_id = loan_conditions_message->loan_id;
          loan.outstanding_principal = credit_accepted;
          loan.interest_rate = loan_conditions_message->proposed_interest_rate;
          loan.installment_amount = credit_accepted/CONST_INSTALLMENT_PERIODS;
          loan.bad_debt = 0.0;
          loan.nr_periods_before_repayment = CONST_INSTALLMENT_PERIODS+1;

            //ADD LOAN
             if(loan.outstanding_principal < 0.0 && fabs(loan.outstanding_principal)>EPSILON)
              fprintf(stderr,"\n ERROR in function Firm_get_loan: loan_value is NEGATIVE (%f)\n", loan.outstanding_principal);

            //Only take out loans that are sufficiently large
            if (credit_accepted>1e-4)
            {
	            total_credit_taken += credit_accepted;  //Add amount to total_credit_taken, possibly obtain mutltiple loand from different banks

                add_debt_item(&LOANS, loan.bank_id, loan.loan_id, loan.outstanding_principal, loan.interest_rate, loan.installment_amount, loan.bad_debt, loan.nr_periods_before_repayment);

            		//transfer_bank_id: BANK_ID, bank to transfer money to, if different from own bank
                
                //New-style: send debt_item loan
                add_loan_acceptance_message(ID, loan.bank_id, loan, BANK_ID);  

                FIRM_NO_FIRM_LOANS_NEW++;

                LAST_INTEREST_RATE_FOR_LOANS = loan.interest_rate;   

                //update the payment_account with the amount of credit obtained
                PAYMENT_ACCOUNT += credit_accepted;
                log_inflow("firm", "payment_account", "loans", "loan_accepted", ID, credit_accepted);

                FIRM_INFLOWS_CALENDAR.new_loans += credit_accepted;
                TOTAL_EXTERNAL_FINANCING_OBTAINED += credit_accepted;
            }				

        	#ifdef _DEBUG_MODE        
            if (PRINT_DEBUG_CREDIT)
            {
                printf("\n\n Firm_get_loan ID: %d",ID); 
                printf("\n\t Bank id: %d credit_accepted: %f interest_rate: %f", loan.bank_id, credit_accepted, loan.interest_rate);
            }
        	#endif

    FINISH_LOAN_CONDITIONS_MESSAGE_LOOP

    //Sense check
    assert__(TOTAL_EXTERNAL_FINANCING_OBTAINED < EXTERNAL_FINANCIAL_NEEDS + 1e-3)
    {
      printf("\n\n ERROR in %s/%s(), line %d", __FILE__, __FUNCTION__, __LINE__); 
      printf("\n\t Firm %d TOTAL_EXTERNAL_FINANCING_OBTAINED %f EXTERNAL_FINANCIAL_NEEDS %f", ID, TOTAL_EXTERNAL_FINANCING_OBTAINED, EXTERNAL_FINANCIAL_NEEDS);
    }

    if (PAYMENT_ACCOUNT >= TOTAL_FINANCIAL_NEEDS)
    {           
              //printf("Firm_financial_needs, External financing: case 1.");
      EXTERNAL_FINANCIAL_NEEDS = 0.0;                   
    }
    else
    {
              //external financing needed
      EXTERNAL_FINANCIAL_NEEDS = TOTAL_FINANCIAL_NEEDS - PAYMENT_ACCOUNT;
    }

      #ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_CREDIT)
    {
      printf("\n\n Firm_get_loan ID: %d",ID); 
      printf("\n\t total_credit_taken: %f", total_credit_taken);
      printf("\n\t PAYMENT_ACCOUNT: %f EXTERNAL_FINANCIAL_NEEDS: %f",PAYMENT_ACCOUNT,EXTERNAL_FINANCIAL_NEEDS);
    }                  
      #endif

    return 0;
}
    
int Firm_idle()
{
    return 0;
}
