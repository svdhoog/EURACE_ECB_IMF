//#include <assert.h>
#include "../header.h"
#include "../Eurostat_agent_header.h"
#include "../my_library_header.h"
#include "Eurostat_aux_header.h"

/** \Eurostat_idle()
  */
int Eurostat_idle()
{
    return 0;   
}

/** \Eurostat_initialization()
  */
int Eurostat_initialization()
{
    #ifdef _DEBUG_MODE    
    if (PRINT_EUROSTAT_INFO)
    {
        printf("\n Eurostat_initialization");
    }
    #endif
        

    if(TOTAL_REGIONS>REGION_FIRM_DATA.size)
    {   
		#ifdef _DEBUG_MODE    
		if (PRINT_EUROSTAT_INFO)
		{
		    printf("\nIn Eurostat_initialization:");
		    printf("\nTOTAL_REGIONS (%d) > size of REGION_FIRM_DATA array (%d)", TOTAL_REGIONS, REGION_FIRM_DATA.size);
		    printf("\n      Running Eurostat_reset_data function to reset data structures");
		}
		#endif

        Eurostat_reset_data();

    }

	#ifdef _DEBUG_MODE    
	if (PRINT_EUROSTAT_INFO)
	{
	    if(TOTAL_REGIONS==REGION_FIRM_DATA.size)
            printf("\n      TOTAL_REGIONS (%d) == size of REGION_FIRM_DATA array (%d)\n", TOTAL_REGIONS, REGION_FIRM_DATA.size);
	}
	#endif

    assert(TOTAL_REGIONS==REGION_FIRM_DATA.size);

    return 0;   
}



/** \Eurostat_compute_house_price_index()
  * \brief: Function to update the price indices for HMR, OP and RENT. Executed every last day of month, after initial transient period.
  * \brief: Input mesgs:
  *         - mortgage_payment_message(household_id, bank_id, loan_loss, loan_default, principal, interest)
  *         - rent_payment_message(tenant_id, landlord_id, monthly_rent)
  * \brief: Output mesgs:
  *         - index_info_message(hmr_price_index, op_price_index, rent_price_index);
  * \brief Condition: Every 1st day of calendar month; If (iteration_loop GT a.init_transient_period) && (period==monthly, phase==1)
  * \brief UNITTEST: 
  * TODO Make sure this function runs every day to collect data on house and rental prices
  * Compute index values only on 1st day of calander month + reset the data arrays
  */
int Eurostat_compute_house_price_index()
{    
    int i;

    //MONTHLY: 2nd day of calendar month: Compute indices and reset data arrays
    //Note: this function runs monthly on day 2, because iteration_loop >= init_transient_period ==6002
    //Assume: All Households send their price info on (DAY%MONTH)==2
    if (DAY%MONTH == 2)
    {
        COUNT_TENANTS = 0;
        COUNT_OWNERS = 0;
        COUNT_LANDLORDS = 0;
        COUNT_HOMELESS = 0;
        double sum = 0;

        //Read all HMR housing unit prices from owner-occupiers:
        START_OWNER_INFO_MESSAGE_LOOP
            add_double(&HMR_PRICE_DATA, owner_info_message->hmr_value);
            sum+=owner_info_message->hmr_value;
            COUNT_OWNERS++;
            //printf("%d - %f\n", COUNT_OWNERS, owner_info_message->hmr_value);
        FINISH_OWNER_INFO_MESSAGE_LOOP
        //double avg = sum/COUNT_OWNERS;
        //printf("-> %f \n", avg);

        //Read all HMR housing unit + rental unit prices from landlords:
        START_LANDLORD_INFO_MESSAGE_LOOP
            //add_double(&HMR_PRICE_DATA, landlord_info_message->hmr_value);
            add_double(&OP_PRICE_DATA, landlord_info_message->op_value);
            COUNT_LANDLORDS++;
        FINISH_LANDLORD_INFO_MESSAGE_LOOP

        //Read all rent prices from tenants:
        START_TENANT_INFO_MESSAGE_LOOP
            add_double(&RENT_PRICE_DATA, tenant_info_message->monthly_rent);
            COUNT_TENANTS++;
        FINISH_TENANT_INFO_MESSAGE_LOOP

        //Read all homeless:
        START_HOMELESS_INFO_MESSAGE_LOOP            
            COUNT_HOMELESS++;
        FINISH_HOMELESS_INFO_MESSAGE_LOOP

        #ifdef _DEBUG_MODE
        if (PRINT_DEBUG_RENTAL_UNITTEST)
        {
            printf("\n IT %d", DAY);
            printf("\t HMR_PRICE_DATA=[");
            for (i=0; i<HMR_PRICE_DATA.size; i++)
                printf(" %.2f", HMR_PRICE_DATA.array[i]);
            printf(" ]\n");

            printf("\n OP_PRICE_DATA=[");
            for (i=0; i<OP_PRICE_DATA.size; i++)
                printf(" %.2f", OP_PRICE_DATA.array[i]);
            printf(" ]\n");

            printf("\n RENT_PRICE_DATA=[");
            for (i=0; i<RENT_PRICE_DATA.size; i++)
                printf(" %.2f", RENT_PRICE_DATA.array[i]);
            printf(" ]\n");
        }      
        #endif

        init_price_index_adt(&HMR_PRICE_INDEX);
        init_price_index_adt(&OP_PRICE_INDEX);
        init_price_index_adt(&RENT_PRICE_INDEX);

        HMR_PRICE_INDEX.mean = my_stats_mean(&HMR_PRICE_DATA);
        HMR_PRICE_INDEX.sd = my_stats_sd(&HMR_PRICE_DATA);
        HMR_PRICE_MEAN = HMR_PRICE_INDEX.mean;
        HMR_PRICE_CV = HMR_PRICE_INDEX.sd / HMR_PRICE_INDEX.mean;

        //printf("--> %f \n", HMR_PRICE_MEAN);

        OP_PRICE_INDEX.mean = my_stats_mean(&OP_PRICE_DATA);
        OP_PRICE_INDEX.sd = my_stats_sd(&OP_PRICE_DATA);
        OP_PRICE_MEAN = OP_PRICE_INDEX.mean;
        OP_PRICE_CV = OP_PRICE_INDEX.sd / OP_PRICE_INDEX.mean;

        RENT_PRICE_INDEX.mean = my_stats_mean(&RENT_PRICE_DATA);
        RENT_PRICE_INDEX.sd = my_stats_sd(&RENT_PRICE_DATA);
        RENT_PRICE_MEAN = RENT_PRICE_INDEX.mean;
        RENT_PRICE_CV = RENT_PRICE_INDEX.sd / RENT_PRICE_INDEX.mean;

        add_house_price_index_message(HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX);

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_RENTAL)
        {
            printf("\n\n ITS: %d ID: %d Eurostat_compute_house_price_index:", DAY, ID);
            printf("\n HMR_PRICE_INDEX.mean %f HMR_PRICE_INDEX.sd %f", HMR_PRICE_INDEX.mean, HMR_PRICE_INDEX.sd);
            printf("\n OP_PRICE_INDEX.mean %f OP_PRICE_INDEX.sd %f", OP_PRICE_INDEX.mean, OP_PRICE_INDEX.sd);
            printf("\n RENT_PRICE_INDEX.mean %f RENT_PRICE_INDEX.sd %f", RENT_PRICE_INDEX.mean, RENT_PRICE_INDEX.sd);
        }      
        #endif

        reset_double_array(&HMR_PRICE_DATA);
        reset_double_array(&OP_PRICE_DATA);
        reset_double_array(&RENT_PRICE_DATA);
    }

    return 0;   
}

int Eurostat_send_subsistence_level()
{
    add_subsistence_level_message(PRICE_INDEX, MINIMUM_SUBSISTENCE_LEVEL, HOUSING_MARKET_PRICE_GROWTH_FACTOR, RENTAL_MARKET_PRICE_GROWTH_FACTOR, ECB_INTEREST_RATE);

    return 0;
}

/** \Eurostat_send_data_to_government()
 * \brief Eurostat send data: mean wage  ...
 * OBSOLETE FUNCTION
 */   
int Eurostat_send_data_to_government()
{
    int i, region;
    double gdp;
    
    //printf("AVERAGE_WAGE %f\n",AVERAGE_WAGE);
    //add_mean_wage_for_government_message(1, AVERAGE_WAGE);

    //add message for each region
    for (i=0; i<TOTAL_REGIONS; i++)
    {
        region = i+1;
        
        gdp = REGION_FIRM_DATA.array[i].gdp;
        
		#ifdef _DEBUG_MODE    
		if (PRINT_EUROSTAT_INFO)
		{
		    printf("\n Region %d GDP=%2.2f\n", region, gdp);
		}      
		#endif

        add_data_for_government_message(region, gdp, AVERAGE_WAGE);
    }
    
    return 0;
}

/** \Eurostat_send_data()
 * \brief Eurostat send data: specific skills ...
 */
