#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"
#include "Household_aux_header.h"
#include "../../Mortgage_Market/Household_aux_headers.h"

//Must be repeated here:
//Global GSL RNG: global constant variable: continues to exist for whole duration of main
extern gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/***************** TEMPLATE ****************/
/** \fn Household_TEMPLATE()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: 
 */
/*
int Household_TEMPLATE()
{
    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MORTGAGE)
    {
        printf("\n\n Household_ ID: %d", ID);
    }      
    #endif
      
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

/** \fn Household_sample_mortgage_attributes()
 * \brief Function to implement the endogenous mortgage loan application process.
 * \brief Household samples the mortgage attributes DSTI, LTV and Maturity (years).
 * \brief This version uses an online sampling procedure for values of the attributes.
 * \brief A structural parametric estimation needs to be provided, based on a joint log normal distribution of the HFCS data (10,000 Households sampled).
 * \brief Means are stored in the variable: MORTGAGE_SAMPLING_MEANS[3]
 * \brief The variance covariance matrix is stored in the variable: MORTGAGE_SAMPLING_COVAR[9]
 *
 * \brief We draw from a multivariate normal distribution with parameters (means and covariance matrix) as given by input files reflectING the KDE:
 * \brief File: ../populations/mortgage_sampling/means.csv --> import values from this file into MORTGAGE_SAMPLING_MEANS[3] in the 0.xml file.
 * \brief File: ../populations/mortgage_sampling/covar.csv --> import values from this file into MORTGAGE_SAMPLING_COVAR[9] in the 0.xml file.
 *
 * \brief REF to GSL library functions:
 *
 * Definition of the gsl_matrix structure:  
 * URL: https://www.gnu.org/software/gsl/manual/html_node/Matrices.html
 *
 * Definition of the gsl_vector structure:    
 * URL: https://www.gnu.org/software/gsl/manual/html_node/Vectors.html
 *
 * \brief 1. For sampling from a multi-variate normal distribution:
 *             https://www.gnu.org/software/gsl/manual/html_node/The-Multivariate-Gaussian-Distribution.html#The-Multivariate-Gaussian-Distribution
 * \brief 2. Convert the 3x1 draw of the (DSTI, LTV, M) to levels by applying the exponential to each of them: dsti=exp(sampled_dsti) etc.
 * \brief 3. Restrict values to floors and ceilings:
 *  Initial DSTI in the 5%-60% range, initial LTV within 20%-120%, and maturity of the loan in years inbetween 10 and 40 years."
 *
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: 
 */
int Household_sample_mortgage_attributes()
{
	double sampled_loan_to_value=0.0;
	double exp_loan_to_value=0.0;

	double sampled_dsti=0.0;
	double exp_dsti=0.0;
	
	double sampled_maturity=0.0;
	double exp_maturity=0.0;

	double floor, ceil;

	int i,j,k;
	const int n=3;

		//Initialize vectors
	gsl_vector * v = gsl_vector_alloc (n);
	gsl_vector * result = gsl_vector_alloc (n);

	for (i = 0; i < n; i++)
	{
		gsl_vector_set (v, i, MORTGAGE_SAMPLING_MEANS[i]);
	}

		//Initialize matrix
	gsl_matrix * m = gsl_matrix_alloc (n, n);
	
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			k=i*n+j;
			gsl_matrix_set (m, i, j, MORTGAGE_SAMPLING_COVAR[k]);
		}

	// Draw one time a vector of length 3: (DSTI,LTV,M)	
	gsl_ran_multivariate_gaussian (FLAME_GSL_RNG, v, m, result);

		//Print result vector:
	/*
		printf("\nResult from gsl_ran_multivariate_gaussian:"); 
		printf("\t Sampled DSTI: %f", gsl_vector_get(result,1));
		printf("\t Sampled LTV: %f", gsl_vector_get(result,0) );
		printf("\t Sampled Maturity: %f", gsl_vector_get(result,2));
	*/

	int redraw = (random_unif() < HOUSING_REDRAW_PROBABILITY);

	if(DAY == CONST_INIT_TRANSIENT_PERIOD || redraw)
	{
		HH_MORTGAGES_LTV_REDRAW = 0;
		if(redraw)
			HH_MORTGAGES_LTV_REDRAW = 1;

		//Sample DSTI: use result from gsl_ran_multivariate_gaussian()
		sampled_dsti = gsl_vector_get(result,0);
		exp_dsti = exp(sampled_dsti);

		//transform using floor/ceiling:
		floor = MORTGAGE_SAMPLING_FLOOR_CAPS[0];
		ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[3];
		MORTGAGE_SAMPLING_DRAW.dsti = min(max(floor, exp_dsti+DSTI_UPWARD_SHIFT), ceil);

		SAMPLED_DSTI = MORTGAGE_SAMPLING_DRAW.dsti;

		//Sample LTV: use result from gsl_ran_multivariate_gaussian()
		sampled_loan_to_value =  gsl_vector_get(result,1);
		exp_loan_to_value = exp(sampled_loan_to_value);

		//transform using floor/ceiling:
		floor = MORTGAGE_SAMPLING_FLOOR_CAPS[1];
		ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[4];
		MORTGAGE_SAMPLING_DRAW.loan_to_value = min(max(floor, exp_loan_to_value), ceil);
		
		//Sample Maturity in years: use result from gsl_ran_multivariate_gaussian()
		sampled_maturity =  gsl_vector_get(result,2);
		exp_maturity = exp(sampled_maturity);

		//transform using floor/ceiling:
		floor = MORTGAGE_SAMPLING_FLOOR_CAPS[2];
		ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[5];
		MORTGAGE_SAMPLING_DRAW.maturity = min(max(floor, exp_maturity), ceil);
	}

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_MORTGAGE_HH_UNITTEST)
	{
		printf("\n IT: %d ID: %d Household_sample_mortgage_attributes", DAY, ID);
		printf("\n\tSampled: dsti = %f\n\tSampled: loan_to_value = %f\n\tSampled: maturity = %f	\n\tExp-transformed: dsti = %f\n\tExp-transformed: loan_to_value = %f\n\tExp-transformed: maturity = %f \n\tRestricted: dsti = %f\n\tRestricted: loan_to_value = %f\n\tRestricted: maturity = %f",
				sampled_dsti, sampled_loan_to_value, sampled_maturity, exp_dsti, exp_loan_to_value, exp_maturity, MORTGAGE_SAMPLING_DRAW.dsti, MORTGAGE_SAMPLING_DRAW.loan_to_value, MORTGAGE_SAMPLING_DRAW.maturity);
	}      
	#endif
	
	gsl_vector_free (v);
	gsl_matrix_free (m);

	return 0;   
}

