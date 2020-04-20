#include "../../header.h"
#include "../../Household_agent_header.h"
#include "../../my_library_header.h"

#include "../Housing_Market/centralized_auction/Household_aux_header.h"
#include "../../Sanity_Checks/sanity_checks_aux_headers.h"
#include "Household_aux_headers.h"

/***************** TEMPLATE ****************/
/** \fn Household_TEMPLATE()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
int Household_TEMPLATE()
{

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MORTGAGE_HH)
    {
        printf("\n\n Household_ ID: %d", ID);
    }      
    #endif
      
 return 0;   
}
/***************** END OF TEMPLATE ****************/

/** \fn Household_check_mortgage_conditions()
 * \brief Function to check the mortgage conditions.
 * \brief Mortgage NPL is triggered if ARREARS_COUNTER==3.
 * \brief Mortgage default is triggered if NPL_COUNTER==3.
 * \brief NOTE: Order is first to check NPL, then arrears. 
 * \brief UNITTEST: PASS 6.12.16
 */
int Household_check_mortgage_conditions()
{
	int i;

	// Reset status variables, will be set later
	HMR_MORTGAGE_STATUS = 0;
	OP_MORTGAGE_STATUS = 0;


	//Reset indicators if both HMR and Other Property have no mortgages:
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
		HAVE_HMR_MORTGAGE = 1;
	else
		HAVE_HMR_MORTGAGE = 0;

	SHARE_OP_WITH_MORTGAGE = 0.0;

	HAVE_OP_MORTGAGE = 0;
	for (i=0; i< HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
		{
			HAVE_OP_MORTGAGE = 1;
			SHARE_OP_WITH_MORTGAGE++;
		}
	}

	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size > 0)
		SHARE_OP_WITH_MORTGAGE = SHARE_OP_WITH_MORTGAGE / HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;

	if (HAVE_HMR_MORTGAGE == 0 && HAVE_OP_MORTGAGE == 0)
		HAVE_MORTGAGE = 0;
	else
		HAVE_MORTGAGE = 1;