int Eurostat_send_data()
{
    int i, region;
    double gdp;
    double average_output = 0.0;    

    /*First of every month*/
    /*Send the data*/

	//send message about the total market size
	add_msg_market_sizes_message(0, HH_CONSUMPTION_BUDGET);
    
   
  for(i = 0; i < REGION_HOUSEHOLD_DATA.size; i++)
  {
    average_output = 0.0;

    if(HISTORY_MONTHLY[0].region_data.array[i].no_active_firms>0)
      average_output = HISTORY_MONTHLY[0].region_data.array[i].output/HISTORY_MONTHLY[0].region_data.array[i].no_active_firms;

    add_eurostat_send_specific_skills_message(
      REGION_HOUSEHOLD_DATA.array[i].region_id,  
	    REGION_HOUSEHOLD_DATA.array[i].average_s_skill, 
      REGION_HOUSEHOLD_DATA.array[i].average_s_skill_1, 
      REGION_HOUSEHOLD_DATA.array[i].average_s_skill_2,
      REGION_HOUSEHOLD_DATA.array[i].average_s_skill_3, 
      REGION_HOUSEHOLD_DATA.array[i].average_s_skill_4, 
      REGION_HOUSEHOLD_DATA.array[i].average_s_skill_5,
      REGION_FIRM_DATA.array[i].productivity_progress,
      REGION_FIRM_DATA.array[i].total_mean_wage,
      average_output,
      CPI,PRICE_INDEX,NO_FIRMS,NO_ACTIVE_FIRMS,FIRM_AVERAGE_QUALITY,FIRM_AVERAGE_PRICE,TOTAL_CAPITAL_STOCK_UNITS);

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_EUROSTAT)
	{
		printf("\n\n Eurostat_send_data: added eurostat_send_specific_skills_message, region %d\n", i);
	}          
	#endif        

	
	//send message about the regional market size
	add_msg_market_sizes_message(REGION_HOUSEHOLD_DATA.array[i].region_id,  REGION_HOUSEHOLD_DATA.array[i].consumption_budget );
	

	/*Here Eurostat sends average general skills, aberage specific skills and the average productivity progress of the total economy to the IG firm
	There it is needed for determining the IG price.*/
	if(DAY>1)
	{
		add_msg_skills_eurostat_to_igfirm_message(FIRM_AVERAGE_G_SKILL,FIRM_AVERAGE_S_SKILL,FIRM_AVERAGE_PRODUCTIVITY_PROGRESS,TOTAL_CAPITAL_STOCK_UNITS);

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_EUROSTAT)
		{
			printf("\n\n Eurostat_send_data: added msg_skills_eurostat_to_igfirm_message, region %d\n", i);
		}          
		#endif        
	}




	if(DAY%20 == 1)
	{
	    #ifdef _DEBUG_MODE    
	    if (PRINT_DEBUG_EUROSTAT)
	    {
		    printf("\n\n IT %d Eurostat send data",DAY);  
		    printf("\n\t REGION_FIRM_DATA.array[i].productivity_progress %f",REGION_FIRM_DATA.array[i].productivity_progress);
		    printf("\n\t REGION_HOUSEHOLD_DATA.size = %d REGION_HOUSEHOLD_DATA.array[i].average_s_skill = %f", REGION_HOUSEHOLD_DATA.size, REGION_HOUSEHOLD_DATA.array[i].average_s_skill);
	    }          
	    #endif        
	}
    }
	
	//Add message for each region
	for (i=0; i<TOTAL_REGIONS; i++)
	{
		region = i+1;
		
		gdp = REGION_FIRM_DATA.array[i].gdp;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_EUROSTAT_INFO)
		{
		    printf("\t\t\t\tRegion %d GDP=%2.2f\n", region, gdp);
		}      
		#endif

		add_data_for_government_message(region, gdp, AVERAGE_WAGE);
	}
	
	//Add message for general economy-wide macrodata 
	//add_eurostat_send_macrodata_message(ANNUAL_GROWTH_RATES_MONTHLY.cpi, GDP, UNEMPLOYMENT_RATE);    

    //3.5.17: Add potential_output for monetary policy Taylor rules
    add_eurostat_send_macrodata_message(ANNUAL_GROWTH_RATES_MONTHLY.cpi, GDP, UNEMPLOYMENT_RATE, MONTHLY_OUTPUT, MONTHLY_POTENTIAL_OUTPUT);    
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_EUROSTAT)
	{
		printf("\n\n Eurostat_send_data: added eurostat_send_macrodata_message\n");
	}          
	#endif        

    return 0;
}

/** \fn Eurostat_read_macroprudential_policy_announcements
 * \brief Eurostat receives macroprudential policy announcements from ECB
 */
int Eurostat_read_policy_announcements()
{
   ECB_INTEREST_RATE = CONST_ECB_INTEREST_RATE;

   START_POLICY_RATE_MESSAGE_LOOP
          ECB_INTEREST_RATE = policy_rate_message->policy_rate_value;      
   FINISH_POLICY_RATE_MESSAGE_LOOP

  // Central Bank Policy Stance announcements (Cap, LTV cap, DSTI cap)
  // Set these to propagate from ECB values to agent-specific variables
  START_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
    REGULATORY_CAPITAL_BUFFER   = ecb_policy_announcement_message->regulatory_capital_buffer;
    REGULATORY_LTV_CAP          = ecb_policy_announcement_message->regulatory_ltv_cap;
    REGULATORY_DSTI_CAP         = ecb_policy_announcement_message->regulatory_dsti_cap;
  FINISH_ECB_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

  // Central Bank Macroprudential Policy announcements
  START_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
      REGULATORY_CAPITAL_BUFFER = ecb_macroprudential_policy_announcement_message->regulatory_capital_buffer;
      REGULATORY_LTV_CAP        = ecb_macroprudential_policy_announcement_message->regulatory_ltv_cap;
      REGULATORY_DSTI_CAP       = ecb_macroprudential_policy_announcement_message->regulatory_dsti_cap;
  FINISH_ECB_MACROPRUDENTIAL_POLICY_ANNOUNCEMENT_MESSAGE_LOOP

  return 0;
}

/** \fn Eurostat_calculate_data
 * \brief Eurostat receives micro data and calculates macro data
 */
int Eurostat_calculate_data()
{
    /* Eurostat auxiliary functions */
     Eurostat_reset_data();
     Eurostat_compute_mean_price();
     Eurostat_read_firm_data();
     Eurostat_compute_region_firm_data();
     Eurostat_compute_global_firm_data();

     Eurostat_read_household_data();
     Eurostat_compute_subsistence_level();

     Eurostat_compute_region_household_data();
     Eurostat_compute_global_household_data();

     Eurostat_calc_macro_data();
    
     Eurostat_compute_timeseries_statistics();

	   Eurostat_compute_macro_indicators();
	 
    return 0;
}

/******************************* STORING MONTHLY DATA **************************************/
/* Datatype:

      struct history_item
      {
            double cpi;
            double gdp;
            double output;
            double employment;
            double unemployment_rate;
            double average_wage;    
       }

    //Static array of history items:
     history_item history_monthly[13];
     history_item history_quarterly[5];
    
    //Single history_items for storing the growth rates: 
    history_item monthly_growth_rates: month to month growth rate statistics
    history_item quarterly__growth_rates: quarter to quarter growth rate statistics
    history_item annual_growth_rates_monthly: annual growth rates (per month), measured as the percentage change over the same month in the previous year.
    history_item annual_growth_rates_quarterly: annual growth rates (per quarter), measured as the percentage change over the same quarter in the previous year.
*/

/* \fn: int Eurostat_store_history_monthly()
 * \brief: Central_Eurostat agent stores statistics to monthly history structure. 
 */