/** \fn Household_check_mortgage_eligibility()
 * \brief Function to check mortgage eligibility criteria and decide whether to enter the endogenous mortgage loan application process.
 * \brief The household checks the regulatory LTC and DSTI caps, and checks whether its own contribution can be covered by its current payment account (liquid savings).
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: 
 */
int Household_check_mortgage_eligibility()
{
	int i;

	MORTGAGE_ELIGIBILITY = 1;

	// Static or exogenous regulation uses global constants for the regulatory parameters.
	// Dynamic or endogenous regulation uses endogenously determined regulatory parameters: ECB overrides the global constant.
	// See function: Central_Bank_activate_policy_responses()
	if(SWITCH_REGULATORY_LTV_DSTI_CAPS)
	{
		//Check sampled LTV and DSTI against regulatory LTV and DSTI caps
		if(MORTGAGE_SAMPLING_DRAW.dsti > REGULATORY_DSTI_CAP || MORTGAGE_SAMPLING_DRAW.loan_to_value > REGULATORY_LTV_CAP)
		{
			//Set flags
			HH_MORTGAGES_DSTI_CAP_ACTIVE = 0;
			if(MORTGAGE_SAMPLING_DRAW.dsti > REGULATORY_DSTI_CAP)
				HH_MORTGAGES_DSTI_CAP_ACTIVE = 1;

			HH_MORTGAGES_LTV_CAP_ACTIVE = 0;
			if(MORTGAGE_SAMPLING_DRAW.loan_to_value > REGULATORY_LTV_CAP)
				HH_MORTGAGES_LTV_CAP_ACTIVE = 1;

			//New feature: set draw values equal to caps IF it exceeded the cap
			if(SWITCH_SET_LTV_VALUE_EQUAL_TO_CAP_ON_EXCEEDING)
			{
				if(MORTGAGE_SAMPLING_DRAW.dsti > REGULATORY_DSTI_CAP)
					MORTGAGE_SAMPLING_DRAW.dsti = REGULATORY_DSTI_CAP;

				if(MORTGAGE_SAMPLING_DRAW.loan_to_value > REGULATORY_LTV_CAP)
					MORTGAGE_SAMPLING_DRAW.loan_to_value = REGULATORY_LTV_CAP;
			}
			else
				MORTGAGE_ELIGIBILITY = 0;

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_MORTGAGE_HH)
			{
				printf("\n IT: %d ID: %d Household_check_mortgage_eligibility", DAY, ID);
				printf("\n\t Mortgage_eligibility check failed: %d. Reason:", MORTGAGE_ELIGIBILITY);
				if(MORTGAGE_SAMPLING_DRAW.dsti > REGULATORY_DSTI_CAP)			printf(" DSTI_CAP triggered [dsti > DSTI_CAP: %f > %f]", MORTGAGE_SAMPLING_DRAW.dsti, REGULATORY_DSTI_CAP);
				if(MORTGAGE_SAMPLING_DRAW.loan_to_value > REGULATORY_LTV_CAP) printf(" LTV_CAP triggered  [ltv > LTV_CAP:  %f > %f]", MORTGAGE_SAMPLING_DRAW.loan_to_value, REGULATORY_LTV_CAP);
			}      
			#endif
		}
	}


	// Check if any of the existing mortgages are in NPL status.
	// If so, do not allow this HH to get a new mortgage
	if (HAVE_HMR_MORTGAGE)
	{
		//Check HMR NPL status
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 2)
			MORTGAGE_ELIGIBILITY = 0;
	}

	if (HAVE_OP_MORTGAGE)
	{
		for (i=0; i< HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		{
			//Check Other property NPL status
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status == 2)
				MORTGAGE_ELIGIBILITY = 0;
		}
	}


	// Check if household is unemployed
	if (EMPLOYEE_FIRM_ID == -1)
		MORTGAGE_ELIGIBILITY = 0;

	// Check if household has negative income (due to losses from OP business);
	if (MEAN_NET_INCOME<0.0)
		MORTGAGE_ELIGIBILITY = 0;

	return 0;   
}

