#include "../../header.h"     
#include "../../CentralBank_agent_header.h"
#include "../../my_library_header.h"
#include "Central_Bank_aux_header.h"
    
    
/************Central_Bank Role: ********************************/

/** \fn Central_Bank_()
 * \brief 
 */
int Central_Bank_template()
{
    return 0;
}

/** \fn Central_Bank_monetary_policy()
 * \brief Function to implement monetary policy: send msg with policy_rate.
 * \brief Frequency: Runs only on 1st of the month, top of stategraph
 * \brief currentState:	CB_reset
 * \brief nextState:	CB_market_operations
 */
int Central_Bank_monetary_policy()
{
	
    double inflation=0.0; // YoY at monthly frequency, decimal on [0,1]
    double potential_output=0.0; // real units
    double output=0.0; // real units
 
 	// Note: Inflation is computed by Eurostat: ANNUAL_GROWTH_RATES_MONTHLY.cpi so it only exists for first time after it 240
    START_EUROSTAT_SEND_MACRODATA_MESSAGE_LOOP
		INFLATION_RATE = eurostat_send_macrodata_message->inflation;
        potential_output  = eurostat_send_macrodata_message->potential_output;
        output = eurostat_send_macrodata_message->output;
    FINISH_EUROSTAT_SEND_MACRODATA_MESSAGE_LOOP 

	//if( isnan(1/potential_output)) printf("potential_output=", potential_output);

    if(potential_output>0.0)
		OUTPUT_GAP = output/potential_output;
	else OUTPUT_GAP = 0.0;
	INFLATION_GAP = INFLATION_RATE/100.0 - CONST_TARGET_INFLATION_RATE; // target inflation rate (decimal) at 0.019


    // Initialize ECB policy rate at a constant, decimal on [0,1]
	ECB_INTEREST_RATE = 0.0;
    
    // If Taylor Rule inactive: This setting is to ensure that Banks' income on excess reserves will be zero.
    // Set ECB_INTEREST_RATE equal to constant (default: 0.040) after transient.
    // Setting 1. CONST_ECB_INTEREST_RATE = 0.004 (40 basis points), which equals the interest rate markdown from the ECB_INTEREST_RATE for excess reserves.
    // Setting 2. CONST_ECB_INTEREST_RATE = 0.026 (2.6%, which is the average of the baseline ECB policy rate), 
	if (DAY >= TRANSITION_PHASE_TAYLOR_RULE)
		ECB_INTEREST_RATE = CONST_ECB_INTEREST_RATE;

	//Set bounds on inputs of the Taylor Rule:
	double x,l,u;

	x = OUTPUT_GAP;
	l = CONST_OUTPUT_GAP_LBND; //0.6
	u = CONST_OUTPUT_GAP_UBND; //0.9
	double output_gap = min(max(l, x), u);

	x = INFLATION_GAP;
	l = CONST_INFLATION_GAP_LBND; //-0.02
	u = CONST_INFLATION_GAP_UBND; //0.015
	double inflation_gap = min(max(l, x), u);

    // Taylor rule    
    // Note: the term called TARGET_REAL_INTEREST_RATE is not actually the target real interest rate because what's called OUTPUT GAP is not 
    // an output gap but a capacity utilization rate, i.e. it does not have a mean of zero. 
        
	//Start Taylor rule after own transient phase
    /*
    //Option 1: Do not allow negative policy rates (impose strict ZLB)
    if(SWITCH_POLICY_TAYLOR_RULE && DAY >= TRANSITION_PHASE_TAYLOR_RULE)
        ECB_INTEREST_RATE = max(0.0, CONST_TARGET_REAL_INTEREST_RATE + CONST_MON_POL_1 * output_gap + CONST_MON_POL_2 * inflation_gap);
    */
    
    //Option 2: Allow negative policy rates (no ZLB)
    if(SWITCH_POLICY_TAYLOR_RULE)
        ECB_INTEREST_RATE = max(0.0, CONST_TARGET_REAL_INTEREST_RATE + CONST_MON_POL_1 * output_gap + CONST_MON_POL_2 * inflation_gap);

	//Taylor Rule experiment:
    if(SWITCH_POLICY_TAYLOR_RULE==0)
    {
	    if(DAY <= TRANSITION_PHASE_TAYLOR_RULE)
			ECB_INTEREST_RATE = max(0.0, CONST_TARGET_REAL_INTEREST_RATE + CONST_MON_POL_1 * output_gap + CONST_MON_POL_2 * inflation_gap);
		else
	        ECB_INTEREST_RATE = CONST_ECB_INTEREST_RATE;
	}

    // Send policy rate message: on 1st of the month, top of stategraph
	add_policy_rate_message(ECB_INTEREST_RATE);
    
    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_CB)
    {
        printf("\n Central_Bank_monetary_policy ID: %d",ID); 
        printf("\n\t Inflation: %f",inflation);
        printf("\n\t TARGET_INFLATION_RATE: %f", CONST_TARGET_INFLATION_RATE);
        printf("\n\t INFLATION_GAP: %f Capacity utilization rate: %f", INFLATION_GAP, OUTPUT_GAP);
        printf("\n\t ECB_INTEREST_RATE: %f", ECB_INTEREST_RATE);   
        printf("\n\t [CONST_ECB_INTEREST_RATE]: %f", CONST_ECB_INTEREST_RATE);   
        //getchar(); 
    }              
    #endif   
         
	//Enable counter-factual policy after TRANSITION_PHASE_POLICY
	if(DAY >= TRANSITION_PHASE_POLICY)
	{
		// Set these to propagate from global constants to agent-specific variables
		REGULATORY_CAPITAL_BUFFER 	= CONST_COUNTERFACTUAL_CAPITAL_BUFFER;
		REGULATORY_LTV_CAP 			= CONST_COUNTERFACTUAL_LTV_CAP;
		REGULATORY_DSTI_CAP 		= CONST_COUNTERFACTUAL_DSTI_CAP;
	
		add_ecb_policy_announcement_message(REGULATORY_CAPITAL_BUFFER, REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP);
	}

    return 0;
}