int Eurostat_store_history_monthly()
{   
    FILE *file1;
    char *filename;
    int i, region;
    region=0;
    
    //Shift history backwards
    /*
     * history_monthly[4].GDP = history_monthly[3].GDP;     //t-4 gets filled with value from t-3
     * history_monthly[3].GDP = history_monthly[2].GDP;     //t-3 gets filled with value from t-2
     * history_monthly[2].GDP = history_monthly[1].GDP;     //t-2 gets filled with value from t-1
     * history_monthly[1].GDP = history_monthly[0].GDP;     //t-1 gets filled with value from t
     * history_monthly[0].GDP = GDP;                        //t gets filled with value from t
     */

    //*********************************** Economy-wide data
    for (i=12; i>0; i--)
    {
      //t-i-1 gets filled with value from t-i
      HISTORY_MONTHLY[i].cpi = HISTORY_MONTHLY[i-1].cpi;
      HISTORY_MONTHLY[i].gdp = HISTORY_MONTHLY[i-1].gdp;
      HISTORY_MONTHLY[i].output = HISTORY_MONTHLY[i-1].output;
      HISTORY_MONTHLY[i].employment = HISTORY_MONTHLY[i-1].employment;
      HISTORY_MONTHLY[i].unemployment_rate = HISTORY_MONTHLY[i-1].unemployment_rate;
      HISTORY_MONTHLY[i].unemployment_rate_skill_1 = HISTORY_MONTHLY[i-1].unemployment_rate_skill_1;
      HISTORY_MONTHLY[i].unemployment_rate_skill_2 = HISTORY_MONTHLY[i-1].unemployment_rate_skill_2;
      HISTORY_MONTHLY[i].unemployment_rate_skill_3 = HISTORY_MONTHLY[i-1].unemployment_rate_skill_3;
      HISTORY_MONTHLY[i].unemployment_rate_skill_4 = HISTORY_MONTHLY[i-1].unemployment_rate_skill_4;
      HISTORY_MONTHLY[i].unemployment_rate_skill_5 = HISTORY_MONTHLY[i-1].unemployment_rate_skill_5;
      HISTORY_MONTHLY[i].average_wage = HISTORY_MONTHLY[i-1].average_wage;
      HISTORY_MONTHLY[i].no_firms = HISTORY_MONTHLY[i-1].no_firms;
      HISTORY_MONTHLY[i].no_active_firms = HISTORY_MONTHLY[i-1].no_active_firms;
      HISTORY_MONTHLY[i].no_firm_births = HISTORY_MONTHLY[i-1].no_firm_births;
      HISTORY_MONTHLY[i].no_firm_deaths = HISTORY_MONTHLY[i-1].no_firm_deaths;
      HISTORY_MONTHLY[i].investment_value = HISTORY_MONTHLY[i-1].investment_value;
    }
    
    //Store current value of history: [0] gets filled with current value
    HISTORY_MONTHLY[0].cpi = CPI;
    HISTORY_MONTHLY[0].gdp = GDP;                   
    HISTORY_MONTHLY[0].output = MONTHLY_OUTPUT;     
    HISTORY_MONTHLY[0].employment = EMPLOYED;       
    HISTORY_MONTHLY[0].unemployment_rate = UNEMPLOYMENT_RATE;
    HISTORY_MONTHLY[0].unemployment_rate_skill_1 = UNEMPLOYMENT_RATE_SKILL_1;
    HISTORY_MONTHLY[0].unemployment_rate_skill_2 = UNEMPLOYMENT_RATE_SKILL_2;
    HISTORY_MONTHLY[0].unemployment_rate_skill_3 = UNEMPLOYMENT_RATE_SKILL_3;
    HISTORY_MONTHLY[0].unemployment_rate_skill_4 = UNEMPLOYMENT_RATE_SKILL_4;
    HISTORY_MONTHLY[0].unemployment_rate_skill_5 = UNEMPLOYMENT_RATE_SKILL_5;
    HISTORY_MONTHLY[0].average_wage = FIRM_AVERAGE_WAGE;        
    HISTORY_MONTHLY[0].no_firms = NO_FIRMS;         
    HISTORY_MONTHLY[0].no_active_firms = NO_ACTIVE_FIRMS;         
    HISTORY_MONTHLY[0].no_firm_births = NO_FIRM_BIRTHS; 
    HISTORY_MONTHLY[0].no_firm_deaths = NO_FIRM_DEATHS; 
    HISTORY_MONTHLY[0].investment_value = MONTHLY_INVESTMENT_VALUE;

    //*********************************** Code to be tested: region data

    for ( region=0; region<REGION_FIRM_DATA.size; region++)
    {
        //Shift history backwards
        for (i=12; i>0; i--)
        {
            HISTORY_MONTHLY[i].region_data.array[region].cpi = HISTORY_MONTHLY[i-1].region_data.array[region].cpi;
            HISTORY_MONTHLY[i].region_data.array[region].gdp = HISTORY_MONTHLY[i-1].region_data.array[region].gdp;
            HISTORY_MONTHLY[i].region_data.array[region].output = HISTORY_MONTHLY[i-1].region_data.array[region].output;
            HISTORY_MONTHLY[i].region_data.array[region].employment = HISTORY_MONTHLY[i-1].region_data.array[region].employment;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_1 = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate_skill_1;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_2 = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate_skill_2;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_3 = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate_skill_3;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_4 = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate_skill_4;
            HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_5 = HISTORY_MONTHLY[i-1].region_data.array[region].unemployment_rate_skill_5;
            HISTORY_MONTHLY[i].region_data.array[region].average_wage = HISTORY_MONTHLY[i-1].region_data.array[region].average_wage;
            HISTORY_MONTHLY[i].region_data.array[region].no_firms = HISTORY_MONTHLY[i-1].region_data.array[region].no_firms;
			      HISTORY_MONTHLY[i].region_data.array[region].no_active_firms = HISTORY_MONTHLY[i-1].region_data.array[region].no_active_firms;
            HISTORY_MONTHLY[i].region_data.array[region].no_firm_births = HISTORY_MONTHLY[i-1].region_data.array[region].no_firm_births;
            HISTORY_MONTHLY[i].region_data.array[region].no_firm_deaths = HISTORY_MONTHLY[i-1].region_data.array[region].no_firm_deaths;
            HISTORY_MONTHLY[i].region_data.array[region].investment_value = HISTORY_MONTHLY[i-1].region_data.array[region].investment_value;
        }
        
        //Store current value of history
        HISTORY_MONTHLY[0].region_data.array[region].cpi = REGION_FIRM_DATA.array[region].cpi;
        HISTORY_MONTHLY[0].region_data.array[region].gdp = REGION_FIRM_DATA.array[region].gdp;                   
        HISTORY_MONTHLY[0].region_data.array[region].output = REGION_FIRM_DATA.array[region].monthly_output;
        HISTORY_MONTHLY[0].region_data.array[region].employment = REGION_FIRM_DATA.array[region].no_employees;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_1 = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate_skill_1;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_2 = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate_skill_2;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_3 = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate_skill_3;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_4 = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate_skill_4;
        HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_5 = REGION_HOUSEHOLD_DATA.array[region].unemployment_rate_skill_5;
        HISTORY_MONTHLY[0].region_data.array[region].average_wage = REGION_HOUSEHOLD_DATA.array[region].average_wage;
        HISTORY_MONTHLY[0].region_data.array[region].no_firms = REGION_FIRM_DATA.array[region].no_firms;
		    HISTORY_MONTHLY[0].region_data.array[region].no_active_firms = REGION_FIRM_DATA.array[region].no_active_firms;
        HISTORY_MONTHLY[0].region_data.array[region].no_firm_births = REGION_FIRM_DATA.array[region].no_firm_births; 
        HISTORY_MONTHLY[0].region_data.array[region].no_firm_deaths = REGION_FIRM_DATA.array[region].no_firm_deaths;   
        HISTORY_MONTHLY[0].region_data.array[region].investment_value = REGION_FIRM_DATA.array[region].monthly_investment_value;   
        
        if (PRINT_DEBUG_FILES)
        {                       
            filename = malloc(40*sizeof(char));
            filename[0]=0;
            strcpy(filename, "its/eurostat_regional.txt");      
            file1 = fopen(filename,"a");
            fprintf(file1,"\n %d %f %f %f ",DAY,REGION_FIRM_DATA.array[region].gdp,REGION_FIRM_DATA.array[region].cpi,REGION_FIRM_DATA.array[region].monthly_output);
            fprintf(file1,"%d %f ",REGION_FIRM_DATA.array[region].no_employees,REGION_HOUSEHOLD_DATA.array[region].unemployment_rate);
            fprintf(file1," %f %d",REGION_HOUSEHOLD_DATA.array[region].average_wage,REGION_FIRM_DATA.array[region].no_firm_deaths);
            fprintf(file1," %d",region);
            fclose(file1);
            free(filename);
        }
    }

    int sum = 0;
    int diff = 0;

    #ifdef _DEBUG_MODE
    if (PRINT_EUROSTAT_INFO)
    {
        printf("\nMonthly data recorded by Eurostat:\n");
        printf(" - monthly CPI: %f\n", HISTORY_MONTHLY[0].cpi);
        printf(" - monthly GDP: %f\n", HISTORY_MONTHLY[0].gdp);
        printf(" - monthly output: %f\n", HISTORY_MONTHLY[0].output);
        printf(" - monthly average employment: %d\n", HISTORY_MONTHLY[0].employment);
        printf(" - monthly average unemployment rate: %f\n", HISTORY_MONTHLY[0].unemployment_rate);
        printf(" - monthly average unemployment rate_skill_1: %f\n", HISTORY_MONTHLY[0].unemployment_rate_skill_1);
        printf(" - monthly average unemployment rate_skill_2: %f\n", HISTORY_MONTHLY[0].unemployment_rate_skill_2);
        printf(" - monthly average unemployment rate_skill_3: %f\n", HISTORY_MONTHLY[0].unemployment_rate_skill_3);
        printf(" - monthly average unemployment rate_skill_4: %f\n", HISTORY_MONTHLY[0].unemployment_rate_skill_4);
        printf(" - monthly average unemployment rate_skill_5: %f\n", HISTORY_MONTHLY[0].unemployment_rate_skill_5);        
        printf(" - monthly average wage: %f\n", HISTORY_MONTHLY[0].average_wage);
        printf(" - monthly average number of firms: %d\n", HISTORY_MONTHLY[0].no_firms);
        printf(" - monthly average number of active firms: %d\n", HISTORY_MONTHLY[0].no_active_firms); 
        printf(" - monthly total number of firm births: %d\n", HISTORY_MONTHLY[0].no_firm_births);
        printf(" - monthly total number of firm deaths: %d\n", HISTORY_MONTHLY[0].no_firm_deaths);
        printf(" - monthly investment_value: %f\n", HISTORY_MONTHLY[0].investment_value);
        
        printf(" - =========================\n");
        printf(" - monthly BANK MORTGAGE PORTFOLIO:\t%d\n", BANK_NO_MORTGAGES);
        printf(" - \t\tNEW\t%4d\n", BANK_NO_MORTGAGES_NEW);
        printf(" - \t\tPAYING\t\t%4d\n", BANK_MORTGAGES_PAYING);
        printf(" - \t\tARREARS\t\t%4d\n", BANK_MORTGAGES_ARREARS);
        printf(" - \t\tNPL\t\t%4d\n", BANK_MORTGAGES_NPL);
        printf(" - \t------------------\n");
        printf(" - \tTotal:\t\t\t%4d\n", BANK_NO_MORTGAGES_NEW + BANK_MORTGAGES_PAYING + BANK_MORTGAGES_ARREARS + BANK_MORTGAGES_NPL);
        printf(" - \t\tWRITEOFF\t%4d\n", BANK_MORTGAGES_WRITEOFF);
        printf(" - \t\tPAID_OFF\t%4d\n", BANK_MORTGAGES_PAID_OFF);
        printf(" - \t\tREQUESTED\t%4d\n", BANK_MORTGAGES_REQUESTED);
        printf(" - \tTotal+writeoffs:\t%4d\n", BANK_NO_MORTGAGES_NEW + BANK_MORTGAGES_PAYING + BANK_MORTGAGES_ARREARS + BANK_MORTGAGES_NPL + BANK_MORTGAGES_WRITEOFF);
        
        printf(" - monthly HH MORTGAGE PORTFOLIO:\t%4d", HH_NO_MORTGAGES);

        diff = BANK_NO_MORTGAGES - HH_NO_MORTGAGES;
        if (diff)
            printf(" <-- diff %4d", diff);
        printf("\n");

        printf(" - \tNEWLY ISSUED/ACCEPTED\t%4d\n", HH_MORTGAGES_ACCEPTED);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_ACCEPTED);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_ACCEPTED);

        printf(" - \tPAYING\t\t\t%4d\n", HH_MORTGAGES_PAYING);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_PAYING);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_PAYING);

        printf(" - \tARREARS\t\t\t%4d\n", HH_MORTGAGES_ARREARS);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_ARREARS);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_ARREARS);
        
        printf(" - \tNPL\t\t\t%4d\n", HH_MORTGAGES_NPL);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_NPL);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_NPL);
        printf(" - \t------------------\n");
        
        sum = HH_MORTGAGES_ACCEPTED + HH_MORTGAGES_PAYING + HH_MORTGAGES_ARREARS + HH_MORTGAGES_NPL;
        printf(" - \tTotal:\t\t\t%4d\n", sum);
        printf(" - \tWRITEOFF\t\t%4d\n", HH_MORTGAGES_WRITEOFF);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_WRITEOFF);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_WRITEOFF);
        printf(" - \tTotal + writeoffs:\t\t%4d\n", sum + HH_MORTGAGES_WRITEOFF);

        printf(" - \tPAIDOFF\t\t\t%4d\n", HH_MORTGAGES_PAIDOFF);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_PAIDOFF);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_PAIDOFF);
        printf(" - \tTotal -- paidoffs:\t\t%4d\n", sum - HH_MORTGAGES_PAIDOFF);
        
        diff = HH_NO_MORTGAGES - (sum - HH_MORTGAGES_PAIDOFF);

        printf(" - Diff HH total / sum - paidoff:\t%4d", diff);
        if (diff == 0)
            printf("\tOK HH MORTGAGES ARE CONSISTENT");
        else
            printf("\tFAIL: HH MORTGAGES ARE INCONSISTENT WITH TYPES COUNTED");
        printf("\n");

        printf(" - \tREQUESTED\t\t%4d\n", HH_MORTGAGES_REQUESTED);
        printf(" - \t\tof which HMR\t%4d\n", HH_MORTGAGES_HMR_REQUESTED);
        printf(" - \t\tof which OP\t%4d\n", HH_MORTGAGES_OP_REQUESTED);

        printf(" - Diff Bank / HH portfolio:\t\t%4d", diff);
        if (diff == 0)
            printf("\tOK BANK MORTGAGES ARE CONSISTENT");
        else
            printf("\tFAIL: BANK MORTGAGES ARE INCONSISTENT WITH HH MORTGAGE PORTFOLIOS");

        printf("\n");
        printf(" - =========================\n");

        printf(" - monthly BANK NO. FIRM LOANS:\t%4d\n", BANK_NO_FIRM_LOANS);
        printf(" - \t\tNEW\t\t%4d\n", BANK_NO_FIRM_LOANS_NEW);
        printf(" - \t\tPAYING\t\t%4d\n", BANK_NO_FIRM_LOANS_PAYING);
        printf(" - \t\tWRITEOFF\t%4d\n", BANK_NO_FIRM_LOANS_WRITEOFF);

        sum = BANK_NO_FIRM_LOANS_NEW + BANK_NO_FIRM_LOANS_PAYING + BANK_NO_FIRM_LOANS_WRITEOFF;
        printf(" - \t------------------\n");
        printf(" - \tTotal:\t\t\t%4d\n", sum);

        printf(" - monthly FIRM NO. FIRM LOANS:\t%4d\n", FIRM_NO_FIRM_LOANS);
        printf(" - \t\tNEW\t\t%4d\n", FIRM_NO_FIRM_LOANS_NEW);
        if (BANK_NO_FIRM_LOANS_NEW != FIRM_NO_FIRM_LOANS_NEW) printf("\tFAIL: FIRM NEW LOANS ARE INCONSISTENT");
        
        printf(" - \t\tPAYING\t\t%4d\n", FIRM_NO_FIRM_LOANS_PAYING);
        if (BANK_NO_FIRM_LOANS_PAYING != FIRM_NO_FIRM_LOANS_PAYING) printf("\tFAIL: FIRM PAYING LOANS ARE INCONSISTENT");
        
        printf(" - \t\tWRITE DOWN\t%4d\n", FIRM_NO_FIRM_LOANS_WRITEOFF);
        if (BANK_NO_FIRM_LOANS_WRITEOFF != FIRM_NO_FIRM_LOANS_WRITEOFF) printf("\tFAIL: FIRM WRITEOFF LOANS ARE INCONSISTENT");
        
        sum = FIRM_NO_FIRM_LOANS_NEW + FIRM_NO_FIRM_LOANS_PAYING;
        printf(" - \t------------------\n");
        printf(" - \tTotal:\t\t\t%4d\n", sum);
        printf(" - \t\tPAID_OFF\t%4d\n", FIRM_NO_FIRM_LOANS_PAID_OFF);
        printf(" - \tTotal -- paidoff:\t%4d\n", sum - FIRM_NO_FIRM_LOANS_PAID_OFF);

        diff = FIRM_NO_FIRM_LOANS - (sum - FIRM_NO_FIRM_LOANS_PAID_OFF);
        printf(" - Diff Firm total / sum:\t\t%4d", diff);
        if (diff == 0)
            printf("\tOK FIRM LOANS ARE CONSISTENT");
        else
        {
            printf("\tFAIL: FIRM LOANS ARE INCONSISTENT");

            int diff_per_class = (BANK_NO_FIRM_LOANS_NEW != FIRM_NO_FIRM_LOANS_NEW) + (BANK_NO_FIRM_LOANS_PAYING != FIRM_NO_FIRM_LOANS_PAYING) + (BANK_NO_FIRM_LOANS_WRITEOFF != FIRM_NO_FIRM_LOANS_WRITEOFF);

            if(diff_per_class == 0 )
                printf("\tBUT: PER TYPE IT IS CONSISTENT");  
            else
                printf("\tAND: PER TYPE IT IS ALSO INCONSISTENT");            
        }
        printf("\n");

        diff = BANK_NO_FIRM_LOANS_NEW + FIRM_NO_FIRM_LOANS_PAYING + BANK_NO_FIRM_LOANS_WRITEOFF - (FIRM_NO_FIRM_LOANS_NEW + FIRM_NO_FIRM_LOANS_PAYING + FIRM_NO_FIRM_LOANS_WRITEOFF);
        printf(" - Diff Bank / Firm counter:\t\t%4d", diff);
        if (diff == 0)
            printf("\tOK BANK FIRM LOANS ARE CONSISTENT");
        else
          printf("\tFAIL: BANK FIRM LOANS ARE INCONSISTENT WITH FIRM LOAN PORTFOLIOS");           

        printf("\n");
        printf(" - =========================\n");

    }    
    #endif

   // #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_FILES)
    {                       
        filename = malloc(40*sizeof(char));
        filename[0]=0;
        strcpy(filename, "its/eurostat.txt");      
        file1 = fopen(filename,"a");
        fprintf(file1,"\n %d %f %f %f ",DAY,HISTORY_MONTHLY[0].gdp,HISTORY_MONTHLY[0].cpi,HISTORY_MONTHLY[0].output);
        fprintf(file1,"%d %f ",HISTORY_MONTHLY[0].employment,HISTORY_MONTHLY[0].unemployment_rate);
        fprintf(file1," %f %d",HISTORY_MONTHLY[0].average_wage,HISTORY_MONTHLY[0].no_firm_deaths);
        fclose(file1);
        free(filename);
    }
   // #endif
        
    return 0;
}

