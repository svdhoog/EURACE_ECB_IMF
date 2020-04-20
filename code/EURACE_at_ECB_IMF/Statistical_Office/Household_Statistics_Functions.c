#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"



/************************************ Household agent functions ************************************/
/** \fn Household_reset_variables()
 * \brief Function to reset variables at start of new month
 * \brief Periodicity: monthly on day 1, at the top of the stategraph
*/
int Household_reset_variables()
{
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.payment_account_day_1 = PAYMENT_ACCOUNT;
	
	HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure = 0.0;
	HOUSEHOLD_OUTFLOWS_CALENDAR.tax_payment = 0.0;
	HOUSEHOLD_OUTFLOWS_CALENDAR.restitution_payment = 0.0;
	HOUSEHOLD_OUTFLOWS_CALENDAR.asset_purchases = 0.0;
	HOUSEHOLD_OUTFLOWS_CALENDAR.total_expenses = 0.0;
	
	HOUSEHOLD_INFLOWS_CALENDAR.wage = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.unemployment_benefit = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.subsidies = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.transfer = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.gov_interest = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.asset_sales = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.total_dividends = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.total_income = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.deposit_interest = 0.0;
	HOUSEHOLD_INFLOWS_CALENDAR.net_inflow = 0.0;

	/* Variables for analysing LM Flows.*/
	JUST_EMPLOYED = 0;
	JUST_UNEMPLOYED = 0;
	START_EMPLOYED = 0;
	START_UNEMPLOYED = 0;
	ENTER_MATCHING = 0;

	/* Employment status at the beginning of a month.*/
	if(EMPLOYEE_FIRM_ID == -1)
		START_UNEMPLOYED = 1;	
	else
		START_EMPLOYED = 1;

	// Variables related to mortgage counters

	HH_MORTGAGES_REQUESTED = 0;
	HH_MORTGAGES_ACCEPTED = 0;
	HH_MORTGAGES_PAYING = 0;
	HH_MORTGAGES_ARREARS = 0;
	HH_MORTGAGES_NPL = 0;
	HH_MORTGAGES_WRITEOFF = 0;
	HH_MORTGAGES_PAIDOFF = 0;

	HH_MORTGAGES_HMR_REQUESTED = 0;
	HH_MORTGAGES_HMR_ACCEPTED = 0;
	HH_MORTGAGES_HMR_PAYING = 0;
	HH_MORTGAGES_HMR_ARREARS = 0;
	HH_MORTGAGES_HMR_NPL = 0;
	HH_MORTGAGES_HMR_WRITEOFF = 0;
	HH_MORTGAGES_HMR_PAIDOFF = 0;

	HH_MORTGAGES_OP_REQUESTED = 0;
	HH_MORTGAGES_OP_ACCEPTED = 0;
	HH_MORTGAGES_OP_PAYING = 0;
	HH_MORTGAGES_OP_ARREARS = 0;
	HH_MORTGAGES_OP_NPL = 0;
	HH_MORTGAGES_OP_WRITEOFF = 0;
	HH_MORTGAGES_OP_PAIDOFF = 0;

	return 0;
}


/* \fn: int Household_initialization()
 * \brief Function to initialize. Note: this overwrites pop settings for the NEIGHBORING_REGION_IDS.
 */
