#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"
#include "../ECB_Model/Mortgage_Market/Household_aux_headers.h"
#include "../Sanity_Checks/sanity_checks_aux_headers.h"


/********************************* Household agent functions *************************************/



/**********************************Household Role: Consumption Goods Market*********************/


/*\fn Household_determine_consumption_budget()
 * brief: If a household is unemployed then it receives an unemployment benefit payment
 * 
 */
int Household_determine_consumption_budget()
{
    int i;
    double asset_wealth;
    double principal_hmr=0.0, principal_op=0.0, interest_hmr=0.0, interest_op=0.0;

    /* Determining the consumption budget of the month */
    
    /* Rule 0: 
     * Previous rule based on Deaton: uses PAYMENT_ACCOUNT only
     */
/*
            if(PAYMENT_ACCOUNT > (INITIAL_CONSUMPTION_PROPENSITY*MEAN_INCOME))
            {
                
                CONSUMPTION_BUDGET=CONSUMPTION_PROPENSITY*PAYMENT_ACCOUNT+(1<<-CONSUMPTION_PROPENSITY)
                *INITIAL_CONSUMPTION_PROPENSITY*MEAN_INCOME;
            }
            else
            {
                CONSUMPTION_BUDGET = PAYMENT_ACCOUNT;
            }
*/

        
              
    /* Rules based on Carroll-Rule: Determination of the consumption budget based on buffers
     * 
     * Note: CONSUMPTION_BUDGET > PAYMENT_ACCOUNT means household wants to sell assets to consume.
     * A check should be performed after transactions if the consumption budget can be financed.
     */

    // Rule 1:
    //CONSUMPTION_BUDGET = MEAN_NET_INCOME + CARROL_CONSUMPTION_PARAMETER*(WEALTH - WEALTH_INCOME_RATIO_TARGET*MEAN_NET_INCOME);

	// Rule 2: Simplest form using a target savings rate
	//CONSUMPTION_BUDGET = (1-TARGET_SAVINGS_RATE)*MEAN_NET_INCOME;

    // Rule 3: The following rule is the same, but split into liquid and non-liquid asset wealth:
    /* 10.9.2010, Sander van der Hoog
     * 29.11.2016: Updated to prevent negative CONSUMPTION_BUDGET, or CONSUMPTION_BUDGET > PAYMENT_ACCOUNT
     * Liquid wealth: mean income plus a percentage of payment account (excl. mean income) -> consumption budget grows along with the payment account
     * Non-liquid asset wealth: if trading is switched on, add a percentage of the wealth buffer (asset wealth - target wealth) to the consumption budget -> (dis)investment in assets
     */

	asset_wealth = ASSETSOWNED.units*ASSETSOWNED.lastprice;
	
	/* SvdH, 29.11.2016
	 * Adjust liquid wealth buffer term to: PAYMENT_ACCOUNT - MEAN_NET_INCOME
	 * Restrict non-liquid wealth buffer term to: max(0, a-w) to prevent negative contribution from wealth buffer to CONSUMPTION_BUDGET
	 * If TRADING_ACTIVITY==0: no effect from wealth buffer
	 */

	//Original consumption budget in Eurace@Unibi:
	//CONSUMPTION_BUDGET = MEAN_NET_INCOME + CARROL_CONSUMPTION_PARAMETER * (PAYMENT_ACCOUNT - MEAN_NET_INCOME) +
	//						TRADING_ACTIVITY * CARROL_CONSUMPTION_PARAMETER * max(0.0, asset_wealth - WEALTH_INCOME_RATIO_TARGET*MEAN_NET_INCOME);
							
	if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
	{
        if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.is_variable_rate)
            recompute_variable_rate_mortgage_contract(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage, VARIABLE_RATE_LOANS_BASE_RATE);

        recompute_mortgage_attributes_including_arrears(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage);

		principal_hmr = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal;
		interest_hmr  = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest;

        HMR_DEBT_SERVICE_TO_INCOME_RATIO = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment / MEAN_NET_INCOME;
        HMR_DEBT_SERVICE_BUFFER_IN_MONTHS = PAYMENT_ACCOUNT / (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment+MINIMUM_SUBSISTENCE_LEVEL);
    }
    else
        HMR_DEBT_SERVICE_TO_INCOME_RATIO = 0.0;

    principal_op = 0.0;
    interest_op = 0.0;
    for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
    {
        if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
        {
            if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.is_variable_rate)
                recompute_variable_rate_mortgage_contract(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage, VARIABLE_RATE_LOANS_BASE_RATE);

            recompute_mortgage_attributes_including_arrears(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage);
            principal_op += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal;
            interest_op  += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest;
        }
    }
    
	if(MEAN_NET_INCOME>1e-10)
		TOTAL_DSTI = (principal_hmr + principal_op + interest_hmr + interest_op)/MEAN_NET_INCOME;
	else
		TOTAL_DSTI = 999.0;

    HOUSING_BUDGET = principal_hmr + interest_hmr;
    OP_DEBT_SERVICE_BUDGET = principal_op + interest_op;

	if(IS_TENANT && RENTAL_UNIT.owner_id > 0)
    {
        // Calculate CPI adjusted rent
        if (RENTAL_UNIT.cpi_base > 0.0)
            MONTHLY_RENT = RENTAL_UNIT.monthly_rent * PRICE_INDEX / RENTAL_UNIT.cpi_base;
        else
            MONTHLY_RENT = RENTAL_UNIT.monthly_rent;
		HOUSING_BUDGET += MONTHLY_RENT;

        RENT_TO_INCOME_RATIO = MONTHLY_RENT / MEAN_NET_INCOME;
        RENT_BUFFER_IN_MONTHS = PAYMENT_ACCOUNT / (MONTHLY_RENT+MINIMUM_SUBSISTENCE_LEVEL);
    }
    else
    {
        RENT_TO_INCOME_RATIO = 0.0;
    }

    /*Compute the wealth income ratio*/        
    WEALTH_INCOME_RATIO_ACTUAL = WEALTH/ (MEAN_NET_INCOME-HOUSING_BUDGET);
    PAYMENT_ACCOUNT_TO_INCOME_RATIO = PAYMENT_ACCOUNT / MEAN_NET_INCOME;

    /* *** PROPOSAL DIRK *** */
	double disposable_income = max(0.0, MEAN_NET_INCOME - HOUSING_BUDGET);
	double disposable_payment_account = max(0.0, PAYMENT_ACCOUNT - OP_DEBT_SERVICE_BUDGET);
	double desired_consumption = disposable_income + CARROL_CONSUMPTION_PARAMETER * (disposable_payment_account - LIQUID_WEALTH_INCOME_RATIO_TARGET*disposable_income) +
							TRADING_ACTIVITY * CARROL_CONSUMPTION_PARAMETER * max(0.0, asset_wealth - WEALTH_INCOME_RATIO_TARGET*disposable_income);

	if (desired_consumption < (1-CONSUMPTION_MAX_DOWNWARD_ADJUSTMENT)*CONSUMPTION_BUDGET_IN_MONTH)
		desired_consumption = (1-CONSUMPTION_MAX_DOWNWARD_ADJUSTMENT)*CONSUMPTION_BUDGET_IN_MONTH;

	double remaining_liquidity = PAYMENT_ACCOUNT;
    assert__(remaining_liquidity > -EPSILON)
    {
        fprintf(stderr, "\n\t In %s:%s(), line %d: remaining_liquidity %f", __FILE__, __FUNCTION__, __LINE__, remaining_liquidity);
    }
    
	// 1. Secure consumption for minimum subsistence level
	if (remaining_liquidity > MINIMUM_SUBSISTENCE_LEVEL)
		CONSUMPTION_BUDGET = MINIMUM_SUBSISTENCE_LEVEL;
	else
		CONSUMPTION_BUDGET = remaining_liquidity;
	remaining_liquidity -= CONSUMPTION_BUDGET;
    
    assert__(remaining_liquidity > -EPSILON)
    {
        fprintf(stderr, "\n\t In %s:%s(), line %d: remaining_liquidity %f", __FILE__, __FUNCTION__, __LINE__, remaining_liquidity);
    }
    
	// 2. Secure payment for HMR or rent
	if (remaining_liquidity < HOUSING_BUDGET)
		HOUSING_BUDGET = 0.0;
	remaining_liquidity -= HOUSING_BUDGET;
    
    assert__(remaining_liquidity > -EPSILON)
    {
        fprintf(stderr, "\n\t In %s:%s(), line %d: remaining_liquidity %f", __FILE__, __FUNCTION__, __LINE__, remaining_liquidity);
    }
    
	// 3. Secure payments for OPs
	if (remaining_liquidity < OP_DEBT_SERVICE_BUDGET)
		OP_DEBT_SERVICE_BUDGET = remaining_liquidity;
	remaining_liquidity -= OP_DEBT_SERVICE_BUDGET;
    
    assert__(remaining_liquidity > -EPSILON)
    {
        fprintf(stderr, "\n\t In %s:%s(), line %d: remaining_liquidity %f", __FILE__, __FUNCTION__, __LINE__, remaining_liquidity);
    }
    
	// 4. Budget for additional consumption
	double additional_consumption = max(0.0, desired_consumption - MINIMUM_SUBSISTENCE_LEVEL);
	if (remaining_liquidity < additional_consumption)
		additional_consumption = remaining_liquidity;
	CONSUMPTION_BUDGET += additional_consumption;
	remaining_liquidity -= additional_consumption;
    
    if (remaining_liquidity < -EPSILON)
        fprintf(stderr, "\n\n Failed assert(remaining_liquidity>-EPSILON): remaining_liquidity %f", remaining_liquidity);
    
    assert__(remaining_liquidity > -EPSILON)
    {
        fprintf(stderr, "\n\t In %s:%s(), line %d: remaining_liquidity %f", __FILE__, __FUNCTION__, __LINE__, remaining_liquidity);
    }

    if (SWITCH_IC_MECHANISM)
    {
        double l=IC_MECHANISM_L;
        double u=IC_MECHANISM_U;

        double x = max(0.0,min(1.0, (DEPOSIT_INTEREST_RATE - u)/(l-u) ));

        remaining_liquidity += IC_MECHANISM_ALPHA*(1-x)*CONSUMPTION_BUDGET;

        CONSUMPTION_BUDGET = IC_MECHANISM_ALPHA*x*CONSUMPTION_BUDGET+(1-IC_MECHANISM_ALPHA)*CONSUMPTION_BUDGET;

        if (CONSUMPTION_BUDGET < MINIMUM_SUBSISTENCE_LEVEL)
        	CONSUMPTION_BUDGET = min(MINIMUM_SUBSISTENCE_LEVEL, remaining_liquidity);
    }

    #ifdef _DEBUG_MODE 
    {
		if (PRINT_DEBUG_CONSUMPTION)
			if(CONSUMPTION_BUDGET<0.0)
			{
				fprintf(stderr, "\n\n IT %d ID %d Household_determine_consumption_budget:", DAY, ID); 
				
				fprintf(stderr, "\n\t PAYMENT_ACCOUNT=%f CONSUMPTION_BUDGET=%f", PAYMENT_ACCOUNT, CONSUMPTION_BUDGET);
				
				fprintf(stderr, "\n\t TRADING_ACTIVITY=%d, CARROL_CONSUMPTION_PARAMETER=%f WEALTH_INCOME_RATIO_TARGET=%f", TRADING_ACTIVITY, CARROL_CONSUMPTION_PARAMETER, WEALTH_INCOME_RATIO_TARGET);
				
				fprintf(stderr, "\n\t CONSUMPTION_BUDGET = MEAN_NET_INCOME + CARROL_CONSUMPTION_PARAMETER * PAYMENT_ACCOUNT +	TRADING_ACTIVITY * CARROL_CONSUMPTION_PARAMETER * max(0.0, asset_wealth - WEALTH_INCOME_RATIO_TARGET*MEAN_NET_INCOME)");
				
				fprintf(stderr, "\n\t MEAN_NET_INCOME=%f CARROL_CONSUMPTION_PARAMETER*PAYMENT_ACCOUNT=%f TRADING_ACTIVITY*CARROL_CONSUMPTION_PARAMETER=%f",
										MEAN_NET_INCOME, CARROL_CONSUMPTION_PARAMETER*PAYMENT_ACCOUNT, TRADING_ACTIVITY*CARROL_CONSUMPTION_PARAMETER);
				
				fprintf(stderr, "\n\t asset_wealth=%f WEALTH_INCOME_RATIO_TARGET*MEAN_NET_INCOME=%f", ASSETSOWNED.units*ASSETSOWNED.lastprice, WEALTH_INCOME_RATIO_TARGET*MEAN_NET_INCOME);
			}
    }
    #endif


    #ifdef _DEBUG_MODE 
    if (PRINT_DEBUG_CONSUMPTION)
    {
	    //Prints to std err:
    	if(PAYMENT_ACCOUNT < 0)
    	{
    		fprintf(stderr, "\n\n IT %d Household_determine_consumption_budget: PAYMENT ACCOUNT<0", DAY); 
    		fprintf(stderr, "\n\t ID %d PAYMENT_ACCOUNT %f CONSUMPTION_BUDGET %f\n", ID, PAYMENT_ACCOUNT, CONSUMPTION_BUDGET);
    	}

    	if(CONSUMPTION_BUDGET < 0)
    	{
    		fprintf(stderr, "\n\n IT %d Household_determine_consumption_budget: CONSUMPTION_BUDGET < 0", DAY);
    		fprintf(stderr, "\n\t ID %d PAYMENT_ACCOUNT %f CONSUMPTION_BUDGET %f", ID, PAYMENT_ACCOUNT, CONSUMPTION_BUDGET);
    	}	    
    	if (PAYMENT_ACCOUNT < CONSUMPTION_BUDGET)
    	{
    		fprintf(stderr, "\n\n IT %d Household_determine_consumption_budget: PAYMENT_ACCOUNT < CONSUMPTION_BUDGET (household wants to sell shares).", DAY);
            if (TRADING_ACTIVITY==0) fprintf(stderr, "but TRADING_ACTIVITY==0.");
    	}
    }
    #endif            

	//SvdH, 29.11.2016
	// Restrict CONSUMPTION_BUDGET to PAYMENT_ACCOUNT: since FM is open only 1st day of calendar month (such that all households can trade simultaneously)
	// no selling of assets is possible after CONSUMPTION_BUDGET is set, so do not consume more than your liquid assets:
    if (CONSUMPTION_BUDGET > PAYMENT_ACCOUNT)
    {
	    #ifdef _DEBUG_MODE 
    	if (PRINT_DEBUG_CONSUMPTION)
    	{
    		fprintf(stderr, "\n\t IT %d ID %d Household_determine_consumption_budget: Set CONSUMPTION_BUDGET (since CONSUMPTION_BUDGET > PAYMENT_ACCOUNT)", DAY, ID);
    		fprintf(stderr, "\n\t PAYMENT_ACCOUNT=%f, asset_wealth=%f CONSUMPTION_BUDGET=%f, HOUSING_BUDGET=%f", PAYMENT_ACCOUNT, asset_wealth, CONSUMPTION_BUDGET, HOUSING_BUDGET);
    		fprintf(stderr, "\n\t Disposable income: MEAN_NET_INCOME - HOUSING_BUDGET=%f, ", MEAN_NET_INCOME - HOUSING_BUDGET );
    		fprintf(stderr, "\n\t Actual disposable income: PAYMENT_ACCOUNT - HOUSING_BUDGET=%f, ", PAYMENT_ACCOUNT - HOUSING_BUDGET );
    		fprintf(stderr, "\n\t Liquid wealth delta: PAYMENT_ACCOUNT - (MEAN_NET_INCOME - HOUSING_BUDGET)=%f, ", PAYMENT_ACCOUNT - (MEAN_NET_INCOME - HOUSING_BUDGET) );
    		fprintf(stderr, "\n\t Illiquid wealth delta: asset_wealth - WEALTH_INCOME_RATIO_TARGET*(MEAN_NET_INCOME - HOUSING_BUDGET)=%f, ", asset_wealth - WEALTH_INCOME_RATIO_TARGET*(MEAN_NET_INCOME - HOUSING_BUDGET) );
    		fprintf(stderr, "\n\t CONSUMPTION_BUDGET=%f = MEAN_NET_INCOME + %.3f*Liquid wealth delta + %.3f*Illiquid wealth delta \n", CONSUMPTION_BUDGET, CARROL_CONSUMPTION_PARAMETER, CARROL_CONSUMPTION_PARAMETER);
    	}
        #endif

    	CONSUMPTION_BUDGET = PAYMENT_ACCOUNT;		

        #ifdef _DEBUG_MODE 
        if (PRINT_CONSUMPTION_INFO)
        {
            fprintf(stderr, "\n\t IT %d id %d Set: CONSUMPTION_BUDGET = PAYMENT_ACCOUNT=%f\n", DAY, ID, CONSUMPTION_BUDGET);
        }
        #endif
    }

    CONSUMPTION_BUDGET_IN_MONTH = CONSUMPTION_BUDGET;
    HOUSING_BUDGET_IN_MONTH = HOUSING_BUDGET;
    OP_BUDGET_IN_MONTH = OP_DEBT_SERVICE_BUDGET;
                        
    WEEKLY_BUDGET = CONSUMPTION_BUDGET/4;
    WEEK_OF_MONTH = 4;

    vlog_micro_ts(ID,"mean_net_income",MEAN_NET_INCOME);
    vlog_micro_ts(ID,"debt_service_hmr", principal_hmr+interest_hmr);
    vlog_micro_ts(ID,"payment_account", PAYMENT_ACCOUNT);
    vlog_micro_ts(ID,"consumption_budget", CONSUMPTION_BUDGET_IN_MONTH);
    
    #ifdef _DEBUG_MODE 
    if (PRINT_DEBUG_CONSUMPTION)
    {
        fprintf(stderr, "\n\t CONSUMPTION_BUDGET_IN_MONTH = %f", CONSUMPTION_BUDGET_IN_MONTH);
        fprintf(stderr, "\n\t WEEKLY_BUDGET = %f", WEEKLY_BUDGET);
    }
    #endif

    //Variable was used for consumption delaying  (old code)
    EXCESS_WEEKLY_BUDGET = -1;

    return 0;   
}


