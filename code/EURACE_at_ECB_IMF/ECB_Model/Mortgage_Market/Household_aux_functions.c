#include "../../header.h"
#include "../../Household_agent_header.h"
#include "../../my_library_header.h"
#include "Household_aux_headers.h"
#include "../Housing_Market/centralized_auction/Household_aux_header.h"
#include "../../Sanity_Checks/sanity_checks_aux_headers.h"

/** \fn recompute_mortgage_attributes(mortgage_contract_adt * mortgage)
 * \brief Function to redo mortgage calculation to update values each month
 * \brief Usage: recompute_mortgage_attributes(&mortgage);
 * \brief Example: recompute_mortgage_attributes(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.mortgage_main_residence);
 */
void recompute_mortgage_attributes(mortgage_contract_adt * mortgage)
{

	double monthly_principal;
	double monthly_interest;

	/** Get values from current mortgage **/
	int initial_duration  = (*mortgage).initial_duration;
	int current_duration = (*mortgage).current_duration;

	double a = (*mortgage).monthly_total_repayment;
	double i = (*mortgage).interest_rate_annual; //decimal form, annualized interest

	double outstanding_principal = (*mortgage).outstanding_principal;
	
	double loan_balance_outstanding = outstanding_principal;

	/** Compute new values **/
	int m = initial_duration; //months

	//Eqn (11): original_principal is needed for Eqn (13) and Eqn (14) below
	double original_principal = a * (pow(1+i/12, m) -1)/pow(1+i/12, m) / (i/12);

	// Counter for repayment schedule: s=0,...,m
	int s = initial_duration - current_duration;

	// Set initial values at s==0
	if (s==0)
	{
		//Eqn (13): loan balance outstanding (recomputed each month), given initial principle (sampled), annuity (eqn 11), interest rate (eqn 10)
		if (i>0.0)
			loan_balance_outstanding = original_principal * pow( 1+i/12, s) - (a/(i/12)) * (pow(1+i/12, s) -1);
		else
            loan_balance_outstanding = a*initial_duration;

		(*mortgage).outstanding_principal = loan_balance_outstanding;

		if (i>0.0)
		{
			(*mortgage).outstanding_interest = initial_duration * a - original_principal;
		}
		else
			(*mortgage).outstanding_interest = 0.0;

		monthly_principal = 0.0; //do not pay in first period
		monthly_interest = 0.0;
	}

	if (s>0 && s<=initial_duration) //Defensive: stop when s>initial_duration, to prevent negative principal+interest payments (should never happen since no mortgage should have a negative current_duration)
	{
		s=s-1;	// s=0 gives you the first payment, so we have to adjust s here.
		
		//Eqn (13): loan balance outstanding (recomputed each month), given initial principle (sampled), annuity (eqn 11), interest rate (eqn 10)
		if (i>0.0)
			loan_balance_outstanding = original_principal * pow( 1+i/12, s) - (a/(i/12)) * (pow(1+i/12, s) -1);
		else
			loan_balance_outstanding = a*current_duration;


		//Eqn (14): monthly interest payment, recomputed each month given outstanding balance (eqn 13), interest rate (eqn 10) 
		monthly_interest = (i/12) * loan_balance_outstanding;


		//Eqn (15): monthly principal payment,  recomputed each month given annuity (time-invariant, eqn 11),  monthly interest payment (eqn 14)
		monthly_principal = a - monthly_interest;      
    }
	
	/** Set values as computed above **/
	(*mortgage).monthly_principal = monthly_principal;
	(*mortgage).monthly_interest = monthly_interest;

/*
	if((*mortgage).object_id==1004)
	{
		printf("\n*** mortgage %d ***\n", (*mortgage).object_id);
		printf("a: %f, r: %f\n",a, i);
		printf("duration: %d/%d\n",current_duration,initial_duration);
		printf("monthly principal/interest: %f, %f\n",(*mortgage).monthly_principal,(*mortgage).monthly_interest);
		printf("outstanding principal/interest: %f, %f\n",(*mortgage).outstanding_principal,(*mortgage).outstanding_interest);
	}
*/
/*
	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		//printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
		//printf("\nIT\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
		//printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance\n");
		//printf("%d", DAY);
		printf("\t%d", ID);
		printf("\t%d",(*mortgage).object_id);
		printf("\t%d", s);
		printf("\t%d", initial_duration);
		printf("\t\t%d", current_duration);
		printf("\t\t%f", monthly_interest);
		printf("\t%f", monthly_principal);
		printf("\t%f", (*mortgage).outstanding_interest);
		printf("\t%f", (*mortgage).outstanding_principal);
		printf("\t%f\n", loan_balance_outstanding);		
	}      
	#endif
*/
}