int Household_initialization()
{   
	//ADD HERE SOME FUNCTIONS THAT SHOULD BE RUN ONCE AT START OF SIMULATION
	
	//Set target buffer stock savings as multiple of freely dispoable income
	//See Household_determine_consumption_budget()
	//PAYMENT_ACCOUNT = LIQUID_WEALTH_INCOME_RATIO_TARGET*(MEAN_NET_INCOME - HOUSING_BUDGET)
	LIQUID_WEALTH_INCOME_RATIO_TARGET = CONST_LIQUID_WEALTH_INCOME_RATIO_TARGET;

	// Set these in order to enable usage of constants in XML function conditions!
	INIT_TRANSIENT_PERIOD 		= CONST_INIT_TRANSIENT_PERIOD;
	
	// Set these to propagate from global constants to agent-specific variables
	REGULATORY_LTV_CAP			= CONST_REGULATORY_LTV_CAP;
	REGULATORY_DSTI_CAP			= CONST_REGULATORY_DSTI_CAP;

	//ADD HERE SOME FUNCTIONS THAT SET THE NEIGHBORHOOD STRUCTURE OF REGIONS

    /*********** NETWORK: complete graph **************/
/*    
    int j, k=TOTAL_REGIONS; //total number of regions
    
    reset_int_array(&NEIGHBORING_REGION_IDS);
    
    for (j=1; j<=k; j++) 
        add_int(&NEIGHBORING_REGION_IDS,j);
*/

    /*********** NETWORK: k neighbors with wrap-around */
    int j, k=2;
    for (j=1; j<=k; j++)
    { 
        if((REGION_ID+j)<=TOTAL_REGIONS)
            add_int(&NEIGHBORING_REGION_IDS,(REGION_ID+j));
        else
            add_int(&NEIGHBORING_REGION_IDS,(REGION_ID+j)%TOTAL_REGIONS);
    }
    /*********** NETWORK: k neighbors without wrap-around */
    //for (REGION_ID>k): use backward neighborhood
    //for (REGION_ID<=): use forward neighborhood
    /******************************************************/
/*    
    int j, k=2; //each region has k neighbors
    
    reset_int_array(&NEIGHBORING_REGION_IDS);

    //add the region_id of regions that are in the household's neighbourhood
    if (REGION_ID>k)
    {
        //set neighbourhood_regions={REGION_ID-k,...,REGION_ID-1}
        for (j=1; j<=k; j++) 
            add_int(&NEIGHBORING_REGION_IDS,REGION_ID-j);
    }
    if (REGION_ID<=k)
    {
        //set neighbourhood_regions={REGION_ID+1,...,REGION_ID+k}
        for (j=1; j<=k; j++) 
            add_int(&NEIGHBORING_REGION_IDS,REGION_ID+j);
    }
*/

 
    return 0;   
}

/*************************************Household Role: Statistics *********************************/
/** \fn Household_read_policy_announcements()
 * \brief This function reads messages send by the government announcing tax rates, benefits, transfer payments and subsidies.
 */
int Household_read_policy_announcements()
{
    /*Read tax announcement*/
    /*Read unemployment benefit announcement*/
    /*Read transfer announcement*/
    /*Read subsidy announcement*/
    START_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
    if(policy_announcement_message->gov_id == GOV_ID)
    {
        TAX_RATE_HH_LABOUR  = policy_announcement_message->tax_rate_hh_labour;
        TAX_RATE_HH_CAPITAL = policy_announcement_message->tax_rate_hh_capital;

        /*This is the endogeneous unemployment percentage (the global parameter is: GOV_POLICY_UNEMPLOYMENT_BENEFIT_PCT)*/
        UNEMPLOYMENT_BENEFIT_PCT = policy_announcement_message->unemployment_benefit_pct;
        TRANSFER_PAYMENT = policy_announcement_message->hh_transfer_payment;
        SUBSIDY_PCT = policy_announcement_message->hh_subsidy_pct;
    }
    FINISH_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSEHOLD)
    {
        printf("\n Household_read_policy_announcements ID: %d",ID);
        printf("\n \t TAX_RATE_HH_LABOUR: %f TAX_RATE_HH_CAPITAL: %f",TAX_RATE_HH_LABOUR,TAX_RATE_HH_CAPITAL);
        printf("\n \t UNEMPLOYMENT_BENEFIT_PCT: %f TRANSFER_PAYMENT: %f SUBSIDY_PCT: %f", UNEMPLOYMENT_BENEFIT_PCT, TRANSFER_PAYMENT, SUBSIDY_PCT);
    }
    #endif

	// HC Policy: Announcement
	if(GOV_POLICY_SWITCH_HUMAN_CAPITAL_IMPROVEMENT==1)
	{
	// If human capital policy is installed:
	START_HUMAN_CAPITAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

		HUMAN_CAPITAL_POLICY_FLAG = 1;

		add_human_capital_policy_general_skill_information_message(ID,GOV_ID,GENERAL_SKILL);
	
	FINISH_HUMAN_CAPITAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
    }

    // Central Bank Policy Stance announcements (Cap, LTV cap, DSTI cap)
    // Set these to propagate from ECB values to agent-specific variables
	START_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
		REGULATORY_LTV_CAP 			= ecb_policy_announcement_message->regulatory_ltv_cap;
		REGULATORY_DSTI_CAP 		= ecb_policy_announcement_message->regulatory_dsti_cap;
	FINISH_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

    // Central Bank Macroprudential Policy announcements
	START_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
		REGULATORY_LTV_CAP	= ecb_macroprudential_policy_announcement_message->regulatory_ltv_cap;
		REGULATORY_DSTI_CAP	= ecb_macroprudential_policy_announcement_message->regulatory_dsti_cap;    
    FINISH_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

	/*
    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MACROPRU)
    {
        printf("\nIT %d ID %d HH RECEIVED MACROPRUDENTIAL POLICY ANNOUNCEMENT:", DAY, ID);
        printf("\tREGULATORY_LTV_CAP: %f REGULATORY_DSTI_CAP: %f", REGULATORY_LTV_CAP, REGULATORY_DSTI_CAP);
    }
    #endif
	*/
    
    return 0;
}