/** \fn Household_rank_and_buy_goods_1()
 * \brief Household receives information about the offered range of goods in the malls.
 *  Depending on these infos the household sends its good request       
 */
int Household_rank_and_buy_goods_1()
{
    
    //Logit model parameter:
    double sum_weighted_qual_pric_ratios = 0; 
    int j=0;
    int i=0;
    double logit;

    logit_firm_id_array  logit_firm_id_list;
    init_logit_firm_id_array(&logit_firm_id_list);

    mall_quality_price_info_array mall_quality_price_info_list;
    init_mall_quality_price_info_array(&mall_quality_price_info_list);

 
	
    // Restrict Budget to available funds
    if (WEEKLY_BUDGET > PAYMENT_ACCOUNT)
            WEEKLY_BUDGET = PAYMENT_ACCOUNT;


    // Reset the CURRENT_PRICE_QUALITY_LIST
    // Reset using build-in functions (see memory.c)
    // Note: do not use free_ here, since we want to retain the memory variable for future use!
    reset_consumption_goods_offer_array(&CURRENT_PRICE_QUALITY_LIST);


        /*Household reads quality price info mesasges sent by malls   */
        START_QUALITY_PRICE_INFO_1_MESSAGE_LOOP

    	if( quality_price_info_1_message->available==1)
             {   
        add_mall_quality_price_info(&mall_quality_price_info_list,  quality_price_info_1_message->mall_id, quality_price_info_1_message->firm_id,               quality_price_info_1_message->mall_region_id,                   quality_price_info_1_message->quality,                  quality_price_info_1_message->price, 
        quality_price_info_1_message->available);
  			  }

		//Update the price and quantity list (even those which are not available)
		
			if( quality_price_info_1_message->price>1e-5)
			{
				add_consumption_goods_offer(&CURRENT_PRICE_QUALITY_LIST, quality_price_info_1_message->firm_id, quality_price_info_1_message->quality,  quality_price_info_1_message->price);
			}
					
        FINISH_QUALITY_PRICE_INFO_1_MESSAGE_LOOP

        
        //+++++ Logit Model +++++++++:

        
        /*Sum of weighted exponents of quality price ratios*/
        for(i = 0; i < mall_quality_price_info_list.size;i++) 
        {
            sum_weighted_qual_pric_ratios += (mall_quality_price_info_list.array[i]
            .available) * exp(GAMMA_CONST*(GAMMA_QUALITY*log(mall_quality_price_info_list.array[i].quality) -log(mall_quality_price_info_list.array[i].price)));    
        }

        //Prevent buffer overflow error: denominator logit sum <DBL_MAX
        assert(sum_weighted_qual_pric_ratios<DBL_MAX);

        /* Compute logits and add on temporary logit array   */
        for(i = 0; i < mall_quality_price_info_list.size;i++) 
        {
            logit = (mall_quality_price_info_list.array[i].available) *
			 exp(GAMMA_CONST*(GAMMA_QUALITY*log(mall_quality_price_info_list.array[i].quality) -log(mall_quality_price_info_list.array[i].price))) / 
            sum_weighted_qual_pric_ratios;
            
            logit = logit * 100;
			
            if(logit > 0)
            {
                add_logit_firm_id(&logit_firm_id_list, logit, 
                mall_quality_price_info_list.array[i].firm_id);
            }
        }

        if(sum_weighted_qual_pric_ratios > 0)
        {
            MALL_COMPLETELY_SOLD_OUT = 0;
            int random_number = random_int(0,100);
            j=0;
            int x =0, index_selected_good=j;

            for(j = 0; j < logit_firm_id_list.size;j++)
            {
            
                /*if randum number <= logit then select the corresponding good  */ 
                if(((double)random_number <= logit_firm_id_list.array[j].logit+1e-4) && (x != 1))
                {
                    ORDER_QUANTITY[0].firm_id = logit_firm_id_list.
                    array[j].firm_id;
                
                    x = 1;
                    index_selected_good= j;
                }
                /*else sum logits and go to the next iteration step  */
                else
                {
                    if((j < logit_firm_id_list.size-1) )
                    {   
                        logit_firm_id_list.array[j+1].logit =
                        logit_firm_id_list.array[j+1].logit+
                        logit_firm_id_list.array[j].logit;
                    }
                }
            }

            /*This computes and stores the order quantity of the selected good and saves the price in memory */
            ORDER_QUANTITY[0].quantity = WEEKLY_BUDGET/ mall_quality_price_info_list
            .array[index_selected_good].price;
        
            ORDER_QUANTITY[0].price = mall_quality_price_info_list.
            array[index_selected_good].price;

			ORDER_QUANTITY[0].quality= mall_quality_price_info_list
            .array[index_selected_good].quality;

            /*The consumption request message is sent  */ 
            add_consumption_request_1_message(
            mall_quality_price_info_list.array[index_selected_good].mall_id, ID, REGION_ID,
            ORDER_QUANTITY[0].firm_id, ORDER_QUANTITY[0].quantity);

            assert(ORDER_QUANTITY[0].firm_id>0);
			
			#ifdef _DEBUG_MODE
			if (PRINT_DEBUG_CONSUMPTION)
			{ 
				if(ORDER_QUANTITY[0].quantity<0)
				{
					fprintf(stderr, "\n\n IT %d ID: %d Household_rank_and_buy_goods_1", DAY, ID);
					fprintf(stderr, "\n\t HH adds consumption_request_1_message: ONLY NEGATIVE CONSUMPTION REQUESTS ARE SHOWN!!");
					fprintf(stderr, "\n\t HH ID\t Firm ID\t region ID\t quantity");
					fprintf(stderr, "\n\t %02d \t %02d\t %d\t %f", ID, ORDER_QUANTITY[0].firm_id, REGION_ID, ORDER_QUANTITY[0].quantity);
				}
			}
			#endif
        }
        else
        {
            EXPENDITURES=0;
            MALL_COMPLETELY_SOLD_OUT =1;
            ORDER_QUANTITY[0].quantity=0;
            ORDER_QUANTITY[0].price=0;
            ORDER_QUANTITY[0].firm_id=0;
			ORDER_QUANTITY[0].quality= 0.0;
        }

    free_mall_quality_price_info_array(&mall_quality_price_info_list);
    free_logit_firm_id_array(&logit_firm_id_list);

    return 0;

}