/*
	if (HAVE_MORTGAGE)
	{
		printf("\nHH %d\tHMR %d\tOP %d", ID, HAVE_HMR_MORTGAGE, HAVE_OP_MORTGAGE);
	}
*/	
	//NOTE: Order is important:
	//1. Cure to previous state, 2. Trigger to next state, 3. Remain in same state.
	
	//Check HMR NPL status
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 2)
	{

		HMR_MORTGAGE_STATUS = 2;

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH)
		{
			printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
			printf("\n\t HMR_MORTGAGE_STATUS = %d (NPL)", HMR_MORTGAGE_STATUS);
		}      
		#endif

		//Cure NPL status to arrears status: not possible
		/*
		if (Condition && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter < 3)
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 1;
			
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
			
			HMR_MORTGAGE_STATUS = 0;
		}
		*/

		//Trigger writeoff:
		//Use probability to remain in NPL to determine if loan transitions into WRITE-OFF status:
		if (random_unif() < 1-PROB_MORTGAGE_STAYS_IN_NPL)
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 3;
			HMR_MORTGAGE_STATUS = 3;
		}

		//Update NPL counter
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status==2) 
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter++;
		}	
	}
	

	//Loop over all OP mortgages in array
	for (i=0;i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		//Check Other property NPL status
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status == 2)
		{
			OP_MORTGAGE_STATUS = 2; //TODO: is this correct?

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
				printf("\n\t OP_MORTGAGE_STATUS = %d (NPL)", OP_MORTGAGE_STATUS);
			}      
			#endif

			//Cure NPL status to arrears status:
			//5.5.17: Currently not possible
			/*
			if (Condition && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter < 3)
			{
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 1;
				
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter = 0;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter = 0;
				
				OP_MORTGAGE_STATUS = 1;
			}
			*/

			//Trigger writeoff:
			//Use probability to remain in NPL to determine if loan transitions into WRITE-OFF status:
			if (random_unif() < 1-PROB_MORTGAGE_STAYS_IN_NPL)
			{
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 3;
				OP_MORTGAGE_STATUS = 3; //TODO: is this correct?
				
				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_MORTGAGE_HH)
				{
					printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
					printf("\n\t OP_MORTGAGE_STATUS = %d (D)", OP_MORTGAGE_STATUS);
				}      
				#endif
			}

			//Update NPL counter
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status==2) 
			{
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter++;
			}	
		}
	}
	
	
	//Check HMR arrears_status
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 1)
	{
		HMR_MORTGAGE_STATUS = 1;
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH)
		{
			printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
			printf("\n\t HMR_MORTGAGE_STATUS = %d (AR)", HMR_MORTGAGE_STATUS);
		}      
		#endif

		//Cure arrears status to normal status:
		//5.5.17: Currently not possible
		/*
		if (Condition && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter<MAX_DURATION_MORTGAGE_LOANS_ARREARS)
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
			
			HMR_MORTGAGE_STATUS = 0;
		}
		*/
		
		//Trigger NPL status:
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter == MAX_DURATION_MORTGAGE_LOANS_ARREARS)
		{
			vlog_event(ID,"hmr_npl");
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 2;
			HMR_MORTGAGE_STATUS = 2;

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter = 0;
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
				printf("\n\t HMR_MORTGAGE_STATUS = %d (NPL)", HMR_MORTGAGE_STATUS);
			}      
			#endif
		}
		else
		{
			//Remain in arrears status, Update arrears counter --> done in Household_service_mortgage_debt()
			//if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS) 
			//	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter++;
		}
	}
	
	//Loop over all OP mortgages in array
	for (i=0;i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{

		//Check Other property arrears status
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status == 1)
		{
			OP_MORTGAGE_STATUS = 1;
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
				printf("\n\t OP_MORTGAGE_STATUS = %d (AR)", OP_MORTGAGE_STATUS);
			}      
			#endif
			
			//Cure arrears status to normal status:
			//5.5.17: Currently not possible
			/*
			if ( Condition && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
			{
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 0;

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter = 0;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter = 0;
				
				OP_MORTGAGE_STATUS = 0;
			}
			*/

			//Trigger NPL status:
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter == MAX_DURATION_MORTGAGE_LOANS_ARREARS)
			{
				OP_MORTGAGE_STATUS = 2;
				vlog_event(ID,"op_npl");
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 2;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter = 0;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter = 0;

				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_MORTGAGE_HH)
				{
					printf("\n\n ID: %d Household_check_mortgage_conditions", ID);
					printf("\n\t OP_MORTGAGE_STATUS = %d (NPL)", OP_MORTGAGE_STATUS);
				}      
				#endif
			}

			//Remain in arrears status, Update arrears counter --> done in Household_service_mortgage_debt()
			//if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS) 
			//	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter++;
		}
	}

	return 0;   
}

/** \fn Household_service_mortgage_debt()
 * \brief Function to execute monthly mortgage debt servicing.
 * \brief Condition: MORTGAGE_STATUS = 0,1
 * \brief - Mortgage main residence: 
 * \brief - repay principal and interest
 * \brief - reduce debt burden
 * \brief - check if full repayment: reset mortgage_contract_adt
 * \brief - Mortgage other property: idem
 * \brief Message input: None
 * \brief Message output: 
 * \brief - add_mortgage_payment_message(ID, bank_id, mortgage)
 * \brief - add_mortgage_arrears_message(ID, bank_id, mortgage);
 * \brief UNITTEST: PASS 6.12.16
 */
int Household_service_mortgage_debt()
{
 	Household_service_mortgage_debt_hmr();

	Household_service_mortgage_debt_op();

	return 0;   
}