/******************************* STORING QUARTERLY DATA **************************************/
/* \fn: int Eurostat_store_history_quarterly()
 * \brief: Central_Eurostat agent stores statistics to quarterly history structure. 
 */
int Eurostat_store_history_quarterly()
{
    int i, region;
    region=0;
    
    //*********************************** Economy-wide data
    //Shift history backwards
    for (i=4; i>0; i--)
    {
      //t-i-1 gets filled with value from t-i
      HISTORY_QUARTERLY[i].cpi = HISTORY_QUARTERLY[i-1].cpi;
      HISTORY_QUARTERLY[i].gdp = HISTORY_QUARTERLY[i-1].gdp;
      HISTORY_QUARTERLY[i].output = HISTORY_QUARTERLY[i-1].output;
      HISTORY_QUARTERLY[i].employment = HISTORY_QUARTERLY[i-1].employment;      
      HISTORY_QUARTERLY[i].unemployment_rate = HISTORY_QUARTERLY[i-1].unemployment_rate;
      HISTORY_QUARTERLY[i].unemployment_rate_skill_1 = HISTORY_QUARTERLY[i-1].unemployment_rate_skill_1;
      HISTORY_QUARTERLY[i].unemployment_rate_skill_2 = HISTORY_QUARTERLY[i-1].unemployment_rate_skill_2;
      HISTORY_QUARTERLY[i].unemployment_rate_skill_3 = HISTORY_QUARTERLY[i-1].unemployment_rate_skill_3;
      HISTORY_QUARTERLY[i].unemployment_rate_skill_4 = HISTORY_QUARTERLY[i-1].unemployment_rate_skill_4;
      HISTORY_QUARTERLY[i].unemployment_rate_skill_5 = HISTORY_QUARTERLY[i-1].unemployment_rate_skill_5;
      HISTORY_QUARTERLY[i].average_wage = HISTORY_QUARTERLY[i-1].average_wage;
      HISTORY_QUARTERLY[i].no_firms = HISTORY_QUARTERLY[i-1].no_firms;
      HISTORY_QUARTERLY[i].no_active_firms = HISTORY_QUARTERLY[i-1].no_active_firms;
      HISTORY_QUARTERLY[i].no_firm_births = HISTORY_QUARTERLY[i-1].no_firm_births;
      HISTORY_QUARTERLY[i].no_firm_deaths = HISTORY_QUARTERLY[i-1].no_firm_deaths;
      HISTORY_QUARTERLY[i].investment_value = HISTORY_QUARTERLY[i-1].investment_value;
    }
        
    //Reset first elements for sum
    HISTORY_QUARTERLY[0].cpi=1.0;
    HISTORY_QUARTERLY[0].gdp=0.0;
    HISTORY_QUARTERLY[0].output=0.0;
    HISTORY_QUARTERLY[0].employment=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_1=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_2=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_3=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_4=0.0;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_5=0.0;    
    HISTORY_QUARTERLY[0].average_wage=0.0;
    HISTORY_QUARTERLY[0].no_firms=0;
    HISTORY_QUARTERLY[0].no_active_firms=0;
    HISTORY_QUARTERLY[0].no_firm_births=0;
    HISTORY_QUARTERLY[0].no_firm_deaths=0;
    HISTORY_QUARTERLY[0].investment_value=0.0;

    //Store first value: construct quarterly sums from monthly history
    for (i=0; i<3; i++)
    {
        HISTORY_QUARTERLY[0].cpi                *= HISTORY_MONTHLY[i].cpi;
        HISTORY_QUARTERLY[0].gdp                += HISTORY_MONTHLY[i].gdp;
        HISTORY_QUARTERLY[0].output             += HISTORY_MONTHLY[i].output;
        HISTORY_QUARTERLY[0].employment         += HISTORY_MONTHLY[i].employment;        
        HISTORY_QUARTERLY[0].unemployment_rate  += HISTORY_MONTHLY[i].unemployment_rate;
        HISTORY_QUARTERLY[0].unemployment_rate_skill_1  += HISTORY_MONTHLY[i].unemployment_rate_skill_1;
        HISTORY_QUARTERLY[0].unemployment_rate_skill_2  += HISTORY_MONTHLY[i].unemployment_rate_skill_2;
        HISTORY_QUARTERLY[0].unemployment_rate_skill_3  += HISTORY_MONTHLY[i].unemployment_rate_skill_3;
        HISTORY_QUARTERLY[0].unemployment_rate_skill_4  += HISTORY_MONTHLY[i].unemployment_rate_skill_4;
        HISTORY_QUARTERLY[0].unemployment_rate_skill_5  += HISTORY_MONTHLY[i].unemployment_rate_skill_5;
        HISTORY_QUARTERLY[0].average_wage       += HISTORY_MONTHLY[i].average_wage;
        HISTORY_QUARTERLY[0].no_firms           += HISTORY_MONTHLY[i].no_firms;
		    HISTORY_QUARTERLY[0].no_active_firms    += HISTORY_MONTHLY[i].no_active_firms;
        HISTORY_QUARTERLY[0].no_firm_births     += HISTORY_MONTHLY[i].no_firm_births;
        HISTORY_QUARTERLY[0].no_firm_deaths     += HISTORY_MONTHLY[i].no_firm_deaths;
        HISTORY_QUARTERLY[0].investment_value   += HISTORY_MONTHLY[i].investment_value;
    }
    //The following quarterly statistics are averages of monthly statistics
    //HISTORY_QUARTERLY[0].cpi                    = HISTORY_QUARTERLY[0].cpi/3;
    HISTORY_QUARTERLY[0].gdp                    = HISTORY_QUARTERLY[0].gdp;
    HISTORY_QUARTERLY[0].output                 = HISTORY_QUARTERLY[0].output;
    HISTORY_QUARTERLY[0].employment             = HISTORY_QUARTERLY[0].employment/3;
    HISTORY_QUARTERLY[0].unemployment_rate      = HISTORY_QUARTERLY[0].unemployment_rate/3;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_1 = HISTORY_QUARTERLY[0].unemployment_rate_skill_1/3;    
    HISTORY_QUARTERLY[0].unemployment_rate_skill_2 = HISTORY_QUARTERLY[0].unemployment_rate_skill_2/3;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_3 = HISTORY_QUARTERLY[0].unemployment_rate_skill_3/3;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_4 = HISTORY_QUARTERLY[0].unemployment_rate_skill_4/3;
    HISTORY_QUARTERLY[0].unemployment_rate_skill_5 = HISTORY_QUARTERLY[0].unemployment_rate_skill_5/3;
    HISTORY_QUARTERLY[0].average_wage           = HISTORY_QUARTERLY[0].average_wage/3;
    HISTORY_QUARTERLY[0].no_firms               = HISTORY_QUARTERLY[0].no_firms/3;
    HISTORY_QUARTERLY[0].no_active_firms        = HISTORY_QUARTERLY[0].no_active_firms/3;
    HISTORY_QUARTERLY[0].no_firm_births         = HISTORY_QUARTERLY[0].no_firm_births;
    HISTORY_QUARTERLY[0].no_firm_deaths         = HISTORY_QUARTERLY[0].no_firm_deaths;
    HISTORY_QUARTERLY[0].investment_value       = HISTORY_QUARTERLY[0].investment_value;
    
    //*********************************** Code to be tested: region data

    for ( region=0; region<REGION_FIRM_DATA.size; region++)
    {
        //Shift history backwards
        for (i=4; i>0; i--)
        {
            HISTORY_QUARTERLY[i].region_data.array[region].cpi = HISTORY_QUARTERLY[i-1].region_data.array[region].cpi;
            HISTORY_QUARTERLY[i].region_data.array[region].gdp = HISTORY_QUARTERLY[i-1].region_data.array[region].gdp;
            HISTORY_QUARTERLY[i].region_data.array[region].output = HISTORY_QUARTERLY[i-1].region_data.array[region].output;
            HISTORY_QUARTERLY[i].region_data.array[region].employment = HISTORY_QUARTERLY[i-1].region_data.array[region].employment;      
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate;
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate_skill_1 = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate_skill_1;
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate_skill_2 = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate_skill_2;
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate_skill_3 = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate_skill_3;
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate_skill_4 = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate_skill_4;
            HISTORY_QUARTERLY[i].region_data.array[region].unemployment_rate_skill_5 = HISTORY_QUARTERLY[i-1].region_data.array[region].unemployment_rate_skill_5;
            HISTORY_QUARTERLY[i].region_data.array[region].average_wage = HISTORY_QUARTERLY[i-1].region_data.array[region].average_wage;
            HISTORY_QUARTERLY[i].region_data.array[region].no_firms = HISTORY_QUARTERLY[i-1].region_data.array[region].no_firms;
			      HISTORY_QUARTERLY[i].region_data.array[region].no_active_firms = HISTORY_QUARTERLY[i-1].region_data.array[region].no_active_firms;
            HISTORY_QUARTERLY[i].region_data.array[region].no_firm_births = HISTORY_QUARTERLY[i-1].region_data.array[region].no_firm_births;
            HISTORY_QUARTERLY[i].region_data.array[region].no_firm_deaths = HISTORY_QUARTERLY[i-1].region_data.array[region].no_firm_deaths;
            HISTORY_QUARTERLY[i].region_data.array[region].investment_value = HISTORY_QUARTERLY[i-1].region_data.array[region].investment_value;
        }

        //Reset first elements for sum
        HISTORY_QUARTERLY[0].region_data.array[region].cpi=1.0;
        HISTORY_QUARTERLY[0].region_data.array[region].gdp=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].output=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].employment=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5=0.0;    
        HISTORY_QUARTERLY[0].region_data.array[region].average_wage=0.0;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firms=0;
		    HISTORY_QUARTERLY[0].region_data.array[region].no_active_firms=0;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births=0;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths=0;
        HISTORY_QUARTERLY[0].region_data.array[region].investment_value=0.0;

        //Store first value: construct quarterly sums from monthly history
        for (i=0; i<3; i++)
        {
            HISTORY_QUARTERLY[0].region_data.array[region].cpi                *= HISTORY_MONTHLY[i].region_data.array[region].cpi;
            HISTORY_QUARTERLY[0].region_data.array[region].gdp                += HISTORY_MONTHLY[i].region_data.array[region].gdp;
            HISTORY_QUARTERLY[0].region_data.array[region].output             += HISTORY_MONTHLY[i].region_data.array[region].output;
            HISTORY_QUARTERLY[0].region_data.array[region].employment         += HISTORY_MONTHLY[i].region_data.array[region].employment;        
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate;
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_1;
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_2;
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_3;
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_4;
            HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5  += HISTORY_MONTHLY[i].region_data.array[region].unemployment_rate_skill_5;
            HISTORY_QUARTERLY[0].region_data.array[region].average_wage       += HISTORY_MONTHLY[i].region_data.array[region].average_wage;
            HISTORY_QUARTERLY[0].region_data.array[region].no_firms           += HISTORY_MONTHLY[i].region_data.array[region].no_firms;
            HISTORY_QUARTERLY[0].region_data.array[region].no_active_firms    += HISTORY_MONTHLY[i].region_data.array[region].no_active_firms;
            HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births     += HISTORY_MONTHLY[i].region_data.array[region].no_firm_births;
            HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths     += HISTORY_MONTHLY[i].region_data.array[region].no_firm_deaths;           
            HISTORY_QUARTERLY[0].region_data.array[region].investment_value   += HISTORY_MONTHLY[i].region_data.array[region].investment_value;           
        }
        
        //The following quarterly statistics are averages of monthly statistics
        //HISTORY_QUARTERLY[0].region_data.array[region].cpi                    = HISTORY_QUARTERLY[0].region_data.array[region].cpi/3;
        HISTORY_QUARTERLY[0].region_data.array[region].gdp                    = HISTORY_QUARTERLY[0].region_data.array[region].gdp;
        HISTORY_QUARTERLY[0].region_data.array[region].output                 = HISTORY_QUARTERLY[0].region_data.array[region].output;        
        HISTORY_QUARTERLY[0].region_data.array[region].employment             = HISTORY_QUARTERLY[0].region_data.array[region].employment/3;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate      = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate/3;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1 = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1/3;    
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2 = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2/3;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3 = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3/3;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4 = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4/3;
        HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5 = HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5/3;
        HISTORY_QUARTERLY[0].region_data.array[region].average_wage           = HISTORY_QUARTERLY[0].region_data.array[region].average_wage/3;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firms               = HISTORY_QUARTERLY[0].region_data.array[region].no_firms/3;
		    HISTORY_QUARTERLY[0].region_data.array[region].no_active_firms        = HISTORY_QUARTERLY[0].region_data.array[region].no_active_firms/3;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births         = HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births;
        HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths         = HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths;    
        HISTORY_QUARTERLY[0].region_data.array[region].investment_value       = HISTORY_QUARTERLY[0].region_data.array[region].investment_value;    
    }

    #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_EUROSTAT)
    {
        printf("Quarterly data recorded by Eurostat:\n");
        printf(" - quarterly CPI: %f\n", HISTORY_QUARTERLY[0].cpi);
        printf(" - quarterly GDP: %f\n", HISTORY_QUARTERLY[0].gdp);
        printf(" - quarterly output: %f\n", HISTORY_QUARTERLY[0].output);
        printf(" - quarterly average employment: %d\n", HISTORY_QUARTERLY[0].employment);
        printf(" - quarterly average unemployment rate: %f\n", HISTORY_QUARTERLY[0].unemployment_rate);
        printf(" - quarterly average wage: %f\n", HISTORY_QUARTERLY[0].average_wage);
        printf(" - quarterly average number of firms: %d\n", HISTORY_QUARTERLY[0].no_firms);
        printf(" - quarterly average number of active firms: %d\n", HISTORY_QUARTERLY[0].no_active_firms);
        printf(" - quarterly total number of firm births: %d\n", HISTORY_QUARTERLY[0].no_firm_births);
        printf(" - quarterly total number of firm deaths: %d\n", HISTORY_QUARTERLY[0].no_firm_deaths);
        printf(" - quarterly total investment value: %f\n", HISTORY_QUARTERLY[0].investment_value);
    }
    #endif
        
    return 0;
}