/** \fn Household_read_rationing()
 * \brief The household gets information about the accepted amount of goods and,
 * if the mall is completely sold out then the households set the order and delivery volumes
 * from the second step equal 0          
 */
int Household_receive_goods_read_rationing()
{
	//Set LAST_WEEKLY_BUDGET = WEEKLY_BUDGET
	LAST_WEEKLY_BUDGET = WEEKLY_BUDGET;
	
    if(MALL_COMPLETELY_SOLD_OUT == 0)
    {
        EXPENDITURES = 0;

        /*Household reads messages containing the realized consumption of the first round*/
        START_ACCEPTED_CONSUMPTION_1_MESSAGE_LOOP

                RATIONED = accepted_consumption_1_message->rationed;

                /*Update of Budget  */
                WEEKLY_BUDGET = WEEKLY_BUDGET - accepted_consumption_1_message
                ->offered_consumption_volume * ORDER_QUANTITY[0].price;
                
                EXPENDITURES =accepted_consumption_1_message
                ->offered_consumption_volume * ORDER_QUANTITY[0].price;
                HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure += 	
                accepted_consumption_1_message->offered_consumption_volume * ORDER_QUANTITY[0].price;
                
             
                RECEIVED_QUANTITY[0].quantity = accepted_consumption_1_message
                ->offered_consumption_volume;

                RECEIVED_QUANTITY[0].firm_id = ORDER_QUANTITY[0].firm_id;

        FINISH_ACCEPTED_CONSUMPTION_1_MESSAGE_LOOP
    }
    else if(MALL_COMPLETELY_SOLD_OUT == 1)
    {
        EXPENDITURES= 0;
        RECEIVED_QUANTITY[0].quantity = 0;
        RECEIVED_QUANTITY[0].firm_id = 0;
        ORDER_QUANTITY[1].quantity = 0;
        ORDER_QUANTITY[1].firm_id = 0;
        ORDER_QUANTITY[1].price = 0;
        RECEIVED_QUANTITY[1].quantity = 0;
        RECEIVED_QUANTITY[1].firm_id = 0;
        
    }

    assert(WEEKLY_BUDGET > -1e-4);    
   
    	//Reset on first day of calendar month
    if (DAY%20==1)
        MONTHLY_CONSUMPTION_EXPENDITURE = 0.0;    

    return 0;
}