/** \fn recompute_mortgage_attributes_including_arrears(mortgage_contract_adt * mortgage)
 * \brief Function to redo mortgage calculation to update values each month
 * \brief This version takes into account the missed payments (principal+interest) due to arrears.
 * \brief The missed payments are recomputed each time, based on the current value of arrears_counter in the ADT.
 * \brief The so computed accumulated_principal is added to the monthly_principal (current payment due), and 
 * \brief accumulated_interest is added to the monthly_interest.
 */
void recompute_mortgage_attributes_including_arrears(mortgage_contract_adt * mortgage)
{
	double accumulated_principal = 0.0;
	double accumulated_interest = 0.0;
	int duration = 0;

	recompute_mortgage_attributes(mortgage);

	accumulated_principal += (*mortgage).monthly_principal;
	accumulated_interest += (*mortgage).monthly_interest;
	duration = (*mortgage).current_duration;

	int i=0;
	for (i=0;i<(*mortgage).arrears_counter;i++)
	{
		(*mortgage).current_duration--;

		if ((*mortgage).current_duration >=0)
		{
			recompute_mortgage_attributes(mortgage);

			accumulated_principal += (*mortgage).monthly_principal;
			accumulated_interest += (*mortgage).monthly_interest;
		}
	}

	(*mortgage).monthly_principal = accumulated_principal;
	(*mortgage).monthly_interest = accumulated_interest;
	(*mortgage).current_duration = duration;

	// Counter for repayment schedule: s=0,...,m
	int s = (*mortgage).initial_duration - (*mortgage).current_duration;

	#ifdef _DEBUG_MODE    
	if (PRINT_MORTGAGE_INFO)
	{
		//printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
		//printf("\nIT\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
		//printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance\n");
		//printf("%d", DAY);
		printf("\t%d", ID);
		printf("\t%d",(*mortgage).object_id);
		printf("\t%d", s);
		printf("\t%d", (*mortgage).initial_duration);
		printf("\t\t%d", (*mortgage).current_duration);
		printf("\t\t%f", (*mortgage).monthly_interest);
		printf("\t%f", (*mortgage).monthly_principal);
		printf("\t%f", (*mortgage).outstanding_interest);
		printf("\t%f\n", (*mortgage).outstanding_principal);
	}      
	#endif
}


/** \fn adjust_mortgage_contract(mortgage_contract_adt * mortgage, double new_principal)
 * \brief Function to adjust the mortgage contract to use integer maturity (in months).
 * \brief Due to the rescaling of the total principal, the maturity can be non-integer.
 * \brief This function sets integer maturity >=1 month.
 */
void adjust_mortgage_contract(mortgage_contract_adt * mortgage, double new_principal)
{
	double i = (*mortgage).interest_rate_annual;
	double a = (*mortgage).monthly_total_repayment;

	assert(i>=0);
	assert(a>=0);

	int new_m = (int) (-(log(1-(i*new_principal)/(12*a)))/(log(1+i/12)))+1;

	if (new_m == 0)
	{
		new_m = 1;
		printf("\n[WARNING] adjust_mortgage_contract: new maturity would be 0. Setting maturity to 1.");
	}

	assert(new_m >= 1);

	double derived_principal = a * (12/i) * (pow(1 + i/12, new_m)-1)/pow(1 + i/12, new_m);

	assert(derived_principal >= 0);

	double total_interest_owed = ((i/12) * derived_principal * new_m)/(1-pow(1+i/12, -new_m)) - derived_principal;

	assert(total_interest_owed >= 0);

	(*mortgage).current_duration 		= new_m; //months
	(*mortgage).initial_duration 		= new_m;
	(*mortgage).outstanding_principal 	= derived_principal;
	(*mortgage).outstanding_interest 	= total_interest_owed;

}