/************ Computation ********************************/
/* \fn: int Eurostat_compute_growth_rates_monthly()
 * \brief: Computes rates of change from the current history at the end of every month.
 */
int Eurostat_compute_growth_rates_monthly()
{
    int region = 0;

    //*********************************** Economy-wide data
    MONTHLY_GROWTH_RATES.cpi                       = 1.0;
    MONTHLY_GROWTH_RATES.gdp                       = 0.0; 
    MONTHLY_GROWTH_RATES.output                    = 0.0;
    MONTHLY_GROWTH_RATES.employment                = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate         = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate_skill_1         = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate_skill_2         = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate_skill_3         = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate_skill_4         = 0.0;
    MONTHLY_GROWTH_RATES.unemployment_rate_skill_5         = 0.0;
    MONTHLY_GROWTH_RATES.average_wage                      = 0.0;
    MONTHLY_GROWTH_RATES.no_firms                  = 0.0;
    MONTHLY_GROWTH_RATES.no_firm_births            = 0.0;
    MONTHLY_GROWTH_RATES.no_firm_deaths            = 0.0;
    MONTHLY_GROWTH_RATES.investment_value          = 0.0;
    
    ANNUAL_GROWTH_RATES_MONTHLY.cpi                = 1.0;           //CPI: compounded inflation rate in pct over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.gdp                = 0.0;           //GDP: percentage change in GDP over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.output             = 0.0;           //percentage change in output over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.employment         = 0.0;           //percentage change in total employment over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate  = 0.0;           //percentage change in unemployment_rate over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_1  = 0.0;
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_2  = 0.0;
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_3  = 0.0;
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_4  = 0.0;
    ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_5  = 0.0;
    ANNUAL_GROWTH_RATES_MONTHLY.average_wage               = 0.0;   //percentage change in average_wage over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.no_firms           = 0.0;           //percentage change in no_firms over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.no_firm_births     = 0.0;           //percentage change in no_firm_births over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.no_firm_deaths     = 0.0;           //percentage change in no_firm_deaths over last 12 months
    ANNUAL_GROWTH_RATES_MONTHLY.investment_value   = 0.0;
    
    //compute monthly rates of change: change over the previous month
	  if(HISTORY_MONTHLY[1].cpi>0.0)              {MONTHLY_GROWTH_RATES.cpi                       = (HISTORY_MONTHLY[0].cpi/HISTORY_MONTHLY[1].cpi -1)*100;}
    if(HISTORY_MONTHLY[1].gdp>0.0)              {MONTHLY_GROWTH_RATES.gdp                       = (HISTORY_MONTHLY[0].gdp / HISTORY_MONTHLY[1].gdp -1)*100;}
    if(HISTORY_MONTHLY[1].output>0.0)           {MONTHLY_GROWTH_RATES.output                    = (HISTORY_MONTHLY[0].output / HISTORY_MONTHLY[1].output  -1)*100;}
    if(HISTORY_MONTHLY[1].employment>0)         {MONTHLY_GROWTH_RATES.employment                = (HISTORY_MONTHLY[0].employment / HISTORY_MONTHLY[1].employment  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate>0.0){MONTHLY_GROWTH_RATES.unemployment_rate         = (HISTORY_MONTHLY[0].unemployment_rate / HISTORY_MONTHLY[1].unemployment_rate  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate_skill_1>0.0){MONTHLY_GROWTH_RATES.unemployment_rate_skill_1         = (HISTORY_MONTHLY[0].unemployment_rate_skill_1 / HISTORY_MONTHLY[1].unemployment_rate_skill_1  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate_skill_2>0.0){MONTHLY_GROWTH_RATES.unemployment_rate_skill_2         = (HISTORY_MONTHLY[0].unemployment_rate_skill_2 / HISTORY_MONTHLY[1].unemployment_rate_skill_2  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate_skill_3>0.0){MONTHLY_GROWTH_RATES.unemployment_rate_skill_3         = (HISTORY_MONTHLY[0].unemployment_rate_skill_3 / HISTORY_MONTHLY[1].unemployment_rate_skill_3  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate_skill_4>0.0){MONTHLY_GROWTH_RATES.unemployment_rate_skill_4         = (HISTORY_MONTHLY[0].unemployment_rate_skill_4 / HISTORY_MONTHLY[1].unemployment_rate_skill_4  -1)*100;}
    if(HISTORY_MONTHLY[1].unemployment_rate_skill_5>0.0){MONTHLY_GROWTH_RATES.unemployment_rate_skill_5         = (HISTORY_MONTHLY[0].unemployment_rate_skill_5 / HISTORY_MONTHLY[1].unemployment_rate_skill_5  -1)*100;}
    if(HISTORY_MONTHLY[1].average_wage>0.0)     {MONTHLY_GROWTH_RATES.average_wage              = (HISTORY_MONTHLY[0].average_wage / HISTORY_MONTHLY[1].average_wage  -1)*100;}
    if(HISTORY_MONTHLY[1].no_firms>0)           {MONTHLY_GROWTH_RATES.no_firms                  = (HISTORY_MONTHLY[0].no_firms / HISTORY_MONTHLY[1].no_firms  -1)*100;}
    if(HISTORY_MONTHLY[1].no_firm_births>0)     {MONTHLY_GROWTH_RATES.no_firm_births            = (HISTORY_MONTHLY[0].no_firm_births / HISTORY_MONTHLY[1].no_firm_births  -1)*100;}
    if(HISTORY_MONTHLY[1].no_firm_deaths>0)     {MONTHLY_GROWTH_RATES.no_firm_deaths            = (HISTORY_MONTHLY[0].no_firm_deaths / HISTORY_MONTHLY[1].no_firm_deaths  -1)*100;}   
    if(HISTORY_MONTHLY[1].investment_value>0)     {MONTHLY_GROWTH_RATES.investment_value        = (HISTORY_MONTHLY[0].investment_value / HISTORY_MONTHLY[1].investment_value  -1)*100;}   

    //compute annual rates of change over the previous 12 months: respective to same month in previous year
	  if(HISTORY_MONTHLY[12].cpi>0.0)             {ANNUAL_GROWTH_RATES_MONTHLY.cpi                = (HISTORY_MONTHLY[0].cpi / HISTORY_MONTHLY[12].cpi  -1)*100;}
	  if(HISTORY_MONTHLY[12].gdp>0.0)             {ANNUAL_GROWTH_RATES_MONTHLY.gdp                = (HISTORY_MONTHLY[0].gdp / HISTORY_MONTHLY[12].gdp  -1)*100;}
    if(HISTORY_MONTHLY[12].output>0.0)          {ANNUAL_GROWTH_RATES_MONTHLY.output             = (HISTORY_MONTHLY[0].output / HISTORY_MONTHLY[12].output  -1)*100;}
    if(HISTORY_MONTHLY[12].employment>0)        {ANNUAL_GROWTH_RATES_MONTHLY.employment         = (HISTORY_MONTHLY[0].employment / HISTORY_MONTHLY[12].employment  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate  = (HISTORY_MONTHLY[0].unemployment_rate / HISTORY_MONTHLY[12].unemployment_rate  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate_skill_1>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_1  = (HISTORY_MONTHLY[0].unemployment_rate_skill_1 / HISTORY_MONTHLY[12].unemployment_rate_skill_1  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate_skill_2>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_2  = (HISTORY_MONTHLY[0].unemployment_rate_skill_2 / HISTORY_MONTHLY[12].unemployment_rate_skill_2  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate_skill_3>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_3  = (HISTORY_MONTHLY[0].unemployment_rate_skill_3 / HISTORY_MONTHLY[12].unemployment_rate_skill_3  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate_skill_4>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_4  = (HISTORY_MONTHLY[0].unemployment_rate_skill_4 / HISTORY_MONTHLY[12].unemployment_rate_skill_4  -1)*100;}
    if(HISTORY_MONTHLY[12].unemployment_rate_skill_5>0) {ANNUAL_GROWTH_RATES_MONTHLY.unemployment_rate_skill_5  = (HISTORY_MONTHLY[0].unemployment_rate_skill_5 / HISTORY_MONTHLY[12].unemployment_rate_skill_5  -1)*100;}
    if(HISTORY_MONTHLY[12].average_wage>0)      {ANNUAL_GROWTH_RATES_MONTHLY.average_wage       = (HISTORY_MONTHLY[0].average_wage / HISTORY_MONTHLY[12].average_wage  -1)*100;}
    if(HISTORY_MONTHLY[12].no_firms>0)          {ANNUAL_GROWTH_RATES_MONTHLY.no_firms           = (HISTORY_MONTHLY[0].no_firms / HISTORY_MONTHLY[12].no_firms  -1)*100;}
    if(HISTORY_MONTHLY[12].no_firm_births>0)    {ANNUAL_GROWTH_RATES_MONTHLY.no_firm_births     = (HISTORY_MONTHLY[0].no_firm_births / HISTORY_MONTHLY[12].no_firm_births  -1)*100;}
    if(HISTORY_MONTHLY[12].no_firm_deaths>0)    {ANNUAL_GROWTH_RATES_MONTHLY.no_firm_deaths     = (HISTORY_MONTHLY[0].no_firm_deaths / HISTORY_MONTHLY[12].no_firm_deaths  -1)*100;}
    if(HISTORY_MONTHLY[12].investment_value>0)    {ANNUAL_GROWTH_RATES_MONTHLY.investment_value     = (HISTORY_MONTHLY[0].investment_value / HISTORY_MONTHLY[12].investment_value  -1)*100;}

    //19.5.17: Set annual inflation rate based on pct change in the CPI of current month wrt.the same month in the previous year
    ANNUAL_INFLATION_RATE_MONTHLY = ANNUAL_GROWTH_RATES_MONTHLY.cpi;
	
    //*********************************** Code to be tested: region data
    
    for ( region=0; region<REGION_FIRM_DATA.size; region++)
    {
        MONTHLY_GROWTH_RATES.region_data.array[region].cpi                       = 1.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].gdp                       = 0.0; 
        MONTHLY_GROWTH_RATES.region_data.array[region].output                    = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].employment                = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate         = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_1 = 0.0;        
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_2 = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_3 = 0.0;        
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_4 = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_5 = 0.0;        
        MONTHLY_GROWTH_RATES.region_data.array[region].average_wage              = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].no_firms                  = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].no_firm_births            = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].no_firm_deaths            = 0.0;
        MONTHLY_GROWTH_RATES.region_data.array[region].investment_value          = 0.0;
        
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].cpi                = 1.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].gdp                = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].output             = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].employment         = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_1  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_2  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_3  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_4  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_5  = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].average_wage       = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firms           = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firm_births     = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firm_deaths     = 0.0;
        ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].investment_value   = 0.0;
        
        //compute monthly rates of change: change over the previous month
        if(HISTORY_MONTHLY[1].region_data.array[region].cpi>0.0)              {MONTHLY_GROWTH_RATES.region_data.array[region].cpi                       = (HISTORY_MONTHLY[0].region_data.array[region].cpi-1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].gdp>0.0)              {MONTHLY_GROWTH_RATES.region_data.array[region].gdp                       = (HISTORY_MONTHLY[0].region_data.array[region].gdp / HISTORY_MONTHLY[1].region_data.array[region].gdp -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].output>0.0)           {MONTHLY_GROWTH_RATES.region_data.array[region].output                    = (HISTORY_MONTHLY[0].region_data.array[region].output / HISTORY_MONTHLY[1].region_data.array[region].output  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].employment>0)         {MONTHLY_GROWTH_RATES.region_data.array[region].employment                = (HISTORY_MONTHLY[0].region_data.array[region].employment / HISTORY_MONTHLY[1].region_data.array[region].employment  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate         = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_1>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_1 = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_1 / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_1  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_2>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_2 = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_2 / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_2  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_3>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_3 = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_3 / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_3  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_4>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_4 = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_4 / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_4  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_5>0.0){MONTHLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_5 = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_5 / HISTORY_MONTHLY[1].region_data.array[region].unemployment_rate_skill_5  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].average_wage>0.0)     {MONTHLY_GROWTH_RATES.region_data.array[region].average_wage              = (HISTORY_MONTHLY[0].region_data.array[region].average_wage / HISTORY_MONTHLY[1].region_data.array[region].average_wage  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].no_firms>0)           {MONTHLY_GROWTH_RATES.region_data.array[region].no_firms                  = (HISTORY_MONTHLY[0].region_data.array[region].no_firms / HISTORY_MONTHLY[1].region_data.array[region].no_firms  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].no_firm_births>0)     {MONTHLY_GROWTH_RATES.region_data.array[region].no_firm_births            = (HISTORY_MONTHLY[0].region_data.array[region].no_firm_births / HISTORY_MONTHLY[1].region_data.array[region].no_firm_births  -1)*100;}
        if(HISTORY_MONTHLY[1].region_data.array[region].no_firm_deaths>0)     {MONTHLY_GROWTH_RATES.region_data.array[region].no_firm_deaths            = (HISTORY_MONTHLY[0].region_data.array[region].no_firm_deaths / HISTORY_MONTHLY[1].region_data.array[region].no_firm_deaths  -1)*100;}   
        if(HISTORY_MONTHLY[1].region_data.array[region].investment_value>0)     {MONTHLY_GROWTH_RATES.region_data.array[region].investment_value        = (HISTORY_MONTHLY[0].region_data.array[region].investment_value / HISTORY_MONTHLY[1].region_data.array[region].investment_value  -1)*100;}   
        

        //compute annual rates of change over the previous 12 months: respective to same month in previous year
        if(HISTORY_MONTHLY[12].region_data.array[region].cpi>0.0)             {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].cpi                = (HISTORY_MONTHLY[0].region_data.array[region].cpi / HISTORY_MONTHLY[12].region_data.array[region].cpi  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].gdp>0.0)             {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].gdp                = (HISTORY_MONTHLY[0].region_data.array[region].gdp / HISTORY_MONTHLY[12].region_data.array[region].gdp  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].output>0.0)          {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].output             = (HISTORY_MONTHLY[0].region_data.array[region].output / HISTORY_MONTHLY[12].region_data.array[region].output  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].employment>0)        {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].employment         = (HISTORY_MONTHLY[0].region_data.array[region].employment / HISTORY_MONTHLY[12].region_data.array[region].employment  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_1>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_1  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_1 / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_1  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_2>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_2  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_2 / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_2  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_3>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_3  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_3 / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_3  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_4>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_4  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_4 / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_4  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_5>0) {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].unemployment_rate_skill_5  = (HISTORY_MONTHLY[0].region_data.array[region].unemployment_rate_skill_5 / HISTORY_MONTHLY[12].region_data.array[region].unemployment_rate_skill_5  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].average_wage>0)      {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].average_wage       = (HISTORY_MONTHLY[0].region_data.array[region].average_wage / HISTORY_MONTHLY[12].region_data.array[region].average_wage  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].no_firms>0)          {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firms           = (HISTORY_MONTHLY[0].region_data.array[region].no_firms / HISTORY_MONTHLY[12].region_data.array[region].no_firms  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].no_firm_births>0)    {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firm_births     = (HISTORY_MONTHLY[0].region_data.array[region].no_firm_births / HISTORY_MONTHLY[12].region_data.array[region].no_firm_births  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].no_firm_deaths>0)    {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].no_firm_deaths     = (HISTORY_MONTHLY[0].region_data.array[region].no_firm_deaths / HISTORY_MONTHLY[12].region_data.array[region].no_firm_deaths  -1)*100;}
        if(HISTORY_MONTHLY[12].region_data.array[region].investment_value>0)    {ANNUAL_GROWTH_RATES_MONTHLY.region_data.array[region].investment_value = (HISTORY_MONTHLY[0].region_data.array[region].investment_value / HISTORY_MONTHLY[12].region_data.array[region].investment_value  -1)*100;}
      }

    
    return 0;
}