/** \fn Household_full_repayment_mortgage_debt()
 * \brief Function to execute full repayment of mortgage debt, for example after HMR or OP sale.
 * \brief Condition: MORTGAGE_STATUS = 0,1
 * \brief - Mortgage main residence: 
 * \brief - repay principal and interest 
 * \brief - full repayment: reset mortgage_contract_adt
 * \brief - Mortgage other property:
 * \brief - repay principal and interest 
 * \brief - full repayment: reset mortgage_contract_adt
 * \brief Message input: None
 * \brief Message output: 
 * \brief - add_mortgage_payment_message(ID, bank_id, mortgage)
 * \brief UNITTEST: NOT TESTED
 */
int Household_full_mortgage_payment()
{
	int i __attribute__((unused)) = 0;
	int found __attribute__((unused)) = 0;

	int bank_id;
	double principal;
	double interest;

	//Mortgage main residence
	if (JUST_SOLD_HMR)
	{
		if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_full_mortgage_payment", ID);
			}      
			#endif

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d FULL REPAY", DAY, ID);
			}      
			#endif

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
					//printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
				printf("\n\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
				printf("\nID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance");
			}      
			#endif

			bank_id   = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
			principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;

			if (!FREE_EARLY_REPAYMENT)
				interest = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest;
			else
				interest = 0.0;

			//Case: normal
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\t HMR_MORTGAGE_STATUS = %d (PL)", HMR_MORTGAGE_STATUS);
			}      
			#endif

	   		if (PAYMENT_ACCOUNT - (principal + interest) < -0.01)
			{
				fprintf(stderr, "\n\nIT %d ID %d Household_full_mortgage_payment (JUST_SOLD_HMR):", DAY, ID);
				fprintf(stderr, "\n\t Before payment: PAYMENT_ACCOUNT=%.2f", PAYMENT_ACCOUNT);
				fprintf(stderr, "\n\t Mortgage outstanding: %f", principal + interest);
				fprintf(stderr, "\n\t After: PAYMENT_ACCOUNT < -0.01, PAYMENT_ACCOUNT=%.2f", PAYMENT_ACCOUNT - (principal + interest));
				fprintf(stderr, "\n\t HMR value %f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
			}

			//Reduce account
			PAYMENT_ACCOUNT -= principal + interest;
			log_outflow("household", "payment_account", "mortgages", "mortgage_principal", ID, principal);
			log_outflow("household", "payment_account", "net_worth", "mortgage_interest", ID, interest);
			
	        //Sanity Check:
	        enforce_hh_valid_payment_account("Household_full_mortgage_payment");

			add_full_mortgage_payment_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);



	        HH_MORTGAGES_PAIDOFF++;
    	    HH_MORTGAGES_HMR_PAIDOFF++;
	    }

        SOLD_OBJECT_ID = -1;
        JUST_SOLD_HMR = 0;
		HAVE_HMR_MORTGAGE = 0;

		//Remove HMR completely
		//Reinitialize HMR adt to zero values (no explicit freeing of memory)
		init_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence);

		//State transition
		IS_OWNER = 0;
		IS_HOMELESS = 1;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_HOUSING_BRANCHES)
        {
            printf("\n\nIT %d ID: %d Household_full_mortgage_payment (3): After full_mortgage_payment", DAY, ID);
            Household_print_housing_states();
        }
        #endif
	}
	
	if (JUST_SOLD_OP)
	{
		//Find object
		found = 0;
		for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
		{
			if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==SOLD_OBJECT_ID)
			{
				found=1;
				break;
			}
		}

		if(!found)
			fprintf(stderr, "OP sold not found! [Household_full_mortgage_payment] (ITS %d, ID %d, SOLD_OBJECT_ID %d\n", DAY, ID, SOLD_OBJECT_ID);

		assert(found);

		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
		{
	     
	        #ifdef _DEBUG_MODE    
	        if (PRINT_DEBUG_MORTGAGE_HH)
	        {
	            printf("\n\n ID: %d Household_full_mortgage_payment. OP mortgage object %d index %d", ID, SOLD_OBJECT_ID, SELL_OP_OBJECT_INDEX);
	        }      
	        #endif
	        
	        #ifdef _DEBUG_MODE    
	        if (PRINT_MORTGAGE_INFO)
	        {
	            //printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
	            printf("\n\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
	            printf("\nID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance");
	        }      
	        #endif
	        
	        bank_id   = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;
	        principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;

	        if (!FREE_EARLY_REPAYMENT)
	        	interest = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest;
	        else
				interest = 0.0;
	        
	        #ifdef _DEBUG_MODE    
	        if (PRINT_DEBUG_MORTGAGE_HH)
	        {
	            printf("\n\t OP_MORTGAGE_STATUS = %d (PL)", OP_MORTGAGE_STATUS);
	        }      
	        #endif

	   		if (PAYMENT_ACCOUNT - (principal + interest) < -0.01)
			{
				fprintf(stderr, "\n\nIT %d ID %d Household_full_mortgage_payment (JUST_SOLD_OP):", DAY, ID);
				fprintf(stderr, "\n\t Before payment: PAYMENT_ACCOUNT=%.2f", PAYMENT_ACCOUNT);
				fprintf(stderr, "\n\t Mortgage outstanding: %f", principal + interest);
				fprintf(stderr, "\n\t After: PAYMENT_ACCOUNT < -0.01, PAYMENT_ACCOUNT=%.2f", PAYMENT_ACCOUNT - (principal + interest));
				fprintf(stderr, "\n\t OP value %f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price);
			}

	        //Reduce account
	        PAYMENT_ACCOUNT -= principal + interest;
			log_outflow("household", "payment_account", "mortgages", "mortgage_principal", ID, principal);
			log_outflow("household", "payment_account", "net_worth", "mortgage_interest", ID, interest);

            //Sanity Check:
	        enforce_hh_valid_payment_account("Household_full_mortgage_payment");

	        add_full_mortgage_payment_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);

    	    HH_MORTGAGES_PAIDOFF++;    	    
        	HH_MORTGAGES_OP_PAIDOFF++;

		} //if mortgage.active==1

		//Remove rental_unit at index object_index
        remove_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, i);

		//State transition
        if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size == 0)
        	IS_LANDLORD = 0;

		//This is now done later in Household_check_rental_contract, after reading the potential CONFLICT with tenant_cancel_rental_contract_message (to check for special case)
        //SOLD_OBJECT_ID = -1;
		CHECK_SPECIAL_CASE = 1;

        SELL_OP_OBJECT_ID = -1;
        SELL_OP_OBJECT_INDEX = -1;
		JUST_SOLD_OP = 0;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_HOUSING_BRANCHES)
        {
            printf("\n\nIT %d ID: %d Household_full_mortgage_payment (3): After full_mortgage_payment", DAY, ID);
            Household_print_housing_states();
        }
        #endif
    } //if JUST_SOLD_OP

	//State transition
	HAVE_OP_MORTGAGE = 0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
			{
				HAVE_OP_MORTGAGE = 1;
				break;
			}
	}
    
	return 0;   
}