int Household_check_max_expenditure1()
{
	int_array banks;
	init_int_array(&banks);

	START_BANK_IDENTITY_MESSAGE_LOOP
		add_int(&banks,bank_identity_message->bank_id);
	FINISH_BANK_IDENTITY_MESSAGE_LOOP

	if (MORTGAGES_AT_HOUSEBANK || banks.size==0)
		MORTGAGES_NEXT_BANK_ID = BANK_ID;
	else
		MORTGAGES_NEXT_BANK_ID = banks.array[random_int(0,banks.size-1)];

	free_int_array(&banks);
	
	mortgage_application_adt mortgage;

	init_mortgage_application_adt(&mortgage);

	double annuity = MORTGAGE_SAMPLING_DRAW.dsti * MEAN_NET_INCOME;
	
	mortgage.annuity 				= annuity;
	mortgage.sampled_loan_to_value	= MORTGAGE_SAMPLING_DRAW.loan_to_value;
	mortgage.sampled_maturity		= MORTGAGE_SAMPLING_DRAW.maturity*12;
	
	if (IS_TENANT || BTL_GENE)
		add_mortgage_max_expenditure_request_message(ID, MORTGAGES_NEXT_BANK_ID, mortgage);

	return 0.0;
}

int Household_check_max_expenditure2()
{

	START_MORTGAGE_MAX_EXPENDITURE_RESPONSE_MESSAGE_LOOP
		MAX_EXPENDITURE = mortgage_max_expenditure_response_message->max_expenditure;
	FINISH_MORTGAGE_MAX_EXPENDITURE_RESPONSE_MESSAGE_LOOP

	return 0;
}

int Household_check_real_estate_market1()
{
	if (IS_TENANT)
		add_real_estate_market_info_request_message(ID, 0, MAX_EXPENDITURE);
	else
		if (BTL_GENE)
			add_real_estate_market_info_request_message(ID, 1, MAX_EXPENDITURE);

	return 0;
}

int Household_check_real_estate_market2()
{
	START_REAL_ESTATE_MARKET_INFO_RESPONSE_MESSAGE_LOOP

		ESTIMATED_HOUSE_QUALITY = real_estate_market_info_response_message->housing_market_quality;
		ESTIMATED_HOUSE_PRICE = real_estate_market_info_response_message->housing_market_price;
		ESTIMATED_RENTAL_PRICE = real_estate_market_info_response_message->rental_market_price;

	FINISH_REAL_ESTATE_MARKET_INFO_RESPONSE_MESSAGE_LOOP

	return 0;
}

int Household_asses_houses1()
{
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id>0)
		add_asses_house_request_message(ID,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality);

	int i=0;
	for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
		add_asses_house_request_message(ID,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality);

	return 0;
}

int Household_asses_houses2()
{
	int i=0;

	START_ASSES_HOUSE_RESPONSE_MESSAGE_LOOP
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id==asses_house_response_message->object_id)
			{

				if (asses_house_response_message->value_estimated>EPSILON)
				{
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated = asses_house_response_message->value_estimated;
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base = HOUSING_MARKET_PRICE_INDEX;
					vlog_data("h_estimate_good", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated);
				}
				else
					if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base > EPSILON && HOUSING_MARKET_PRICE_INDEX > EPSILON)
					{
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated * HOUSING_MARKET_PRICE_INDEX / HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base;
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base = HOUSING_MARKET_PRICE_INDEX;
						vlog_data("h_estimate_backup", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated);
					}
					else
					{
						vlog_data("h_estimate_failed", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated);

					}

				assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated>0);
				assert(isfinite(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated));
			}

			for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++) 
			{
				if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==asses_house_response_message->object_id)
				{
					if (asses_house_response_message->value_estimated>EPSILON)
					{
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated = asses_house_response_message->value_estimated;
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base = HOUSING_MARKET_PRICE_INDEX;
						vlog_data("h_estimate_good", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated);
					}
					else
						if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base > EPSILON && HOUSING_MARKET_PRICE_INDEX > EPSILON)
						{
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated * HOUSING_MARKET_PRICE_INDEX / HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base;
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base = HOUSING_MARKET_PRICE_INDEX;
							vlog_data("h_estimate_backup", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated);
						}
						else
						{
							vlog_data("h_estimate_failed", 4, (double) HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price,HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated);
						}

					if (asses_house_response_message->rent_estimated>EPSILON)
					{
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated = asses_house_response_message->rent_estimated;
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rpi_base = RENTAL_MARKET_PRICE_INDEX;
					}
					else
						if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rpi_base > EPSILON && RENTAL_MARKET_PRICE_INDEX > EPSILON)
						{
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated * RENTAL_MARKET_PRICE_INDEX / HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rpi_base;
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rpi_base = RENTAL_MARKET_PRICE_INDEX;
						}		

					assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated>0);
					assert(isfinite(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated));
					assert(isfinite(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated));
				}
			}
	FINISH_ASSES_HOUSE_RESPONSE_MESSAGE_LOOP

	return 0;
}




/** \fn Household_check_housing_state()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: 
 */