int Household_set_values_zero()
{
        ORDER_QUANTITY[1].quantity = 0;
        ORDER_QUANTITY[1].firm_id = 0;
        ORDER_QUANTITY[1].price = 0;
        RECEIVED_QUANTITY[1].quantity = 0;
        RECEIVED_QUANTITY[1].firm_id = 0;

    return 0;
}

/*If rationed repeat ranking and request of goods: */
int Household_rank_and_buy_goods_2()    
{
            int j = 0;
            int i = 0;
            double logit;
            double sum_weighted_qual_pric_ratios = 0; 

            //Temporary arrays
            logit_firm_id_array logit_firm_id_list;
            init_logit_firm_id_array(&logit_firm_id_list);

            mall_quality_price_info_array mall_quality_price_info_list;
            init_mall_quality_price_info_array(&mall_quality_price_info_list);

        /*The updated quality price message is read  */
        START_QUALITY_PRICE_INFO_2_MESSAGE_LOOP

        if( quality_price_info_2_message->available==1)
        {
                add_mall_quality_price_info(&mall_quality_price_info_list,quality_price_info_2_message->mall_id, 
                quality_price_info_2_message->firm_id, 
                quality_price_info_2_message->mall_region_id, 
                quality_price_info_2_message->quality, 
                quality_price_info_2_message->price, 
                quality_price_info_2_message->available);
        }          
        FINISH_QUALITY_PRICE_INFO_2_MESSAGE_LOOP



        //+++++ Logit Model +++++++++:

        /*Sum of weighted exponents of quality price ratios   */
        for(i = 0;i < mall_quality_price_info_list.size; i++) 
        {
            sum_weighted_qual_pric_ratios += (mall_quality_price_info_list.array[i].available) *
            exp(GAMMA_CONST*(GAMMA_QUALITY*log(mall_quality_price_info_list.array[i].quality) -log(mall_quality_price_info_list.array[i].price))); 
        }

        //Prevent buffer overflow error: denominator logit sum <DBL_MAX
        assert(sum_weighted_qual_pric_ratios<DBL_MAX);

        /* This computes the logits */
        for(i = 0; i < mall_quality_price_info_list.size; i++) 
        {
            logit = (mall_quality_price_info_list.array[i].available) * 
			exp(GAMMA_CONST*(GAMMA_QUALITY*log(mall_quality_price_info_list.array[i].quality) -log(mall_quality_price_info_list.array[i].price))) / 
            sum_weighted_qual_pric_ratios;

            logit = logit*100;

            add_logit_firm_id(&logit_firm_id_list, logit,
            mall_quality_price_info_list.array[i].firm_id);
        }

        if(sum_weighted_qual_pric_ratios>0)
        {
            int random_number = random_int(0,100);
            j = 0;
            int x = 0, index_selected_good=j;

            for(j = 0; j < logit_firm_id_list.size;j++)
            {
                /*if random number <= logit then select the corresponding good  */ 
                if(((double)random_number <= logit_firm_id_list.array[j].logit+1e-4)
                && (x!=1))
                {
                    ORDER_QUANTITY[1].firm_id = logit_firm_id_list
                    .array[j].firm_id; //Seleced Good
                
                    x =1;
                    index_selected_good= j;
                }
                /*else sum logits and go to next iteration step */
                else
                {
                    if((j < logit_firm_id_list.size-1) )
                    {
                        logit_firm_id_list.array[j+1].logit = logit_firm_id_list.array[j+1].logit+ 
                        logit_firm_id_list.array[j].logit;
                    }
                }
            }
        
            /*This computes the order quantity  and store the price */
            ORDER_QUANTITY[1].quantity = WEEKLY_BUDGET/
            mall_quality_price_info_list.array[index_selected_good].price;

            ORDER_QUANTITY[1].price = mall_quality_price_info_list
            .array[index_selected_good].price;
		
			ORDER_QUANTITY[1].quality= mall_quality_price_info_list
            .array[index_selected_good].quality;

            /*Sending the second consumption request message  */
            add_consumption_request_2_message(
            mall_quality_price_info_list.array[index_selected_good].mall_id,
            ID, REGION_ID, ORDER_QUANTITY[1].firm_id, ORDER_QUANTITY[1].quantity);

            assert(ORDER_QUANTITY[1].firm_id>0);
			
			#ifdef _DEBUG_MODE
			if (PRINT_DEBUG_CONSUMPTION)
			{ 
				if(ORDER_QUANTITY[0].quantity<0)
				{
					fprintf(stderr, "\n\n IT %d ID: %d Household_rank_and_buy_goods_2", DAY, ID);
					fprintf(stderr, "\n\t HH adds consumption_request_2_message: ONLY NEGATIVE CONSUMPTION REQUESTS ARE SHOWN!!");
					fprintf(stderr, "\n\t HH ID\t Firm ID\t region ID\t quantity");
					fprintf(stderr, "\n\t %02d \t %02d\t %d\t %f", ID, ORDER_QUANTITY[1].firm_id, REGION_ID, ORDER_QUANTITY[1].quantity);
				}
			}
			#endif
        }
        else
        {       
            ORDER_QUANTITY[1].quantity = 0;
            ORDER_QUANTITY[1].firm_id= 0;
            ORDER_QUANTITY[1].price= 0;
			ORDER_QUANTITY[1].quality= 1.0;
        }

    free_mall_quality_price_info_array(&mall_quality_price_info_list);
    free_logit_firm_id_array(&logit_firm_id_list);

    return 0;
}