/** \fn Central_Bank_read_macro_indicators()
 * \brief 
 * \brief currentState:	CB_Start_Macropru
 * \brief nextState:	CB_Macropru_01
 * Status: OK PASS 11.10.17
 */
int Central_Bank_read_macro_indicators()
{
	//Add macro indicators to adt
	macro_indicators_adt macro_indicators;

	START_EUROSTAT_TO_ECB_MACRO_INDICATORS_MESSAGE_LOOP
		copy_macro_indicators_adt(&(eurostat_to_ecb_macro_indicators_message->macro_indicators), &macro_indicators);
	FINISH_EUROSTAT_TO_ECB_MACRO_INDICATORS_MESSAGE_LOOP

	// Set INPUT signals for the policy response function x-axis
	CREDIT_TO_GDP_RATIO_INDICATOR.signal 				 = macro_indicators.credit_to_gdp_ratio; // On [0,1] scale // what GDP flow window? On [0,1] or [0,100]?
	CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal 			 = macro_indicators.credit_minus_gdp_growth/100.0;  // On [0,100] scale // growth rates over what window (MoM, QoQ, YoY)? GDP flows over which window (trailing 3-month)? On [0,1] or [0,100]? 
	CREDIT_GROWTH_INDICATOR.signal 						 = macro_indicators.credit_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?

    //This catches nan values
	if (CREDIT_GROWTH_INDICATOR.signal == CREDIT_GROWTH_INDICATOR.signal)
	{
		fprintf(stderr, "\n %s/%s(), line %d", __FILE__, __FUNCTION__, __LINE__);
		fprintf(stderr, "\nIT %d CREDIT_GROWTH_INDICATOR.signal %f macro_indicators.credit_growth %f\n", DAY, CREDIT_GROWTH_INDICATOR.signal, macro_indicators.credit_growth);
	}

	MORTGAGE_CREDIT_GROWTH_INDICATOR.signal 			 = macro_indicators.mortgage_credit_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?
	HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal		 = macro_indicators.housing_market_price_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?
	HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal = macro_indicators.house_price_minus_rent_price_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?
	MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal 	 = macro_indicators.mortgage_interest_rate_spread_policy_rate; // On [0,1] scale // On [0,1] or [0,100]?

	INVESTMENT_GDP_RATIO_INDICATOR.signal 				 = macro_indicators.investment_gdp_ratio; // On [0,1] scale // not needed
	FIRM_CREDIT_GROWTH_INDICATOR.signal 				 = macro_indicators.firm_credit_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?
	NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal = macro_indicators.new_mortgage_to_housing_value_ratio; // On [0,1] scale // given the new quality aspects in the model this may not make sense anymore
	NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.signal  = macro_indicators.new_firm_loans_to_investment_ratio; // On [0,1] scale // investment flows over what window? On [0,1] or [0,100]?
	RENTAL_MARKET_PRICE_GROWTH_INDICATOR.signal 		 = macro_indicators.rental_market_price_growth/100.0;  // On [0,100] scale // growth rates over what window? On [0,1] or [0,100]?

	return 0;
}    