/** \fn adjust_mortgage_contract2(mortgage_contract_adt * mortgage, double new_principal)
 * \brief Function to adjust the mortgage contract to use integer maturity (in months).
 */
void adjust_mortgage_contract2(mortgage_contract_adt * mortgage, double new_principal)
{
	double i = (*mortgage).interest_rate_annual;
	double a = (*mortgage).monthly_total_repayment;
	double m = (*mortgage).initial_duration;
	double old_principal = (*mortgage).outstanding_principal;

	assert(i>=0);
	assert(a>=0);

	double scale_factor = new_principal / old_principal;

	double new_a = a * scale_factor;

	double total_interest_owed = ((i/12) * new_principal * m)/(1-pow(1+i/12, -m)) - new_principal;

	assert(total_interest_owed >= -0.00001);

	(*mortgage).current_duration 		= m;
	(*mortgage).initial_duration 		= m;
	(*mortgage).outstanding_principal 	= new_principal;
	(*mortgage).outstanding_interest 	= total_interest_owed;
	(*mortgage).monthly_total_repayment = new_a;

}

void recompute_variable_rate_mortgage_contract(mortgage_contract_adt * mortgage, double new_base_rate)
{
	double new_i = max(0.0, new_base_rate + (*mortgage).markup_on_base_rate);

	double principal = (*mortgage).outstanding_principal;
	double duration = (*mortgage).current_duration+1;

	double new_a;
	double total_interest_owed;
	if (duration > 0)
	{
		

		if (new_i>0)
		{
			new_a = principal * (new_i/12*pow(1+new_i/12, duration))/(pow(1+new_i/12, duration)-1);
			total_interest_owed = ((new_i/12) * principal * duration)/(1-pow(1+new_i/12, -duration)) - principal;
		}
		else
		{
			new_a = principal / duration;
			total_interest_owed = 0;
		}
	}
	else
	{
		total_interest_owed = principal * new_i/12;
		new_a = principal + total_interest_owed;
	}

	(*mortgage).monthly_total_repayment = new_a;
	(*mortgage).interest_rate_annual = new_i;
	(*mortgage).base_rate = new_base_rate;
	(*mortgage).outstanding_interest = total_interest_owed;
	(*mortgage).initial_duration = (*mortgage).current_duration+1;
}

/** \fn Household_default_on_all_mortgage_debt_hmr()
 * \brief Function to default_on_all mortgage debt for HMR.
 * \brief Condition: If HH becomes unemployed (optional, not the default)
 * \brief Note: Code re-used from Household_service_mortgage_debt_hmr function, else branch
 */
void Household_default_on_all_mortgage_debt_hmr()
{
	int bank_id = 0;
	int object_id = 0;
	double value __attribute__((unused)) = 0.0;
	double loan_default = 0.0;
	double loan_loss = 0.0;
	int arrears_counter = 0;

	/* Involuntary default condition 
	 * Default on HMR mortgage; mortgage is PL or Arrears, arrears counter less than 3 months
	 */
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active
		&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status < 2 
		&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
		{
			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
			object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id;

			if (object_id==0)
				fprintf(stderr, "\nIT %d ID %d Household_default_on_all_mortgage_debt_hmr(): object_id=%d", DAY, ID, object_id);

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\n\nIT %d ID %d FORCED MORTGAGE DEFAULT [UNEMPLOYED]", DAY, ID);
			}      
			#endif

			//Case: arrears
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
			{
				printf("\n\n\t - Stage 1 HMR_MORTGAGE_STATUS = %d (AR) [HMR FORCED MORTGAGE DEFAULT BY UNEMPLOYED]", HMR_MORTGAGE_STATUS);
			}      
			#endif

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 1;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter++;

			//NOTE: loan_default includes total outstanding principal (no interest)
			loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
			loan_loss = max(0.0, loan_default - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_default	 = loan_default;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_loss		 = loan_loss;

			//HMR mortgages enter arrears status
			arrears_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter;

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d SENDING ARREARS MESSAGE (HMR object %d) -> Bank %d %d", DAY, ID, object_id, bank_id, arrears_counter);
			}      
			#endif

			add_mortgage_arrears_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);

			HH_MORTGAGES_ARREARS++;
			HH_MORTGAGES_HMR_ARREARS++;
		}
}