/** \fn Household_npl_mortgage_debt()
 * \brief Function to send NPL mesg to bank; HH is non-performing on mortgage debt.
 * \brief Condition: MORTGAGE_STATUS = 2
 * \brief Message input: None
 * \brief Message output: 
 * \brief - add_mortgage_npl_message(ID, bank_id, loan_loss, loan_default, npl_counter)
 * \brief UNITTEST: PASS 6.12.16
 */
int Household_npl_mortgage_debt()
{
	int i;
	int bank_id;
	int object_id;
	int npl_counter;
	double loan_default;
	double loan_loss;
	double value;

	//Check HMR NPL status
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active
		&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 2)
	{
		object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id;
		
		if (object_id==0)
			fprintf(stderr, "\nIT %d ID %d Household_npl_mortgage_debt(), inside HMR NPL status == 2: HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id=%d", DAY, ID, object_id);
		
		npl_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter;		
		bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
		
		//NOTE: loan_default includes total outstanding principal (no interest)
		loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
		
		loan_loss = max(0.0, loan_default - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
		
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_default = loan_default;
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_loss 	 = loan_loss;

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH)
		{
			printf("\n\n ID: %d Household_npl_mortgage_debt", ID);
			printf("\n\t HMR_MORTGAGE_STATUS = %d (NPL)", HMR_MORTGAGE_STATUS);
		}      
		#endif
		
		#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d SENDING NPL MESSAGE (HMR object %d) -> Bank %d npl_counter %d", DAY, ID, object_id, bank_id, npl_counter);
		}      
		#endif
		
		//NOTE: NPL mesg is re-send each day
		add_mortgage_npl_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
		//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id);

		HH_MORTGAGES_HMR_NPL++;
		HH_MORTGAGES_NPL++;
	}	
	

	//Check Other property NPL status
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active 
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status == 2)
		{
			object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			if (object_id==0)
				fprintf(stderr, "\nIT %d ID %d Household_npl_mortgage_debt(), inside OP NPL status == 2: HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id=%d", DAY, ID, object_id);
			
			npl_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter;
			
			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;
			
			//NOTE: loan_default includes total outstanding principal (no interest)
			loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;

			value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;

			loan_loss = max(0.0, loan_default - value);
			
			//Setters
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_default = loan_default;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_loss 	= loan_loss;

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_npl_mortgage_debt", ID);
				printf("\n\t OP_MORTGAGE_STATUS = %d (NPL)", OP_MORTGAGE_STATUS);
			}      
			#endif

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d SENDING NPL MESSAGE (OP object %d [%d/%d]) -> Bank %d npl_counter %d", DAY, ID, object_id, i+1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, bank_id, npl_counter);
			}      
			#endif

			//NOTE: NPL mesg is re-send each day
			add_mortgage_npl_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
			//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);

			HH_MORTGAGES_OP_NPL++;
			HH_MORTGAGES_NPL++;
		}
	}	
	return 0;   
}