/* \fn: int Eurostat_compute_growth_rates_quarterly()
 * \brief: Computes rates of change from the current history at the end of every quarterly.
 */
int Eurostat_compute_growth_rates_quarterly()
{
    int region = 0;
    
    //*********************************** Economy-wide data
    QUARTERLY_GROWTH_RATES.cpi                         = 1.0;
    QUARTERLY_GROWTH_RATES.gdp                         = 0.0;
    QUARTERLY_GROWTH_RATES.output                      = 0.0;
    QUARTERLY_GROWTH_RATES.employment                  = 0.0;
    QUARTERLY_GROWTH_RATES.unemployment_rate           = 0.0;
    QUARTERLY_GROWTH_RATES.unemployment_rate_skill_1   = 0.0;    
    QUARTERLY_GROWTH_RATES.unemployment_rate_skill_2   = 0.0;
    QUARTERLY_GROWTH_RATES.unemployment_rate_skill_3   = 0.0;    
    QUARTERLY_GROWTH_RATES.unemployment_rate_skill_4   = 0.0;
    QUARTERLY_GROWTH_RATES.unemployment_rate_skill_5   = 0.0;    
    QUARTERLY_GROWTH_RATES.average_wage                = 0.0;
    QUARTERLY_GROWTH_RATES.no_firms                    = 0.0;
    QUARTERLY_GROWTH_RATES.no_firm_births              = 0.0;
    QUARTERLY_GROWTH_RATES.no_firm_deaths              = 0.0;
    QUARTERLY_GROWTH_RATES.investment_value            = 0.0;
    
    ANNUAL_GROWTH_RATES_QUARTERLY.cpi                  = 1.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.gdp                  = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.output               = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.employment           = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_1    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_2    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_3    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_4    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_5    = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.average_wage         = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.no_firms             = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.no_firm_births       = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.no_firm_deaths       = 0.0;
    ANNUAL_GROWTH_RATES_QUARTERLY.investment_value     = 0.0;

    //compute quarterly rate of change of CPI: compounded inflation rate over the previous 3 months
    /*
	for (i=0; i<3; i++)
    {
        QUARTERLY_GROWTH_RATES.cpi *= HISTORY_MONTHLY[0].cpi;           
    }
    QUARTERLY_GROWTH_RATES.cpi = (QUARTERLY_GROWTH_RATES.cpi-1)*100;
    */
	
    //compute quarterly rates of change: change over the previous 3 months
	if(HISTORY_QUARTERLY[1].cpi>0.0)                {QUARTERLY_GROWTH_RATES.cpi                         = (HISTORY_QUARTERLY[0].cpi / HISTORY_QUARTERLY[1].cpi  -1)*100;}
	if(HISTORY_QUARTERLY[1].gdp>0.0)                {QUARTERLY_GROWTH_RATES.gdp                         = (HISTORY_QUARTERLY[0].gdp / HISTORY_QUARTERLY[1].gdp  -1)*100;}
    if(HISTORY_QUARTERLY[1].output>0.0)             {QUARTERLY_GROWTH_RATES.output                      = (HISTORY_QUARTERLY[0].output / HISTORY_QUARTERLY[1].output  -1)*100;}
    if(HISTORY_QUARTERLY[1].employment>0)           {QUARTERLY_GROWTH_RATES.employment                  = (HISTORY_QUARTERLY[0].employment / HISTORY_QUARTERLY[1].employment  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate           = (HISTORY_QUARTERLY[0].unemployment_rate / HISTORY_QUARTERLY[1].unemployment_rate  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate_skill_1>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate_skill_1   = (HISTORY_QUARTERLY[0].unemployment_rate_skill_1 / HISTORY_QUARTERLY[1].unemployment_rate_skill_1  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate_skill_2>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate_skill_2   = (HISTORY_QUARTERLY[0].unemployment_rate_skill_2 / HISTORY_QUARTERLY[1].unemployment_rate_skill_2  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate_skill_3>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate_skill_3   = (HISTORY_QUARTERLY[0].unemployment_rate_skill_3 / HISTORY_QUARTERLY[1].unemployment_rate_skill_3  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate_skill_4>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate_skill_4   = (HISTORY_QUARTERLY[0].unemployment_rate_skill_4 / HISTORY_QUARTERLY[1].unemployment_rate_skill_4  -1)*100;}
    if(HISTORY_QUARTERLY[1].unemployment_rate_skill_5>0.0)  {QUARTERLY_GROWTH_RATES.unemployment_rate_skill_5   = (HISTORY_QUARTERLY[0].unemployment_rate_skill_5 / HISTORY_QUARTERLY[1].unemployment_rate_skill_5  -1)*100;}
    if(HISTORY_QUARTERLY[1].average_wage>0.0)       {QUARTERLY_GROWTH_RATES.average_wage                = (HISTORY_QUARTERLY[0].average_wage / HISTORY_QUARTERLY[1].average_wage  -1)*100;}
    if(HISTORY_QUARTERLY[1].no_firms>0)             {QUARTERLY_GROWTH_RATES.no_firms                    = (HISTORY_QUARTERLY[0].no_firms / HISTORY_QUARTERLY[1].no_firms  -1)*100;}
    if(HISTORY_QUARTERLY[1].no_firm_births>0)       {QUARTERLY_GROWTH_RATES.no_firm_births              = (HISTORY_QUARTERLY[0].no_firm_births / HISTORY_QUARTERLY[1].no_firm_births  -1)*100;}
    if(HISTORY_QUARTERLY[1].no_firm_deaths>0)       {QUARTERLY_GROWTH_RATES.no_firm_deaths              = (HISTORY_QUARTERLY[0].no_firm_deaths / HISTORY_QUARTERLY[1].no_firm_deaths  -1)*100;}
    if(HISTORY_QUARTERLY[1].investment_value>0)       {QUARTERLY_GROWTH_RATES.investment_value          = (HISTORY_QUARTERLY[0].investment_value / HISTORY_QUARTERLY[1].investment_value  -1)*100;}
    
    //compute annual rate of change of CPI: compounded inflation rate over the last 4 quarters (should equal the inflation rate measured over last 12 months)
    /*
    for (i=0; i<4; i++)
     {
        ANNUAL_GROWTH_RATES_QUARTERLY.cpi *= HISTORY_QUARTERLY[i].cpi;
     }
     ANNUAL_GROWTH_RATES_QUARTERLY.cpi = (ANNUAL_GROWTH_RATES_QUARTERLY.cpi-1)*100;
	*/
	
    //compute annual rates of change over the previous 4 quarters: respective to same quarter in previous year    
	if(HISTORY_QUARTERLY[4].cpi>0.0)                {ANNUAL_GROWTH_RATES_QUARTERLY.cpi                  = (HISTORY_QUARTERLY[0].cpi / HISTORY_QUARTERLY[4].cpi  -1)*100;}
	if(HISTORY_QUARTERLY[4].gdp>0.0)                {ANNUAL_GROWTH_RATES_QUARTERLY.gdp                  = (HISTORY_QUARTERLY[0].gdp / HISTORY_QUARTERLY[4].gdp  -1)*100;}
    if(HISTORY_QUARTERLY[4].output>0.0)             {ANNUAL_GROWTH_RATES_QUARTERLY.output               = (HISTORY_QUARTERLY[0].output / HISTORY_QUARTERLY[4].output  -1)*100;}
    if(HISTORY_QUARTERLY[4].employment>0)           {ANNUAL_GROWTH_RATES_QUARTERLY.employment           = (HISTORY_QUARTERLY[0].employment / HISTORY_QUARTERLY[4].employment  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate    = (HISTORY_QUARTERLY[0].unemployment_rate / HISTORY_QUARTERLY[4].unemployment_rate  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate_skill_1>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_1    = (HISTORY_QUARTERLY[0].unemployment_rate_skill_1 / HISTORY_QUARTERLY[4].unemployment_rate_skill_1  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate_skill_2>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_2    = (HISTORY_QUARTERLY[0].unemployment_rate_skill_2 / HISTORY_QUARTERLY[4].unemployment_rate_skill_2  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate_skill_3>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_3    = (HISTORY_QUARTERLY[0].unemployment_rate_skill_3 / HISTORY_QUARTERLY[4].unemployment_rate_skill_3  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate_skill_4>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_4    = (HISTORY_QUARTERLY[0].unemployment_rate_skill_4 / HISTORY_QUARTERLY[4].unemployment_rate_skill_4  -1)*100;}
    if(HISTORY_QUARTERLY[4].unemployment_rate_skill_5>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.unemployment_rate_skill_5    = (HISTORY_QUARTERLY[0].unemployment_rate_skill_5 / HISTORY_QUARTERLY[4].unemployment_rate_skill_5  -1)*100;}
    if(HISTORY_QUARTERLY[4].average_wage>0.0)       {ANNUAL_GROWTH_RATES_QUARTERLY.average_wage         = (HISTORY_QUARTERLY[0].average_wage / HISTORY_QUARTERLY[4].average_wage  -1)*100;}
    if(HISTORY_QUARTERLY[4].no_firms>0)             {ANNUAL_GROWTH_RATES_QUARTERLY.no_firms             = (HISTORY_QUARTERLY[0].no_firms / HISTORY_QUARTERLY[4].no_firms  -1)*100;}
    if(HISTORY_QUARTERLY[4].no_firm_births>0)       {ANNUAL_GROWTH_RATES_QUARTERLY.no_firm_births       = (HISTORY_QUARTERLY[0].no_firm_births / HISTORY_QUARTERLY[4].no_firm_births  -1)*100;}
    if(HISTORY_QUARTERLY[4].no_firm_deaths>0)       {ANNUAL_GROWTH_RATES_QUARTERLY.no_firm_deaths       = (HISTORY_QUARTERLY[0].no_firm_deaths / HISTORY_QUARTERLY[4].no_firm_deaths  -1)*100;}
    if(HISTORY_QUARTERLY[4].investment_value>0)     {ANNUAL_GROWTH_RATES_QUARTERLY.investment_value     = (HISTORY_QUARTERLY[0].investment_value / HISTORY_QUARTERLY[4].investment_value  -1)*100;}
    
    //19.5.17: Set annual inflation rate based on pct change of CPI in current quarter wrt. same quarter in the previous year
    ANNUAL_INFLATION_RATE_QUARTERLY = ANNUAL_GROWTH_RATES_QUARTERLY.cpi;
    
    //*********************************** Code to be tested: region data
    
    for ( region=0; region<REGION_FIRM_DATA.size; region++)
    {
        QUARTERLY_GROWTH_RATES.region_data.array[region].cpi                         = 1.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].gdp                         = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].output                      = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].employment                  = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate           = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_1   = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_2   = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_3   = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_4   = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_5   = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].average_wage                = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].no_firms                    = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].no_firm_births              = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].no_firm_deaths              = 0.0;
        QUARTERLY_GROWTH_RATES.region_data.array[region].investment_value            = 0.0;

        
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].cpi                  = 1.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].gdp                  = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].output               = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].employment           = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_1    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_2    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_3    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_4    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_5    = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].average_wage         = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firms             = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firm_births       = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firm_deaths       = 0.0;
        ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].investment_value     = 0.0;
    
        //compute quarterly rates of change of CPI: compounded inflation rate over the previous 3 months
        /*
		for (i=0; i<3; i++)
        {
            QUARTERLY_GROWTH_RATES.region_data.array[region].cpi *= HISTORY_MONTHLY[0].region_data.array[region].cpi;           
        }
        QUARTERLY_GROWTH_RATES.region_data.array[region].cpi = (QUARTERLY_GROWTH_RATES.region_data.array[region].cpi-1)*100;
        */
		
        //compute quarterly rates of change:
        if(HISTORY_QUARTERLY[1].region_data.array[region].cpi>0.0)                {QUARTERLY_GROWTH_RATES.region_data.array[region].cpi                         = (HISTORY_QUARTERLY[0].region_data.array[region].cpi / HISTORY_QUARTERLY[1].region_data.array[region].cpi  -1)*100;}
		if(HISTORY_QUARTERLY[1].region_data.array[region].gdp>0.0)                {QUARTERLY_GROWTH_RATES.region_data.array[region].gdp                         = (HISTORY_QUARTERLY[0].region_data.array[region].gdp / HISTORY_QUARTERLY[1].region_data.array[region].gdp  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].output>0.0)             {QUARTERLY_GROWTH_RATES.region_data.array[region].output                      = (HISTORY_QUARTERLY[0].region_data.array[region].output / HISTORY_QUARTERLY[1].region_data.array[region].output  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].employment>0)           {QUARTERLY_GROWTH_RATES.region_data.array[region].employment                  = (HISTORY_QUARTERLY[0].region_data.array[region].employment / HISTORY_QUARTERLY[1].region_data.array[region].employment  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate           = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_1>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_1   = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1 / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_1  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_2>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_2   = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2 / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_2  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_3>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_3   = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3 / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_3  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_4>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_4   = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4 / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_4  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_5>0.0)  {QUARTERLY_GROWTH_RATES.region_data.array[region].unemployment_rate_skill_5   = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5 / HISTORY_QUARTERLY[1].region_data.array[region].unemployment_rate_skill_5  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].average_wage>0.0)       {QUARTERLY_GROWTH_RATES.region_data.array[region].average_wage                = (HISTORY_QUARTERLY[0].region_data.array[region].average_wage / HISTORY_QUARTERLY[1].region_data.array[region].average_wage  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].no_firms>0)             {QUARTERLY_GROWTH_RATES.region_data.array[region].no_firms                    = (HISTORY_QUARTERLY[0].region_data.array[region].no_firms / HISTORY_QUARTERLY[1].region_data.array[region].no_firms  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].no_firm_births>0)       {QUARTERLY_GROWTH_RATES.region_data.array[region].no_firm_births              = (HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births / HISTORY_QUARTERLY[1].region_data.array[region].no_firm_births  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].no_firm_deaths>0)       {QUARTERLY_GROWTH_RATES.region_data.array[region].no_firm_deaths              = (HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths / HISTORY_QUARTERLY[1].region_data.array[region].no_firm_deaths  -1)*100;}
        if(HISTORY_QUARTERLY[1].region_data.array[region].investment_value>0)       {QUARTERLY_GROWTH_RATES.region_data.array[region].investment_value              = (HISTORY_QUARTERLY[0].region_data.array[region].investment_value / HISTORY_QUARTERLY[1].region_data.array[region].investment_value  -1)*100;}
        
        //compute annual rate of change of CPI: compounded inflation rate over the last 4 quarters (should equal the inflation rate measured over last 12 months)
        /*
         for (i=0; i<4; i++)
         {
            ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].cpi *= HISTORY_QUARTERLY[i].region_data.array[region].cpi;
         }
         ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].cpi = (ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].cpi-1)*100;
		*/
		
        //compute annual rates of change over the previous 4 quarters: respective to same quarter in previous year
        if(HISTORY_QUARTERLY[4].region_data.array[region].cpi>0.0)                {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].cpi                  = (HISTORY_QUARTERLY[0].region_data.array[region].cpi / HISTORY_QUARTERLY[4].region_data.array[region].cpi  -1)*100;}
		if(HISTORY_QUARTERLY[4].region_data.array[region].gdp>0.0)                {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].gdp                  = (HISTORY_QUARTERLY[0].region_data.array[region].gdp / HISTORY_QUARTERLY[4].region_data.array[region].gdp  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].output>0.0)             {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].output               = (HISTORY_QUARTERLY[0].region_data.array[region].output / HISTORY_QUARTERLY[4].region_data.array[region].output  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].employment>0)           {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].employment           = (HISTORY_QUARTERLY[0].region_data.array[region].employment / HISTORY_QUARTERLY[4].region_data.array[region].employment  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_1>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_1    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_1 / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_1  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_2>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_2    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_2 / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_2  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_3>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_3    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_3 / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_3  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_4>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_4    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_4 / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_4  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_5>0.0)  {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].unemployment_rate_skill_5    = (HISTORY_QUARTERLY[0].region_data.array[region].unemployment_rate_skill_5 / HISTORY_QUARTERLY[4].region_data.array[region].unemployment_rate_skill_5  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].average_wage>0.0)       {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].average_wage  = (HISTORY_QUARTERLY[0].region_data.array[region].average_wage / HISTORY_QUARTERLY[4].region_data.array[region].average_wage  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].no_firms>0)             {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firms             = (HISTORY_QUARTERLY[0].region_data.array[region].no_firms / HISTORY_QUARTERLY[4].region_data.array[region].no_firms  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].no_firm_births>0)       {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firm_births       = (HISTORY_QUARTERLY[0].region_data.array[region].no_firm_births / HISTORY_QUARTERLY[4].region_data.array[region].no_firm_births  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].no_firm_deaths>0)       {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].no_firm_deaths       = (HISTORY_QUARTERLY[0].region_data.array[region].no_firm_deaths / HISTORY_QUARTERLY[4].region_data.array[region].no_firm_deaths  -1)*100;}
        if(HISTORY_QUARTERLY[4].region_data.array[region].investment_value>0)       {ANNUAL_GROWTH_RATES_QUARTERLY.region_data.array[region].investment_value     = (HISTORY_QUARTERLY[0].region_data.array[region].investment_value / HISTORY_QUARTERLY[4].region_data.array[region].investment_value  -1)*100;}
    }
        
    return 0;
}



/* \fn: int Eurostat_measure_recession(void)
 * \brief: Function to measure the start, duration and end of a recession.
 */
int Eurostat_measure_recession()
{
    //Signal start of recesson: 2 quarters of succesive negative growth of GDP
    if (RECESSION_STARTED==0)
    {
        if ( (HISTORY_QUARTERLY[0].gdp < HISTORY_QUARTERLY[1].gdp) && (HISTORY_QUARTERLY[1].gdp < HISTORY_QUARTERLY[2].gdp))
        {
            RECESSION_STARTED=1;
            RECESSION_DURATION = 0;
        }   
    }   
    //Signal end of recesson: 1 quarter of positive growth of GDP after start of recession
    if (RECESSION_STARTED==1)
    {
        RECESSION_DURATION++;

        if (HISTORY_QUARTERLY[0].gdp >= HISTORY_QUARTERLY[1].gdp)
        {
            RECESSION_STARTED=0;
        }
    }

    #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_EUROSTAT)
    {
        printf(" - recession started: %d\n", RECESSION_STARTED);
        printf(" - duration of recession: %d\n", RECESSION_DURATION);
    }
    #endif
        
    return 0;
}