/** \fn Household_default_on_all_mortgage_debt_op()
 * \brief Function to default_on_all mortgage debt for OP.
 * \brief Condition: If HH becomes unemployed (optional, not the default)
 * \brief Note: Code re-used from Household_service_mortgage_debt_op function, else branch
 */
void Household_default_on_all_mortgage_debt_op()
{
	int i = 0;
	int bank_id = 0;
	int object_id = 0;
	double value __attribute__((unused)) = 0.0;
	double loan_default = 0.0;
	double loan_loss = 0.0;
	int arrears_counter = 0;

	// Move ALL OP mortgages from PL to Arrears status,
	// or if already in Arrears remain in arrears (arrears_counter will be updated here)
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++) 
	{
		//Only count OP mortgages that have positive duration, are not yet 3 months in arrears, and are not yet NPL
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status < 2
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
			{
				bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;
				object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
				
				if (object_id==0)
					fprintf(stderr, "\nIT %d ID %d Household_default_on_all_mortgage_debt_op(): object_id=%d", DAY, ID, object_id);
				
				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\n\nIT %d ID %d FORCED MORTGAGE DEFAULT [UNEMPLOYED]", DAY, ID);
				}      
				#endif

				//Case: arrears
				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
				{
					printf("\n\n\t - Stage 1 OP_MORTGAGE_STATUS = %d (AR) [OP FORCED MORTGAGE DEFAULT BY UNEMPLOYED]", OP_MORTGAGE_STATUS);
				}      
				#endif

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 1;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter++;

				//NOTE: loan_default includes total outstanding principal (no interest)
				value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
				loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;
				loan_loss = max(0.0, loan_default - value);

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_default 	= loan_default;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_loss 		= loan_loss;

				//Selected OP mortgages enter arrears status
				add_mortgage_arrears_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
		
				HH_MORTGAGES_OP_ARREARS++;
				HH_MORTGAGES_ARREARS++;
				
				arrears_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter;

				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\nIT %d ID %d SENDING ARREARS MESSAGE (OP object %d [%d/%d]) -> Bank %d arrears_counter %d", DAY, ID, object_id, i+1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, bank_id, arrears_counter);
				}      
				#endif
			}
	}
}


/** \fn Household_service_mortgage_debt_hmr()
 * \brief Function to execute monthly mortgage debt servicing for HMR.
 */