/** \fn Household_writeoff_mortgage_debt()
 * \brief Function to execute mortgage default functions.
 * \brief Condition: MORTGAGE_STATUS = 3
 * \brief Condition: NPL counter has reached maximum (by default 24 months)
 * \brief Message input: None
 * \brief Message output: 
 * \brief - add_mortgage_writeoff_message(ID, bank_id, loan_loss, loan_default, seized_liquidity)
 * \brief UNITTEST: PASS 6.12.16
 */
int Household_writeoff_mortgage_debt()
{
	int i;
	int bank_id;
	int object_id;
	double value __attribute__((unused)) = 0.0;

	//Default on Mortgage other property
	for (i=HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size-1; i>=0; i--)		//NOTE: reversed for-loop for removal from END!!
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status == 3)
		{
			
			value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
			
			bank_id	 = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\n ID: %d Household_writeoff_mortgage_debt", ID);
				printf("\n\t OP_MORTGAGE_STATUS = %d (D)", OP_MORTGAGE_STATUS);
			}      
			#endif
			
			//Foreclosure on property at index i:
			foreclosure_adt foreclosed_house;
			init_foreclosure_adt(&foreclosed_house);
			object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			if (object_id==0)
				fprintf(stderr, "\nIT %d ID %d Household_writeoff_mortgage_debt(), inside OP write-off status == 3: HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id=%d", DAY, ID, object_id);
			
			copy_real_estate_adt(&(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i]), &foreclosed_house.seized_house);
			foreclosed_house.seized_house.owner_id = ID;
			foreclosed_house.seized_liquidity = 0;
			foreclosed_house.seizure_day = DAY;
						
			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d SENDING WRITEOFF MESSAGE (OP object %d [%d/%d]) -> Bank %d", DAY, ID, object_id, i+1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, bank_id);
			}      
			#endif
			
			//NOTE: writeoff mesg is send only once
			add_mortgage_writeoff_message(ID, bank_id, foreclosed_house);


			log_transformation("household", "mortgages", "net_worth", "mortgage_writeoff", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal);

			HH_MORTGAGES_OP_WRITEOFF++;
			HH_MORTGAGES_WRITEOFF++;
			
			//Free
			free_foreclosure_adt(&foreclosed_house);
			
			// Remove tenants from the rental (eviction notice for next month, so tenant should still pay this month's rent)
			int tenant_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id;
			add_landlord_cancel_rental_contract_message(tenant_id, ID, object_id);
			
			#ifdef _DEBUG_MODE    
			if (PRINT_RENTAL_INFO)
			{
				printf("\nIT %d ID %d SENDING LANDLORD CANCEL RENTAL CONTRACT (ID: %d | T: %d L: %d, O: %d)", DAY, ID, ID, tenant_id, ID, object_id);
			}
			#endif

			//Remove item from array
			remove_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, i);
		}


	//Default on Mortgage main residence
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active
		&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 3)
	{
		bank_id 		 = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH)
		{
			printf("\n\n ID: %d Household_writeoff_mortgage_debt", ID);
			printf("\n\t HMR_MORTGAGE_STATUS = %d (D)", HMR_MORTGAGE_STATUS);
		}      
		#endif
				
		//foreclosure_adt		
		foreclosure_adt foreclosed_house;
		init_foreclosure_adt(&foreclosed_house);

		copy_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence, &foreclosed_house.seized_house);
		foreclosed_house.seized_house.owner_id = ID;
		foreclosed_house.seized_liquidity = 0;
		foreclosed_house.seizure_day = DAY;

		//Writing off the mortgage
		
		#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
		{
			printf("\nIT %d ID %d SENDING WRITEOFF MESSAGE (HMR object %d) -> Bank %d", DAY, ID, foreclosed_house.seized_house.object_id, bank_id);
		}      
		#endif
		
		//NOTE: writeoff mesg is send only once
		add_mortgage_writeoff_message(ID, bank_id, foreclosed_house);
		log_transformation("household", "mortgages", "net_worth", "mortgage_writeoff", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);


		
		HH_MORTGAGES_HMR_WRITEOFF++;
		HH_MORTGAGES_WRITEOFF++;

		//Re-initialize values of the mortgage_contract_adt to zero
		init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
		init_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence);

		free_foreclosure_adt(&foreclosed_house);

		// mortgage and house gone
		HAVE_HMR_MORTGAGE = 0;
		IS_OWNER = 0;
		IS_HOMELESS = 1;
		EVICTED_AS_OWNER = 1;
	}
	
	// Prevent a HH that has just written off some mortgage to get a new mortgage
	// This prevents the function Household_mortgage_loan_application() from running.
	if (HH_MORTGAGES_WRITEOFF)
	   REQUEST_MORTGAGE = 0;

	return 0;   
}