/** \fn Household_receive_goods_read_rationing_2()
 * \brief This function stores in memory the realized consumption if HH was rationed in the first round.
 */
int Household_receive_goods_read_rationing_2()
{

    if(RATIONED ==1)
    {
        /*Read the message about accepted consumption */
        START_ACCEPTED_CONSUMPTION_2_MESSAGE_LOOP

                RATIONED = accepted_consumption_2_message->rationed;

                RECEIVED_QUANTITY[1].quantity=
                accepted_consumption_2_message->offered_consumption_volume;
                
                RECEIVED_QUANTITY[1].firm_id = 
                ORDER_QUANTITY[1].firm_id; 

        FINISH_ACCEPTED_CONSUMPTION_2_MESSAGE_LOOP
    }
    else
    {
        RECEIVED_QUANTITY[1].quantity=0.0;
        RECEIVED_QUANTITY[1].firm_id =0; 
    }

    WEEKLY_BUDGET = WEEKLY_BUDGET - RECEIVED_QUANTITY[1].quantity 
    *ORDER_QUANTITY[1].price;

    assert(WEEKLY_BUDGET>-1e-4);
    
    EXPENDITURES += RECEIVED_QUANTITY[1].quantity * ORDER_QUANTITY[1].price;
    HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure += 
    RECEIVED_QUANTITY[1].quantity * ORDER_QUANTITY[1].price;

    return 0;
}