void Household_service_mortgage_debt_hmr()
{
	int bank_id;
	int object_id;

	int arrears_counter=0;
	double loan_loss, loan_default;

	double reservation_hmr __attribute__((unused)) = 0.0;
	double monthly_principal_hmr=0.0;
	double monthly_interest_hmr=0.0;

	double budget_remaining;

	enforce_hh_valid_payment_account(__func__);

	MORTGAGE_PAYMENT_HMR = 0;

	/******* Procedure for Involuntary Arrears for HMR ********/
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n\n ID: %d Household_service_mortgage_debt", ID);
		printf("\n\n\t - Stage 1 HMR processing");
	}      
	#endif

	budget_remaining = min(HOUSING_BUDGET, PAYMENT_ACCOUNT);

	reservation_hmr = 0.0;

	//Mortgage main residence
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
	{
		bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;

		#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
		{
				//printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
			printf("\n\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
			printf("\nID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance");
		}      
		#endif

		recompute_mortgage_attributes_including_arrears(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);

		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration)
		{
			monthly_principal_hmr = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal;
			monthly_interest_hmr  = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest;

			reservation_hmr = monthly_principal_hmr + monthly_interest_hmr;
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_ID_PROBE==ID)
		{
			fprintf(stderr,"\n%d,%d,%f,%f", DAY, ID, monthly_principal_hmr, monthly_interest_hmr);
		}      
		#endif
		
		/* Involuntary default conditions */
		//Case 1: normal payment of HMR mortgage; mortgage is PL or Arrears, arrears counter less than 3 months
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status < 2 
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
		{
			if ( reservation_hmr <= budget_remaining+ 1e-5)
			{
				//Cure the arrears status to PL
				object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 0;

				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\n\n\t HMR_MORTGAGE_STATUS = %d (PL) [HMR monthly_total_repayment < PAYMENT_ACCOUNT]", HMR_MORTGAGE_STATUS);
				}      
				#endif

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal -= monthly_principal_hmr;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest -= monthly_interest_hmr;

				if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter>0)
					vlog_event(ID,"hmr_cured");

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration -= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter = 0;

				if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration<0)
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration = 0;

				//Payment as usual
				add_mortgage_payment_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
				//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id);

				//Reduce account
				PAYMENT_ACCOUNT -= (monthly_principal_hmr + monthly_interest_hmr);
				log_outflow("household", "payment_account", "mortgages", "mortgage_principal", ID, monthly_principal_hmr);
				log_outflow("household", "payment_account", "net_worth", "mortgage_interest", ID, monthly_interest_hmr);

				MORTGAGE_PAYMENT_HMR = monthly_principal_hmr + monthly_interest_hmr;

				MORTGAGE_PRINCIPAL_PAYMENT_HMR = monthly_principal_hmr;
				MORTGAGE_INTEREST_PAYMENT_HMR = monthly_interest_hmr;

				budget_remaining -= (monthly_principal_hmr + monthly_interest_hmr);
				HOUSING_BUDGET -= (monthly_principal_hmr + monthly_interest_hmr);


				//Reduce duration, Recomputation of debt will be doine next month.
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration--;

				//NOTE: loan_default includes total outstanding principal (no interest)
				// What does all this actually do??
				loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
				loan_loss 	 = max(0.0, loan_default - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
				
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_default	 = loan_default;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_loss		 = loan_loss;

				HH_MORTGAGES_PAYING++;
				HH_MORTGAGES_HMR_PAYING++;

				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\nIT %d ID %d SENDING PAYMENT MESSAGE FOR HMR", DAY, ID);
				}      
				#endif

		   		if (PAYMENT_ACCOUNT < -0.01)
					fprintf(stderr, "IT %d ID %d Household_service_mortgage_debt(): PAYMENT_ACCOUNT < -0.01, PAYMENT_ACCOUNT=%.2f", DAY, ID, PAYMENT_ACCOUNT);

				assert__(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal>-1e-3)
				{
					fprintf(stderr, "HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal = %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
				}

				assert__(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest>-1e-3)
				{
					fprintf(stderr, "HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest = %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest);
				}
			}
			else
			{
				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\n\nIT %d ID %d FAILED TO MAKE MONTHLY MORTGAGE PAYMENT [HMR monthly_total_repayment > PAYMENT_ACCOUNT] total payment due %f payment account %f housing budget %f",
					 DAY, ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment, PAYMENT_ACCOUNT, budget_remaining);
				}      
				#endif
				
				//Case: arrears

				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
				{
					printf("\n\n\t - Stage 1 HMR_MORTGAGE_STATUS = %d (AR) [HMR monthly_total_repayment > PAYMENT_ACCOUNT]", HMR_MORTGAGE_STATUS);
				}      
				#endif
				
				object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id;
				
				if (object_id==0)
					fprintf(stderr, "\nIT %d ID %d Household_service_mortgage_debt(), in FAILED TO MAKE MONTHLY MORTGAGE PAYMENT: object_id=%d", DAY, ID, object_id);
				
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status = 1;

				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter++;

				//NOTE: loan_default includes total outstanding principal (no interest)
				loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
				loan_loss = max(0.0, loan_default - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
				
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_default	 = loan_default;
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.loan_loss		 = loan_loss;

				//HMR mortgages enter arrears status
				arrears_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter;

				#ifdef _DEBUG_MODE    
				if (PRINT_MORTGAGE_INFO)
				{
					printf("\nIT %d ID %d SENDING ARREARS MESSAGE (HMR object %d) -> Bank %d %d", DAY, ID, object_id, bank_id, arrears_counter);
				}      
				#endif

				add_mortgage_arrears_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
				if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter==1)
					vlog_event(ID,"hmr_arrears");
				
				HH_MORTGAGES_ARREARS++;
				HH_MORTGAGES_HMR_ARREARS++;
			}
		}

		//Check if this was the final debt payment, and re-initialize (set all values to zero) if so
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration == -1)
		{
			assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal < 0.00001);

			init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
			HAVE_HMR_MORTGAGE = 0;

	        HH_MORTGAGES_PAIDOFF++;
    	    HH_MORTGAGES_HMR_PAIDOFF++;
		}
	}

	enforce_hh_valid_payment_account(__func__);

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n\n\t - Stage 2 (after HMR processing): PAYMENT_ACCOUNT = %f MORTGAGE_PAYMENT_HMR = %f", PAYMENT_ACCOUNT, MORTGAGE_PAYMENT_HMR);
	}      
	#endif	
}