/** \fn Household_mortgage_loan_application()
 * \brief Function to apply for a mortgage loan.
 * \brief See: Household_sample_mortgage_attributes() in Housing_Market for the sampled mortgage attributes (DSTI,LTV,M)
 * \brief See: Bank_process_mortgage_requests() for the resulting mortgage attributes (value_at_acquisition, initial_principal, own_contribution)
 * \brief Message input: None
 * \brief Message output:
 * \brief - add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
 * \brief UNITTEST: PASS 23.5.17
 */
int Household_mortgage_loan_application()
{
	double annuity=0.0;
	
	mortgage_application_adt mortgage;

	init_mortgage_application_adt(&mortgage);


	/************** MORTGAGE SAMPLING CODE WAS HERE, SEE HH DECISION TO RENT VS. BUY HMR AND OP: Household_sample_mortgage_attributes()  in Housing_Market  ********/
	//26.7.2017, SvdH: New derivation from paper, eqn (10-15)
	//REF: Model description paper

	//Set annuity (monthly mortgage payment)
	// Eq (6)
	annuity = MORTGAGE_SAMPLING_DRAW.dsti * MEAN_NET_INCOME;

	assert(annuity>0);

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n\nMORTGAGE_SAMPLING_DRAW.dsti: %f, MEAN_NET_INCOME: %f, annuity: %f, TOTAL_GROSS_ANNUAL_INCOME: %f, annuity: %f", MORTGAGE_SAMPLING_DRAW.dsti, MEAN_NET_INCOME, annuity, TOTAL_GROSS_ANNUAL_INCOME, annuity);
	}      
	#endif
	
	mortgage.annuity 				= annuity;
	mortgage.sampled_loan_to_value	= MORTGAGE_SAMPLING_DRAW.loan_to_value;
	mortgage.sampled_maturity		= MORTGAGE_SAMPLING_DRAW.maturity*12; //Monthly
	
	//add_mortgage_request_message(household_id, bank_id, mortgage_application_adt mortgage);
	add_mortgage_request_message(ID, MORTGAGES_NEXT_BANK_ID, mortgage);
	HH_MORTGAGES_REQUESTED++;
	
	if (PLAN_BUY_OP)
		HH_MORTGAGES_OP_REQUESTED++;
	if (PLAN_BUY_HMR)
		HH_MORTGAGES_HMR_REQUESTED++;


	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d SENDING MORTGAGE REQUEST MESSAGE", DAY, ID);
	}      
	#endif

	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		printf("\nIT %d ID %d MORTGAGE APPLICATION: mortgage.annuity %f, mortgage.sampled_loan_to_value %f, mortgage.sampled_maturity %d", DAY, ID, mortgage.annuity, mortgage.sampled_loan_to_value, mortgage.sampled_maturity);
	}      
	#endif
  
  	//free
  	free_mortgage_application_adt(&mortgage);

	return 0;   
}