/*
 * \fn Household_human_capital_policy()
 * \brief Update of general skills if there is a human capital policy
 */
int Household_human_capital_policy()
{

	START_HUMAN_CAPITAL_POLICY_NEW_GENERAL_SKILL_NOTIFICATION_MESSAGE_LOOP

	//printf("GENERAL_SKILL vorher: %d \n",GENERAL_SKILL);
	GENERAL_SKILL = human_capital_policy_new_general_skill_notification_message->general_skills;

	HUMAN_CAPITAL_POLICY_FLAG = 0;
	//printf("GENERAL_SKILL nachher: %d \n",GENERAL_SKILL);
	FINISH_HUMAN_CAPITAL_POLICY_NEW_GENERAL_SKILL_NOTIFICATION_MESSAGE_LOOP

	return 0;
}


/*
 * \fn Household_receive_account_interest()
 * \brief Read account_interest message from Bank. Daily, at start of iteration.
 */
int Household_receive_account_interest()
{
    double interest =0;
    
    /*For validation check*/
    if(DAY%MONTH == 1)
    {
		RECEIVED_INTEREST_IN_CALENDAR_MONTH=0;
    }
    
    //Read message from bank, set risk-free rate.
    START_ACCOUNT_INTEREST_MESSAGE_LOOP
		if(account_interest_message->bank_id == BANK_ID)
	       DEPOSIT_INTEREST_RATE = account_interest_message->interest_rate; //Used in consumption budget
    FINISH_ACCOUNT_INTEREST_MESSAGE_LOOP
    
    /*Add daily interest on deposits: (1/240) of the interest rate*/
    interest = (1/240.0)*DEPOSIT_INTEREST_RATE*PAYMENT_ACCOUNT;
    PAYMENT_ACCOUNT += interest;
    log_inflow("household", "payment_account", "net_worth", "pa_interest", ID, interest);

    
    /*For validation check*/
    RECEIVED_INTEREST_DAILY = interest;
    RECEIVED_INTEREST_IN_CALENDAR_MONTH += interest;
    HOUSEHOLD_INFLOWS_CALENDAR.deposit_interest += interest; 

    //Sanity Check:
    if (PAYMENT_ACCOUNT < -0.01)
        fprintf(stderr, "\nIT %d ID %d Household_receive_account_interest: PAYMENT_ACCOUNT < -0.01, PAYMENT_ACCOUNT=%.2f", DAY, ID, PAYMENT_ACCOUNT);

    return 0;	
}