/** \fn Household_service_mortgage_debt_op()
 * \brief Function to execute monthly mortgage debt servicing for OP.
 */
void Household_service_mortgage_debt_op()
{
	int i,j;
	int bank_id;
	int object_id;

	int arrears_counter=0;
	double loan_loss, loan_default, value;

	double budget_remaining = min(OP_DEBT_SERVICE_BUDGET, PAYMENT_ACCOUNT);
	
	double reservation_op;
	double monthly_principal_op=0.0;
	double monthly_interest_op=0.0;

	//For Case 2 OP mortgages: Not enough funds to service all OP mortgages -> Select some to go into arrears
	//list: index-list of size n with the successive indices [0,...,n-1]; afterwards: indices of non-selected items
	int_array list;
	init_int_array(&list);

	//select: index-list of selected items going into arrears
	int_array select;
	init_int_array(&select);
	
	MORTGAGE_PAYMENT_OP = 0;

	/******* Procedure: determine OP total_monthly_debt_service (reservation_op) ********/

	reservation_op = 0.0;

	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		//Recompute new values for mortgage attributes: monthly_principal_op, monthly_interest_op
		if (i==0)
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				//printf("\nIT %d ID %4d Recomputing mortgage data:", DAY, ID);
				printf("\n\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
				printf("\nID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance");
			}      
			#endif
		}

		recompute_mortgage_attributes_including_arrears(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_ID_PROBE==ID)
		{
			monthly_principal_op = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal;
			monthly_interest_op  = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest;

			fprintf(stderr,"%f,%f", monthly_principal_op, monthly_interest_op);
		}      
		#endif
		
		//Only count OP mortgages that have positive duration, are not yet 3 months in arrears, and are not yet NPL
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status < 2
			&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
			{
					reservation_op += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal
									+ HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest;

					#ifdef _DEBUG_MODE    
					if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
					{
							printf("\n\n\t\t Item OP [%d] Annuity %f", i, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_total_repayment);
					}      
					#endif
			}
	}
	

	/******* Procedure for Involuntary Arrears on OP mortgages ********/
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n\n\t - Stage 3 (before OP processing): Reservation HMR=xxx Reservation OP (budget)=%f", reservation_op);
	}      
	#endif

	/******* Rank OP mortgages on monthly_total_repayment, selected OPs are updated in status from PL -> Arrears ********/