int Household_check_housing_state()
{
	int i, found;

	PLAN_BUY_REAL_ESTATE = 0;
	PLAN_SELL_REAL_ESTATE = 0;

	HMR_DECISION_BUY = 0;
	HMR_DECISION_NO_AVAILABILITY_ON_RENTAL_MARKET = 0;
	HMR_DECISION_CANNOT_AFFORD = 0;
	OP_DECISION_BUY = 0;

	PLAN_BUY_HMR = 0;

	if (!PERSISTENT_HMR_SELLING)
		PLAN_SELL_HMR = 0;

	// Dont sell if not paying!
	if (HAVE_HMR_MORTGAGE && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status > 0)
		PLAN_SELL_HMR = 0;

	if (!PERSISTENT_OP_SELLING)
	{
		PLAN_SELL_OP = 0;
		SELL_OP_OBJECT_ID =  -1;
	}
	else
	{
		if (PLAN_SELL_OP)
		{
			for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
			{
				if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==SELL_OP_OBJECT_ID)
				{
					found=1;
					if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status!=0)
					{
						PLAN_SELL_OP = 0;
						SELL_OP_OBJECT_ID =  -1;
					}
					assert(found);
					break;
				}
			}
		}
	}
	
	PLAN_RENT_HMR = 0;

	REQUEST_MORTGAGE = 0;

	PLAN_BUY_OP = 0;

	DEFAULTED_ON_RENT = 0;
	EVICTED_AS_TENANT = 0;
	EVICTED_AS_OWNER = 0;

	HMR_MORTGAGE_GRANTED = 0;
	OP_MORTGAGE_GRANTED = 0;

	SOLD_OBJECT_ID = -1;

	//All households store the price index info from ClearingHouse:
	START_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP
		RENTAL_MARKET_PRICE_MEAN = real_estate_market_data_message->rental_market_price_mean;
		RENTAL_MARKET_PRICE_INDEX = real_estate_market_data_message->rental_market_price_index;
		HOUSING_MARKET_PRICE_MEAN = real_estate_market_data_message->housing_market_price_mean;
		HOUSING_MARKET_PRICE_INDEX = real_estate_market_data_message->housing_market_price_index;
	FINISH_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP


	/*
	 * RANDOM ACTIONS: 
	 * 		RANDOM_HMR_BUYING  = 1
	 *		RANDOM_HMR_SELLING = 1
	 *		RANDOM_OP_BUYING   = 1
	 *		RANDOM_OP_SELLING  = 1
 	 *
 	 * REAL BEHAVIOR: 
 	 *		REAL_HMR_BUYING  = 1
 	 *		REAL_HMR_SELLING = 1
 	 *		REAL_OP_BUYING   = 1
 	 *		REAL_OP_SELLING  = 1
 	 */
		if (!DISABLE_HOUSING_AND_RENTAL_MARKET)
		{
			if (IS_HOMELESS==1)
			{
				PLAN_RENT_HMR = 1;
			}
			
			real_hmr_buying();

			random_hmr_buying();
			
			real_hmr_selling();

			random_hmr_selling();

			real_op_selling();

	        random_op_selling();

			real_op_buying();

			random_op_buying();
		}

		if (PLAN_SELL_HMR && PLAN_SELL_OP)
		{
			PLAN_SELL_OP = 0;
			SELL_OP_OBJECT_ID =  -1;
		}

		if (PLAN_BUY_HMR || PLAN_BUY_OP)
			PLAN_BUY_REAL_ESTATE = 1;

		if (PLAN_SELL_HMR || PLAN_SELL_OP)
			PLAN_SELL_REAL_ESTATE = 1;

		if (PLAN_BUY_REAL_ESTATE && PLAN_SELL_REAL_ESTATE)
		{
			PLAN_BUY_REAL_ESTATE = 0;
			PLAN_SELL_REAL_ESTATE = 0;

			PLAN_BUY_HMR = 0;
			PLAN_SELL_HMR = 0;
			PLAN_RENT_HMR = 0;
			REQUEST_MORTGAGE = 0;
			PLAN_BUY_OP = 0;
			PLAN_SELL_OP = 0;
		}

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_check_housing_state (1): After MORTGAGE_ELIGIBILITY", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;
}

/** \fn Household_sell_real_estate()
 * \brief Function to sell a real_estate unit.
 * \brief Message input: 
 * \brief Message output: 
 * \brief UNITTEST: 
 */