/** \fn Household_send_data_to_Eurostat()
 * \brief Households send data to Eurostat
 * \brief Frequency: Last day of month
 */
int Household_send_data_to_Eurostat()
{   
	int i;
	double portfolio_value = 0.0;
	int nr_assets = 0;
	double sum_quarterly_income=0.0;
	double sum;
	int total;

	LIQUID_WEALTH_INCOME_RATIO = PAYMENT_ACCOUNT / MEAN_NET_INCOME;

	if(MORTGAGE_PAYMENT_HMR + MORTGAGE_PAYMENT_OP > 1e-4)
		INTEREST_ANNUITY_RATIO = (MORTGAGE_INTEREST_PAYMENT_HMR + MORTGAGE_INTEREST_PAYMENT_OP)/(MORTGAGE_PAYMENT_HMR + MORTGAGE_PAYMENT_OP);
	else
		INTEREST_ANNUITY_RATIO = 0.0;

	if(MORTGAGE_PAYMENT_HMR > 1e-4)
		HMR_INTEREST_ANNUITY_RATIO = MORTGAGE_INTEREST_PAYMENT_HMR/MORTGAGE_PAYMENT_HMR;
	else
		HMR_INTEREST_ANNUITY_RATIO = 0.0;

	if(MORTGAGE_PAYMENT_OP > 1e-4)
		OP_INTEREST_ANNUITY_RATIO = MORTGAGE_INTEREST_PAYMENT_OP/MORTGAGE_PAYMENT_OP;
	else
		OP_INTEREST_ANNUITY_RATIO = 0.0;

	//Total gross annual income, moving average
	for (i=11; i>0; i--)
		MONTHLY_INCOME_VECTOR[i] = MONTHLY_INCOME_VECTOR[i-1];
	MONTHLY_INCOME_VECTOR[0] = TOTAL_INCOME;

	//Moving avg
	TOTAL_GROSS_ANNUAL_INCOME = 0.0;
	for(i=0;i<12;i++) TOTAL_GROSS_ANNUAL_INCOME += MONTHLY_INCOME_VECTOR[i];
	
	if ( DAY % (MONTH*3)==0 )
	{
		//Get last 3 monthly incomes
		for(i=0;i<4;i++) sum_quarterly_income += MONTHLY_INCOME_VECTOR[i];
		
		//Moving avg
		TOTAL_GROSS_QUARTERLY_INCOME = sum_quarterly_income;
	}

	//Separate items:
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.payment_account_day_20 = PAYMENT_ACCOUNT;

	sum=0.0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
			sum += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_total_repayment;

	HOUSEHOLD_OUTFLOWS_CALENDAR.mortgage_debt_service = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment + sum;
	HOUSEHOLD_OUTFLOWS_CALENDAR.rents = RENTAL_UNIT.monthly_rent;

	//Newly obtained mortgages
	double sum_hmr = 0.0;
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration == HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration)
			 sum_hmr = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;

	sum=0.0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.initial_duration == HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration)
			sum += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;
	}
	HOUSEHOLD_INFLOWS_CALENDAR.mortgage_loan = sum_hmr + sum;

	sum=0.0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		sum += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].monthly_rent;

	HOUSEHOLD_INFLOWS_CALENDAR.rents = sum;

	//Total number of mortgages
	total = 0;
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
		total++;

	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
			total++;
	}