/** \fn Central_Bank_activate_policy_response()
 * \brief 
 * \brief currentState:	
 * \brief nextState:	CB_End_Macropru
 * \brief Unittest status: NOT TESTED
 */
int Central_Bank_activate_policy_responses()
{
	double y_floor, y_ceiling;

	//Set limits on INPUTS: Indicators (x-axis: I-,I+)
	CREDIT_TO_GDP_RATIO_INDICATOR.x_floor					= CONST_MACROPRU_INDICATOR_LBND_1;
	CREDIT_TO_GDP_RATIO_INDICATOR.x_ceiling					= CONST_MACROPRU_INDICATOR_UBND_1;

	CREDIT_MINUS_GDP_GROWTH_INDICATOR.x_floor				= CONST_MACROPRU_INDICATOR_LBND_2;
	CREDIT_MINUS_GDP_GROWTH_INDICATOR.x_ceiling				= CONST_MACROPRU_INDICATOR_UBND_2;

	CREDIT_GROWTH_INDICATOR.x_floor							= CONST_MACROPRU_INDICATOR_LBND_3;
	CREDIT_GROWTH_INDICATOR.x_ceiling						= CONST_MACROPRU_INDICATOR_UBND_3;

	MORTGAGE_CREDIT_GROWTH_INDICATOR.x_floor				= CONST_MACROPRU_INDICATOR_LBND_4;
	MORTGAGE_CREDIT_GROWTH_INDICATOR.x_ceiling				= CONST_MACROPRU_INDICATOR_UBND_4;

	HOUSING_MARKET_PRICE_GROWTH_INDICATOR.x_floor			= CONST_MACROPRU_INDICATOR_LBND_5;
	HOUSING_MARKET_PRICE_GROWTH_INDICATOR.x_ceiling			= CONST_MACROPRU_INDICATOR_UBND_5;

	HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.x_floor	= CONST_MACROPRU_INDICATOR_LBND_6;
	HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.x_ceiling = CONST_MACROPRU_INDICATOR_UBND_6;

	MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.x_floor		= CONST_MACROPRU_INDICATOR_LBND_7;
	MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.x_ceiling 	= CONST_MACROPRU_INDICATOR_UBND_7;

	/* ADDITIONAL INDICATORS */
	INVESTMENT_GDP_RATIO_INDICATOR.x_floor					= CONST_MACROPRU_INDICATOR_LBND_8;
	INVESTMENT_GDP_RATIO_INDICATOR.x_ceiling				= CONST_MACROPRU_INDICATOR_UBND_8;

	FIRM_CREDIT_GROWTH_INDICATOR.x_floor					= CONST_MACROPRU_INDICATOR_LBND_9;
	FIRM_CREDIT_GROWTH_INDICATOR.x_ceiling					= CONST_MACROPRU_INDICATOR_UBND_9;

	NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.x_floor	= CONST_MACROPRU_INDICATOR_LBND_10;
	NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.x_ceiling = CONST_MACROPRU_INDICATOR_UBND_10;

	NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.x_floor	= CONST_MACROPRU_INDICATOR_LBND_11;
	NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.x_ceiling  = CONST_MACROPRU_INDICATOR_UBND_11;

	RENTAL_MARKET_PRICE_GROWTH_INDICATOR.x_floor			= CONST_MACROPRU_INDICATOR_LBND_12;
	RENTAL_MARKET_PRICE_GROWTH_INDICATOR.x_ceiling			= CONST_MACROPRU_INDICATOR_UBND_12;

	// Set flags for positively or negatively sloped policy response function
	switch(SWITCH_MACROPRU_POLICY_CASE)
	{
		//CAPITAL BUFFER
		case 1: 
				CREDIT_TO_GDP_RATIO_INDICATOR.positively_sloped=1;
				CREDIT_MINUS_GDP_GROWTH_INDICATOR.positively_sloped=1;
				CREDIT_GROWTH_INDICATOR.positively_sloped=1;

				MORTGAGE_CREDIT_GROWTH_INDICATOR.positively_sloped=1;
				HOUSING_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=1;
				HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.positively_sloped=1;
				MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.positively_sloped=0; //exception

				INVESTMENT_GDP_RATIO_INDICATOR.positively_sloped=1;
				FIRM_CREDIT_GROWTH_INDICATOR.positively_sloped=1;
				NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.positively_sloped=1;
				NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.positively_sloped=1;
				RENTAL_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=1;

				//Set bounds on policy response function
				y_floor = CONST_MACROPRU_RESPONSE_LBND_1;
				y_ceiling = CONST_MACROPRU_RESPONSE_UBND_1;
		break;

		//LTV CAP
		case 2: 
				CREDIT_TO_GDP_RATIO_INDICATOR.positively_sloped=0;
				CREDIT_MINUS_GDP_GROWTH_INDICATOR.positively_sloped=0;
				CREDIT_GROWTH_INDICATOR.positively_sloped=0;

				MORTGAGE_CREDIT_GROWTH_INDICATOR.positively_sloped=0;
				HOUSING_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=0;
				HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.positively_sloped=0;
				MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.positively_sloped=1; //exception

				INVESTMENT_GDP_RATIO_INDICATOR.positively_sloped=0;
				FIRM_CREDIT_GROWTH_INDICATOR.positively_sloped=0;
				NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.positively_sloped=0;
				NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.positively_sloped=0;
				RENTAL_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=0;
				
				//Set bounds on policy response function
				y_floor = CONST_MACROPRU_RESPONSE_LBND_2;
				y_ceiling = CONST_MACROPRU_RESPONSE_UBND_2;
		break;

		//DSTI CAP
		case 3: 
				CREDIT_TO_GDP_RATIO_INDICATOR.positively_sloped=0;
				CREDIT_MINUS_GDP_GROWTH_INDICATOR.positively_sloped=0;
				CREDIT_GROWTH_INDICATOR.positively_sloped=0;

				MORTGAGE_CREDIT_GROWTH_INDICATOR.positively_sloped=0;
				HOUSING_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=0;
				HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.positively_sloped=0;				
				MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.positively_sloped=1; //exception

				INVESTMENT_GDP_RATIO_INDICATOR.positively_sloped=0;
				FIRM_CREDIT_GROWTH_INDICATOR.positively_sloped=0;
				NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.positively_sloped=0;
				NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR.positively_sloped=0;
				RENTAL_MARKET_PRICE_GROWTH_INDICATOR.positively_sloped=0;
				
				//Set bounds on policy response function
				y_floor = CONST_MACROPRU_RESPONSE_LBND_3;
				y_ceiling = CONST_MACROPRU_RESPONSE_UBND_3;
		break;

		default: break;
	}


	double response;

	// Compute policy response for the correct indicator
	// If CONST_SWITCH_MACROPRU_POLICY=0: policy effect is non-active
	// If CONST_SWITCH_MACROPRU_POLICY=1: policy effect is active
	switch(SWITCH_MACROPRU_POLICY_INDICATOR)
	{
		case 1: response = policy_response(&CREDIT_TO_GDP_RATIO_INDICATOR, y_floor, y_ceiling);
		break;

		case 2: response = policy_response(&CREDIT_MINUS_GDP_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 3: response = policy_response(&CREDIT_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 4: response = policy_response(&MORTGAGE_CREDIT_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 5: response = policy_response(&HOUSING_MARKET_PRICE_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 6: response = policy_response(&HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 7: response = policy_response(&MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR, y_floor, y_ceiling);
		break;

		/* ADDITIONAL INDICATORS */
		case 8: response = policy_response(&INVESTMENT_GDP_RATIO_INDICATOR, y_floor, y_ceiling);
		break;

		case 9: response = policy_response(&FIRM_CREDIT_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		case 10: response = policy_response(&NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR, y_floor, y_ceiling);
		break;

		case 11: response = policy_response(&NEW_FIRM_LOANS_TO_INVESTMENT_RATIO_INDICATOR, y_floor, y_ceiling);
		break;

		case 12: response = policy_response(&RENTAL_MARKET_PRICE_GROWTH_INDICATOR, y_floor, y_ceiling);
		break;

		default: response = 0.0;
	}

	// Set global constant to propagate policy stance
	// If CONST_SWITCH_MACROPRU_POLICY=0: policy effect is non-active
	// If CONST_SWITCH_MACROPRU_POLICY=1: policy effect is active
	switch(SWITCH_MACROPRU_POLICY_CASE)
	{
		//CAPITAL BUFFER CCyB
		//response in pct terms:
		case 1: COUNTER_CYCLICAL_CAPITAL_BUFFER = response;
				REGULATORY_CAPITAL_BUFFER 	= CONST_REGULATORY_CAPITAL_BUFFER + COUNTER_CYCLICAL_CAPITAL_BUFFER + CAPITAL_CONSERVATION_BUFFER;
		break;

		//LTV CAP
		case 2: REGULATORY_LTV_CAP = response;
		break;

		//DSTI CAP
		case 3: REGULATORY_DSTI_CAP = response;
		break;

		default: 
					REGULATORY_CAPITAL_BUFFER 	= CONST_REGULATORY_CAPITAL_BUFFER + COUNTER_CYCLICAL_CAPITAL_BUFFER + CAPITAL_CONSERVATION_BUFFER;
					REGULATORY_LTV_CAP 			= CONST_REGULATORY_LTV_CAP;
					REGULATORY_DSTI_CAP 		= CONST_REGULATORY_DSTI_CAP;
	}

    #ifdef _DEBUG_MODE    
    if (PRINT_MACROPRU_INFO)
    {
        printf("\n\nIT %d ID %d Central_Bank_activate_policy_responses", DAY, ID);
		printf("\n\tSWITCH_MACROPRU_POLICY: %d\n\tSWITCH_MACROPRU_POLICY_CASE: %d\n\tSWITCH_MACROPRU_POLICY_INDICATOR: %d", SWITCH_MACROPRU_POLICY, SWITCH_MACROPRU_POLICY_CASE, SWITCH_MACROPRU_POLICY_INDICATOR);
        
        printf("\n\n\tREGULATORY_CAPITAL_BUFFER: %f (%.2f %%)", REGULATORY_CAPITAL_BUFFER, 100.0*REGULATORY_CAPITAL_BUFFER);

		printf("\n\n\tCONST_REGULATORY_CAPITAL_BUFFER: %f (%.2f %%)", CONST_REGULATORY_CAPITAL_BUFFER, 100.0*CONST_REGULATORY_CAPITAL_BUFFER);
		printf("\n\n\tCOUNTER_CYCLICAL_CAPITAL_BUFFER: %f (%.2f %%)", COUNTER_CYCLICAL_CAPITAL_BUFFER, 100.0*COUNTER_CYCLICAL_CAPITAL_BUFFER);
		printf("\n\n\tCAPITAL_CONSERVATION_BUFFER: %f (%.2f %%)", CAPITAL_CONSERVATION_BUFFER, 100.0*CAPITAL_CONSERVATION_BUFFER);

        printf("\n\tREGULATORY_LTV_CAP:\t%f (%.2f %%)", REGULATORY_LTV_CAP, 100.0*REGULATORY_LTV_CAP);

        printf("\n\tREGULATORY_DSTI_CAP:\t%f (%.2f %%)", REGULATORY_DSTI_CAP, 100*REGULATORY_DSTI_CAP);
        
        //printf("\n\t(**: Policy response is in the interior of the policy response function)");

		switch(SWITCH_MACROPRU_POLICY_INDICATOR)
		{
			case 1: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_1 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_1 %.2f", CONST_MACROPRU_INDICATOR_LBND_1, CONST_MACROPRU_INDICATOR_UBND_1);
					if (CONST_MACROPRU_INDICATOR_LBND_1 < CREDIT_TO_GDP_RATIO_INDICATOR.signal && CREDIT_TO_GDP_RATIO_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_1)
						printf("\tSignal (active): %f", CREDIT_TO_GDP_RATIO_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", CREDIT_TO_GDP_RATIO_INDICATOR.signal);
			break;

			case 2: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_2 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_2 %.2f", CONST_MACROPRU_INDICATOR_LBND_2, CONST_MACROPRU_INDICATOR_UBND_2);
					if (CONST_MACROPRU_INDICATOR_LBND_2 < CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal && CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_2)
						printf("\tSignal (active): %f", CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", CREDIT_MINUS_GDP_GROWTH_INDICATOR.signal);
			break;

			case 3:	printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_3 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_3 %.2f", CONST_MACROPRU_INDICATOR_LBND_3, CONST_MACROPRU_INDICATOR_UBND_3);
					if (CONST_MACROPRU_INDICATOR_LBND_3 < CREDIT_GROWTH_INDICATOR.signal && CREDIT_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_3)
						printf("\tSignal (active): %f", CREDIT_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", CREDIT_GROWTH_INDICATOR.signal);
			break;

			case 4:	printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_4 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_4 %.2f", CONST_MACROPRU_INDICATOR_LBND_4, CONST_MACROPRU_INDICATOR_UBND_4);
					if (CONST_MACROPRU_INDICATOR_LBND_4 < MORTGAGE_CREDIT_GROWTH_INDICATOR.signal && MORTGAGE_CREDIT_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_4)
						printf("\tSignal (active): %f", MORTGAGE_CREDIT_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", MORTGAGE_CREDIT_GROWTH_INDICATOR.signal);
			break;

			case 5:	printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_5 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_5 %.2f", CONST_MACROPRU_INDICATOR_LBND_5, CONST_MACROPRU_INDICATOR_UBND_5);
					if (CONST_MACROPRU_INDICATOR_LBND_5 < HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal && HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_5)
						printf("\tSignal (active): %f", HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", HOUSING_MARKET_PRICE_GROWTH_INDICATOR.signal);
			break;

			case 6:	printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_6 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_6 %.2f", CONST_MACROPRU_INDICATOR_LBND_6, CONST_MACROPRU_INDICATOR_UBND_6);
					if (CONST_MACROPRU_INDICATOR_LBND_6 < HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal && HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_6)
						printf("\tSignal (active): %f", HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH_INDICATOR.signal);
			break;

			case 7: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_7 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_7 %.2f", CONST_MACROPRU_INDICATOR_LBND_7, CONST_MACROPRU_INDICATOR_UBND_7);
					if (CONST_MACROPRU_INDICATOR_LBND_7 < INVESTMENT_GDP_RATIO_INDICATOR.signal && INVESTMENT_GDP_RATIO_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_7)
						printf("\tSignal (active): %f", INVESTMENT_GDP_RATIO_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", INVESTMENT_GDP_RATIO_INDICATOR.signal);
			break;

			case 8: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_8 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_8 %.2f", CONST_MACROPRU_INDICATOR_LBND_8, CONST_MACROPRU_INDICATOR_UBND_8);
					if (CONST_MACROPRU_INDICATOR_LBND_8 < MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal && MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_8)
						printf("\tSignal (active): %f", MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", MORTGAGE_INTEREST_SPREAD_POLICY_INDICATOR.signal);
			break;

			case 9: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_9 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_9 %.2f", CONST_MACROPRU_INDICATOR_LBND_9, CONST_MACROPRU_INDICATOR_UBND_9);
					if (CONST_MACROPRU_INDICATOR_LBND_9 < FIRM_CREDIT_GROWTH_INDICATOR.signal && FIRM_CREDIT_GROWTH_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_9)
						printf("\tSignal (active): %f", FIRM_CREDIT_GROWTH_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", FIRM_CREDIT_GROWTH_INDICATOR.signal);
			break;

			case 10: printf("\n\n\tCONST_MACROPRU_INDICATOR_LBND_10 %.2f\n\tCONST_MACROPRU_INDICATOR_UBND_10 %.2f", CONST_MACROPRU_INDICATOR_LBND_10, CONST_MACROPRU_INDICATOR_UBND_10);
					if (CONST_MACROPRU_INDICATOR_LBND_10 < NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal && NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal < CONST_MACROPRU_INDICATOR_UBND_10)
						printf("\tSignal (active): %f", NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal);
					else
						printf("\tSignal (out of bounds): %f", NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO_INDICATOR.signal);
			break;

			default: printf("\n\tNo indicator selected (SWITCH_MACROPRU_POLICY_INDICATOR %d)", SWITCH_MACROPRU_POLICY_INDICATOR);
		}


		switch(SWITCH_MACROPRU_POLICY_CASE)
		{
			//CAPITAL BUFFER
			case 1: printf("\n\tCONST_MACROPRU_RESPONSE_LBND_1 %f\n\tCONST_MACROPRU_RESPONSE_UBND_1 %f", CONST_MACROPRU_RESPONSE_LBND_1, CONST_MACROPRU_RESPONSE_UBND_1);
			break;

			//LTV CAP
			case 2: printf("\n\tCONST_MACROPRU_RESPONSE_LBND_2 %f\n\tCONST_MACROPRU_RESPONSE_UBND_2 %f", CONST_MACROPRU_RESPONSE_LBND_2, CONST_MACROPRU_RESPONSE_UBND_2);
			break;

			//DSTI CAP
			case 3: printf("\n\tCONST_MACROPRU_RESPONSE_LBND_3 %f\n\tCONST_MACROPRU_RESPONSE_UBND_3 %f", CONST_MACROPRU_RESPONSE_LBND_3, CONST_MACROPRU_RESPONSE_UBND_3);
			break;

			default: printf("\n\tNo policy response selected (SWITCH_MACROPRU_POLICY_CASE %d)", SWITCH_MACROPRU_POLICY_CASE);
		}
		printf("\tResponse: %f\n", response);
    }
    #endif

	return 0;
}    

/** \fn Central_Bank_macroprudential_policy()
 * \brief Function to communicate the policy stance of the CB to all Banks
 * \brief After function:  Central_Bank_monetary_policy
 * \brief Before function: CB_dummy on Fin. Market
 * \brief Frequency: Day 1 of Month
 * \brief currentState:	CB_policy_01
 * \brief nextState:	CB_open_market_operations
 * \brief Unittest status: NOT TESTED
 */
int Central_Bank_macroprudential_policy()
{
	//Condition XML: a.switch_macropru_policy==1
	if(SWITCH_MACROPRU_POLICY && DAY >= CONST_TRANSITION_PHASE_POLICY)
		add_ecb_macroprudential_policy_announcement_message(REGULATORY_CAPITAL_BUFFER, REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP);


    #ifdef _DEBUG_MODE    
    if (PRINT_MACROPRU_INFO)
    {
        printf("\nIT %d ID %d CB SENDS MACROPRUDENTIAL POLICY ANNOUNCEMENT:", DAY, ID);
        printf("\n\tREGULATORY_CAPITAL_BUFFER: %f (%f %%)\n\tREGULATORY_LTV_CAP: %f (%f %%)\n\tREGULATORY_DSTI_CAP: %f (%f %%)", REGULATORY_CAPITAL_BUFFER, 100.0*REGULATORY_CAPITAL_BUFFER, REGULATORY_LTV_CAP, 100.0*REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP, 100*REGULATORY_DSTI_CAP);
    }
    #endif

	return 0;
}    