int Household_sell_real_estate()
{
	int object_id;
	double value;
	double ask_price;
	double reservation_price;

	ask_adt ask;
	init_ask_adt(&ask);

	//If selling HMR:
	if(PLAN_SELL_HMR)
	{
		if (!FREE_EARLY_REPAYMENT)
			reservation_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal
								+ HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest;
		else
			reservation_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;

		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market==0)
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.selling_init_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated;

		value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.selling_init_price;

 		object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id;

		if (object_id==0)
            fprintf(stderr, "\n\nIT %d ID: %d WARNING in Household_sell_real_estate: object_id=0 in main_residence  at moment of selling", DAY, ID);

        assert(object_id>0);

        ask.price = value;



		ask.object_id = object_id;
		ask.quality = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.quality;
		ask.id = ID;
		ask.months_on_market = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market;

		//HMR selling: price decrease
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market>0)
			ask.price = ask.price  * pow(1.0 - CONST_HOUSE_ASK_PRICE_ADJUSTMENT_HOUSEHOLD, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market);

	
		ask.price = max(reservation_price, ask.price);

		if (ask.price < (1.0 + CONST_HOUSING_MARKET_MAX_MARKUP)*value)
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market += 1;
			add_real_estate_price_ask_message(ask.price, ask);
		}
		else
		{
			PLAN_SELL_HMR = 0;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.months_on_housing_market = 0;
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_UNITTEST)
		{
			printf("\n\n ITS %d ID %4d Household_sell_real_estate:", DAY, ID);
			printf(" HMR ask: ask.object_id %d ask.id %d ask.price %f", ask.object_id, ask.id, ask.price);
		}      
		#endif

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_ID_PROBE)
		{
			printf("\n\n ITS %d ID %4d Household_sell_real_estate:", DAY, ID);
			printf(" HMR ask: ask.object_id %d ask.id %d ask.price %f", ask.object_id, ask.id, ask.price);
		}      
		#endif
	}

	//If selling Other Properties:
	if(PLAN_SELL_OP)
	{

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_UNITTEST)
		{
			printf("\n\n ITS %d ID %4d Household_sell_real_estate:", DAY, ID);
		}      
		#endif

		//Find object
		int found=0;
		int i=0;
		for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
		{
			if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==SELL_OP_OBJECT_ID)
			{
				found=1;
				break;
			}
		}

		if(!found)
			fprintf(stderr, "OP to be sold not found! (ITS %d, ID %d, SELL_OP_OBJECT_ID %d", DAY, ID, SELL_OP_OBJECT_ID);

		assert(found);

		reservation_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal
							+ HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest;

		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market == 0)
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].selling_init_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated;

		value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].selling_init_price;
		
        if(!(value>0))
        {
            fprintf(stderr, "\n\nIT %d ID: %d BUG in Household_sell_real_estate: value<=0 value %.2f", DAY, ID, value);
        }
		assert(value>0);

 		object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;

		if (object_id==0)
			fprintf(stderr, "\n\nIT %d ID: %d BUG in Household_sell_real_estate: object_id=0 in rental_property_list at moment of selling", DAY, ID);
		assert(object_id>0);

		ask_price = value;

 		if(!(ask_price > 0.0))
 		{
            fprintf(stderr, "\n\nIT %d ID: %d BUG in Household_sell_real_estate: ask_price<=0 ask_price %.2f", DAY, ID, ask_price);
 			fprintf(stderr, "\n\t HOUSING_MARKET_PRICE_INDEX %.2f", HOUSING_MARKET_PRICE_INDEX);
 			fprintf(stderr, "\n\t hpi_base %.2f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base);
			fprintf(stderr, "\n\t value %.2f", value);
            fprintf(stderr, "\n\t ask_price %.2f = %.2f * %.2f  / %.2f", ask_price, value, HOUSING_MARKET_PRICE_INDEX, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base);
 		}
		assert(ask_price>0);
		
		ask.price =ask_price;
		ask.object_id = object_id;
		ask.quality = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality;
		ask.id = ID;
		ask.months_on_market = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market;

		//OP selling: price decrease
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market>0)
			ask.price = ask.price  * pow(1.0 - CONST_HOUSE_ASK_PRICE_ADJUSTMENT_HOUSEHOLD, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market);
		
		ask.price = max(reservation_price, ask.price);

		if (ask.price < (1.0 + CONST_HOUSING_MARKET_MAX_MARKUP)*value)
		{
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market += 1;
			add_real_estate_price_ask_message(ask.price, ask);
		}
		else
		{
			PLAN_SELL_OP = 0;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_housing_market = 0;
		}




		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_UNITTEST)
		{
			printf("\n\n ITS %d ID %4d Household_sell_real_estate:", DAY, ID);
			printf(" OP ask: ask.object_id %d ask.id %d ask.price %f", ask.object_id, ask.id, ask.price);
		}      
		#endif
	}

	//free
	free_ask_adt(&ask);

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_sell_real_estate (3): After add_real_estate_price_ask", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;
}

/** \fn Household_buy_real_estate()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: PASS 5.1.17
 */