/** \fn Household_receive_dividends_dummy()
 * \brief Dummy to prevent households from reading the dividend payments from firms.
 */
int Household_receive_dividends_dummy()
{
        return 0;   
}


/** \fn Household_handle_leftover_budget()
 * \brief This function converts the remaining budget, that is not spent in both consumption steps, into the PAYMENT_ACCOUNT            
 */
int Household_handle_leftover_budget()
{   
    CONSUMPTION_BUDGET -= EXPENDITURES; 
    
    // Update monthly consumption tracker
    MONTHLY_CONSUMPTION_EXPENDITURE += EXPENDITURES;

    assert(EXPENDITURES < PAYMENT_ACCOUNT+1e-4);
        
    if(WEEK_OF_MONTH == 1)
    {               
        PAYMENT_ACCOUNT -= EXPENDITURES;
        log_outflow("household", "payment_account", "net_worth", "consumption", ID, EXPENDITURES);

        WEEK_OF_MONTH--;
    }
    else
    {
        PAYMENT_ACCOUNT -= EXPENDITURES;
        log_outflow("household", "payment_account", "net_worth", "consumption", ID, EXPENDITURES);

        WEEK_OF_MONTH--;
        WEEKLY_BUDGET = CONSUMPTION_BUDGET / WEEK_OF_MONTH;
    }
    
    //set rationed back to zero:
    RATIONED = 0;
    
    //Sanity Check:
    enforce_hh_valid_payment_account("Household_handle_leftover_budget");
    
    return 0;
}


/** \fn Household_send_account_update()
 * \brief This function sends the PAYMENT_ACCOUNT to the bank            
 */
int Household_send_account_update()
{
	//send msg to bank
	add_bank_account_update_message(ID, BANK_ID, PAYMENT_ACCOUNT);

    log_stock("household", "payment_account", ID, PAYMENT_ACCOUNT);

    double mortgages=0.0;
    if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
        mortgages += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
        
    int z;
    for (z=0; z<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; z++)
        if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[z].mortgage.active)
            mortgages += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[z].mortgage.outstanding_principal;

    log_stock("household", "mortgages", ID, mortgages);
    log_stock("household", "net_worth", ID, PAYMENT_ACCOUNT - mortgages);


    return 0;
}