/*
	if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
	{
		//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);
		if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id==150)
		{
			printf("GOTCHA!\n");
			print_agent_info(ID);
			print_agent_object_info();
			printf("\n");
			printf("active: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active);
			printf("object_id: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);
			printf("current_duration: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
			printf("initial_duration: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration);
			printf("outstanding_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
			printf("outstanding_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest);
			printf("monthly_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal);
			printf("monthly_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest);
			printf("status: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
		}
	}

	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
		{
			//printf("%d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);
			if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id==150)
			{
				printf("GOTCHA!\n");
				print_agent_info(ID);
				print_agent_object_info();
				printf("\n");
				printf("active: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active);
				printf("object_id: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);
				printf("current_duration: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration);
				printf("initial_duration: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.initial_duration);
				printf("outstanding_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal);
				printf("outstanding_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest);
				printf("monthly_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal);
				printf("monthly_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest);
				printf("status: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status);
				printf("arrears_counter: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.arrears_counter);

			}
		}
	}
*/
	HH_NO_MORTGAGES = total;

	//Total outflow
	HOUSEHOLD_OUTFLOWS_CALENDAR.total_expenses = 
	HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure +
	HOUSEHOLD_OUTFLOWS_CALENDAR.tax_payment +
	HOUSEHOLD_OUTFLOWS_CALENDAR.restitution_payment +
	HOUSEHOLD_OUTFLOWS_CALENDAR.asset_purchases +
	HOUSEHOLD_OUTFLOWS_CALENDAR.mortgage_debt_service +
	HOUSEHOLD_OUTFLOWS_CALENDAR.rents;
	
	//Total inflow
	HOUSEHOLD_INFLOWS_CALENDAR.total_income = 
	HOUSEHOLD_INFLOWS_CALENDAR.wage +
	HOUSEHOLD_INFLOWS_CALENDAR.unemployment_benefit +
	HOUSEHOLD_INFLOWS_CALENDAR.subsidies +
	HOUSEHOLD_INFLOWS_CALENDAR.transfer +
	HOUSEHOLD_INFLOWS_CALENDAR.gov_interest +
	HOUSEHOLD_INFLOWS_CALENDAR.asset_sales +
	HOUSEHOLD_INFLOWS_CALENDAR.total_dividends + 
	HOUSEHOLD_INFLOWS_CALENDAR.deposit_interest +
	HOUSEHOLD_INFLOWS_CALENDAR.mortgage_loan +
	HOUSEHOLD_INFLOWS_CALENDAR.rents; 	
	
	HOUSEHOLD_INFLOWS_CALENDAR.net_inflow = 
	HOUSEHOLD_INFLOWS_CALENDAR.total_income -
	HOUSEHOLD_OUTFLOWS_CALENDAR.total_expenses;
	
	portfolio_value = ASSETSOWNED.units*ASSETSOWNED.lastprice;
	nr_assets 	= ASSETSOWNED.units;
	
	//Stocks
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.payment_account = PAYMENT_ACCOUNT;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.portfolio_value = portfolio_value;

	sum=0.0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		sum += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_real_estate_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price + sum;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_assets = PAYMENT_ACCOUNT + portfolio_value + HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_real_estate_value;
	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.nr_assets = nr_assets;
	
	sum=0.0;
	for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		sum += HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal;

	HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_liabilities = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal + sum;

	//Copy all flows and stocks into balance sheet
	copy_household_inflows_adt(&HOUSEHOLD_INFLOWS_CALENDAR, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.inflows);
	copy_household_outflows_adt(&HOUSEHOLD_OUTFLOWS_CALENDAR, &HOUSEHOLD_BALANCE_SHEET_CALENDAR.outflows);
	
	/* Old-style message:
	add_household_send_data_message(ID, REGION_ID, GENERAL_SKILL,EMPLOYEE_FIRM_ID,
	WAGE, SPECIFIC_SKILL,JUST_EMPLOYED,JUST_UNEMPLOYED,START_EMPLOYED,START_UNEMPLOYED,ENTER_MATCHING,UNEMPLOYED_DURATION, 		
	CONSUMPTION_BUDGET_IN_MONTH,PAYMENT_ACCOUNT,MEAN_NET_INCOME, HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure);
	*/
	household_info_adt household_info;
	init_household_info_adt(&household_info);

	household_info.region_id = REGION_ID;
	household_info.general_skill = GENERAL_SKILL;
	household_info.employment_status = EMPLOYEE_FIRM_ID; //if -1 then unemployed
	household_info.employee_firm_id = EMPLOYEE_FIRM_ID;
	household_info.wage = WAGE;
	
	household_info.specific_skill = SPECIFIC_SKILL;
	household_info.just_employed = JUST_EMPLOYED;
	household_info.just_unemployed = JUST_UNEMPLOYED;
	
	household_info.start_employed = START_EMPLOYED;
	household_info.start_unemployed = START_UNEMPLOYED;
	household_info.enter_matching = ENTER_MATCHING;
	household_info.unemployed_duration = UNEMPLOYED_DURATION;

	household_info.payment_account = PAYMENT_ACCOUNT;
	household_info.mean_net_income = MEAN_NET_INCOME;
	household_info.total_gross_annual_income = TOTAL_GROSS_ANNUAL_INCOME;
	household_info.total_gross_quarterly_income = TOTAL_GROSS_QUARTERLY_INCOME;

	household_info.consumption_budget = CONSUMPTION_BUDGET_IN_MONTH;
	household_info.consumption_expenditure = HOUSEHOLD_OUTFLOWS_CALENDAR.consumption_expenditure;
	household_info.monthly_rent_expense = RENTAL_UNIT.monthly_rent;
	household_info.monthly_rent_income = HOUSEHOLD_INFLOWS_CALENDAR.rents;
	household_info.monthly_mortgage_payment = HOUSEHOLD_OUTFLOWS_CALENDAR.mortgage_debt_service;

	household_info.is_homeless 	= IS_HOMELESS;
	household_info.is_tenant 	= IS_TENANT;
	household_info.is_owner 	= IS_OWNER;
	household_info.is_landlord 	= IS_LANDLORD;
	household_info.has_btl_gene = BTL_GENE;
	household_info.defaulted_on_rent = DEFAULTED_ON_RENT;
	household_info.evicted_as_tenant = EVICTED_AS_TENANT;
	household_info.evicted_as_owner = EVICTED_AS_OWNER;

	household_info.interest_annuity_ratio = INTEREST_ANNUITY_RATIO;
	household_info.hmr_interest_annuity_ratio = HMR_INTEREST_ANNUITY_RATIO;
	household_info.op_interest_annuity_ratio = OP_INTEREST_ANNUITY_RATIO;

	household_info.hmr_debt_service_to_income_ratio = HMR_DEBT_SERVICE_TO_INCOME_RATIO;
	household_info.rent_to_income_ratio = RENT_TO_INCOME_RATIO;
	household_info.payment_account_to_income_ratio = PAYMENT_ACCOUNT_TO_INCOME_RATIO;
	household_info.hmr_debt_service_buffer_in_months = HMR_DEBT_SERVICE_BUFFER_IN_MONTHS;
	household_info.rent_buffer_in_months = RENT_BUFFER_IN_MONTHS;

	household_info.op_income = OP_INCOME; 
	household_info.op_expense = OP_EXPENSE;
	household_info.op_profit = OP_PROFIT;
	household_info.share_op_with_mortgage = SHARE_OP_WITH_MORTGAGE; 

	household_info.hh_mortgages_ltv_redraw		= HH_MORTGAGES_LTV_REDRAW;
	household_info.hh_mortgages_ltv_cap_active	= HH_MORTGAGES_LTV_CAP_ACTIVE;
	household_info.hh_mortgages_dsti_cap_active = HH_MORTGAGES_DSTI_CAP_ACTIVE;

	household_info.hh_no_mortgages  		 = HH_NO_MORTGAGES;

	household_info.hh_mortgages_requested    = HH_MORTGAGES_REQUESTED;
	household_info.hh_mortgages_accepted     = HH_MORTGAGES_ACCEPTED;
	household_info.hh_mortgages_paying       = HH_MORTGAGES_PAYING;
	household_info.hh_mortgages_arrears      = HH_MORTGAGES_ARREARS;
	household_info.hh_mortgages_npl          = HH_MORTGAGES_NPL;
	household_info.hh_mortgages_writeoff     = HH_MORTGAGES_WRITEOFF;
	household_info.hh_mortgages_paidoff      = HH_MORTGAGES_PAIDOFF;

	household_info.hh_mortgages_hmr_requested= HH_MORTGAGES_HMR_REQUESTED;
	household_info.hh_mortgages_hmr_accepted = HH_MORTGAGES_HMR_ACCEPTED;
	household_info.hh_mortgages_hmr_paying   = HH_MORTGAGES_HMR_PAYING;
	household_info.hh_mortgages_hmr_arrears  = HH_MORTGAGES_HMR_ARREARS;
	household_info.hh_mortgages_hmr_npl      = HH_MORTGAGES_HMR_NPL;
	household_info.hh_mortgages_hmr_writeoff = HH_MORTGAGES_HMR_WRITEOFF;
	household_info.hh_mortgages_hmr_paidoff  = HH_MORTGAGES_HMR_PAIDOFF;

	household_info.hh_mortgages_op_requested = HH_MORTGAGES_OP_REQUESTED;
	household_info.hh_mortgages_op_accepted  = HH_MORTGAGES_OP_ACCEPTED;
	household_info.hh_mortgages_op_paying    = HH_MORTGAGES_OP_PAYING;
	household_info.hh_mortgages_op_arrears   = HH_MORTGAGES_OP_ARREARS;
	household_info.hh_mortgages_op_npl       = HH_MORTGAGES_OP_NPL;
	household_info.hh_mortgages_op_writeoff  = HH_MORTGAGES_OP_WRITEOFF;
	household_info.hh_mortgages_op_paidoff   = HH_MORTGAGES_OP_PAIDOFF;

	// New-style message:
	add_household_send_data_message(ID, household_info);
	
    return 0;
}