/** \fn Household_receive_mortgage_loan()
 * \brief Function to
 * \brief Message input:
 * \brief - mortgage_offer_message(bank_id, household_id, mortgage_contract_adt mortgage);
 * \brief UNITTEST: TODO
 */
int Household_receive_mortgage_loan()
{
	//Assume: MORTGAGE is of type mortgage_contract_adt
	START_MORTGAGE_CONTRACT_MESSAGE_LOOP
		//Mortgage for puchasing HMR
		if (PLAN_BUY_HMR)
		{
			//Add mortgage contract to HMR mortgage (copy)
			copy_mortgage_contract_adt(&(mortgage_contract_message->mortgage), &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence);
			
			HAVE_MORTGAGE = 1;
		    HMR_MORTGAGE_GRANTED = 1;
		    HAVE_HMR_MORTGAGE = 1;

		   	#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
			    printf("MORTGAGE GRANTED (HMR) HH %d (principal %f)\n", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.outstanding_principal);
			}      
			#endif
		}

		//Mortgage for purchasing OP		
		if (PLAN_BUY_OP)
		{
			//Add ADT to initial_mortgage_other_property in stocks: temporary placeholder for mortgage info, see Household_accept_mortgage_loan() below for copy into other_property.array[new].mortgage
			copy_mortgage_contract_adt( &(mortgage_contract_message->mortgage), &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);

			HAVE_MORTGAGE = 1;
			HAVE_OP_MORTGAGE = 1;
	    	OP_MORTGAGE_GRANTED = 1;

		   	#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
			    printf("MORTGAGE GRANTED (OP) HH %d (principal %f)\n", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal);
			}      
			#endif
		}		
	FINISH_MORTGAGE_CONTRACT_MESSAGE_LOOP
	
	return 0;   
}

/** \fn Household_accept_mortgage_loan()
 * \brief Function to let HHs send msg to Bank to accept mortgage loan
 * \brief Message output:
 * \brief - mortgage_accepted_message(bank_id, household_id, mortgage_contract_adt mortgage);
 * \brief UNITTEST: TODO
 */