int Household_buy_real_estate()
{
	double principal = 0.0;
	double ltv = 0.0;
	double own_contribution = 0.0;
	double price = 0.0;
	double sum = 0.0;

	bid_adt bid;
	init_bid_adt(&bid);

	if (PLAN_BUY_HMR)
	{
		if (HMR_MORTGAGE_GRANTED == 1)
		{
			principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.outstanding_principal;
			
			//EQN (16-17): re-use data from sample_draw
			ltv = MORTGAGE_SAMPLING_DRAW.loan_to_value;

			price 			 = principal * (1/ltv);
			own_contribution = price * (1-ltv);

			//Sanity checks:
			sum = own_contribution + principal;

			if ( sum < price*0.99)
				fprintf(stderr, "\n\n ITS %d ID %4d PLAN_BUY_HMR Household_buy_real_estate: Incorrect computation\n own_contribution + principal = %f < price %f [%f + %f < %f]", DAY, ID, sum, price, own_contribution, principal, price);
			if ( sum > price*1.01)
				fprintf(stderr, "\n\n ITS %d ID %4d PLAN_BUY_HMR Household_buy_real_estate: Incorrect computation\n own_contribution + principal = %f > price %f: [%f + %f > %f]", DAY, ID, sum, price, own_contribution, principal, price);

			if (own_contribution < PAYMENT_ACCOUNT)
			{
				vlog_data("hh_hmr_own_contribution", 2, MORTGAGE_SAMPLING_DRAW.dsti, 1.0);

				bid.id = ID;
				bid.price =  price;
				bid.for_op = 0;
				if (HOUSING_MARKET_DEMAND_MIN_QUALITY)
					bid.min_quality = max(0,ESTIMATED_HOUSE_QUALITY-100);
				else
					bid.min_quality = 0;

				if (bid.price<0)
				{
					printf("--- :( ---\n");
					printf("ltv: %f principal: %f price: %f\n", ltv, principal,price);
					printf("m: %d, a: %f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.initial_duration, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.monthly_total_repayment);
					printf("\n");
				}

				//add_real_estate_price_bid_message(double price, bid_adt bid);
				add_real_estate_price_bid_message(bid.price, bid);
			
				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_HOUSING_UNITTEST)
				{
					printf("\n\n ITS %d ID %4d Household_buy_real_estate:", DAY, ID);
					printf(" HMR bid: bid.id %d bid.price %f", bid.id, bid.price);
				}      
				#endif

				#ifdef _DEBUG_MODE    
				if (PRINT_HOUSING_INFO)
				{
					printf("\nIT %d INFO ID %4d (HMR mortgage): OK: mortgage granted by Bank ID %d and own_contribution < PAYMENT_ACCOUNT\n", DAY, ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.bank_id);
				}
				#endif
			}
			else
			{
				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("\nIT %d INFO ID %4d (HMR mortgage): Not enough money to satisfy LTV: ltv %f own_contribution %f payment_account %f\n", DAY, ID, ltv, own_contribution, PAYMENT_ACCOUNT);
					printf("Start Saving... Target ratio %f -> ", LIQUID_WEALTH_INCOME_RATIO_TARGET);
				}
				#endif

				vlog_data("hh_hmr_own_contribution", 2, MORTGAGE_SAMPLING_DRAW.dsti, 0.0);

				//Endogenous updating of the buffer stock savings parameter
				LIQUID_WEALTH_INCOME_RATIO_TARGET = max(LIQUID_WEALTH_INCOME_RATIO_TARGET, own_contribution / (MEAN_NET_INCOME-HOUSING_BUDGET));

				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("%f \n", LIQUID_WEALTH_INCOME_RATIO_TARGET);
				}
				#endif
			}
		}
		else
		{
			#ifdef _DEBUG_MODE
			if (PRINT_HOUSING_INFO)
			{
				printf("\nIT %d INFO ID %4d (HMR mortgage): Failed to get mortgage\n", DAY, ID);
			}
			#endif
		}
	}

	//Only allow OP buying if no HMR buying to prevent collisions in determination of own_contribution and budget
	if (PLAN_BUY_OP && PLAN_BUY_HMR==1)
		PLAN_BUY_OP=0;

	if (PLAN_BUY_OP && PLAN_BUY_HMR==0)
	{
		if (OP_MORTGAGE_GRANTED == 1)
		{
			principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal;

			//EQN (16-17): re-use data from sample_draw
			ltv = MORTGAGE_SAMPLING_DRAW.loan_to_value;

			price 			 = principal * (1/ltv);
			own_contribution = price * (1-ltv);


			//Sanity checks:
			sum = own_contribution + principal;

			if ( sum < price*0.99)
				fprintf(stderr, "\n\n ITS %d ID %4d PLAN_BUY_OP Household_buy_real_estate: Incorrect computation\n own_contribution + principal = %f < price %f [%f + %f < %f]", DAY, ID, sum, price, own_contribution, principal, price);
			if ( sum > price*1.01)
				fprintf(stderr, "\n\n ITS %d ID %4d PLAN_BUY_OP Household_buy_real_estate: Incorrect computation\n own_contribution + principal = %f > price %f: [%f + %f > %f]", DAY, ID, sum, price, own_contribution, principal, price);

			if (own_contribution < PAYMENT_ACCOUNT)
			{
				bid.id = ID;
				bid.price =  price;
				bid.for_op = 1;
				bid.min_quality = 0;

				if (bid.price<0)
				{
					printf("--- :( ---\n");
					printf("ltv: %f principal: %f price: %f\n", ltv, principal,price);
					printf("m: %d, a: %f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.initial_duration, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.monthly_total_repayment);
					printf("\n");
				}

				//add_real_estate_price_bid_message(double price, bid_adt bid);
				add_real_estate_price_bid_message(bid.price, bid);
			
				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_HOUSING_UNITTEST)
				{
					printf("\n\n ITS %d ID %4d Household_buy_real_estate:", DAY, ID);
					printf(" HMR bid: bid.id %d bid.price %f", bid.id, bid.price);
				}      
				#endif

				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("\nIT %d INFO ID %4d (OP mortgage): OK: mortgage granted by Bank ID %d and own_contribution < PAYMENT_ACCOUNT\n", DAY, ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.bank_id);
				}
				#endif
			}
			else
			{
				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("\nIT %d INFO ID %4d (OP mortgage): Not enough money to satisfy LTV: ltv %f own_contribution %f payment_account %f\n", DAY, ID, ltv, own_contribution, PAYMENT_ACCOUNT);
					printf("Start Saving... Target ratio %f -> ", LIQUID_WEALTH_INCOME_RATIO_TARGET);
				}
				#endif

				//Endogenous updating of the buffer stock savings parameter
				LIQUID_WEALTH_INCOME_RATIO_TARGET = max(LIQUID_WEALTH_INCOME_RATIO_TARGET, own_contribution / (MEAN_NET_INCOME-HOUSING_BUDGET));

				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("%f \n", LIQUID_WEALTH_INCOME_RATIO_TARGET);
				}
				#endif

			}
		}
		else
		{
			#ifdef _DEBUG_MODE
			if (PRINT_HOUSING_INFO)
			{
				printf("\nIT %d INFO ID %4d (OP mortgage): Failed to get OP mortgage\n", DAY, ID);
			}
			#endif
		}
	}

	//free
	free_bid_adt(&bid);

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_buy_real_estate (3): After real_estate_price_bid", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;   
}


/** \fn Household_receive_real_estate_transaction_as_seller()
 * \brief Function to read real_estate transactions
 * \brief Message input: real_estate_transaction, filtered on m.seller_id == a.id
 * \brief Message output:
 * \brief UNITTEST: PASS 5.1.17
 */