/** \fn Household_read_data_from_Eurostat()
 * \brief Households read data from Eurostat
 */
int Household_read_data_from_Eurostat()
{   
    //The household reads the mean wage in its region (we assume this is a country)
    START_DATA_FOR_GOVERNMENT_MESSAGE_LOOP
            if(data_for_government_message->region_id==REGION_ID)
            {
                //Read region mean wage
                REGION_WIDE_MEAN_WAGE = data_for_government_message->mean_wage;
            }
    FINISH_DATA_FOR_GOVERNMENT_MESSAGE_LOOP
    
    return 0;
}


/** \fn Household_send_loan_portfolio_data()
 * \brief Function to send Household's complete debt portfolio (loan_adts) to the NSA agent for network analysis.
 */
int Household_send_loan_portfolio_data()
{
  // Declare, init financial_data
  financial_data_adt financial_data;
  
  financial_data.equity		= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_assets - HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_liabilities;
  financial_data.total_assets	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_assets;
	  financial_data.total_debt	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.total_liabilities;
  financial_data.equity_asset_ratio = financial_data.equity/financial_data.total_assets;
  financial_data.debt_equity_ratio = financial_data.total_debt/financial_data.total_assets;

  //Create a temporary debt_item
  debt_item loan_adt;

  //Init zero values
  init_debt_item(&loan_adt);
  
  // Convert Mortgage adt to debt_item adt to conform with NSA network data type
  if(HAVE_MORTGAGE==1)
  {
	loan_adt.bank_id	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.bank_id;
	loan_adt.loan_value	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal;
	loan_adt.interest_rate	= HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual;
	loan_adt.installment_amount = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal;
	loan_adt.nr_periods_before_repayment = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration;
	loan_adt.bad_debt = (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status==3)*loan_adt.loan_value;
  }

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_NSA)
    {
        printf("\n\n Household_send_loan_portfolio_data ID: %d Sent %d loan_adts (debt_items)", ID, 1);
    }      
    #endif
    
    // Send financial_data
    add_hh_loan_portfolio_message(ID, financial_data, loan_adt);
	
	//Free allocated memory
	free_debt_item(&loan_adt);
	
	return 0;   
}
