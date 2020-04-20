#include "../../header.h"
#include "../../Household_agent_header.h"
#include "../../my_library_header.h"
#include "../Mortgage_Market/Household_aux_headers.h"

/***************** TEMPLATE ****************/
/** \fn Household_TEMPLATE()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
/*
int Household_TEMPLATE()
{

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MORTGAGE_HH)
    {
        printf("\n\n Household_ ID %d", ID);
    }      
    #endif
      
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

extern gsl_rng * FLAME_GSL_RNG; 
/** \fn Household_init_mortgages()
 * \brief Function to initialize the synthetic mortgage calculation
 *	1.8.2017, SvdH: New derivations from description paper, eqns (10-15)
 * REF: Model description paper
 * \brief UNITTEST: PASS 3.11.16
 */
int Household_init_mortgages()
{
	int i __attribute__((unused));
	double sum __attribute__((unused)) = 0.0;
	double sum1 __attribute__((unused)) = 0.0;
	double sum2 __attribute__((unused)) = 0.0;

	double annuity __attribute__((unused)) = 0.0;
	double r __attribute__((unused)) = 0.0;
	double  m_double __attribute__((unused)) = 0.0;
	int m __attribute__((unused)) = 0;

	double derived_outstanding_principal __attribute__((unused)) = 0.0;
	double ratio __attribute__((unused));
	double q __attribute__((unused));

	if (IS_OWNER)
	{
		q = random_unif_interval(0, 1000);

		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price = q;
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated = q;
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality = q;
	}

	if (IS_LANDLORD)
	{
		q = random_unif_interval(0, 1000);
		
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.purchase_price = q;
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.value_estimated = q;
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.quality = q;
	}

	return 0;
}


/** \fn Household_disaggregate_initial_mortgages()
 * \brief Function to disaggregate initial OP mortgages
 * \brief Condition: If (IS_LANDLORD==1)
 * \brief UNITTEST: TODO
 */
int Household_disaggregate_initial_mortgages()
{
	int i,n;
	mortgage_contract_adt mortgage;
	init_mortgage_contract_adt(&mortgage);

	/********** HMR mortgages allocate object_id *************/
	if (HAVE_HMR_MORTGAGE)
	{
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active = 1;

		//object_id is now known after Household_read_rent_matching_info():
		HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id; 
		
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id==0)
			fprintf(stderr, "\n\nIT %d ID: %d WARNING object_id=0 in Household_disaggregate_initial_mortgages", DAY, ID);

		add_mortgage_info_message(ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);
		
		//#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
			//if (1)
		{
			//printf("\n\nIT %d ID %d HMR synthetic mortgage", DAY, ID);
			printf("\n\n\tHMR final synthetic mortgage (after object_id is allocated)");
			
			printf("\n\t active\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active);
			printf("\n\t object_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id);
			printf("\n\t household_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.household_id);
			printf("\n\t bank_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id);
			printf("\n\t interest_rate_annual\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual);
			printf("\n\t current_duration (months)\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
			printf("\n\t outstanding_principal\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
			printf("\n\t monthly_total_repayment\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
			printf("\n\t status\t\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
			printf("\n\t arrears_counter\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter);
			printf("\n\t npl_counter\t\t\t %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter);
		}      
		//#endif
	}
	
	
	/********** OP mortgages disaggregated *************/
	//Value of rental unit
	//OPTION 1: Set value for individual rental units, based on total value of Other Property: assume uniform value
	//See Initialization/Household_init_functions.c, lines 359 - 362
	//18.8.17, SvdH	: uniformly divide total value among all rental units

	double q;
	
	n = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;
	if(n>0)
	{
		for (i=0; i<n; i++)
		{
			q = random_unif_interval(0, 1000);
			
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price = q;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated = q;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality = q;
		}
	}
	
	for (i=0; i<n; i++)
		init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
	
	//Only create disaggregated synthetic mortgages if the original synthetic mortgage is active (has a non-zero derived_outstanding_principal)
	if (n>0 && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.active)
	{
		//Copy values from original mortgage
		copy_mortgage_contract_adt( &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property, &mortgage);
		
		//Adjust values for unit mortgages:
		//mortgage.active
		//mortgage.object_id
		//mortgage.household_id
		//mortgage.bank_id
		//mortgage.initial_duration
		//mortgage.interest_rate_annual
		//mortgage.current_duration
		mortgage.outstanding_principal		= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal/n;
		mortgage.outstanding_interest		= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_interest/n;
		mortgage.monthly_total_repayment	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_total_repayment/n;
		mortgage.monthly_principal			= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_principal/n;
		mortgage.monthly_interest			= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_interest/n;
		//mortgage.status
		//mortgage.arrears_counter
		//mortgage.npl_counter
		
		//Disaggregate synthetic OP mortgage into n equal-sized small mortgages for rental units
		for (i=0; i<n; i++)
		{
			copy_mortgage_contract_adt(&mortgage, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
			
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n\nIT %d ID %d add_mortgage_info_message in Rental_Market/centralized_auction/Household_Functions.c:308 Household_read_rent_matching_info()", DAY, ID);
			}      
			#endif
			
			add_mortgage_info_message(ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id,
			 HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);

		}
		
		//#ifdef _DEBUG_MODE    
		if (PRINT_MORTGAGE_INFO)
			//if (1)
		{
			printf("\n\n\tOP initial synthetic mortgage");
			printf("\n\t active\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.active);
			printf("\n\t object_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.object_id);
			printf("\n\t household_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.household_id);
			printf("\n\t bank_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.bank_id);
			
			printf("\n\t interest_rate_annual\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.interest_rate_annual);
			printf("\n\t current_duration (months)\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.current_duration);
			
			printf("\n\t outstanding_principal\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal);
			printf("\n\t outstanding_interest\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_interest);
			printf("\n\t monthly_total_repayment\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_total_repayment);
			
			printf("\n\t monthly_principal\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_principal);
			printf("\n\t monthly_interest\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_interest);
			
			printf("\n\t status\t\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.status);
			printf("\n\t arrears_counter\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.arrears_counter);
			printf("\n\t npl_counter\t\t\t %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.npl_counter);
			
			printf("\n\t OP total_value\t\t\t %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.purchase_price);
			
			printf("\n\n\tOP final disaggregated mortgages (%d):", n);
			for (i=0;i<n; i++)
			{
				printf("\n\t OP [%d]:", i);
				printf("\n\t active\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active);
				printf("\n\t object_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);
				printf("\n\t household_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.household_id);
				printf("\n\t bank_id\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.bank_id);
				
				printf("\n\t interest_rate_annual\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.interest_rate_annual);
				printf("\n\t current_duration (months)\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration);
				
				printf("\n\t outstanding_principal\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal);
				printf("\n\t outstanding_interest\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest);
				printf("\n\t monthly_total_repayment\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_total_repayment);

				printf("\n\t monthly_principal\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal);
				printf("\n\t monthly_interest\t\t %2.3f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest);
				
				printf("\n\t status\t\t\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status);
				printf("\n\t arrears_counter\t\t %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter);
				printf("\n\t npl_counter\t\t\t %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.npl_counter);
				
				printf("\n\t current_value\t\t\t %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price);
			}
		}      
	} //if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.active)

	free_mortgage_contract_adt(&mortgage);

	return 0;
}