int Household_receive_real_estate_transaction_as_seller()
{
	double value __attribute__((unused)) = 0.0;
	double price __attribute__((unused)) = 0.0;
    int object_id __attribute__((unused)) = 0;

    int tenant_id __attribute__((unused)) = 0;
    double profit __attribute__((unused)) = 0.0;
    
   	START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

        object_id = real_estate_transaction_message->house.object_id;
        
        if (object_id==0) fprintf(stderr, "\n\nIT %d ID: %d WARNING in Household_receive_real_estate_transaction_as_seller: object_id=0 in real_estate_transaction mesg", DAY, ID);
        
        price = real_estate_transaction_message->house.purchase_price;

		PAYMENT_ACCOUNT += price;
		log_inflow("household", "payment_account", "net_worth", "real_estate_transaction", ID, price);

		assert(PAYMENT_ACCOUNT>=0);

		assert(PLAN_SELL_HMR || PLAN_SELL_OP);

		//If selling HMR:
		if(PLAN_SELL_HMR)
		{
            //State transitions
            JUST_SOLD_HMR = 1;
            PLAN_SELL_HMR = 0;
		}

		//If selling Other Properties:
		

		//Multiple OPs:
		value=0.0;
			            
        //Use to find correct mortgage in Household_full_mortgage_payment()
		SOLD_OBJECT_ID = object_id;

		if(PLAN_SELL_OP)
		{
            //Find object
			int found=0;
			int i=0;
			for(i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
			{
				if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==object_id)
				{
					found=1;
					break;
				}
			}

			if(!found)
				fprintf(stderr, "OP sold not found! [Household_receive_real_estate_transaction_as_seller] (ITS %d, ID %d, SOLD_OBJECT_ID %d\n", DAY, ID, SOLD_OBJECT_ID);

			assert(found);

			tenant_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id;

			// Cancel rental contract
			add_landlord_cancel_rental_contract_message(tenant_id, ID, object_id);
				
			//State transitions
			JUST_SOLD_OP = 1;

			// Unit will be removed from property list later
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
        {
            printf("\n\nIT %d ID: %d Household_receive_real_estate_transaction_as_seller (3): After REAL_ESTATE_TRANSACTION_MESSAGE_LOOP", DAY, ID);
            Household_print_housing_states();
        }
        #endif
        
    FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

	//State transition for landlords: Multiple OP mesgs so do this after msg loop:
	if (JUST_SOLD_OP)
	{
		PLAN_SELL_OP = 0;
		SELL_OP_OBJECT_ID =  -1;
	}

	return 0;   
}


/** \fn Household_receive_real_estate_transaction_as_buyer()
 * \brief Function to read real_estate transactions
 * \brief Message input: real_estate_transaction, filtered on m.buyer_id == a.id
 * \brief Message output:
 * \brief UNITTEST: PASS 5.1.17
 * \brief UNITTEST: 
 */
int Household_receive_real_estate_transaction_as_buyer()
{
	double price;
	double value __attribute__((unused)) = 0.0;
    int sold __attribute__((unused)) = 0;
	double principal = 0.0;

	double monthly_rent;
	
	int object_id;
	double quality;
	int tenant_id;
	int landlord_id;

   	START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

		price = real_estate_transaction_message->house.purchase_price;

		//If buying HMR:
		if(PLAN_BUY_HMR)
		{
			// Adjust mortgage contract
			adjust_mortgage_contract2(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence, MORTGAGE_SAMPLING_DRAW.loan_to_value*price);

			vlog_data("hh_hmr_adjusted_mortgage", 1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.monthly_total_repayment / MEAN_NET_INCOME);

			// Buy House
			principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence.outstanding_principal;

			if (PAYMENT_ACCOUNT>0.0 && PAYMENT_ACCOUNT - (price - principal)<0.0)
			{
				fprintf(stderr, "\n\nIT %d ID %d Household_receive_real_estate_transaction_as_buyer (PLAN_BUY_HMR)", DAY, ID);
				fprintf(stderr, "\n\tPAYMENT_ACCOUNT before %f>0.0\n\t PAYMENT_ACCOUNT after %f<0.0", PAYMENT_ACCOUNT, PAYMENT_ACCOUNT - (price - principal));
				fprintf(stderr, "\n\tPAYMENT_ACCOUNT %f", PAYMENT_ACCOUNT);
				fprintf(stderr, "\n\t(price - principal)=%f", (price - principal));
				fprintf(stderr, "\n\tprice %f", price);
				fprintf(stderr, "\n\tprincipal %f", principal);
			}

			PAYMENT_ACCOUNT -= (price - principal); //Cause for negative PAYMENT_ACCOUNT ?
			log_outflow("household", "payment_account", "net_worth", "real_estate_transaction", ID, price);

			assert(PAYMENT_ACCOUNT >= 0);

			// Stop rent: Cancel rental contract with landlord
			//This code comes from: Household_check_rental_state(), line 367 - 392
			landlord_id = RENTAL_UNIT.owner_id;
			object_id 	= RENTAL_UNIT.object_id;
            if (IS_TENANT)
				add_tenant_cancel_rental_contract_message(ID, landlord_id, object_id);

			//State transitions
            PLAN_BUY_HMR = 0;
			JUST_PURCHASED_HMR = 1;

			IS_HOMELESS = 0;
			IS_OWNER = 1;
            IS_TENANT = 0;

			#ifdef _DEBUG_MODE    
			if (PRINT_RENTAL_INFO)
			{
				printf("\nIT %d ID %d SENDING TENANT CANCEL RENTAL CONTRACT (ID: %d | T: %d L: %d, O: %d)", DAY, ID, ID, ID, landlord_id, object_id);
			}
			#endif

			init_real_estate_adt(&RENTAL_UNIT);

			copy_real_estate_adt(&(real_estate_transaction_message->house), &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence);

			//Set attributes at purchase day
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base = HOUSING_MARKET_PRICE_INDEX;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_day = DAY;

			//Copy mortgage info into house_adt
			copy_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);

			// Activate mortgage
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active = 1;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.type = 1;

			//Set object_id inside HMR mortgage adt
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id;

			//Reset info in initial_mortgage_main_residence
			init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_main_residence);

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING)
			{
                printf("\n HH ID %4d HMR purchase\tobject %4d\tprice=%.3f\tmortgage object id %d", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id, price, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id);
			}      
			#endif

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING_BRANCHES)
		    {
		        printf("\n\nIT %d ID: %d Household_receive_real_estate_transaction_as_buyer (3): PLAN_BUY_HMR", DAY, ID);
		        Household_print_housing_states();
		    }
		    #endif

			//Now reset the buffer stock savings to its default value (prevents overly high savings)
			LIQUID_WEALTH_INCOME_RATIO_TARGET = CONST_LIQUID_WEALTH_INCOME_RATIO_TARGET;

		}

		//If buying Other Properties: get info from HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property
		if(PLAN_BUY_OP)
		{
			// Adjust mortgage contract
			adjust_mortgage_contract2(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property, MORTGAGE_SAMPLING_DRAW.loan_to_value*price);

			// Buy House
			principal = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.outstanding_principal;

			if (PAYMENT_ACCOUNT>0.0 && PAYMENT_ACCOUNT - (price - principal)<0.0)
			{
				fprintf(stderr, "\n\nIT %d ID %d Household_receive_real_estate_transaction_as_buyer (PLAN_BUY_OP)", DAY, ID);
				fprintf(stderr, "\n\tPAYMENT_ACCOUNT before %f>0.0\n\t PAYMENT_ACCOUNT after %f<0.0", PAYMENT_ACCOUNT, PAYMENT_ACCOUNT - (price - principal));
				fprintf(stderr, "\n\tPAYMENT_ACCOUNT %f", PAYMENT_ACCOUNT);
				fprintf(stderr, "\n\t(price - principal)=%f", (price - principal));
				fprintf(stderr, "\n\tprice %f", price);
				fprintf(stderr, "\n\tprincipal %f", principal);
			}

			PAYMENT_ACCOUNT -= (price - principal);  //Cause for negative PAYMENT_ACCOUNT ?
			log_outflow("household", "payment_account", "net_worth", "real_estate_transaction", ID, price);

			//State transitions
			IS_LANDLORD = 1;
			JUST_PURCHASED_OP = 1;

			//Multiple OPs:
			object_id = real_estate_transaction_message->house.object_id;
			quality = real_estate_transaction_message->house.quality;
			tenant_id = -1; //no tenant yet
			landlord_id = ID;
			monthly_rent = ESTIMATED_RENTAL_PRICE;
			sold = 0;

			price = real_estate_transaction_message->house.purchase_price;
			value = real_estate_transaction_message->house.purchase_price;

			if (object_id==0) fprintf(stderr, "\n\nIT %d ID: %d WARNING object_id=0 in real_estate_transaction mesg (Household_receive_real_estate_transaction_as_buyer)", DAY, ID);

			//Set object_id inside HMR mortgage adt before copying into rental property array
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.object_id = object_id;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.active = 1;
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property.type = 2;

			//Copy data from new OP into rental_property_list
			//NOTE: Cannot use the copy_house_adt() function here, due to dynamic array of adt, not simple adt like in HMR
			add_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list, 
				object_id, 
				ID, 
				tenant_id, 
				quality, 
				monthly_rent, 
				DAY, 
				price,
				HOUSING_MARKET_PRICE_INDEX, 
				RENTAL_MARKET_PRICE_INDEX, 
				PRICE_INDEX, 
				price, 
				ESTIMATED_RENTAL_PRICE, 
				0, 
				0,
				0.0,
				0.0, 
				&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);

			//Reset info in initial_mortgage_other_property: 
			// Postpone removing the initial mortgage contract until after we send the mortgage acceptance message.
			// see: Household_accept_mortgage_loan()
			//init_mortgage_contract_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_mortgage_other_property);

			int last = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size-1;
            
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING)
			{
                printf("\n HH ID %4d  OP purchase\tobject %4d\tprice=%.3f\tmortgage object id %d", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[last].object_id, price, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[last].object_id);
			}      
			#endif

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING_BRANCHES)
	        {
	            printf("\n\nIT %d ID: %d Household_receive_real_estate_transaction_as_buyer (3): PLAN_BUY_OP", DAY, ID);
	            Household_print_housing_states();
	        }
	        #endif

     		//Do NOT reset the buffer stock savings to its default value for BTL Households (always want to save for own contribution to purchase OPs)
			//LIQUID_WEALTH_INCOME_RATIO_TARGET = CONST_LIQUID_WEALTH_INCOME_RATIO_TARGET;
		}        
	FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

	//State transition for landlords: Multiple OP mesgs so do this after msg loop:
	if (JUST_PURCHASED_OP)
	{
		PLAN_BUY_OP = 0;
	}
    
	return 0;   
}

/** \fn Household_receive_seizure_recompensation()
 * \brief Function to receive recompensation from Bank for seizure of house (diff between value of real estate - collateral value)
 * \brief Message input: seized_liquidity_recompensation_message
 * \brief Message output: None
 * \brief UNITTEST: 
 */
int Household_receive_seizure_recompensation()
{
	START_SEIZED_COLLATERAL_RECOMPENSATION_MESSAGE_LOOP
		
		PAYMENT_ACCOUNT += seized_collateral_recompensation_message->amount;
		log_inflow("household", "payment_account", "net_worth", "seized_collateral_recompensation", ID, seized_collateral_recompensation_message->amount);


		#ifdef _DEBUG_MODE    
		if (PRINT_HOUSING_INFO)
		{
			printf("\n Former_owner_id %d PAYMENT_ACCOUNT=%.3f (+%.3f)", seized_collateral_recompensation_message->former_owner_id, PAYMENT_ACCOUNT, seized_collateral_recompensation_message->amount);
		}      
		#endif

	FINISH_SEIZED_COLLATERAL_RECOMPENSATION_MESSAGE_LOOP

	return 0;   
}