/*
// 5.10.17 SH: Case 1 is redundant, since we can simply run Case 2 always.
// If Case 1 applies, then the while-loop (reservation_op > budget_remaining && list.size>0) will result in an empty select list for OPs to go into arrears.
// After this, the full list of OP mortgages is processed, which is equivalent to the code previously under the if-condition (reservation_op < budget_remaining + 1e-5).
*/
	//Case 1: normal payment of all OP mortgages
	if ( reservation_op < budget_remaining + 1e-5)
	{
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
		{
			printf("\n\n\t\t processing OP\t [reservation_op < PAYMENT_ACCOUNT]");
		}      
		#endif
	}
	else
	{
		//Case 2: (reservation_op > budget_remaining + 1e-5)
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
		{
			printf("\n\n\t\t selecting OP for arrears\t [reservation_op > PAYMENT_ACCOUNT]");
		}      
		#endif

		#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
		{
			printf("\n\nIT %d ID %d CANNOT PAY ALL OP MORTGAGES [reservation_op > PAYMENT_ACCOUNT] total payment due: %f, payment account: %f housing budget: %f\n", DAY, ID, reservation_op, PAYMENT_ACCOUNT, budget_remaining);
		}      
		#endif
	}

    /* Start here the routine for defaulting on a sub-set of OP mortgages 
     * Build two lists:
     * list.array[i] : consists of all indices of OP units that should be paid as usual
     * select.array[i] : consists of all indices of OP units that should be going into arrears
     */

        MORTGAGE_PRINCIPAL_PAYMENT_OP = 0.0;
		MORTGAGE_INTEREST_PAYMENT_OP = 0.0;
		OP_EXPENSE = 0.0;

		// Only consider OP mortgages that are active,
		// and not already 3 months in arrears (status 1),
		// and not already in NPL (status 2),
		// or ready for writeoff (status 3)
		for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		{ 
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active
				&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status < 2
				&& HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter < MAX_DURATION_MORTGAGE_LOANS_ARREARS)
				{
					add_int(&list, i);
				}
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
		{
			printf("\n\t\t before while-loop: list.size = %d select.size = %d", list.size, select.size);
			
			if (list.size>0)
			{
				printf("\n\t\t OP_MORTGAGE_STATUS = (PAYING) for this list of indices of OP mortgages:");
				for (i=0; i<list.size; i++)
					printf("\t [%d]", list.array[i]);
			}
		}      
		#endif

   /* From list.array select OP mortgages that should move to select.array to move into arrears (or advance in arrears).
	* OP mortgages are ranked on: monthly_total_repayment; we remove mortgages with the highest monthly_total_repayment first.
	* Loop over the list.array (a list of indices), move highest item to select.array (another list of indices).
	* Repeat until monthly total debt service reserved for OP mortgages (reservation_op) drops below budget_remaining.
    */
		int ix = -1;
		int argmax = 0;
		double maxi = 0.0;
		while (reservation_op > budget_remaining + 1e-5 && list.size>0)
		{
			//Start search for highest ranked item in current list
			argmax=0;
			ix = list.array[argmax]; //get first index from list
			maxi = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[ix].mortgage.monthly_principal+HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[ix].mortgage.monthly_interest;

			for (i=1; i<list.size; i++)
			{
				j = list.array[i]; //get current index from list 
				if ( HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[j].mortgage.monthly_principal+HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[j].mortgage.monthly_interest > maxi )
				{
					argmax = i;
					maxi = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[j].mortgage.monthly_principal+HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[j].mortgage.monthly_interest;
				}
			}

			//Reduce & check budget condition
			reservation_op -= maxi;

			//add value to select list: these are the OP mortgages we default on
			//Note: add_int(&list, value) adds a value at end of a dynamic array. Here: value=ix=list.array[argmax].
			add_int(&select, list.array[argmax]);

			//remove value from list at index i: this is the mortgage index with the maxi value
			//Note: remove_int(&list, index) removed an int from list at index. Here: index=argmax.
			remove_int(&list, argmax);
		
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
			{
				printf("\n\t\t in while-loop: list.size = %d select.size = %d", list.size, select.size);
				printf("\t found maxi at list[%d] OP[%d]", argmax, ix); 
				printf("\n\t\t\t\t list:");
				for (i=0; i<list.size; i++)
					printf("\t [%d]", list.array[i]);
				printf("\n\t\t\t\t select:");
				for (i=0; i<select.size; i++)
					printf("\t [%d]", select.array[i]);
			}      
			#endif
	
		} //while-loop
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
		{
			printf("\n\t\t after while-loop: list.size = %d select.size = %d", list.size, select.size);
			
			if (list.size>0)
			{
				printf("\n\t\t OP_MORTGAGE_STATUS = (PAYING) for this list of indices of OP mortgages:");
				for (i=0; i<list.size; i++)
					printf("\t [%d]", list.array[i]);
			}

			if (select.size>0)
			{
				printf("\n\t\t OP_MORTGAGE_STATUS = (ARREARS) for these selected indices of OP mortgages:");
				for (i=0; i<select.size; i++)
					printf("\t [%d]", select.array[i]);
			}
		}      
		#endif


		// For selected list:
		// Move selected OP mortgages from PL to Arrears status,
		// or if already in Arrears remain in arrears (arrears_counter will be updated here)
		for (j=0; j<select.size; j++) 
		{
			i = select.array[j];

			object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			if (object_id==0)
				fprintf(stderr, "\nIT %d ID %d Household_service_mortgage_debt(), line 665, in OP select list: object_id=%d", DAY, ID, object_id);
			
			
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 1;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter++;
			arrears_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter;

			//NOTE: loan_default includes total outstanding principal (no interest)
			value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
			loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;
			loan_loss = max(0.0, loan_default - value);

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_default 	= loan_default;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_loss 		= loan_loss;

			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;

			//TODO Reset old state b/c no payment
			//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration++;
			//recompute_mortgage_attributes(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);

			//Selected OP mortgages enter arrears status
			add_mortgage_arrears_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);

			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter==1)
				vlog_event(ID,"op_arrears");

			// Try again next time
			//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration--;
		
			HH_MORTGAGES_OP_ARREARS++;
			HH_MORTGAGES_ARREARS++;

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d SENDING ARREARS MESSAGE FOR SELECTED LIST (OP object %d [%d/%d]) -> Bank %d arrears_counter %d", DAY, ID, object_id, j+1, select.size, bank_id, arrears_counter);
			}      
			#endif
		}

		// For non-selected list:
		// Pay OP mortgage as usual
		// If it was in arrears it is now cured (arrears_counter set to 0 here)
		for (j=0; j<list.size; j++) 
		{
			i = list.array[j];

			object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			//Cure the arrears status to PL
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status = 0;
			
			arrears_counter = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter;

			bank_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id;
			monthly_principal_op = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal;
			monthly_interest_op  = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest;

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal -= monthly_principal_op;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest -= monthly_interest_op;

			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter>0)
				vlog_event(ID,"op_cured");

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration-=HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter = 0;

			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration<0)
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration = 0;

			//Payment as usual
			add_mortgage_payment_message(ID, bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
			//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);

			//Reduce duration, recompute mnortgage next period.
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration--;

			//Reduce account
			PAYMENT_ACCOUNT -= (monthly_principal_op + monthly_interest_op);
			log_outflow("household","payment_account", "mortgages", "mortgage_principal", ID, monthly_principal_op);
			log_outflow("household","payment_account", "net_worth", "mortgage_interest", ID, monthly_interest_op);

			MORTGAGE_PAYMENT_OP += monthly_principal_op + monthly_interest_op;

			MORTGAGE_PRINCIPAL_PAYMENT_OP += monthly_principal_op;
			MORTGAGE_INTEREST_PAYMENT_OP  += monthly_interest_op;

			budget_remaining -= (monthly_principal_op + monthly_interest_op);

			OP_EXPENSE += (monthly_principal_op + monthly_interest_op);

			//NOTE: loan_default includes total outstanding principal (no interest)
			value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
			loan_default = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;
			loan_loss = max(0.0, loan_default - value);

			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.collateral_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_default 	= loan_default;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.loan_loss 		= loan_loss;

			HH_MORTGAGES_OP_PAYING++;
			HH_MORTGAGES_PAYING++;

			assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal>-0.00001);
			assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest>-0.00001);

			#ifdef _DEBUG_MODE    
			if (PRINT_MORTGAGE_INFO)
			{
				printf("\nIT %d ID %d SENDING PAYMENT MESSAGE FOR NON-SELECTED LIST (OP object %d [%d/%d]) -> Bank %d arrears_counter %d", DAY, ID, object_id, j+1, list.size, bank_id, arrears_counter);
			}      
			#endif
			
			

			if (PAYMENT_ACCOUNT < -0.01)
				fprintf(stderr, "\nIT %d ID %d Household_service_mortgage_debt (OP): PAYMENT_ACCOUNT < -0.01, PAYMENT_ACCOUNT=%.2f", DAY, ID, PAYMENT_ACCOUNT);

			

			//Check if this was the final debt payment for this OP mortgage, and if so reset mortgage item
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration == -1)
			{
				assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal < 0.00001);

				init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
				
		        HH_MORTGAGES_PAIDOFF++;
	        	HH_MORTGAGES_OP_PAIDOFF++;
			}
		}

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n\n\t - Stage 4 (after OP processing): reservation_hmr=xxx reservation_op=%2.3f", reservation_op);
		printf("\n\n ID: %d End of Household_mortgage_debt_servicing: PAYMENT_ACCOUNT = %f MORTGAGE_PAYMENT_HMR = %f MORTGAGE_PAYMENT_OP = %f", ID, PAYMENT_ACCOUNT, MORTGAGE_PAYMENT_HMR, MORTGAGE_PAYMENT_OP);
	}      
	#endif

	enforce_hh_valid_payment_account(__func__);

	free_int_array(&list);
	free_int_array(&select);
}