int Household_accept_mortgage_loan()
{
	int bank_id=0;	

	if (HMR_MORTGAGE_GRANTED)
	{
		if (JUST_PURCHASED_HMR)
		{
			HMR_MORTGAGE_ACCEPTED = 1;

			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
			
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.type = 1; //1: HMR

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active = 1;	//Activate Mortgage

			add_mortgage_accepted_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
			log_inflow("household", "payment_account", "mortgages", "mortgage_accepted", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);

			HH_MORTGAGES_ACCEPTED++;
			HH_MORTGAGES_HMR_ACCEPTED++;

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nHMR MORTGAGE ACCEPTED %d", ID);
				printf("\nIT %d ID %d SENDING ACCEPTED MESSAGE", DAY, ID);
			}      
			#endif
		}
		else
		{
			HMR_MORTGAGE_DELETED = 1;

			// Reset mortgage contract (was added at moment of securing funds)
			init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nHMR MORTGAGE DELETED %d", ID);
			}      
			#endif			
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
		{
			printf("\n\nIT %d ID: %d Household_accept_mortgage_loan (4): HMR_MORTGAGE_GRANTED=%d", DAY, ID, HMR_MORTGAGE_GRANTED);
			Household_print_housing_states();
		}
	    #endif
	}

	if (OP_MORTGAGE_GRANTED)
	{
		if (JUST_PURCHASED_OP)
		{
			OP_MORTGAGE_ACCEPTED = 1;

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.type = 2; //OP

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.active=1;

			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.bank_id;
			
			add_mortgage_accepted_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);
			log_inflow("household", "payment_account", "mortgages", "mortgage_accepted", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal);
			
			//Reset info in initial_mortgage_other_property
			init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);
			
			HH_MORTGAGES_ACCEPTED++;
			HH_MORTGAGES_OP_ACCEPTED++;

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nOP MORTGAGE ACCEPTED %d", ID);
				printf("\nIT %d ID %d SENDING ACCEPTED MESSAGE", DAY, ID);
			}      
			#endif
		}
		else
		{
			OP_MORTGAGE_DELETED = 1;

			// Reset mortgage contract (was added at moment of securing funds)
			init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nOP MORTGAGE DELETED %d", ID);
			}      
			#endif
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
		{
			printf("\n\nIT %d ID: %d Household_accept_mortgage_loan (4): OP_MORTGAGE_GRANTED=%d", DAY, ID, OP_MORTGAGE_GRANTED);
			Household_print_housing_states();
		}
	    #endif
	}


	//Reset all flags: this was the last step of the mortgage process

	HMR_MORTGAGE_GRANTED  = 0;
	JUST_PURCHASED_HMR 	  = 0;
	HMR_MORTGAGE_ACCEPTED = 0;
	HMR_MORTGAGE_DELETED  = 0;

	OP_MORTGAGE_GRANTED  = 0;
	JUST_PURCHASED_OP 	 = 0;
	OP_MORTGAGE_ACCEPTED = 0;
	OP_MORTGAGE_DELETED  = 0;


// See function: Household_check_mortgage_conditions()

	int i;

	// Reset status variables, will be set later
	HMR_MORTGAGE_STATUS = 0;
	OP_MORTGAGE_STATUS = 0;
	
	//Reset indicators if both HMR and Other Property have no mortgages:
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
		HAVE_HMR_MORTGAGE = 1;
	else
		HAVE_HMR_MORTGAGE = 0;

	HAVE_OP_MORTGAGE = 0;
	for (i=0; i< HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
			HAVE_OP_MORTGAGE = 1;
	}

	if (HAVE_HMR_MORTGAGE == 0 && HAVE_OP_MORTGAGE == 0)
		HAVE_MORTGAGE = 0;
	else
		HAVE_MORTGAGE = 1;

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
		{
			printf("\n\nIT %d ID: %d Household_accept_mortgage_loan (6):", DAY, ID);
			Household_print_housing_states();
		}
	    #endif


	return 0;
}
