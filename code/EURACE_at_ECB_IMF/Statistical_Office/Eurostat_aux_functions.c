/*********************************
 * Eurostat_aux_functions.c 
 * Eurostat auxiliary functions.
 * *********************************
 * History:
 * 16/09/08 Sander 
 *********************************/
#include <limits.h> //required to test for max. double: LONG_MAX
#include <gsl/gsl_sort.h>

#include "../header.h"
#include "../my_library_header.h"
#include "../Eurostat_agent_header.h"
#include "Eurostat_aux_header.h"
#include "../Sanity_Checks/sanity_checks_aux_headers.h"

/* \fn: void Eurostat_reset_data(void)
 * \brief: Function to reset the region data strucures for firms and households.
 */
void Eurostat_reset_data(void)
{
    int i;
    double mean_specific_skills;

    /*delete the content of the data arrays in order to store the data for the new month*/
    //free(REGION_HOUSEHOLD_DATA);
    //free(REGION_FIRM_DATA);
    
    
	/*Save the mean specific skills from REGION_HOUSEHOLD_DATA in a temproray memory variable in order to have an initial value for the reconstruction of the array*/

	mean_specific_skills = REGION_HOUSEHOLD_DATA.array[0].average_s_skill;


    //Reset all data arrays
    reset_firm_data_array(&REGION_FIRM_DATA);

    reset_household_data_array(&REGION_HOUSEHOLD_DATA);
    
    //Reconstruct empty data arrays
    int region_id;
    for(i = 0; i < TOTAL_REGIONS; i++)
    {
        region_id=i+1;

        add_firm_data(&REGION_FIRM_DATA,
                region_id,0,0,0,         //region_id, no_firms, no_active_firms, vacancies
                0,0,0,0,0,0,             //no_employees_skill
                0.0,0.0,0.0,0.0,0.0,0.0, //total_mean_wage_skill
                0.0,0.0,0.0,0.0,0.0,0.0, //average_s_skill
                0.0,0.0,0.0,0.0,0.0,     //total_earnings -> average_debt_earnings_ratio
                0.0,0.0,0.0,0.0,0.0,0.0, //average_debt_equity_ratio -> monthly_planned_output
                0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
                0.0,1.0,1.0,             //gdp, cpi, cpi_last_month 
                0,0,                     //no_firm_births, no_firm_deaths
                0,0);                    //productivity_progress, productivity
        
        /* Old-style code */
        add_household_data(&REGION_HOUSEHOLD_DATA,
                region_id,
                0,0,0,0,0,0,
                0,0,0,0,0,0,

                0.0,0.0,0.0,0.0,0.0,0.0,
                0.0,0.0,0.0,0.0,0.0,0.0,0.0,
                mean_specific_skills,mean_specific_skills,mean_specific_skills,mean_specific_skills,mean_specific_skills,mean_specific_skills,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);//8 attributes added 30.10.17 (is_owner etc.)
    }
    

/*
VARIABLES:

    GDP_DATA
    CPI_DATA

    OUTPUT_DATA
    SALES_DATA
    SOLD_QUANTITY_DATA
    INVESTMENT_DATA

    FIRM_ASSETS_DATA
    FIRM_CREDIT_DATA
    FIRM_EQUITY_DATA

    BANK_EQUITY_DATA
    BANK_REA_DATA

    MORTGAGE_CREDIT_DATA
    TOTAL_CREDIT_DATA

    HOUSING_PRICE_INDEX_DATA
    RENTAL_PRICE_INDEX_DATA

    //Ratios
    INVESTMENT_GDP_RATIO_DATA
    BANK_EQUITY_REA_RATIO_DATA
    BANK_EQUITY_ASSET_RATIO_DATA
*/

    //NOTE: extra & needed before arguments because regions is dynamic array of history_adt:
    //void add_history_adt(history_adt_array * array, /*@out@*/ series_adt * levels, /*@out@*/ series_adt * pct_change_qoq_series, /*@out@*/ series_adt * pct_change_yoy_series);

    //Initialize empty history for time series data structures
    history_adt history;
    init_history_adt(&history);

    for (i=0; i<TOTAL_REGIONS; i++)
    {
        add_history_adt(&GDP_DATA.regions, 
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&CPI_DATA.regions, 
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&OUTPUT_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&SALES_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&SOLD_QUANTITY_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&INVESTMENT_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&FIRM_ASSETS_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&FIRM_CREDIT_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&FIRM_EQUITY_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&BANK_EQUITY_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&BANK_REA_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&MORTGAGE_CREDIT_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&TOTAL_CREDIT_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&HOUSING_PRICE_INDEX_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&RENTAL_PRICE_INDEX_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&INVESTMENT_GDP_RATIO_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));

        add_history_adt(&BANK_EQUITY_REA_RATIO_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));
        
        add_history_adt(&BANK_EQUITY_ASSET_RATIO_DATA.regions,
                                &(history.levels), 
                                &(history.pct_change_qoq_series),
                                &(history.pct_change_yoy_series));
    }
}

/* \fn: void Eurostat_compute_mean_price(void)
 * \brief: Function to compute a mean price.
 */
void Eurostat_compute_mean_price(void)
{
    double sum_consumption_good_supply = 0.0;
    PRICE_INDEX = 0.0;

    FIRM_NO_FIRM_LOANS = 0;
    FIRM_NO_FIRM_LOANS_NEW = 0;
    FIRM_NO_FIRM_LOANS_PAYING = 0;
    FIRM_NO_FIRM_LOANS_PAID_OFF = 0;
    FIRM_NO_FIRM_LOANS_WRITEOFF = 0;

    FIRM_INTEREST_ANNUITY_RATIO = 0.0;
    FIRM_DSTI = 0.0;

    NO_FIRMS=0;

    START_FIRM_SEND_DATA_MESSAGE_LOOP

        NO_FIRMS++;

        /*Compute a weighted mean price*/
        sum_consumption_good_supply += firm_send_data_message->firm_info.total_supply;

        FIRM_NO_FIRM_LOANS          += firm_send_data_message->firm_info.firm_no_firm_loans ;
        FIRM_NO_FIRM_LOANS_NEW      += firm_send_data_message->firm_info.firm_no_firm_loans_new ;
        FIRM_NO_FIRM_LOANS_PAYING   += firm_send_data_message->firm_info.firm_no_firm_loans_paying ;
        FIRM_NO_FIRM_LOANS_PAID_OFF += firm_send_data_message->firm_info.firm_no_firm_loans_paid_off ;
        FIRM_NO_FIRM_LOANS_WRITEOFF += firm_send_data_message->firm_info.firm_no_firm_loans_writeoff ;

        FIRM_INTEREST_ANNUITY_RATIO += firm_send_data_message->firm_info.interest_annuity_ratio ;
        FIRM_DSTI += firm_send_data_message->firm_info.dsti ;

    FINISH_FIRM_SEND_DATA_MESSAGE_LOOP
    
    FIRM_INTEREST_ANNUITY_RATIO = FIRM_INTEREST_ANNUITY_RATIO / NO_FIRMS;
    FIRM_DSTI = FIRM_DSTI / NO_FIRMS;

    if (sum_consumption_good_supply>0.0)
    {
        START_FIRM_SEND_DATA_MESSAGE_LOOP
            PRICE_INDEX += (firm_send_data_message->firm_info.price*firm_send_data_message->firm_info.total_supply)/ sum_consumption_good_supply;
        FINISH_FIRM_SEND_DATA_MESSAGE_LOOP
    }
    
    //19.5.17: Set cpi equal to price_index
    CPI = PRICE_INDEX;
}

/* \fn: void Eurostat_read_firm_data(void)
 * \brief: Function to read and store the region data of the firms.
 */
void Eurostat_read_firm_data(void)
{
    int i;

    NO_VACANCIES = 0.0;
    NO_POSTED_VACANCIES = 0.0;
    NO_EMPLOYEES = 0.0;
    NO_EMPLOYEES_PRODUCTION = 0.0;
    NO_EMPLOYEES_SKILL_1 = 0.0;
    NO_EMPLOYEES_SKILL_2 = 0.0;
    NO_EMPLOYEES_SKILL_3 = 0.0;
    NO_EMPLOYEES_SKILL_4 = 0.0;
    NO_EMPLOYEES_SKILL_5 = 0.0;
    NO_EMPLOYEES_R_AND_D = 0.0;

    FIRM_AVERAGE_WAGE = 0.0;

    FIRM_AVERAGE_S_SKILL = 0.0;
    FIRM_AVERAGE_S_SKILL_1 = 0.0;
    FIRM_AVERAGE_S_SKILL_2 = 0.0;
    FIRM_AVERAGE_S_SKILL_3 = 0.0;
    FIRM_AVERAGE_S_SKILL_4 = 0.0;
    FIRM_AVERAGE_S_SKILL_5 = 0.0;
    
    FIRM_AVERAGE_PRODUCTIVITY_PROGRESS = 0.0;
    FIRM_AVERAGE_PRODUCTIVITY = 0.0;

    TOTAL_CAPITAL_STOCK_UNITS = 0.0;
    TOTAL_VALUE_MALL_INVENTORIES = 0.0;
    TOTAL_DIVIDENDS=0.0;	 
    BASE_WAGE_OFFER = 0.0;	

    FIRM_AVERAGE_QUALITY =0.0;
    FIRM_AVERAGE_PRICE = 0.0;
    
    NO_FIRMS =0;
    NO_ACTIVE_FIRMS=0;

    if (DAY % MONTH ==0) MONTHLY_POTENTIAL_OUTPUT = 0.0; //Target output: output if all firms operate at full capacity. Used as proxy for the potential output (full employment of labour)

    for(i = 0; i < REGION_FIRM_DATA.size; i++)
    {
        REGION_FIRM_DATA.array[i].no_firms = 0;

        START_FIRM_SEND_DATA_MESSAGE_LOOP
        
        if(firm_send_data_message->firm_info.region_id == 
            REGION_FIRM_DATA.array[i].region_id)
        {
            REGION_FIRM_DATA.array[i].no_firms += 1;
            NO_FIRMS++;

            if (firm_send_data_message->firm_info.age != -1)
            {
            	NO_ACTIVE_FIRMS++;
                REGION_FIRM_DATA.array[i].no_active_firms += 1;
            }

            
            REGION_FIRM_DATA.array[i].vacancies += 
            firm_send_data_message->firm_info.vacancies;
            NO_VACANCIES += firm_send_data_message->firm_info.vacancies;
            NO_POSTED_VACANCIES += firm_send_data_message->firm_info.posted_vacancies;

            REGION_FIRM_DATA.array[i].no_employees += 
            firm_send_data_message->firm_info.no_employees;
            NO_EMPLOYEES += firm_send_data_message->firm_info.no_employees;

            REGION_FIRM_DATA.array[i].no_employees_skill_1 += 
            firm_send_data_message->firm_info.no_employees_skill_1;
            NO_EMPLOYEES_SKILL_1 += 
            firm_send_data_message->firm_info.no_employees_skill_1;

            REGION_FIRM_DATA.array[i].no_employees_skill_2 += 
            firm_send_data_message->firm_info.no_employees_skill_2;
            NO_EMPLOYEES_SKILL_2 += 
            firm_send_data_message->firm_info.no_employees_skill_2;

            REGION_FIRM_DATA.array[i].no_employees_skill_3 += 
            firm_send_data_message->firm_info.no_employees_skill_3;
            NO_EMPLOYEES_SKILL_3 += 
            firm_send_data_message->firm_info.no_employees_skill_3;

            REGION_FIRM_DATA.array[i].no_employees_skill_4 += 
            firm_send_data_message->firm_info.no_employees_skill_4;
            NO_EMPLOYEES_SKILL_4 += 
            firm_send_data_message->firm_info.no_employees_skill_4;

            REGION_FIRM_DATA.array[i].no_employees_skill_5 += 
            firm_send_data_message->firm_info.no_employees_skill_5;
            NO_EMPLOYEES_SKILL_5 += 
            firm_send_data_message->firm_info.no_employees_skill_5;

            NO_EMPLOYEES_PRODUCTION += 
            firm_send_data_message->firm_info.no_employees_production;

            NO_EMPLOYEES_R_AND_D +=
            firm_send_data_message->firm_info.no_employees_innovation;

            /********sum of wages of the firms++++++++*/
            REGION_FIRM_DATA.array[i].total_mean_wage += 
            firm_send_data_message->firm_info.total_mean_wage*
            firm_send_data_message->firm_info.no_employees;

            FIRM_AVERAGE_WAGE += firm_send_data_message->firm_info.total_mean_wage*
            firm_send_data_message->firm_info.no_employees_production;
            
            FIRM_AVERAGE_PRODUCTION_WAGE += firm_send_data_message->firm_info.mean_wage*
            firm_send_data_message->firm_info.no_employees;
            
            FIRM_AVERAGE_R_AND_D_WAGE += firm_send_data_message->firm_info.mean_wage_r_and_d*
            firm_send_data_message->firm_info.no_employees_innovation;

            /********sum of specific skills of the firms++++++++*/
            REGION_FIRM_DATA.array[i].average_s_skill += 
            firm_send_data_message->firm_info.mean_specific_skills *
            firm_send_data_message->firm_info.no_employees;
            FIRM_AVERAGE_S_SKILL += 
            firm_send_data_message->firm_info.mean_specific_skills *
            firm_send_data_message->firm_info.no_employees;


            REGION_FIRM_DATA.array[i].average_s_skill_1 +=
            firm_send_data_message->firm_info.average_s_skill_of_1*
            firm_send_data_message->firm_info.no_employees_skill_1;
            FIRM_AVERAGE_S_SKILL_1 +=
            firm_send_data_message->firm_info.average_s_skill_of_1*
            firm_send_data_message->firm_info.no_employees_skill_1;


            REGION_FIRM_DATA.array[i].average_s_skill_2 +=
            firm_send_data_message->firm_info.average_s_skill_of_2*
            firm_send_data_message->firm_info.no_employees_skill_2;
            FIRM_AVERAGE_S_SKILL_2 +=
            firm_send_data_message->firm_info.average_s_skill_of_2*
            firm_send_data_message->firm_info.no_employees_skill_2;


            REGION_FIRM_DATA.array[i].average_s_skill_3 +=
            firm_send_data_message->firm_info.average_s_skill_of_3*
            firm_send_data_message->firm_info.no_employees_skill_3;
            FIRM_AVERAGE_S_SKILL_3 +=
            firm_send_data_message->firm_info.average_s_skill_of_3*
            firm_send_data_message->firm_info.no_employees_skill_3;


            REGION_FIRM_DATA.array[i].average_s_skill_4 +=
            firm_send_data_message->firm_info.average_s_skill_of_4*
            firm_send_data_message->firm_info.no_employees_skill_4;
            FIRM_AVERAGE_S_SKILL_4 +=
            firm_send_data_message->firm_info.average_s_skill_of_4*
            firm_send_data_message->firm_info.no_employees_skill_4;

            
            REGION_FIRM_DATA.array[i].average_s_skill_5 +=
            firm_send_data_message->firm_info.average_s_skill_of_5*
            firm_send_data_message->firm_info.no_employees_skill_5;
            FIRM_AVERAGE_S_SKILL_5 +=
            firm_send_data_message->firm_info.average_s_skill_of_5*
            firm_send_data_message->firm_info.no_employees_skill_5;
            
            REGION_FIRM_DATA.array[i].productivity_progress += firm_send_data_message->firm_info.firm_productivity_progress;
            FIRM_AVERAGE_PRODUCTIVITY_PROGRESS += firm_send_data_message->firm_info.firm_productivity_progress;
            
            REGION_FIRM_DATA.array[i].productivity += firm_send_data_message->firm_info.firm_productivity;
            FIRM_AVERAGE_PRODUCTIVITY += firm_send_data_message->firm_info.firm_productivity;

            if(firm_send_data_message->firm_info.age!=-1)
            {
              FIRM_AVERAGE_QUALITY+= firm_send_data_message->firm_info.quality;
              FIRM_AVERAGE_PRICE+= firm_send_data_message->firm_info.price;
          }


          TOTAL_CAPITAL_STOCK_UNITS += firm_send_data_message->firm_info.total_units_capital_stock;

          TOTAL_VALUE_MALL_INVENTORIES += firm_send_data_message->firm_info.total_value_local_inventory;

          TOTAL_DIVIDENDS += firm_send_data_message->firm_info.total_dividend_payment;	 

          if(NO_ACTIVE_FIRMS>1)
              BASE_WAGE_OFFER +=firm_send_data_message->firm_info.wage_offer/(NO_ACTIVE_FIRMS-1) ;

      }

      MONTHLY_POTENTIAL_OUTPUT += firm_send_data_message->firm_info.potential_output;

      FINISH_FIRM_SEND_DATA_MESSAGE_LOOP

  }
}

/* \fn: void Eurostat_compute_region_firm_data(void)
 * \brief: Function to compute regional data related to firms.
 */
void Eurostat_compute_region_firm_data(void)
{
    int i;

    /*Create the REGIONAL data which feed backs to firms*/
    for(i = 0; i < REGION_FIRM_DATA.size; i++)
    {
        /*********************WAGES************************/
        if(REGION_FIRM_DATA.array[i].no_employees > 0)
        {
            REGION_FIRM_DATA.array[i].total_mean_wage = 
            REGION_FIRM_DATA.array[i].total_mean_wage/
            REGION_FIRM_DATA.array[i].no_employees;
        }
    
    
        /*****************SPECIFIC SKILLS*********************/
        if(REGION_FIRM_DATA.array[i].no_employees > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill =
            REGION_FIRM_DATA.array[i].average_s_skill/
            REGION_FIRM_DATA.array[i].no_employees;
        }
    
        if(REGION_FIRM_DATA.array[i].no_employees_skill_1 > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill_1 =
            REGION_FIRM_DATA.array[i].average_s_skill_1/
            REGION_FIRM_DATA.array[i].no_employees_skill_1;
        }
    
        if(REGION_FIRM_DATA.array[i].no_employees_skill_2 > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill_2 =
            REGION_FIRM_DATA.array[i].average_s_skill_2/
            REGION_FIRM_DATA.array[i].no_employees_skill_2;
        }
    
    
        if(REGION_FIRM_DATA.array[i].no_employees_skill_3 > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill_3 =
            REGION_FIRM_DATA.array[i].average_s_skill_3/
            REGION_FIRM_DATA.array[i].no_employees_skill_3;
        }
    
    
        if(REGION_FIRM_DATA.array[i].no_employees_skill_4 > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill_4 =
            REGION_FIRM_DATA.array[i].average_s_skill_4/
            REGION_FIRM_DATA.array[i].no_employees_skill_4;
        }
    
        if(REGION_FIRM_DATA.array[i].no_employees_skill_5 > 0)
        {
            REGION_FIRM_DATA.array[i].average_s_skill_5 =
            REGION_FIRM_DATA.array[i].average_s_skill_5/
            REGION_FIRM_DATA.array[i].no_employees_skill_5;
        }  
        
        if(REGION_FIRM_DATA.array[i].no_active_firms > 0)
        {
            REGION_FIRM_DATA.array[i].productivity_progress =
            REGION_FIRM_DATA.array[i].productivity_progress/
            REGION_FIRM_DATA.array[i].no_active_firms;
           
            REGION_FIRM_DATA.array[i].productivity =
            REGION_FIRM_DATA.array[i].productivity/
            REGION_FIRM_DATA.array[i].no_active_firms;
        }
    	else
    	{
    	    REGION_FIRM_DATA.array[i].productivity_progress = 0.0;
    	    REGION_FIRM_DATA.array[i].productivity = 0.0;
    	}
    }
}

/* \fn: void Eurostat_compute_global_firm_data(void)
 * \brief: Function to compute global data that is related to firms.
 */
void Eurostat_compute_global_firm_data(void)
{   
    /*Create the GLOBAL data which is needed for controlling the results or sending 
     *         back to the Households*/
    
    /*********************WAGES****************/
    if(NO_EMPLOYEES > 0)
    {
        FIRM_AVERAGE_WAGE = FIRM_AVERAGE_WAGE/(double)NO_EMPLOYEES;
    }

	 if(NO_EMPLOYEES_R_AND_D > 0)
    {
        FIRM_AVERAGE_R_AND_D_WAGE = FIRM_AVERAGE_R_AND_D_WAGE/(double)NO_EMPLOYEES_R_AND_D;
    }

	 if(NO_EMPLOYEES_PRODUCTION > 0)
    {
        FIRM_AVERAGE_PRODUCTION_WAGE = FIRM_AVERAGE_PRODUCTION_WAGE/(double)NO_EMPLOYEES_PRODUCTION;
    }

 /*****************GENERAL_SKILLS******************/
    if(NO_EMPLOYEES > 0)
    {
        FIRM_AVERAGE_G_SKILL = (NO_EMPLOYEES_SKILL_1*1+ NO_EMPLOYEES_SKILL_2*2+NO_EMPLOYEES_SKILL_3*3+NO_EMPLOYEES_SKILL_4*4+NO_EMPLOYEES_SKILL_5*5)/(double)NO_EMPLOYEES;
    }
    
    
    /*****************SPECIFIC_SKILLS******************/
    if(NO_EMPLOYEES > 0)
    {
        FIRM_AVERAGE_S_SKILL = FIRM_AVERAGE_S_SKILL/(double)NO_EMPLOYEES;
    }
    
    if(NO_EMPLOYEES_SKILL_1 > 0)
    {
        FIRM_AVERAGE_S_SKILL_1 = FIRM_AVERAGE_S_SKILL_1/
                    (double)NO_EMPLOYEES_SKILL_1;
    }
    
    if(NO_EMPLOYEES_SKILL_2 > 0)
    {
        FIRM_AVERAGE_S_SKILL_2 = FIRM_AVERAGE_S_SKILL_2/
                    (double)NO_EMPLOYEES_SKILL_2;
    }
    
    if(NO_EMPLOYEES_SKILL_3 > 0)
    {
        FIRM_AVERAGE_S_SKILL_3 = FIRM_AVERAGE_S_SKILL_3/
                    (double)NO_EMPLOYEES_SKILL_3;
    }
    
    if(NO_EMPLOYEES_SKILL_4 > 0)
    {
        FIRM_AVERAGE_S_SKILL_4 = FIRM_AVERAGE_S_SKILL_4/
                    (double)NO_EMPLOYEES_SKILL_4;
    }
    
    if(NO_EMPLOYEES_SKILL_5 > 0)
    {
        FIRM_AVERAGE_S_SKILL_5 = FIRM_AVERAGE_S_SKILL_5/
                    (double)NO_EMPLOYEES_SKILL_5;
    }
    
    if(NO_ACTIVE_FIRMS > 1)
    {
        FIRM_AVERAGE_PRODUCTIVITY_PROGRESS = FIRM_AVERAGE_PRODUCTIVITY_PROGRESS/
       ( NO_ACTIVE_FIRMS-1);
        
        FIRM_AVERAGE_PRODUCTIVITY = FIRM_AVERAGE_PRODUCTIVITY/(NO_ACTIVE_FIRMS-1);

		FIRM_AVERAGE_QUALITY= FIRM_AVERAGE_QUALITY/(NO_ACTIVE_FIRMS-1);

		FIRM_AVERAGE_PRICE= FIRM_AVERAGE_PRICE/(NO_ACTIVE_FIRMS-1);
        
    }
    else
    {
    	FIRM_AVERAGE_PRODUCTIVITY_PROGRESS = 0.0;
    	FIRM_AVERAGE_PRODUCTIVITY = 0.0;

        fprintf(stderr, "\nWARNING: In %s, %s(), line %d:\n\t NO_ACTIVE_FIRMS==%d (this includes the IGFirm)", __FILE__, __FUNCTION__, __LINE__, NO_ACTIVE_FIRMS);
    }
}


/* \fn: void Eurostat_read_household_data(void)
 * \brief: Function to read household_send_data_messages and store the global/region data of the households.
 * \brief: Used in: Eurostat_calculate_data()
 * \brief: Frequency: monthly, phase 0 (on last day, ie 20, 40, etc.) AND switch_datastorage=1 in Eurostat
 */
void Eurostat_read_household_data(void)
{   
    int i;
    NUM_HOUSEHOLDS=0;
    
    EMPLOYED = 0;
    EMPLOYED_SKILL_1 = 0;
    EMPLOYED_SKILL_2 = 0;
    EMPLOYED_SKILL_3 = 0;
    EMPLOYED_SKILL_4 = 0;
    EMPLOYED_SKILL_5 = 0;
    UNEMPLOYED = 0;  
    
    AVERAGE_WAGE = 0.0;
    AVERAGE_WAGE_SKILL_1 = 0.0;
    AVERAGE_WAGE_SKILL_2 = 0.0;
    AVERAGE_WAGE_SKILL_3 = 0.0;
    AVERAGE_WAGE_SKILL_4 = 0.0;
    AVERAGE_WAGE_SKILL_5 = 0.0;

    AVERAGE_S_SKILL = 0.0;
    AVERAGE_S_SKILL_1 = 0.0;
    AVERAGE_S_SKILL_2 = 0.0;
    AVERAGE_S_SKILL_3 = 0.0;
    AVERAGE_S_SKILL_4 = 0.0;
    AVERAGE_S_SKILL_5 = 0.0;

	JUST_EMPLOYED = 0;
	JUST_UNEMPLOYED = 0;
	START_EMPLOYED = 0;
	START_UNEMPLOYED = 0;
	ENTER_MATCHING = 0;
	UNEMPLOYED_DURATION = 0;

	HH_PAYMENT_ACCOUNT              	= 0.0;
	HH_MEAN_NET_INCOME              	= 0.0;
	HH_TOTAL_GROSS_ANNUAL_INCOME    	= 0.0;
	HH_TOTAL_GROSS_QUARTERLY_INCOME 	= 0.0;

	HH_CONSUMPTION_BUDGET            	= 0.0;
	HH_CONSUMPTION_EXPENDITURE      	= 0.0;
	HH_MONTHLY_RENT_EXPENSE         	= 0.0;
	HH_MONTHLY_RENT_INCOME          	= 0.0;
	HH_MONTHLY_MORTGAGE_PAYMENT     	= 0.0;

	HH_IS_HOMELESS                         = 0.0;
	HH_IS_TENANT                           = 0.0;
	HH_IS_OWNER                            = 0.0;
	HH_IS_LANDLORD                         = 0.0;
	HH_HAS_BTL_GENE                        = 0.0;

	HH_DEFAULTED_ON_RENT                   = 0.0;
	HH_EVICTED_AS_TENANT                   = 0.0;
	HH_EVICTED_AS_OWNER                    = 0.0;
	
    HH_INTEREST_ANNUITY_RATIO              = 0.0;
    HH_HMR_INTEREST_ANNUITY_RATIO          = 0.0;
    HH_OP_INTEREST_ANNUITY_RATIO           = 0.0;

    HH_HMR_DEBT_SERVICE_TO_INCOME_RATIO    = 0.0;
    HH_RENT_TO_INCOME_RATIO                = 0.0;
    HH_PAYMENT_ACCOUNT_TO_INCOME_RATIO     = 0.0;

    HH_HMR_DEBT_SERVICE_BUFFER_IN_MONTHS = 0.0;
    HH_RENT_BUFFER_IN_MONTHS = 0.0;

    HH_OP_INCOME = 0.0;
    HH_OP_EXPENSE = 0.0;
    HH_SHARE_OP_WITH_MORTGAGE = 0.0;
    HH_OP_PROFIT = 0.0;

    HH_NO_MORTGAGES                        = 0.0;

    HH_MORTGAGES_LTV_REDRAW      = 0;
    HH_MORTGAGES_LTV_CAP_ACTIVE  = 0;
    HH_MORTGAGES_DSTI_CAP_ACTIVE = 0;

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

    NO_HOUSEHOLD_CLASS_1 = 0;
    NO_HOUSEHOLD_CLASS_2 = 0;
    NO_HOUSEHOLD_CLASS_3 = 0;
    NO_HOUSEHOLD_CLASS_4 = 0;

	// Store individual consumption levels to calculate median consumption level
    init_double_array(&CONSUMPTION_LEVELS);

    
    // Re-initialize all ADTs household_data in region data array (set attributes to zero, set region_id = 1,...n )
    for(i = 0; i < REGION_HOUSEHOLD_DATA.size; i++)
    {
        init_household_data(&REGION_HOUSEHOLD_DATA.array[i]);
        REGION_HOUSEHOLD_DATA.array[i].region_id = i+1;
	}

    int dsti_outlier = 0;
    int rti_outlier = 0;

    START_HOUSEHOLD_SEND_DATA_MESSAGE_LOOP
    
        /*Store the global/region data of the households*/
        for(i = 0; i < REGION_HOUSEHOLD_DATA.size; i++)
        {
            //printf("\n\t REGION %d", i);
            //printf("\n\t REGION_HOUSEHOLD_DATA.array[%d].region_id %d", i, REGION_HOUSEHOLD_DATA.array[i].region_id);
            //printf("\n\t household_send_data_message->household_info.region_id %d", household_send_data_message->household_info.region_id);

            if(household_send_data_message->household_info.region_id == REGION_HOUSEHOLD_DATA.array[i].region_id)
            {
                //if(DAY == 20)
                //{
                    NUM_HOUSEHOLDS++;
                //}
                
    
                REGION_HOUSEHOLD_DATA.array[i].no_households++;
    
                REGION_HOUSEHOLD_DATA.array[i].average_wage += household_send_data_message->household_info.wage;

                AVERAGE_WAGE += household_send_data_message->household_info.wage;
    
                REGION_HOUSEHOLD_DATA.array[i].average_s_skill += household_send_data_message->household_info.specific_skill;
                AVERAGE_S_SKILL += household_send_data_message->household_info.specific_skill;

				JUST_EMPLOYED   += household_send_data_message->household_info.just_employed;
				JUST_UNEMPLOYED += household_send_data_message->household_info.just_unemployed;
				START_EMPLOYED  += household_send_data_message->household_info.start_employed;
				START_UNEMPLOYED += household_send_data_message->household_info.start_unemployed;
				ENTER_MATCHING	+= household_send_data_message->household_info.enter_matching;

                if (household_send_data_message->household_info.just_employed)
                    UNEMPLOYED_DURATION+=household_send_data_message->household_info.unemployed_duration;

                /*(Region) data is subdivided by general skill level*/
                switch(household_send_data_message->household_info.general_skill)
                {  
                    case 1:/*General skill level 1*/
                    if(household_send_data_message->household_info.employment_status != -1)
                    {
                        REGION_HOUSEHOLD_DATA.array[i].employed++;
                        REGION_HOUSEHOLD_DATA.array[i].employed_skill_1++;
                        EMPLOYED_SKILL_1++;
                        EMPLOYED++;
                    }
                    else
                    {
                        REGION_HOUSEHOLD_DATA.array[i].unemployed++;
                        UNEMPLOYED++;
                    }
        
                    if(DAY == 20)
                    {   
                        NO_HOUSEHOLDS_SKILL_1++;
                    }
        
                    REGION_HOUSEHOLD_DATA.array[i].no_households_skill_1++;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_1 +=
                    household_send_data_message->household_info.wage;
        
                    AVERAGE_WAGE_SKILL_1 += household_send_data_message->household_info.wage;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_s_skill_1 += household_send_data_message->household_info.specific_skill;
        
                    AVERAGE_S_SKILL_1 += household_send_data_message->household_info.specific_skill;
                    break;
        
                    case 2:/*General skill level 2*/
                    if(household_send_data_message->household_info.employment_status != -1)
                    {
                        REGION_HOUSEHOLD_DATA.array[i].employed++;
                        REGION_HOUSEHOLD_DATA.array[i].employed_skill_2++;
                        EMPLOYED_SKILL_2++;
                        EMPLOYED++;
                    }
                    else
                    {
                        REGION_HOUSEHOLD_DATA.array[i].unemployed++;
                        UNEMPLOYED++;
                    }       
            
                    if(DAY == 20)
                    {   
                        NO_HOUSEHOLDS_SKILL_2++;
                    }
                    
                    REGION_HOUSEHOLD_DATA.array[i].no_households_skill_2++;
                    
                    REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_2 += household_send_data_message->household_info.wage;
        
                    AVERAGE_WAGE_SKILL_2 += household_send_data_message->household_info.wage;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_s_skill_2 +=  household_send_data_message->household_info.specific_skill;
        
                    AVERAGE_S_SKILL_2 += household_send_data_message->household_info.specific_skill;
                    break;
        
                    case 3:/*General skill level 3*/
                    if(household_send_data_message->household_info.employment_status != -1)
                    {
                        REGION_HOUSEHOLD_DATA.array[i].employed++;
                        REGION_HOUSEHOLD_DATA.array[i].employed_skill_3++;
                        EMPLOYED_SKILL_3++;
                        EMPLOYED++;
                    }
                    else
                    {
                        REGION_HOUSEHOLD_DATA.array[i].unemployed++;
                        UNEMPLOYED++;
                    }
            
                    if(DAY == 20)
                    {
                        NO_HOUSEHOLDS_SKILL_3++;
                    }
        
                    REGION_HOUSEHOLD_DATA.array[i].no_households_skill_3++;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_3 += household_send_data_message->household_info.wage;
        
                    AVERAGE_WAGE_SKILL_3 += household_send_data_message->household_info.wage;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_s_skill_3 += household_send_data_message->household_info.specific_skill;
        
                    AVERAGE_S_SKILL_3 +=
                    household_send_data_message->household_info.specific_skill;
                    break;
        
                    case 4:/*General skill level 4*/
                    if(household_send_data_message->household_info.employment_status != -1)
                    {
                        REGION_HOUSEHOLD_DATA.array[i].employed++;
                        REGION_HOUSEHOLD_DATA.array[i].employed_skill_4++;
                        EMPLOYED_SKILL_4++;
                        EMPLOYED++;
                    }
                    else
                    {
                        REGION_HOUSEHOLD_DATA.array[i].unemployed++;
                        UNEMPLOYED++;
                    }
                    
                    if(DAY == 20)
                    {
                        NO_HOUSEHOLDS_SKILL_4++;
                    }
        
                    REGION_HOUSEHOLD_DATA.array[i].no_households_skill_4++;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_4 += household_send_data_message->household_info.wage;
        
                    AVERAGE_WAGE_SKILL_4 += household_send_data_message->household_info.wage;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_s_skill_4 += household_send_data_message->household_info.specific_skill;
        
                    AVERAGE_S_SKILL_4 += household_send_data_message->household_info.specific_skill;
                    break;
                
                    case 5:/*General skill level 5*/
                    if(household_send_data_message->household_info.employment_status != -1)
                    {
                        REGION_HOUSEHOLD_DATA.array[i].employed++;
                        REGION_HOUSEHOLD_DATA.array[i].employed_skill_5++;
                        EMPLOYED_SKILL_5++;
                        EMPLOYED++;
                    }
                    else
                    {
                        REGION_HOUSEHOLD_DATA.array[i].unemployed++;
                        UNEMPLOYED++;
                    }
        
                    if(DAY == 20)
                    {
                        NO_HOUSEHOLDS_SKILL_5++;
                    }
        
                    REGION_HOUSEHOLD_DATA.array[i].no_households_skill_5++;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_5 += household_send_data_message->household_info.wage;
        
                    AVERAGE_WAGE_SKILL_5 += household_send_data_message->household_info.wage;
        
                    REGION_HOUSEHOLD_DATA.array[i].average_s_skill_5 += household_send_data_message->household_info.specific_skill;
        
                    AVERAGE_S_SKILL_5 += household_send_data_message->household_info.specific_skill;
                    break;
                } //switch

	   		    //Regional data
        		REGION_HOUSEHOLD_DATA.array[i].payment_account              += household_send_data_message->household_info.payment_account;
        		REGION_HOUSEHOLD_DATA.array[i].mean_net_income              += household_send_data_message->household_info.mean_net_income;
        		REGION_HOUSEHOLD_DATA.array[i].total_gross_annual_income    += household_send_data_message->household_info.total_gross_annual_income;
        		REGION_HOUSEHOLD_DATA.array[i].total_gross_quarterly_income += household_send_data_message->household_info.total_gross_quarterly_income;
                                                                                    
        		REGION_HOUSEHOLD_DATA.array[i].consumption_budget           += household_send_data_message->household_info.consumption_budget;
        		REGION_HOUSEHOLD_DATA.array[i].consumption_expenditure      += household_send_data_message->household_info.consumption_expenditure;
        		REGION_HOUSEHOLD_DATA.array[i].monthly_rent_expense         += household_send_data_message->household_info.monthly_rent_expense;
        		REGION_HOUSEHOLD_DATA.array[i].monthly_rent_income          += household_send_data_message->household_info.monthly_rent_income;
        		REGION_HOUSEHOLD_DATA.array[i].monthly_mortgage_payment     += household_send_data_message->household_info.monthly_mortgage_payment;

    			REGION_HOUSEHOLD_DATA.array[i].is_homeless                  += household_send_data_message->household_info.is_homeless;
    			REGION_HOUSEHOLD_DATA.array[i].is_tenant                    += household_send_data_message->household_info.is_tenant;
    			REGION_HOUSEHOLD_DATA.array[i].is_owner                     += household_send_data_message->household_info.is_owner;
    			REGION_HOUSEHOLD_DATA.array[i].is_landlord                  += household_send_data_message->household_info.is_landlord;
    			REGION_HOUSEHOLD_DATA.array[i].has_btl_gene                 += household_send_data_message->household_info.has_btl_gene;

    			REGION_HOUSEHOLD_DATA.array[i].defaulted_on_rent            += household_send_data_message->household_info.defaulted_on_rent;
    			REGION_HOUSEHOLD_DATA.array[i].evicted_as_tenant            += household_send_data_message->household_info.evicted_as_tenant;
    			REGION_HOUSEHOLD_DATA.array[i].evicted_as_owner             += household_send_data_message->household_info.evicted_as_owner;
			
    			//Global data
        		HH_PAYMENT_ACCOUNT              += household_send_data_message->household_info.payment_account;
        		HH_MEAN_NET_INCOME              += household_send_data_message->household_info.mean_net_income;
        		HH_TOTAL_GROSS_ANNUAL_INCOME    += household_send_data_message->household_info.total_gross_annual_income;
        		HH_TOTAL_GROSS_QUARTERLY_INCOME += household_send_data_message->household_info.total_gross_quarterly_income;
                         
        		HH_CONSUMPTION_BUDGET           += household_send_data_message->household_info.consumption_budget;
        		HH_CONSUMPTION_EXPENDITURE      += household_send_data_message->household_info.consumption_expenditure;
        		HH_MONTHLY_RENT_EXPENSE         += household_send_data_message->household_info.monthly_rent_expense;
        		HH_MONTHLY_RENT_INCOME          += household_send_data_message->household_info.monthly_rent_income;
        		HH_MONTHLY_MORTGAGE_PAYMENT     += household_send_data_message->household_info.monthly_mortgage_payment;

    			HH_IS_HOMELESS                  += household_send_data_message->household_info.is_homeless;
    			HH_IS_TENANT                    += household_send_data_message->household_info.is_tenant;
    			HH_IS_OWNER                     += household_send_data_message->household_info.is_owner;
    			HH_IS_LANDLORD                  += household_send_data_message->household_info.is_landlord;
    			HH_HAS_BTL_GENE                 += household_send_data_message->household_info.has_btl_gene;

    			HH_DEFAULTED_ON_RENT 		+= household_send_data_message->household_info.defaulted_on_rent;	
    			HH_EVICTED_AS_TENANT 		+= household_send_data_message->household_info.evicted_as_tenant;
    			HH_EVICTED_AS_OWNER 		+= household_send_data_message->household_info.evicted_as_owner;

                HH_INTEREST_ANNUITY_RATIO       += household_send_data_message->household_info.interest_annuity_ratio;
                HH_HMR_INTEREST_ANNUITY_RATIO   += household_send_data_message->household_info.hmr_interest_annuity_ratio;
                HH_OP_INTEREST_ANNUITY_RATIO    += household_send_data_message->household_info.op_interest_annuity_ratio;

                if (household_send_data_message->household_info.hmr_debt_service_to_income_ratio>=0 && household_send_data_message->household_info.hmr_debt_service_to_income_ratio<=10)
                    HH_HMR_DEBT_SERVICE_TO_INCOME_RATIO += household_send_data_message->household_info.hmr_debt_service_to_income_ratio;
                else
                    dsti_outlier++;
                
                if (household_send_data_message->household_info.rent_to_income_ratio>=0 && household_send_data_message->household_info.rent_to_income_ratio<10)
                    HH_RENT_TO_INCOME_RATIO += household_send_data_message->household_info.rent_to_income_ratio;
                else
                    rti_outlier++;

                HH_PAYMENT_ACCOUNT_TO_INCOME_RATIO += household_send_data_message->household_info.payment_account_to_income_ratio;
                HH_HMR_DEBT_SERVICE_BUFFER_IN_MONTHS += household_send_data_message->household_info.hmr_debt_service_buffer_in_months;
                HH_RENT_BUFFER_IN_MONTHS += household_send_data_message->household_info.rent_buffer_in_months;

                HH_OP_INCOME += household_send_data_message->household_info.op_income;
                HH_OP_EXPENSE += household_send_data_message->household_info.op_expense;
                HH_OP_PROFIT +=  household_send_data_message->household_info.op_profit;
                HH_SHARE_OP_WITH_MORTGAGE += household_send_data_message->household_info.share_op_with_mortgage;

                HH_MORTGAGES_LTV_REDRAW      += household_send_data_message->household_info.hh_mortgages_ltv_redraw;
                HH_MORTGAGES_LTV_CAP_ACTIVE  += household_send_data_message->household_info.hh_mortgages_ltv_cap_active;
                HH_MORTGAGES_DSTI_CAP_ACTIVE += household_send_data_message->household_info.hh_mortgages_dsti_cap_active;

                HH_NO_MORTGAGES             += household_send_data_message->household_info.hh_no_mortgages;

                HH_MORTGAGES_REQUESTED    	 += household_send_data_message->household_info.hh_mortgages_requested;
                HH_MORTGAGES_ACCEPTED     	 += household_send_data_message->household_info.hh_mortgages_accepted;
                HH_MORTGAGES_PAYING          += household_send_data_message->household_info.hh_mortgages_paying;
                HH_MORTGAGES_ARREARS         += household_send_data_message->household_info.hh_mortgages_arrears;
                HH_MORTGAGES_NPL             += household_send_data_message->household_info.hh_mortgages_npl;
                HH_MORTGAGES_WRITEOFF        += household_send_data_message->household_info.hh_mortgages_writeoff;
                HH_MORTGAGES_PAIDOFF         += household_send_data_message->household_info.hh_mortgages_paidoff;

                HH_MORTGAGES_HMR_REQUESTED   += household_send_data_message->household_info.hh_mortgages_hmr_requested;
                HH_MORTGAGES_HMR_ACCEPTED    += household_send_data_message->household_info.hh_mortgages_hmr_accepted;
                HH_MORTGAGES_HMR_PAYING      += household_send_data_message->household_info.hh_mortgages_hmr_paying;
                HH_MORTGAGES_HMR_ARREARS     += household_send_data_message->household_info.hh_mortgages_hmr_arrears;
                HH_MORTGAGES_HMR_NPL         += household_send_data_message->household_info.hh_mortgages_hmr_npl;
                HH_MORTGAGES_HMR_WRITEOFF    += household_send_data_message->household_info.hh_mortgages_hmr_writeoff;
                HH_MORTGAGES_HMR_PAIDOFF     += household_send_data_message->household_info.hh_mortgages_hmr_paidoff;
            
                HH_MORTGAGES_OP_REQUESTED    += household_send_data_message->household_info.hh_mortgages_op_requested;
                HH_MORTGAGES_OP_ACCEPTED     += household_send_data_message->household_info.hh_mortgages_op_accepted;
                HH_MORTGAGES_OP_PAYING       += household_send_data_message->household_info.hh_mortgages_op_paying;
                HH_MORTGAGES_OP_ARREARS      += household_send_data_message->household_info.hh_mortgages_op_arrears;
                HH_MORTGAGES_OP_NPL          += household_send_data_message->household_info.hh_mortgages_op_npl;
                HH_MORTGAGES_OP_WRITEOFF     += household_send_data_message->household_info.hh_mortgages_op_writeoff;
                HH_MORTGAGES_OP_PAIDOFF      += household_send_data_message->household_info.hh_mortgages_op_paidoff;

                add_double(&CONSUMPTION_LEVELS, household_send_data_message->household_info.consumption_expenditure);    

                /* 
                * Household classifications:
                * n1 = employed + mortgage
                * n2 = employed + no mortgage
                * n3 = unemployed + mortgage
                * n4 = unemployed + no mortgage
                */                
                if(household_send_data_message->household_info.employment_status != -1 && household_send_data_message->household_info.hh_mortgages_paying > 0)
                    NO_HOUSEHOLD_CLASS_1 = NO_HOUSEHOLD_CLASS_1+1;
                if(household_send_data_message->household_info.employment_status != -1 && household_send_data_message->household_info.hh_mortgages_paying == 0)
                    NO_HOUSEHOLD_CLASS_2 = NO_HOUSEHOLD_CLASS_2+1;
                if(household_send_data_message->household_info.employment_status == -1 && household_send_data_message->household_info.hh_mortgages_paying > 0)
                    NO_HOUSEHOLD_CLASS_3 = NO_HOUSEHOLD_CLASS_3+1;
                if(household_send_data_message->household_info.employment_status == -1 && household_send_data_message->household_info.hh_mortgages_paying == 0)
                    NO_HOUSEHOLD_CLASS_4 = NO_HOUSEHOLD_CLASS_4+1;

            }
        }
    FINISH_HOUSEHOLD_SEND_DATA_MESSAGE_LOOP
    
    if (JUST_EMPLOYED > 0)
        UNEMPLOYED_DURATION = UNEMPLOYED_DURATION / JUST_EMPLOYED;

    if (HH_IS_TENANT - rti_outlier>0)
        HH_RENT_TO_INCOME_RATIO = HH_RENT_TO_INCOME_RATIO / (HH_IS_TENANT - rti_outlier);
    
    if ((HH_MORTGAGES_HMR_PAYING + HH_MORTGAGES_HMR_ARREARS + HH_MORTGAGES_HMR_NPL)-dsti_outlier>0)
        HH_HMR_DEBT_SERVICE_TO_INCOME_RATIO = HH_HMR_DEBT_SERVICE_TO_INCOME_RATIO / (HH_MORTGAGES_HMR_PAYING + HH_MORTGAGES_HMR_ARREARS + HH_MORTGAGES_HMR_NPL - dsti_outlier);
    
    HH_PAYMENT_ACCOUNT_TO_INCOME_RATIO = HH_PAYMENT_ACCOUNT_TO_INCOME_RATIO / NUM_HOUSEHOLDS;

    HH_INTEREST_ANNUITY_RATIO = HH_INTEREST_ANNUITY_RATIO / NUM_HOUSEHOLDS;
    HH_HMR_INTEREST_ANNUITY_RATIO = HH_HMR_INTEREST_ANNUITY_RATIO / NUM_HOUSEHOLDS;
    HH_OP_INTEREST_ANNUITY_RATIO = HH_OP_INTEREST_ANNUITY_RATIO / NUM_HOUSEHOLDS;

    if ((HH_MORTGAGES_HMR_PAYING + HH_MORTGAGES_HMR_ARREARS + HH_MORTGAGES_HMR_NPL)>0)
        HH_HMR_DEBT_SERVICE_BUFFER_IN_MONTHS = HH_HMR_DEBT_SERVICE_BUFFER_IN_MONTHS / (HH_MORTGAGES_HMR_PAYING + HH_MORTGAGES_HMR_ARREARS + HH_MORTGAGES_HMR_NPL);
    
    if (HH_IS_TENANT>0)
        HH_RENT_BUFFER_IN_MONTHS = HH_RENT_BUFFER_IN_MONTHS / HH_IS_TENANT;

    if (HH_IS_LANDLORD>0)
    {
        RETURN_ON_OP = HH_OP_INCOME / HH_OP_EXPENSE * 100;

        HH_OP_INCOME = HH_OP_INCOME / HH_IS_LANDLORD;
        HH_OP_EXPENSE = HH_OP_EXPENSE / HH_IS_LANDLORD;
        HH_OP_PROFIT = HH_OP_PROFIT /HH_IS_LANDLORD;
        HH_SHARE_OP_WITH_MORTGAGE =  HH_SHARE_OP_WITH_MORTGAGE / HH_IS_LANDLORD;
    }

}

/* \fn: void Eurostat_compute_subsistence_level(void)
 * \brief: Function to compute the minimum subsistence consumption level
 * \brief: Used in: Eurostat_calculate_data()
 * \brief: Frequency: monthly, phase 0 (on last day, ie 20, 40, etc.) AND switch_datastorage=1 in Eurostat
 */
void Eurostat_compute_subsistence_level(void)
{   
    int i;

    // Find median of this month's population distribution
    MEDIAN_CONSUMPTION_LEVEL = my_stats_median(&CONSUMPTION_LEVELS);

    //Add to history array (fixed size of 12 months)
    add_double(&HISTORY_MEDIAN_CONSUMPTION_LEVELS, MEDIAN_CONSUMPTION_LEVEL);

    //Check size: only start with removing first item once size has reached 12
    if(HISTORY_MEDIAN_CONSUMPTION_LEVELS.size>12)
        remove_double(&HISTORY_MEDIAN_CONSUMPTION_LEVELS, 0);

    // Find median of the history array
    MEDIAN_MEDIAN_CONSUMPTION_LEVEL = my_stats_median(&HISTORY_MEDIAN_CONSUMPTION_LEVELS);

    MINIMUM_SUBSISTENCE_LEVEL = MEDIAN_MEDIAN_CONSUMPTION_LEVEL * CONST_MINIMUM_CONSUMPTION_OF_MEDIAN;

    #ifdef _DEBUG_MODE
    if (PRINT_DEBUG_EUROSTAT)
    {
        printf("\n\n Eurostat_compute_subsistence_level():");
        printf("\n\n MEDIAN_CONSUMPTION_LEVEL\t%f", MEDIAN_CONSUMPTION_LEVEL);
        printf("\n\n HISTORY_MEDIAN_CONSUMPTION_LEVELS: size=%d\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.size);
        for (i=0; i<HISTORY_MEDIAN_CONSUMPTION_LEVELS.size; i++)
            printf(" %f", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[i]);
        printf("\n\n MEDIAN_MEDIAN_CONSUMPTION_LEVEL\t0.5*(item[5]+item[6])=0.5*(%2.2f+%2.2f) = %2.2f", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[5], HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[6], MEDIAN_MEDIAN_CONSUMPTION_LEVEL);
        printf("\n\n MINIMUM_SUBSISTENCE_LEVEL\t%2.2f*%2.2f=%2.2f", MEDIAN_MEDIAN_CONSUMPTION_LEVEL, CONST_MINIMUM_CONSUMPTION_OF_MEDIAN, MINIMUM_SUBSISTENCE_LEVEL);
    }
    #endif
}


/* \fn: void Eurostat_compute_region_household_data(void)
 * \brief: Function to create the region data related to Households.
 */
void Eurostat_compute_region_household_data(void)
{
    int i;
    
    /*Create the REGIONAL data which is needed for controlling the results or sending
     *            back to the Households*/
    for(i = 0; i < REGION_HOUSEHOLD_DATA.size; i++)
    {

        /**************UNEMPLOYMENT-RATE******************/
        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate = 
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households);

        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate_skill_1 =
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed_skill_1/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households_skill_1);

        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate_skill_2 =
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed_skill_2/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households_skill_2);

        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate_skill_3 =
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed_skill_3/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households_skill_3);

        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate_skill_4 =
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed_skill_4/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households_skill_4);

        REGION_HOUSEHOLD_DATA.array[i].unemployment_rate_skill_5 =
            100*(1 - (double)REGION_HOUSEHOLD_DATA.array[i].employed_skill_5/
        (double)REGION_HOUSEHOLD_DATA.array[i].no_households_skill_5);
    

    
        /*******************WAGES*************************/
        if(REGION_HOUSEHOLD_DATA.array[i].employed > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage/
            REGION_HOUSEHOLD_DATA.array[i].employed;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage = 0;
        }

        if(REGION_HOUSEHOLD_DATA.array[i].employed_skill_1 > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_1 = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_1/
            REGION_HOUSEHOLD_DATA.array[i].employed_skill_1;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_1 = 0;
        }   

        if(REGION_HOUSEHOLD_DATA.array[i].employed_skill_2 > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_2 = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_2/
            REGION_HOUSEHOLD_DATA.array[i].employed_skill_2;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_2 = 0;
        }

        if(REGION_HOUSEHOLD_DATA.array[i].employed_skill_3 > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_3 = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_3/
            REGION_HOUSEHOLD_DATA.array[i].employed_skill_3;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_3 = 0;
        }

        if(REGION_HOUSEHOLD_DATA.array[i].employed_skill_4 > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_4 = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_4/
            REGION_HOUSEHOLD_DATA.array[i].employed_skill_4;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_4 = 0;
        }

        if(REGION_HOUSEHOLD_DATA.array[i].employed_skill_5 > 0)
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_5 = 
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_5/
            REGION_HOUSEHOLD_DATA.array[i].employed_skill_5;
        }
        else
        {
            REGION_HOUSEHOLD_DATA.array[i].average_wage_skill_5 = 0;
        }

        /*******************SPECIFIC SKILLS**********************/
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill = 
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill/
        REGION_HOUSEHOLD_DATA.array[i].no_households;

        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_1 =
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_1/
        REGION_HOUSEHOLD_DATA.array[i].no_households_skill_1;

        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_2 =
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_2/
        REGION_HOUSEHOLD_DATA.array[i].no_households_skill_2;

        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_3 =
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_3/
        REGION_HOUSEHOLD_DATA.array[i].no_households_skill_3;

        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_4 =
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_4/
        REGION_HOUSEHOLD_DATA.array[i].no_households_skill_4;
    
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_5 =
        REGION_HOUSEHOLD_DATA.array[i].average_s_skill_5/
        REGION_HOUSEHOLD_DATA.array[i].no_households_skill_5;   
    }
}
    
/* \fn: void Eurostat_compute_global_household_data(void)
 * \brief: Function to compute the global macro data related to households.
 */
void Eurostat_compute_global_household_data(void)
{
    /*Create the GLOBAL data which is needed for controlling the results or sending back to Households
    */
    
    /*******************UNEMPLOYMENT RATE**********************/
    UNEMPLOYMENT_RATE = 100*(1 - (double)EMPLOYED/(double)NUM_HOUSEHOLDS);
	EMPLOYMENT_RATE = 100-UNEMPLOYMENT_RATE;
    UNEMPLOYMENT_RATE_SKILL_1 = 100*(1 - (double)EMPLOYED_SKILL_1/
                    (double)NO_HOUSEHOLDS_SKILL_1);
    UNEMPLOYMENT_RATE_SKILL_2 = 100*(1 - (double)EMPLOYED_SKILL_2/
                    (double)NO_HOUSEHOLDS_SKILL_2);
    UNEMPLOYMENT_RATE_SKILL_3 = 100*(1 - (double)EMPLOYED_SKILL_3/
                    (double)NO_HOUSEHOLDS_SKILL_3);
    UNEMPLOYMENT_RATE_SKILL_4 = 100*(1 - (double)EMPLOYED_SKILL_4/
                    (double)NO_HOUSEHOLDS_SKILL_4);
    UNEMPLOYMENT_RATE_SKILL_5 = 100*(1 - (double)EMPLOYED_SKILL_5/
                    (double)NO_HOUSEHOLDS_SKILL_5);

    /*******************WAGES**********************/
    
    if(EMPLOYED > 0)
    {
        AVERAGE_WAGE = AVERAGE_WAGE/(double)EMPLOYED;
    }
    else
    {
        AVERAGE_WAGE = 0;
    }
    
    if(EMPLOYED_SKILL_1 > 0)
    {
        AVERAGE_WAGE_SKILL_1 = AVERAGE_WAGE_SKILL_1/(double)EMPLOYED_SKILL_1;
    }
    else
    {
        AVERAGE_WAGE_SKILL_1 = 0;
    }
    
    if(EMPLOYED_SKILL_2 > 0)
    {
        AVERAGE_WAGE_SKILL_2 = AVERAGE_WAGE_SKILL_2/(double)EMPLOYED_SKILL_2;
    }
    else
    {
        AVERAGE_WAGE_SKILL_2 = 0;
    }
    
    if(EMPLOYED_SKILL_3 > 0)
    {
        AVERAGE_WAGE_SKILL_3 = AVERAGE_WAGE_SKILL_3/(double)EMPLOYED_SKILL_3;
    }
    else
    {
        AVERAGE_WAGE_SKILL_3 = 0;
    }
    
    if(EMPLOYED_SKILL_4 > 0)
    {
        AVERAGE_WAGE_SKILL_4 = AVERAGE_WAGE_SKILL_4/(double)EMPLOYED_SKILL_4;
    }
    else
    {
        AVERAGE_WAGE_SKILL_4 = 0;
    }
    
    if(EMPLOYED_SKILL_5 > 0)
    {
        AVERAGE_WAGE_SKILL_5 = AVERAGE_WAGE_SKILL_5/(double)EMPLOYED_SKILL_5;
    }
    else
    {
        AVERAGE_WAGE_SKILL_5 = 0;
    }
    
    /*******************SPECIFIC SKILLS**********************/
    AVERAGE_S_SKILL   = AVERAGE_S_SKILL/(double)NUM_HOUSEHOLDS;
    AVERAGE_S_SKILL_1 = AVERAGE_S_SKILL_1/(double)NO_HOUSEHOLDS_SKILL_1;
    AVERAGE_S_SKILL_2 = AVERAGE_S_SKILL_2/(double)NO_HOUSEHOLDS_SKILL_2;
    AVERAGE_S_SKILL_3 = AVERAGE_S_SKILL_3/(double)NO_HOUSEHOLDS_SKILL_3;
    AVERAGE_S_SKILL_4 = AVERAGE_S_SKILL_4/(double)NO_HOUSEHOLDS_SKILL_4;
    AVERAGE_S_SKILL_5 = AVERAGE_S_SKILL_5/(double)NO_HOUSEHOLDS_SKILL_5;

    /*******************HOUSEHOLD CLASSES: PROPENSITIES TO BE (UN)EMPLOYED AND (NOT)HAVE A MORTGAGE **********************/
    /* 
     * Household classifications:
     * n1 = employed + mortgage
     * n2 = employed + no mortgage
     * n3 = unemployed + mortgage
     * n4 = unemployed + no mortgage
     */                

    HOUSEHOLD_PROPENSITY_1 = 0.0;
    HOUSEHOLD_PROPENSITY_2 = 0.0;
    HOUSEHOLD_PROPENSITY_3 = 0.0;
    HOUSEHOLD_PROPENSITY_4 = 0.0;

    if(NO_HOUSEHOLD_CLASS_1+NO_HOUSEHOLD_CLASS_2>0) HOUSEHOLD_PROPENSITY_1 = ((double)NO_HOUSEHOLD_CLASS_1)/(NO_HOUSEHOLD_CLASS_1+NO_HOUSEHOLD_CLASS_2);
    if(NO_HOUSEHOLD_CLASS_1+NO_HOUSEHOLD_CLASS_2>0) HOUSEHOLD_PROPENSITY_2 = ((double)NO_HOUSEHOLD_CLASS_2)/(NO_HOUSEHOLD_CLASS_1+NO_HOUSEHOLD_CLASS_2);
    if(NO_HOUSEHOLD_CLASS_3+NO_HOUSEHOLD_CLASS_4>0) HOUSEHOLD_PROPENSITY_3 = ((double)NO_HOUSEHOLD_CLASS_3)/(NO_HOUSEHOLD_CLASS_3+NO_HOUSEHOLD_CLASS_4);
    if(NO_HOUSEHOLD_CLASS_3+NO_HOUSEHOLD_CLASS_4>0) HOUSEHOLD_PROPENSITY_4 = ((double)NO_HOUSEHOLD_CLASS_4)/(NO_HOUSEHOLD_CLASS_3+NO_HOUSEHOLD_CLASS_4);
}
    
/* \fn: void Eurostat_calc_macro_data(void)
 * \brief: Function to compute macro data from firm micro data.
 */
void Eurostat_calc_macro_data(void)
{
    int i;
    FILE *file1;
     char *filename;

    //Auxiliary sums:
    double sum_total_debt_earnings_ratios = 0.0;
    double sum_total_debt_equity_ratios = 0.0;
    double sum_total_labour_share_ratios = 0.0;

    double sum_total_sold_quantity = 0.0;
    double sum_total_output = 0.0;
    double sum_total_cum_revenue = 0.0;
    double sum_total_planned_output = 0.0;
    
    double sum_region_debt_earnings_ratios = 0.0;
    double sum_region_debt_equity_ratios = 0.0;
    double sum_region_labour_share_ratios = 0.0;
    
    double sum_region_sold_quantity = 0.0;
    double sum_region_output = 0.0;
    double sum_region_cum_revenue = 0.0;
    double sum_region_planned_output = 0.0;
        
    int counter_firms_in_region;
    
    GDP = 0.0;
    TOTAL_EARNINGS = 0.0;

    FIRM_CREDIT = 0.0;
    FIRM_ASSETS = 0.0;
    FIRM_EQUITY = 0.0;

    AVERAGE_DEBT_EARNINGS_RATIO = 0.0;
    AVERAGE_DEBT_EQUITY_RATIO = 0.0;
    LABOUR_SHARE_RATIO = 0.0;

    MONTHLY_SOLD_QUANTITY = 0.0;
    MONTHLY_OUTPUT = 0.0;
    MONTHLY_REVENUE = 0.0;
    MONTHLY_PLANNED_OUTPUT = 0.0;
    MONTHLY_INVESTMENT_VALUE =0.0;

	
	//Household gross annual income: 12-month trailing sum, converted to proxy for net annual income by tax rate
	//double hh_total_net_annual_income_proxy = (1-TAX_RATE_HH_LABOUR)*HH_TOTAL_GROSS_ANNUAL_INCOME;

	// HH_DEBT_TO_INCOME_RATIO: sum of HH TOTAL_MORTGAGE_CREDIT (current stock) / annual gross income (12-month trailing sum)
	HH_DEBT_TO_INCOME_RATIO = MORTGAGE_CREDIT / HH_TOTAL_GROSS_ANNUAL_INCOME;

	//HH_DEBT_SERVICE_TO_INCOME_RATIO: MORTGAGE_PAYMENT (current flow) / annual gross income (12-month trailing sum)
	HH_DEBT_SERVICE_TO_INCOME_RATIO =  12.0*HH_MONTHLY_MORTGAGE_PAYMENT / HH_TOTAL_GROSS_ANNUAL_INCOME;

	//HH_HOUSING_SERVICE_TO_INCOME_RATIO: MONTHLY_RENT_EXPENSE (current flow) / annual gross income (12-month trailing sum)
	HH_HOUSING_SERVICE_TO_INCOME_RATIO = 12.0*HH_MONTHLY_RENT_EXPENSE / HH_TOTAL_GROSS_ANNUAL_INCOME;
 
	/*****************LABOR MARKET FLOWS****************/

	if(START_EMPLOYED > 0)
	{
		JOB_SEPERATION_RATE = (double)JUST_UNEMPLOYED/(double)START_EMPLOYED;
	}
	else
	{
		JOB_SEPERATION_RATE = 0.0;
	}

	if(ENTER_MATCHING > 0)
	{
		JOB_FINDING_RATE = (double)JUST_EMPLOYED/(double)ENTER_MATCHING;
	}
	else
	{
		JOB_FINDING_RATE = 0.0;
	}

	if(UNEMPLOYED > 0)
	{	
		V_U_RATIO = (double)NO_VACANCIES/(double)UNEMPLOYED;
	}
	else
	{
		V_U_RATIO = 0.0;
	}

	if(NO_POSTED_VACANCIES > 0)
	{
		MATCHING_RATE = (double)JUST_EMPLOYED/(double)NO_POSTED_VACANCIES;
	}
	else
	{
		MATCHING_RATE = 0.0;
	}



    /*Store the region data of the firms*/
    for(i = 0; i < REGION_FIRM_DATA.size; i++)
    {
//      printf("\n Entering region %d", i+1);
        
        counter_firms_in_region =0;
        
        //Reset region sums: these are updated to sum across all firms in the region
        sum_region_debt_earnings_ratios = 0.0;
        sum_region_debt_equity_ratios   = 0.0;
        sum_region_labour_share_ratios  = 0.0;
        sum_region_sold_quantity        = 0.0;
        sum_region_output               = 0.0;
        sum_region_cum_revenue          = 0.0;
        sum_region_planned_output       = 0.0;

        //Reading loop
        START_FIRM_SEND_DATA_MESSAGE_LOOP
        if(firm_send_data_message->firm_info.region_id == REGION_FIRM_DATA.array[i].region_id)
        {
            counter_firms_in_region++;

            /********sum of GDP: total consumption and investment costs++++++++*/
            REGION_FIRM_DATA.array[i].gdp += firm_send_data_message->firm_info.cum_revenue + firm_send_data_message->firm_info.capital_costs;
            GDP += firm_send_data_message->firm_info.cum_revenue + firm_send_data_message->firm_info.capital_costs;

            /********sum of: total investment costs++++++++*/
            REGION_FIRM_DATA.array[i].monthly_investment_value += firm_send_data_message->firm_info.capital_costs;
            MONTHLY_INVESTMENT_VALUE += firm_send_data_message->firm_info.capital_costs;
            
            /********sum of net earnings of the firms++++++++*/
            REGION_FIRM_DATA.array[i].total_earnings += firm_send_data_message->firm_info.net_earnings;
            TOTAL_EARNINGS += firm_send_data_message->firm_info.net_earnings;
            
            
            /********sum of total debt of the firms++++++++*/
            REGION_FIRM_DATA.array[i].total_debt += firm_send_data_message->firm_info.total_debt;
            FIRM_CREDIT += firm_send_data_message->firm_info.total_debt;
            
            /********sum of total assets of the firms++++++++*/
            REGION_FIRM_DATA.array[i].total_assets += firm_send_data_message->firm_info.total_assets;
            FIRM_ASSETS += firm_send_data_message->firm_info.total_assets;
            
            
            /********sum of total equity of the firms++++++++*/
            REGION_FIRM_DATA.array[i].total_equity += firm_send_data_message->firm_info.equity;
            FIRM_EQUITY += firm_send_data_message->firm_info.equity;
            
            /***************** average debt/earnings ratio *********************/
            if (firm_send_data_message->firm_info.net_earnings>0.0)
            {
                sum_region_debt_earnings_ratios += firm_send_data_message->firm_info.total_debt/firm_send_data_message->firm_info.net_earnings;
                sum_total_debt_earnings_ratios += firm_send_data_message->firm_info.total_debt/firm_send_data_message->firm_info.net_earnings;
            }
            
            /***************** average debt/equity ratio *********************/
            if (firm_send_data_message->firm_info.equity>0.0)
            {
                sum_region_debt_equity_ratios += firm_send_data_message->firm_info.total_debt/firm_send_data_message->firm_info.equity;
                sum_total_debt_equity_ratios += firm_send_data_message->firm_info.total_debt/firm_send_data_message->firm_info.equity;
            }
            
            /***************** average labour share *********************/
            if (firm_send_data_message->firm_info.net_earnings>0.0)
            {               
                sum_region_labour_share_ratios += firm_send_data_message->firm_info.total_mean_wage*firm_send_data_message->firm_info.no_employees/firm_send_data_message->firm_info.net_earnings;
                sum_total_labour_share_ratios += firm_send_data_message->firm_info.total_mean_wage*firm_send_data_message->firm_info.no_employees/firm_send_data_message->firm_info.net_earnings;
            }
            
            /***************** Sum of: total_sold_quantity *********************/
            sum_region_sold_quantity += firm_send_data_message->firm_info.sold_quantity_in_calendar_month;
            sum_total_sold_quantity  += firm_send_data_message->firm_info.sold_quantity_in_calendar_month;
    
            /***************** Sum of: output *********************/
            //printf("Eurostat reads output: %.2f", firm_send_data_message->firm_info.output);
            sum_region_output += firm_send_data_message->firm_info.output;
            sum_total_output  += firm_send_data_message->firm_info.output;
            
            /***************** Sum of: cum_revenue *********************/                                 
            sum_region_cum_revenue += firm_send_data_message->firm_info.cum_revenue;
            sum_total_cum_revenue  += firm_send_data_message->firm_info.cum_revenue;
            
            /***************** Sum of: planned_output *********************/
            //printf("Eurostat reads planned_output: %.2f\n", firm_send_data_message->firm_info.planned_output);
            sum_region_planned_output += firm_send_data_message->firm_info.planned_output;
            sum_total_planned_output  += firm_send_data_message->firm_info.planned_output;
            
        }
        FINISH_FIRM_SEND_DATA_MESSAGE_LOOP
        
        //Compute regional averages after the regional-specific message loop
        REGION_FIRM_DATA.array[i].average_debt_earnings_ratio = sum_region_debt_earnings_ratios/counter_firms_in_region;
        REGION_FIRM_DATA.array[i].average_debt_equity_ratio = sum_region_debt_equity_ratios/counter_firms_in_region;
        REGION_FIRM_DATA.array[i].labour_share_ratio = sum_region_labour_share_ratios/counter_firms_in_region;
        
        REGION_FIRM_DATA.array[i].monthly_sold_quantity = sum_region_sold_quantity;
        REGION_FIRM_DATA.array[i].monthly_output = sum_region_output;
        REGION_FIRM_DATA.array[i].monthly_revenue = sum_region_cum_revenue;
        REGION_FIRM_DATA.array[i].monthly_planned_output = sum_region_planned_output;
        REGION_FIRM_DATA.array[i].investment_gdp_ratio = REGION_FIRM_DATA.array[i].monthly_investment_value/REGION_FIRM_DATA.array[i].gdp;
    }
    
    //Compute total averages after the region for-loop
    AVERAGE_DEBT_EARNINGS_RATIO = sum_total_debt_earnings_ratios/NO_FIRMS;
    AVERAGE_DEBT_EQUITY_RATIO = sum_total_debt_equity_ratios/NO_FIRMS;
    LABOUR_SHARE_RATIO  = sum_total_labour_share_ratios/NO_FIRMS;

    MONTHLY_SOLD_QUANTITY = sum_total_sold_quantity;
    MONTHLY_OUTPUT = sum_total_output;
    MONTHLY_REVENUE = sum_total_cum_revenue;
    MONTHLY_PLANNED_OUTPUT = sum_total_planned_output;

    INVESTMENT_GDP_RATIO = MONTHLY_INVESTMENT_VALUE/GDP;

    CAPACITY_UTILIZATION_RATE = MONTHLY_OUTPUT/MONTHLY_POTENTIAL_OUTPUT;

	HERFINDAHL_INDEX =0.0;

	START_FIRM_SEND_DATA_MESSAGE_LOOP

	   HERFINDAHL_INDEX += pow(firm_send_data_message->firm_info.sold_quantity_in_calendar_month/MONTHLY_SOLD_QUANTITY,2);

	FINISH_FIRM_SEND_DATA_MESSAGE_LOOP
	 
    //Bank data: send on day 20 of month
    double bank_equity_asset_ratio=0.0;
    double bank_equity_rea_ratio=0.0;
    int no_banks=0;
    int no_active_banks = 0;
    double sum_wacf = 0.0;
    double sum_funding_costs = 0.0;

    //Bank variables: reset counters to zero before summing below
    BANK_EQUITY = 0.0;
    BANK_ASSETS = 0.0;
    BANK_REA = 0.0;
    TOTAL_BANK_ECB_DEBT = 0.0;
    TOTAL_BANK_LLR = 0.0;
    
    BANK_TOTAL_MORTGAGE_LOANS = 0.0;
    BANK_PL_MORTGAGE_LOANS = 0.0;
    BANK_NPL_MORTGAGE_LOANS = 0.0;

	BANK_PL_MORTGAGE_LOANS_OP = 0.0;
	BANK_NPL_MORTGAGE_LOANS_OP = 0.0;
	BANK_PL_MORTGAGE_LOANS_HMR = 0.0;
	BANK_NPL_MORTGAGE_LOANS_HMR = 0.0;

    BANK_TOTAL_FIRM_LOANS = 0.0;
    BANK_PL_FIRM_LOANS = 0.0;
    BANK_NPL_FIRM_LOANS = 0.0;

    BANK_NO_MORTGAGES = 0;
    BANK_NO_MORTGAGES_NEW = 0;
    BANK_MORTGAGES_NEW_BUSINESS = 0.0;

    BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_MORTGAGES = 0.0;

    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = 0.0;
    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = 0.0;
    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = 0.0;

    BANK_MORTGAGES_PAYING = 0;
    BANK_MORTGAGES_ARREARS = 0;
    BANK_MORTGAGES_NPL = 0;
    BANK_MORTGAGES_WRITEOFF = 0;
    BANK_MORTGAGES_PAID_OFF = 0;
    BANK_MORTGAGES_REQUESTED = 0;
    
    CREDIT_TO_GDP_RATIO = 0.0;

    BANK_NO_FIRM_LOANS = 0;
    BANK_NO_FIRM_LOANS_NEW = 0;
    BANK_FIRM_LOANS_NEW_BUSINESS = 0.0;

    BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_FIRM_LOANS = 0.0;

    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = 0.0;
    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = 0.0;
    BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = 0.0;

    BANK_NO_FIRM_LOANS_PAYING = 0;
    BANK_NO_FIRM_LOANS_WRITEOFF = 0;
    BANK_DEPOSIT_INTEREST_RATE = 0.0;

	TOTAL_DEPOSITS = 0.0;

    BANK_RISK_WEIGHTED_CAPITAL_RATIO_ACTIVE = 0.0;

    START_BANK_SEND_DATA_MESSAGE_LOOP
        no_banks++;

        BANK_EQUITY += bank_send_data_message->financial_data.equity;
        BANK_ASSETS += bank_send_data_message->financial_data.total_assets;
        BANK_REA    += bank_send_data_message->financial_data.rea;
        TOTAL_BANK_ECB_DEBT += bank_send_data_message->financial_data.total_debt;
        TOTAL_BANK_LLR += bank_send_data_message->financial_data.llr;

        if (bank_send_data_message->financial_data.active)
        {
            no_active_banks++;
            BANK_RISK_WEIGHTED_CAPITAL_RATIO_ACTIVE += bank_send_data_message->financial_data.risk_weighted_capital_ratio;
        }
        
        //Disaggregate bank ratios
        bank_equity_asset_ratio = BANK_EQUITY/BANK_ASSETS;
        bank_equity_rea_ratio = BANK_EQUITY/BANK_REA;

        BANK_TOTAL_MORTGAGE_LOANS += bank_send_data_message->financial_data.total_mortgage_loans;

        BANK_PL_MORTGAGE_LOANS += bank_send_data_message->financial_data.pl_mortgage_loans;
        BANK_NPL_MORTGAGE_LOANS += bank_send_data_message->financial_data.npl_mortgage_loans;

		BANK_PL_MORTGAGE_LOANS_OP += bank_send_data_message->financial_data.pl_mortgage_loans_op;
		BANK_NPL_MORTGAGE_LOANS_OP += bank_send_data_message->financial_data.npl_mortgage_loans_op;
		BANK_PL_MORTGAGE_LOANS_HMR += bank_send_data_message->financial_data.pl_mortgage_loans_hmr;
		BANK_NPL_MORTGAGE_LOANS_HMR += bank_send_data_message->financial_data.npl_mortgage_loans_hmr;

        BANK_PL_FIRM_LOANS += bank_send_data_message->financial_data.pl_firm_loans;
        BANK_NPL_FIRM_LOANS += bank_send_data_message->financial_data.npl_firm_loans;

        BANK_NO_MORTGAGES += bank_send_data_message->financial_data.bank_no_mortgages;

        BANK_NO_MORTGAGES_NEW += bank_send_data_message->financial_data.bank_no_mortgages_new;
        BANK_MORTGAGES_NEW_BUSINESS += bank_send_data_message->financial_data.bank_mortgages_new_business;
        
        BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_MORTGAGES += bank_send_data_message->financial_data.weighted_sum_interest_rates_new_mortgages;
        BANK_SUM_WEIGHTED_INTEREST_RATES_ALL_MORTGAGES += bank_send_data_message->financial_data.weighted_sum_interest_rates_all_mortgages;

        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_mortgages_funding_costs;
        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_mortgages_policy_rate;
        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_mortgages_wacf;

		BANK_MORTGAGES_REQUESTED += bank_send_data_message->financial_data.bank_mortgages_requested;

        BANK_MORTGAGES_PAYING += bank_send_data_message->financial_data.bank_mortgages_paying;
        BANK_MORTGAGES_ARREARS += bank_send_data_message->financial_data.bank_mortgages_arrears;
        BANK_MORTGAGES_NPL += bank_send_data_message->financial_data.bank_mortgages_npl;
        BANK_MORTGAGES_WRITEOFF += bank_send_data_message->financial_data.bank_mortgages_writeoff;
        BANK_MORTGAGES_PAID_OFF += bank_send_data_message->financial_data.bank_mortgages_paid_off;
        
        BANK_TOTAL_FIRM_LOANS += bank_send_data_message->financial_data.total_firm_loans;

        BANK_NO_FIRM_LOANS += bank_send_data_message->financial_data.bank_no_firm_loans;
        BANK_NO_FIRM_LOANS_NEW  += bank_send_data_message->financial_data.bank_no_firm_loans_new;
        BANK_FIRM_LOANS_NEW_BUSINESS  += bank_send_data_message->financial_data.bank_firm_loans_new_business;
        
        BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_FIRM_LOANS  += bank_send_data_message->financial_data.weighted_sum_interest_rates_new_firm_loans;
        BANK_SUM_WEIGHTED_INTEREST_RATES_ALL_FIRM_LOANS  += bank_send_data_message->financial_data.weighted_sum_interest_rates_all_firm_loans;

        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_funding_costs;
        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_policy_rate;
        BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF += bank_send_data_message->financial_data.weighted_sum_interest_rate_spreads_new_firm_loans_wacf;

        BANK_NO_FIRM_LOANS_PAYING += bank_send_data_message->financial_data.bank_no_firm_loans_paying;
        BANK_NO_FIRM_LOANS_WRITEOFF += bank_send_data_message->financial_data.bank_no_firm_loans_writeoff;
        
        //Aggregate WACF: Annualized Weighted Average Cost of Funding per bank, aggregated across all banks, weighted by the volume of total new loan business (mortgages aNd firm loans)
        //Since the Banks' WACf is already weighted, we do not multiply by any weights here again
        sum_wacf += bank_send_data_message->financial_data.wacf * (bank_send_data_message->financial_data.bank_firm_loans_new_business + bank_send_data_message->financial_data.bank_mortgages_new_business);

        //Weighted funding_costs: Actual Cost of Funding per bank, weighted by the volume of total new loan business (mortgages ad firm loans)
        sum_funding_costs += bank_send_data_message->financial_data.funding_costs * (bank_send_data_message->financial_data.bank_firm_loans_new_business + bank_send_data_message->financial_data.bank_mortgages_new_business);

        BANK_DEPOSIT_INTEREST_RATE += bank_send_data_message->financial_data.deposit_interest_rate;
		TOTAL_DEPOSITS += bank_send_data_message->financial_data.deposits;

    FINISH_BANK_SEND_DATA_MESSAGE_LOOP

    if (no_active_banks>0)
        BANK_RISK_WEIGHTED_CAPITAL_RATIO_ACTIVE = BANK_RISK_WEIGHTED_CAPITAL_RATIO_ACTIVE / no_active_banks;

    //Recomputing total bank reserves from messages
    TOTAL_BANK_RESERVES=0.0;
    START_BANK_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
        TOTAL_BANK_RESERVES += bank_to_central_bank_account_update_message->payment_account;
    FINISH_BANK_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP

    //Recomputing total Gov deposits from messages
    GOV_DEPOSITS=0.0;
    START_GOV_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP
        GOV_DEPOSITS += gov_to_central_bank_account_update_message->payment_account;
    FINISH_GOV_TO_CENTRAL_BANK_ACCOUNT_UPDATE_MESSAGE_LOOP

    //FIAT_MONEY_GOVS is the fiat money created by the CB to finance the budget deficit (debt monetization)
    //TOTAL_DEBT_STOCK_GOVS is the current debt stock of Gov to CB (note it is not reset to 0, only mutated)
    START_REQUEST_FIAT_MONEY_MESSAGE_LOOP
       TOTAL_DEBT_STOCK_GOVS += request_fiat_money_message->nominal_value;
    FINISH_REQUEST_FIAT_MONEY_MESSAGE_LOOP

    WACF = 0.0;
    WEIGHTED_FUNDING_COSTS = 0.0;
    if(TOTAL_NEW_CREDIT_BUSINESS>EPSILON)
	{
	    //Aggregated WACF: Annualized Weighted Average Cost of Funding per bank, weighted by the volume of this month's total new loan business (mortgages and firm loans), aggregated across all banks, then divided by the total new loan business across all banks (sum of the weights)
        WACF = sum_wacf/TOTAL_NEW_CREDIT_BUSINESS;

	    //Weighted Actual Cost of Funding per bank, weighted by the volume of this month's total new loan business (mortgages and firm loans), aggregated across all banks, then divided by the total new loan business across all banks (sum of the weights)
	    WEIGHTED_FUNDING_COSTS = sum_funding_costs/TOTAL_NEW_CREDIT_BUSINESS;
    }

    //Eurostat interest rate spreads:

    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = 0.0;
    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = 0.0;
    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = 0.0;

    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = 0.0;
    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = 0.0;
    WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = 0.0;

    //Eurostat volume-weighted mean interest rates

    if(BANK_MORTGAGES_NEW_BUSINESS>EPSILON)
    {
        WEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES = BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_MORTGAGES/BANK_MORTGAGES_NEW_BUSINESS;

        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS/BANK_MORTGAGES_NEW_BUSINESS;
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE/BANK_MORTGAGES_NEW_BUSINESS;
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF/BANK_MORTGAGES_NEW_BUSINESS;
    }

    if(BANK_FIRM_LOANS_NEW_BUSINESS>EPSILON)
    {
        WEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS = BANK_SUM_WEIGHTED_INTEREST_RATES_NEW_FIRM_LOANS/BANK_FIRM_LOANS_NEW_BUSINESS;

        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS/BANK_FIRM_LOANS_NEW_BUSINESS;
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE/BANK_FIRM_LOANS_NEW_BUSINESS;
        WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = BANK_SUM_WEIGHTED_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF/BANK_FIRM_LOANS_NEW_BUSINESS;
    }

    WEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES  = BANK_SUM_WEIGHTED_INTEREST_RATES_ALL_MORTGAGES  / BANK_TOTAL_MORTGAGE_LOANS;
    WEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS = BANK_SUM_WEIGHTED_INTEREST_RATES_ALL_FIRM_LOANS / BANK_TOTAL_FIRM_LOANS;
    
    //Ratio of new firm loans to investments
    FIRM_NEW_LOANS_TO_INVESTMENT_RATIO=0.0;
    if(MONTHLY_INVESTMENT_VALUE>EPSILON)
        FIRM_NEW_LOANS_TO_INVESTMENT_RATIO = BANK_FIRM_LOANS_NEW_BUSINESS / MONTHLY_INVESTMENT_VALUE;
    
    //Mean bank ratios
    BANK_EQUITY_ASSET_RATIO = bank_equity_asset_ratio/no_banks;
    BANK_EQUITY_REA_RATIO = bank_equity_rea_ratio/no_banks;

    //Aggregate credit
    MORTGAGE_CREDIT = BANK_PL_MORTGAGE_LOANS + BANK_NPL_MORTGAGE_LOANS;

    TOTAL_CREDIT = FIRM_CREDIT + MORTGAGE_CREDIT;
    TOTAL_NEW_CREDIT_BUSINESS = BANK_FIRM_LOANS_NEW_BUSINESS + BANK_MORTGAGES_NEW_BUSINESS;
    
    if (TOTAL_CREDIT>EPSILON)
    {
    	FIRM_TO_TOTAL_CREDIT_RATIO = FIRM_CREDIT/TOTAL_CREDIT;
    	MORTGAGE_TO_TOTAL_CREDIT_RATIO = MORTGAGE_CREDIT/TOTAL_CREDIT;
	}

    INSIDE_TOTAL_BROAD_MONEY_RATIO = 0.0;
    
    if (GOV_DEPOSITS + TOTAL_CREDIT + TOTAL_BANK_RESERVES - TOTAL_BANK_ECB_DEBT > EPSILON)
        INSIDE_TOTAL_BROAD_MONEY_RATIO = (TOTAL_CREDIT + TOTAL_DEBT_STOCK_GOVS)/(GOV_DEPOSITS + TOTAL_CREDIT + TOTAL_BANK_RESERVES - TOTAL_BANK_ECB_DEBT);

    assert__(INSIDE_TOTAL_BROAD_MONEY_RATIO < 1.0 + EPSILON)
    {
        fprintf(stderr, "\nIT %d ID %d %s(), line %d: ERROR: INSIDE_TOTAL_BROAD_MONEY_RATIO %f", DAY, ID, __FUNCTION__, __LINE__, INSIDE_TOTAL_BROAD_MONEY_RATIO);
    }
        
	//Mean bank deposit interest rate
	if (no_banks>0)
		BANK_DEPOSIT_INTEREST_RATE = BANK_DEPOSIT_INTEREST_RATE/no_banks;

    CREDIT_TO_GDP_RATIO = TOTAL_CREDIT/GDP;

	//NPL mortgage ratios
	BANK_NPL_RATIO_MORTGAGE_LOANS_OP = 0.0;
	BANK_NPL_RATIO_MORTGAGE_LOANS_HMR = 0.0;
	BANK_NPL_RATIO_MORTGAGE_LOANS = 0.0;
	
	//OP
	if(BANK_NPL_MORTGAGE_LOANS_OP+BANK_PL_MORTGAGE_LOANS_OP>0.0)
	  BANK_NPL_RATIO_MORTGAGE_LOANS_OP		= BANK_NPL_MORTGAGE_LOANS_OP/(BANK_NPL_MORTGAGE_LOANS_OP+BANK_PL_MORTGAGE_LOANS_OP);
	
	//HMR
	if(BANK_NPL_MORTGAGE_LOANS_HMR+BANK_PL_MORTGAGE_LOANS_HMR>0.0) 
		BANK_NPL_RATIO_MORTGAGE_LOANS_HMR 	= BANK_NPL_MORTGAGE_LOANS_HMR/(BANK_NPL_MORTGAGE_LOANS_HMR+BANK_PL_MORTGAGE_LOANS_HMR);
	
	//Total
	if(BANK_NPL_MORTGAGE_LOANS+BANK_PL_MORTGAGE_LOANS>0.0)
	 	  BANK_NPL_RATIO_MORTGAGE_LOANS		= BANK_NPL_MORTGAGE_LOANS/(BANK_NPL_MORTGAGE_LOANS+BANK_PL_MORTGAGE_LOANS);


    /* ClearingHouse data:
     * Set the price index equal to the population mean price.
     */
    START_CLEARINGHOUSE_TO_EUROSTAT_DATA_MESSAGE_LOOP
            RENTAL_MARKET_PRICE_MEAN = clearinghouse_to_eurostat_data_message->rental_market_price_mean;
            HOUSING_MARKET_PRICE_MEAN = clearinghouse_to_eurostat_data_message->housing_market_price_mean;

            RENTAL_MARKET_PRICE_INDEX = clearinghouse_to_eurostat_data_message->rental_market_price_index;
            HOUSING_MARKET_PRICE_INDEX = clearinghouse_to_eurostat_data_message->housing_market_price_index;

            //Not used: clearinghouse_to_eurostat_data_message->HOUSING_PRICE_index;
    FINISH_CLEARINGHOUSE_TO_EUROSTAT_DATA_MESSAGE_LOOP


    Eurostat_sanity_checks();

     if (PRINT_DEBUG_FILES)
        {
            filename = malloc(40*sizeof(char));
            filename[0]=0;
            strcpy(filename, "its/eurostat2.txt"); 
            file1 = fopen(filename,"a");
            fprintf(file1,"\n %d %f %f",DAY,MONTHLY_SOLD_QUANTITY,MONTHLY_OUTPUT);
            fprintf(file1," %f %f",MONTHLY_REVENUE,MONTHLY_INVESTMENT_VALUE);
            fclose(file1);
            free(filename);
        }
}

/*
 * \fn: sum_array
 * \brief: Sum from start to end in reverse order, sum_array(&x, start, enc) = sum[t-start,...,t-end]
 * \brief: QoQ sum:  sum_array(&X,1,3)=sum[t-1,...,t-3]
 * \brief: YoY sum:  sum_array(&X,1,12)=sum[t-1,...,t-12]
 */
void sum_array(double_array * x, int start, int end, double * sum_out)
{
    int i,n;
    double sum=0.0;

    n = (*x).size;

/*
    printf("\n\nSize in function: %d;", n);
    for (i=0; i<(*x).size; i++)
      printf(" %2.2f", (*x).array[i]);
*/
    if(0<=start && start<=end && end<=n-1)
    {
        //printf("\n\nCondition true: start %d end %d [0<=start (%d) && start<=end (%d) && end<=n-1 (%d)]", start, end, (0<=start), (start<=end), (end<=n-1));
        for(i=start; i<=end; i++)
        {
            sum += (*x).array[i];
            //printf("\nFor-loop sum %2.2f", sum);
        }

    }else
    {
        if (DAY>CONST_INIT_TRANSIENT_PERIOD+24*MONTH)
	        fprintf(stderr, "\n\n WARNING: sum_array fails. Condition is false (1=true, 0=false): start %d end %d [0<=start (%d) && start<=end (%d) && end<=n-1 (%d)]\nExiting sum_array\n", start, end, (0<=start), (start<=end), (end<=n-1));
    }
    
    (*sum_out) = sum;
}

/*
 * \fn: sum_array_backwards
 * \brief: Sum from start to end in reverse order, sum_array(&x, start, enc) = sum[t-start,...,t-end]
 * \brief: QoQ sum:  sum_array(&X,1,3)=sum[t-1,...,t-3]
 * \brief: YoY sum:  sum_array(&X,1,12)=sum[t-1,...,t-12]
 */
void sum_array_backwards(double_array * x, int start, int end, double * sum_out)
{
    int i,n;
    double sum=0.0;

    n = (*x).size;

/*
    printf("\n\nSize in function: %d;", n);
    for (i=0; i<(*x).size; i++)
      printf(" %2.2f", (*x).array[i]);
*/
    if(0<start && start<=end && end<=n)
    {
        //printf("\n\nCondition true: start %d end %d [0<=start (%d) && start<=end (%d) && end<=n-1 (%d)]", start, end, (0<=start), (start<=end), (end<=n-1));
        for(i=n-start; i>=n-end; i--)
        {
            sum += (*x).array[i];
            //printf("\nFor-loop sum %2.2f", sum);
        }
    }else
    {
        if (DAY>CONST_INIT_TRANSIENT_PERIOD+24*MONTH)
        	fprintf(stderr, "\n\nIT %d WARNING: sum_array_backwards fails. Condition is false (1=true, 0=false): start %d end %d [0<=start (%d) && start<=end (%d) && end<=n (%d)]\nExiting sum_array_backwards\n", DAY, start, end, (0<start), (start<=end), (end<=n));
	}

    (*sum_out) = sum;
}

/*
 * Ratio
 * QoQ:  ratio_qoq(&X)=sum[t-1,...,t-3] / sum[t-4,...,t-6] = sum_array(&X,1,3) / sum_array(&X,4,6)
 */
double ratio_qoq(double_array * x)
{
    double a =  0.0;
    double b = 0.0;

    sum_array_backwards(x,1,3, &a);
    sum_array_backwards(x,4,6, &b);

    //printf("\nBefore return a=%2.2f b=%2.2f", a, b);

    return ((double)a)/((double)b);
}

/*
 * Ratio
 * YoY:  ratio_yoy(&X)=sum[t-1,...,t-12] / sum[t-13,...,t-24] = sum_array(&X,1,12) / sum_array(&X,13,24)
 */
double ratio_yoy(double_array * x)
{
    double a =  0.0;
    double b = 0.0;

    sum_array_backwards(x,1,12, &a);
    sum_array_backwards(x,13,24, &b);

    //printf("\nBefore return a=%2.2f b=%2.2f", a, b);

    return ((double)a)/((double)b);
}

/*
 * Growth rates
 * QoQ:
 * pct_change_qoq(&X) = (sum[t-1,...,t-3] / sum[t-4,...,t-6] -1)*100 = (sum_qoq(&X,1,3)/sum_qoq(&X,4,6)-1)*100 = (ratio_qoq(&X)-1)*100`
 * YoY: 
 * pct_change_yoy(&X) = (sum[t-1,...,t-12] / sum[t-13,...,t-24] -1)*100 = (sum_yoy(&X,1,12)/sum_yoy(&X,13,24)-1)*100 = (ratio_yoy(&X)-1)*100`
 */
double pct_change_qoq(double_array * x)
{
    return (ratio_qoq(x)-1)*100;
}

double pct_change_yoy(double_array * x)
{
    return (ratio_yoy(x)-1)*100;
}

void compute_statistics_economy(data_adt * x)
{
    //Compute statistics for time series
    (*x).economy.levels.ts.mean   = my_stats_mean(&(*x).economy.levels.ts.data);
    (*x).economy.levels.ts.median = my_stats_median(&(*x).economy.levels.ts.data);
    (*x).economy.levels.ts.sd     = my_stats_sd(&(*x).economy.levels.ts.data);
    (*x).economy.levels.ts.cv     = my_stats_cv(&(*x).economy.levels.ts.data);
	
	// 3-month trailing sum of current time series
	sum_array_backwards(&(*x).economy.levels.ts.data,1,3, &(*x).economy.levels.ts.trailing_sum_qtr);
	// 3-month trailing avg of current time series
	sum_array_backwards(&(*x).economy.levels.ts.data,1,3, &(*x).economy.levels.ts.trailing_avg_qtr);
	(*x).economy.levels.ts.trailing_avg_qtr = (*x).economy.levels.ts.trailing_avg_qtr/3.0;

	// 12-month trailing sum of current time series
	sum_array_backwards(&(*x).economy.levels.ts.data,1,12, &(*x).economy.levels.ts.trailing_sum_year);
	// 12-month trailing avg of current time series
	sum_array_backwards(&(*x).economy.levels.ts.data,1,12, &(*x).economy.levels.ts.trailing_avg_year);
	(*x).economy.levels.ts.trailing_avg_year = (*x).economy.levels.ts.trailing_avg_year/12.0;

    //Compute growth rates
    //if(DAY%(MONTH*3)==0) //only compute on quarter boundaries
        (*x).economy.pct_change_qoq_series.current = pct_change_qoq(&(*x).economy.levels.ts.data);
    //if(DAY%(MONTH*12)==0) //only compute on annual boundaries
        (*x).economy.pct_change_yoy_series.current = pct_change_yoy(&(*x).economy.levels.ts.data);

    //Compute statistics for time series of growth rates
    //if(DAY%(MONTH*3)==0)   //only compute on quarter boundaries  
    {
        (*x).economy.pct_change_qoq_series.ts.mean  = my_stats_mean(&(*x).economy.pct_change_qoq_series.ts.data);
        (*x).economy.pct_change_qoq_series.ts.median = my_stats_median(&(*x).economy.pct_change_qoq_series.ts.data);
        (*x).economy.pct_change_qoq_series.ts.sd    = my_stats_sd(&(*x).economy.pct_change_qoq_series.ts.data);
        (*x).economy.pct_change_qoq_series.ts.cv    = my_stats_cv(&(*x).economy.pct_change_qoq_series.ts.data);
    }
    
    //if(DAY%(MONTH*12)==0)  //only compute on annual boundaries
    {        
        (*x).economy.pct_change_yoy_series.ts.mean  = my_stats_mean(&(*x).economy.pct_change_yoy_series.ts.data);
        (*x).economy.pct_change_yoy_series.ts.median = my_stats_median(&(*x).economy.pct_change_yoy_series.ts.data);
        (*x).economy.pct_change_yoy_series.ts.sd    = my_stats_sd(&(*x).economy.pct_change_yoy_series.ts.data);
        (*x).economy.pct_change_yoy_series.ts.cv    = my_stats_cv(&(*x).economy.pct_change_yoy_series.ts.data);
    }
}

void compute_statistics_regions(data_adt * x, int i)
{
        //Compute statistics for time series
        (*x).regions.array[i].levels.ts.mean   = my_stats_mean(&(*x).regions.array[i].levels.ts.data);
        (*x).regions.array[i].levels.ts.median = my_stats_median(&(*x).regions.array[i].levels.ts.data);
        (*x).regions.array[i].levels.ts.sd     = my_stats_sd(&(*x).regions.array[i].levels.ts.data);
        (*x).regions.array[i].levels.ts.cv     = my_stats_cv(&(*x).regions.array[i].levels.ts.data);

		// 3-month trailing sum of current time series
		sum_array_backwards(&(*x).regions.array[i].levels.ts.data,1,3, &(*x).regions.array[i].levels.ts.trailing_sum_qtr);
		// 3-month trailing avg of current time series
		sum_array_backwards(&(*x).regions.array[i].levels.ts.data,1,3, &(*x).regions.array[i].levels.ts.trailing_avg_qtr);
		(*x).regions.array[i].levels.ts.trailing_avg_qtr = (*x).regions.array[i].levels.ts.trailing_avg_qtr/3.0;

		// 12-month trailing sum of current time series
		sum_array_backwards(&(*x).regions.array[i].levels.ts.data,1,12, &(*x).regions.array[i].levels.ts.trailing_sum_year);
		// 12-month trailing avg of current time series
		sum_array_backwards(&(*x).regions.array[i].levels.ts.data,1,12, &(*x).regions.array[i].levels.ts.trailing_avg_year);
		(*x).regions.array[i].levels.ts.trailing_avg_year = (*x).regions.array[i].levels.ts.trailing_avg_year/12.0;

        //Compute growth rates
        //if(DAY%(MONTH*3)==0)	//only compute on quarter boundaries    
            (*x).regions.array[i].pct_change_qoq_series.current = pct_change_qoq(&(*x).regions.array[i].levels.ts.data);
    
        //if(DAY%(MONTH*12)==0)   //only compute on annual boundaries
            (*x).regions.array[i].pct_change_yoy_series.current = pct_change_yoy(&(*x).regions.array[i].levels.ts.data);

        //Compute statistics for time series of growth rates
        //if(DAY%(MONTH*3)==0)     //only compute on quarter boundaries    
        {
            (*x).regions.array[i].pct_change_qoq_series.ts.mean  = my_stats_mean(&(*x).regions.array[i].pct_change_qoq_series.ts.data);
            (*x).regions.array[i].pct_change_qoq_series.ts.median = my_stats_median(&(*x).regions.array[i].pct_change_qoq_series.ts.data);
            (*x).regions.array[i].pct_change_qoq_series.ts.sd    = my_stats_sd(&(*x).regions.array[i].pct_change_qoq_series.ts.data);
            (*x).regions.array[i].pct_change_qoq_series.ts.cv    = my_stats_cv(&(*x).regions.array[i].pct_change_qoq_series.ts.data);
        }

        //if(DAY%(MONTH*12)==0)	  //only compute on annual boundaries
        {
            (*x).regions.array[i].pct_change_yoy_series.ts.mean  = my_stats_mean(&(*x).regions.array[i].pct_change_yoy_series.ts.data);
            (*x).regions.array[i].pct_change_yoy_series.ts.median = my_stats_median(&(*x).regions.array[i].pct_change_yoy_series.ts.data);
            (*x).regions.array[i].pct_change_yoy_series.ts.sd    = my_stats_sd(&(*x).regions.array[i].pct_change_yoy_series.ts.data);
            (*x).regions.array[i].pct_change_yoy_series.ts.cv    = my_stats_cv(&(*x).regions.array[i].pct_change_yoy_series.ts.data);
        }
}

void update_timeseries_economy(data_adt * x, double value)
{
    (*x).economy.levels.current = value;
    
    add_double(&(*x).economy.levels.ts.data, value);

    //if(DAY%(MONTH*3)==0)    	//only compute on quarter boundaries    
        add_double(&(*x).economy.pct_change_qoq_series.ts.data, (*x).economy.pct_change_qoq_series.current);
    
    //if(DAY%(MONTH*12)==0)   //only compute on annual boundaries
        add_double(&(*x).economy.pct_change_yoy_series.ts.data, (*x).economy.pct_change_yoy_series.current);
}

void update_timeseries_regions(data_adt * x, int i, double value)
{
    (*x).regions.array[i].levels.current = value;
    
    add_double(&(*x).regions.array[i].levels.ts.data, value);
    
    //if(DAY%(MONTH*3)==0)	//only compute on quarter boundaries    
        add_double(&(*x).regions.array[i].pct_change_qoq_series.ts.data, (*x).regions.array[i].pct_change_qoq_series.current);
    
    //if(DAY%(MONTH*12)==0)	  //only compute on annual boundaries
        add_double(&(*x).regions.array[i].pct_change_yoy_series.ts.data, (*x).regions.array[i].pct_change_yoy_series.current);
}


/* \fn: set_with_inf_nan_checking
 * \brief: Shielded setter function, to check first if assignment rvalue is inf or nan or -nan.
 * \brief: If rvalue = inf: a = inf_replacement
 * \brief: If rvalue = nan: a = nan_replacement
 * \brief: If rvalue = -nan: a = nan_replacement
 */
int set_with_inf_nan_checking(double * a, double b, double inf_replacement, double nan_replacement)
{
    // Catch inf values
    if( b==INFINITY )
    {
        (*a) = inf_replacement;
        return 0;
    }

    // Catch nan values
    if( !( b==b ) )
    {
        (*a) = nan_replacement;
        return 0;
    }


    (*a) = b;

    return 0;
}

/* \fn: Eurostat_compute_growth_rates_quarterly_dynamic
 * \brief: 
 */
void Eurostat_compute_timeseries_statistics(void)
{
    int i __attribute__((unused)) = 0;

/*
VARIABLES:

    GDP_DATA
    CPI_DATA

    OUTPUT_DATA
    SALES_DATA
    SOLD_QUANTITY_DATA
    INVESTMENT_DATA
    INVESTMENT_GDP_RATIO_DATA

    FIRM_ASSETS_DATA
    FIRM_CREDIT_DATA
    FIRM_CREDIT_DATA_NEW_BUSINESS
    FIRM_EQUITY_DATA

    BANK_EQUITY_DATA
    BANK_REA_DATA
    BANK_EQUITY_REA_RATIO_DATA
    BANK_EQUITY_ASSET_RATIO_DATA

    MORTGAGE_CREDIT_DATA
    MORTGAGE_CREDIT_NEW_BUSINESS

    TOTAL_CREDIT_DATA

    HOUSING_PRICE_INDEX_DATA
    RENTAL_PRICE_INDEX_DATA

    CREDIT_TO_GDP_RATIO_DATA
*/

    //printf("\n\nIT %d Starting compute_statistics_economy()", DAY);

    compute_statistics_economy(&GDP_DATA);                  //GDP
    compute_statistics_economy(&CPI_DATA);                  //CPI

    compute_statistics_economy(&OUTPUT_DATA);               //MONTHLY_OUTPUT = sum_total_output;
    compute_statistics_economy(&SALES_DATA);                //MONTHLY_REVENUE = sum_total_cum_revenue;
    compute_statistics_economy(&SOLD_QUANTITY_DATA);        //MONTHLY_SOLD_QUANTITY = sum_total_sold_quantity;
    compute_statistics_economy(&INVESTMENT_DATA);           //MONTHLY_INVESTMENT_VALUE = sum_capital_costs
    compute_statistics_economy(&INVESTMENT_GDP_RATIO_DATA); //MONTHLY_INVESTMENT_VALUE/GDP;
    
    compute_statistics_economy(&FIRM_ASSETS_DATA);          //FIRM_ASSETS 
    compute_statistics_economy(&FIRM_CREDIT_DATA);          //FIRM_CREDIT 
    compute_statistics_economy(&FIRM_CREDIT_NEW_BUSINESS_DATA);          //FIRM_CREDIT     
    compute_statistics_economy(&FIRM_EQUITY_DATA);   
    
    compute_statistics_economy(&BANK_EQUITY_DATA);          //BANK_EQUITY
    compute_statistics_economy(&BANK_REA_DATA);             //BANK_REA
    compute_statistics_economy(&BANK_EQUITY_REA_RATIO_DATA);//BANK_EQUITY_REA_RATIO
    compute_statistics_economy(&BANK_EQUITY_ASSET_RATIO_DATA);//BANK_EQUITY_ASSET_RATIO

    compute_statistics_economy(&MORTGAGE_CREDIT_DATA);      //MORTGAGE_CREDIT
    compute_statistics_economy(&MORTGAGE_CREDIT_NEW_BUSINESS_DATA);      //MORTGAGE_CREDIT

    compute_statistics_economy(&TOTAL_CREDIT_DATA);         //TOTAL_CREDIT

    compute_statistics_economy(&HOUSING_PRICE_INDEX_DATA);    //HOUSING_PRICE_INDEX_DATA
    compute_statistics_economy(&RENTAL_PRICE_INDEX_DATA);     //RENTAL_PRICE_INDEX_DATA

    compute_statistics_economy(&CREDIT_TO_GDP_RATIO_DATA);    //CREDIT_TO_GDP_RATIO_DATA


    //printf("\n\nIT %d Starting update_timeseries_economy()", DAY);

    //Update time series with new data (*after* computing statistics)
    update_timeseries_economy(&GDP_DATA, GDP);
    update_timeseries_economy(&CPI_DATA, CPI);

    update_timeseries_economy(&OUTPUT_DATA, MONTHLY_OUTPUT);
    update_timeseries_economy(&SALES_DATA, MONTHLY_REVENUE);
    update_timeseries_economy(&SOLD_QUANTITY_DATA, MONTHLY_SOLD_QUANTITY);
    update_timeseries_economy(&INVESTMENT_DATA, MONTHLY_INVESTMENT_VALUE);    
    update_timeseries_economy(&INVESTMENT_GDP_RATIO_DATA, INVESTMENT_GDP_RATIO);

    update_timeseries_economy(&FIRM_ASSETS_DATA, FIRM_ASSETS);
    update_timeseries_economy(&FIRM_CREDIT_DATA, FIRM_CREDIT);
    update_timeseries_economy(&FIRM_CREDIT_NEW_BUSINESS_DATA, BANK_FIRM_LOANS_NEW_BUSINESS);
    update_timeseries_economy(&FIRM_EQUITY_DATA, FIRM_EQUITY);
    
    update_timeseries_economy(&BANK_EQUITY_DATA, BANK_EQUITY);
    update_timeseries_economy(&BANK_REA_DATA, BANK_REA);
    update_timeseries_economy(&BANK_EQUITY_REA_RATIO_DATA, BANK_EQUITY_REA_RATIO);
    update_timeseries_economy(&BANK_EQUITY_ASSET_RATIO_DATA, BANK_EQUITY_ASSET_RATIO);
    
    update_timeseries_economy(&MORTGAGE_CREDIT_DATA, MORTGAGE_CREDIT);
    update_timeseries_economy(&MORTGAGE_CREDIT_NEW_BUSINESS_DATA, BANK_MORTGAGES_NEW_BUSINESS);

    update_timeseries_economy(&TOTAL_CREDIT_DATA, TOTAL_CREDIT);
    update_timeseries_economy(&HOUSING_PRICE_INDEX_DATA, HOUSING_MARKET_PRICE_INDEX);
    update_timeseries_economy(&RENTAL_PRICE_INDEX_DATA, RENTAL_MARKET_PRICE_INDEX);

    update_timeseries_economy(&CREDIT_TO_GDP_RATIO_DATA, CREDIT_TO_GDP_RATIO);


/*
    // Compute regions
 
    printf("\nEurostat_compute_timeseries_statistics");
    printf("\n\tTOTAL_REGIONS %d GDP_DATA.regions.size==%d", TOTAL_REGIONS, GDP_DATA.regions.size);
    
	// Initialize region arrays
	if (GDP_DATA.regions.size==0)
	{
		//init empty history_adt
		history_adt history;
		init_history_adt(&history);

		//Add as many items as regions
	    for (i=0; i<TOTAL_REGIONS; i++)
	    {
			add_history_adt(&GDP_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&CPI_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			
			add_history_adt(&OUTPUT_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&SALES_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&SOLD_QUANTITY_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&INVESTMENT_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&INVESTMENT_GDP_RATIO_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);

			add_history_adt(&FIRM_ASSETS_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&FIRM_CREDIT_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&FIRM_EQUITY_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);

			add_history_adt(&BANK_EQUITY_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&BANK_REA_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&BANK_EQUITY_REA_RATIO_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&BANK_EQUITY_ASSET_RATIO_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);

			add_history_adt(&MORTGAGE_CREDIT_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&TOTAL_CREDIT_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&HOUSING_PRICE_INDEX_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
			add_history_adt(&RENTAL_PRICE_INDEX_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);

			add_history_adt(&CREDIT_TO_GDP_RATIO_DATA.regions, &history.levels, &history.pct_change_qoq_series, &history.pct_change_yoy_series);
		}
	}
	
	assert(GDP_DATA.regions.size==TOTAL_REGIONS);

    //This refers to region data of GDP etc:
    for (i=0; i<TOTAL_REGIONS; i++)
    {
        compute_statistics_regions(&GDP_DATA, i);                   //GDP
        compute_statistics_regions(&CPI_DATA, i);                  //CPI

        compute_statistics_regions(&OUTPUT_DATA, i);               //MONTHLY_OUTPUT = sum_total_output;
        compute_statistics_regions(&SALES_DATA, i);                //MONTHLY_REVENUE = sum_total_cum_revenue;
        compute_statistics_regions(&SOLD_QUANTITY_DATA, i);        //MONTHLY_SOLD_QUANTITY = sum_total_sold_quantity;
        compute_statistics_regions(&INVESTMENT_DATA, i);           //MONTHLY_INVESTMENT_VALUE = sum_capital_costs
        compute_statistics_regions(&INVESTMENT_GDP_RATIO_DATA, i); //MONTHLY_INVESTMENT_VALUE/GDP;
        
        compute_statistics_regions(&FIRM_ASSETS_DATA, i);          //FIRM_ASSETS 
        compute_statistics_regions(&FIRM_CREDIT_DATA, i);          //FIRM_CREDIT 
        compute_statistics_regions(&FIRM_EQUITY_DATA, i);   
        
        compute_statistics_regions(&BANK_EQUITY_DATA, i);          //BANK_EQUITY
        compute_statistics_regions(&BANK_REA_DATA, i);             //BANK_REA
        compute_statistics_regions(&BANK_EQUITY_REA_RATIO_DATA, i);//BANK_EQUITY_REA_RATIO
        compute_statistics_regions(&BANK_EQUITY_ASSET_RATIO_DATA, i);//BANK_EQUITY_ASSET_RATIO


        compute_statistics_regions(&MORTGAGE_CREDIT_DATA, i);      //MORTGAGE_CREDIT
        compute_statistics_regions(&TOTAL_CREDIT_DATA, i);         //TOTAL_CREDIT

        compute_statistics_regions(&HOUSING_PRICE_INDEX_DATA, i);    //HOUSING_PRICE_INDEX_DATA
        compute_statistics_regions(&RENTAL_PRICE_INDEX_DATA, i);     //RENTAL_PRICE_INDEX_DATA

        compute_statistics_regions(&CREDIT_TO_GDP_RATIO_DATA, i);    //CREDIT_TO_GDP_RATIO_DATA
    }



    //Update regions

    //This refers to region data of GDP etc:
    for (i=0; i<TOTAL_REGIONS; i++)
    {
        update_timeseries_regions(&GDP_DATA, i, GDP);
        update_timeseries_regions(&CPI_DATA, i, CPI);

        update_timeseries_regions(&OUTPUT_DATA, i, MONTHLY_OUTPUT);
        update_timeseries_regions(&SALES_DATA, i, MONTHLY_REVENUE);
        update_timeseries_regions(&SOLD_QUANTITY_DATA, i, MONTHLY_SOLD_QUANTITY);
        update_timeseries_regions(&INVESTMENT_DATA, i, MONTHLY_INVESTMENT_VALUE);    
        update_timeseries_regions(&INVESTMENT_GDP_RATIO_DATA, i, INVESTMENT_GDP_RATIO);

        update_timeseries_regions(&FIRM_ASSETS_DATA, i, FIRM_ASSETS);
        update_timeseries_regions(&FIRM_CREDIT_DATA, i, FIRM_CREDIT);
        update_timeseries_regions(&FIRM_EQUITY_DATA, i, FIRM_EQUITY);
        
        update_timeseries_regions(&BANK_EQUITY_DATA, i, BANK_EQUITY);
        update_timeseries_regions(&BANK_REA_DATA, i, BANK_REA);
        update_timeseries_regions(&BANK_EQUITY_REA_RATIO_DATA, i, BANK_EQUITY_REA_RATIO);
        update_timeseries_regions(&BANK_EQUITY_ASSET_RATIO_DATA, i, BANK_EQUITY_ASSET_RATIO);
        
        update_timeseries_regions(&MORTGAGE_CREDIT_DATA, i, MORTGAGE_CREDIT);
        update_timeseries_regions(&TOTAL_CREDIT_DATA, i, TOTAL_CREDIT);
        update_timeseries_regions(&HOUSING_PRICE_INDEX_DATA, i, HOUSING_MARKET_PRICE_INDEX);
        update_timeseries_regions(&RENTAL_PRICE_INDEX_DATA, i, RENTAL_MARKET_PRICE_INDEX);

        update_timeseries_regions(&CREDIT_TO_GDP_RATIO_DATA, i, CREDIT_TO_GDP_RATIO);
    }
*/

}

/* \fn: Eurostat_compute_macro_indicators
 * \brief: 
 */
void Eurostat_compute_macro_indicators(void)
{
	int i,s,n;

    #ifdef _DEBUG_MODE    
    if (PRINT_EUROSTAT_INFO)
    {
        printf("\nIT %d Eurostat_compute_macro_indicators()", DAY);
    }
    #endif
    
    // STANDARD MACRO indicators

    //AVERAGE_DEBT_EARNINGS_RATIO //= sum_total_debt_earnings_ratios/NO_FIRMS;
    //AVERAGE_DEBT_EQUITY_RATIO   //= sum_total_debt_equity_ratios/NO_FIRMS;
    //LABOUR_SHARE_RATIO          //= sum_total_labour_share_ratios/NO_FIRMS;

    set_with_inf_nan_checking(&CPI_INFLATION_RATE_QOQ, CPI_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&CPI_INFLATION_RATE_YOY, CPI_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&CPI_INFLATION_RATE_MEDIAN, CPI_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);
    
    GDP_TRAILING_SUM_QTR = GDP_DATA.economy.levels.ts.trailing_sum_qtr;
    GDP_TRAILING_AVG_QTR = GDP_DATA.economy.levels.ts.trailing_avg_qtr;
    GDP_TRAILING_SUM_YEAR = GDP_DATA.economy.levels.ts.trailing_sum_year;
    GDP_TRAILING_AVG_YEAR = GDP_DATA.economy.levels.ts.trailing_avg_year;

    set_with_inf_nan_checking(&GDP_GROWTH_RATE_QOQ,  GDP_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&GDP_GROWTH_RATE_YOY,  GDP_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&GDP_GROWTH_RATE_MEDIAN,  GDP_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);
    
    OUTPUT_TRAILING_SUM_QTR = OUTPUT_DATA.economy.levels.ts.trailing_sum_qtr;
    OUTPUT_TRAILING_AVG_QTR = OUTPUT_DATA.economy.levels.ts.trailing_avg_qtr;
    OUTPUT_TRAILING_SUM_YEAR = OUTPUT_DATA.economy.levels.ts.trailing_sum_year;
    OUTPUT_TRAILING_AVG_YEAR = OUTPUT_DATA.economy.levels.ts.trailing_avg_year;

    set_with_inf_nan_checking(&OUTPUT_GROWTH_RATE_QOQ,  OUTPUT_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&OUTPUT_GROWTH_RATE_YOY,  OUTPUT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);
    vlog_macro_ts("growth_rate", OUTPUT_GROWTH_RATE_YOY);

    set_with_inf_nan_checking(&OUTPUT_GROWTH_RATE_MEDIAN,  OUTPUT_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);

    // Set special indicators
	//INVESTMENT_GDP_RATIO
	//CREDIT_TO_GDP_RATIO

    //Median growth rate of total credit (firm + mortgage loans) minus median growth rate of GDP (year-on-year growth rate)
    set_with_inf_nan_checking(&CREDIT_GROWTH, TOTAL_CREDIT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    //Median growth rate of total credit (firm + mortgage loans) minus median growth rate of GDP (year-on-year growth rate)
    set_with_inf_nan_checking(&CREDIT_MINUS_GDP_GROWTH, TOTAL_CREDIT_DATA.economy.pct_change_yoy_series.current - GDP_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    /*** MORTGAGE_CREDIT***/
    MORTGAGE_CREDIT_TRAILING_SUM_QTR = MORTGAGE_CREDIT_DATA.economy.levels.ts.trailing_sum_qtr;
    MORTGAGE_CREDIT_TRAILING_AVG_QTR = MORTGAGE_CREDIT_DATA.economy.levels.ts.trailing_avg_qtr;
    MORTGAGE_CREDIT_TRAILING_SUM_YEAR = MORTGAGE_CREDIT_DATA.economy.levels.ts.trailing_sum_year;
    MORTGAGE_CREDIT_TRAILING_AVG_YEAR = MORTGAGE_CREDIT_DATA.economy.levels.ts.trailing_avg_year;

	set_with_inf_nan_checking(&MORTGAGE_CREDIT_GROWTH_RATE_QOQ, MORTGAGE_CREDIT_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

	set_with_inf_nan_checking(&MORTGAGE_CREDIT_GROWTH_RATE_YOY, MORTGAGE_CREDIT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&MORTGAGE_CREDIT_GROWTH_RATE_MEDIAN, MORTGAGE_CREDIT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);


    /*** FIRM_CREDIT ***/
    FIRM_CREDIT_TRAILING_SUM_QTR = FIRM_CREDIT_DATA.economy.levels.ts.trailing_sum_qtr;
    FIRM_CREDIT_TRAILING_AVG_QTR = FIRM_CREDIT_DATA.economy.levels.ts.trailing_avg_qtr;
    FIRM_CREDIT_TRAILING_SUM_YEAR = FIRM_CREDIT_DATA.economy.levels.ts.trailing_sum_year;
    FIRM_CREDIT_TRAILING_AVG_YEAR = FIRM_CREDIT_DATA.economy.levels.ts.trailing_avg_year;

	set_with_inf_nan_checking(&FIRM_CREDIT_GROWTH_RATE_QOQ, FIRM_CREDIT_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

	set_with_inf_nan_checking(&FIRM_CREDIT_GROWTH_RATE_YOY, FIRM_CREDIT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&FIRM_CREDIT_GROWTH_RATE_MEDIAN, FIRM_CREDIT_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    /*** MORTGAGE_CREDIT_NEW_BUSINESS ***/
    MORTGAGE_CREDIT_NEW_BUSINESS_TRAILING_SUM_QTR = MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_sum_qtr;
    MORTGAGE_CREDIT_NEW_BUSINESS_TRAILING_AVG_QTR = MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_avg_qtr;
    MORTGAGE_CREDIT_NEW_BUSINESS_TRAILING_SUM_YEAR = MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_sum_year;
    MORTGAGE_CREDIT_NEW_BUSINESS_TRAILING_AVG_YEAR = MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_avg_year;

    set_with_inf_nan_checking(&MORTGAGE_CREDIT_NEW_BUSINESS_GROWTH_RATE_QOQ, MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&MORTGAGE_CREDIT_NEW_BUSINESS_GROWTH_RATE_YOY, MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&MORTGAGE_CREDIT_NEW_BUSINESS_GROWTH_RATE_MEDIAN, MORTGAGE_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);


    /*** FIRM_CREDIT_NEW_BUSINESS ***/
    FIRM_CREDIT_NEW_BUSINESS_TRAILING_SUM_QTR = FIRM_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_sum_qtr;
    FIRM_CREDIT_NEW_BUSINESS_TRAILING_AVG_QTR = FIRM_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_avg_qtr;
    FIRM_CREDIT_NEW_BUSINESS_TRAILING_SUM_YEAR = FIRM_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_sum_year;
    FIRM_CREDIT_NEW_BUSINESS_TRAILING_AVG_YEAR = FIRM_CREDIT_NEW_BUSINESS_DATA.economy.levels.ts.trailing_avg_year;

    set_with_inf_nan_checking(&FIRM_CREDIT_NEW_BUSINESS_GROWTH_RATE_QOQ, FIRM_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&FIRM_CREDIT_NEW_BUSINESS_GROWTH_RATE_YOY, FIRM_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&FIRM_CREDIT_NEW_BUSINESS_GROWTH_RATE_MEDIAN, FIRM_CREDIT_NEW_BUSINESS_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);

	//NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO = MORTGAGE_NEW_LOANS / HOUSING_NEW_VALUE;
	//if(NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO==INFINITY) NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO = 1.0;

	//NEW_FIRM_LOANS_TO_INVESTMENT_RATIO = FIRM_NEW_LOANS / MONTHLY_INVESTMENT_VALUE;
	//if(NEW_FIRM_LOANS_TO_INVESTMENT_RATIO==INFINITY) NEW_FIRM_LOANS_TO_INVESTMENT_RATIO = 1.0;

    // Growth rates of house prices (based on market transactions)
    set_with_inf_nan_checking(&HOUSING_PRICE_INFLATION_RATE_QOQ, HOUSING_PRICE_INDEX_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&HOUSING_PRICE_INFLATION_RATE_YOY, HOUSING_PRICE_INDEX_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);
    
    set_with_inf_nan_checking(&HOUSING_PRICE_INFLATION_RATE_MEDIAN, HOUSING_PRICE_INDEX_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);

	// Growth rates of rental prices (based on market transactions)
    set_with_inf_nan_checking(&RENTAL_PRICE_INFLATION_RATE_QOQ, RENTAL_PRICE_INDEX_DATA.economy.pct_change_qoq_series.current, 0.0/0.0, 0.0/0.0);

    set_with_inf_nan_checking(&RENTAL_PRICE_INFLATION_RATE_YOY, RENTAL_PRICE_INDEX_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);
    
    set_with_inf_nan_checking(&RENTAL_PRICE_INFLATION_RATE_MEDIAN, RENTAL_PRICE_INDEX_DATA.economy.pct_change_yoy_series.ts.median, 0.0/0.0, 0.0/0.0);

    //Median growth rate of house prices minus median growth rate of rent prices (year-on-year growth rate)
    set_with_inf_nan_checking(&HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH, HOUSING_PRICE_INDEX_DATA.economy.pct_change_yoy_series.current - RENTAL_PRICE_INDEX_DATA.economy.pct_change_yoy_series.current, 0.0/0.0, 0.0/0.0);




    /* Set property value growth factor across the BACKWARD looking horizon s in [t-1, t-S-1]
     * Required: a price vector of historical housing market prices for the past S+1 periods
     * price[1] = p_{t-1}
     * price[S+1] = p_{t-(S+1)} = p_{t-S-1}
     * PRICE_GROWTH_FACTOR = (p_{t-1}/p_{t-S-1})
     */

    //Growth factor computation using the backward horizon s for housing market price expectations
	s = CONST_HOUSING_MARKET_BACKWARD_HORIZON;
	
        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
        	printf("\n\tCondition 1 for init:");
            printf("\n\t[DAY==(2+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH)]: %d==(2+%d/%d)*%d (true=%d)", DAY, CONST_INIT_TRANSIENT_PERIOD, MONTH, MONTH, (DAY==(2+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH) );
            
            printf("\n\n\tCondition 2 for init:");
            printf("\n\t[CONST_INIT_TRANSIENT_PERIOD/MONTH > s+2]: %d/%d=%d > %d (true=%d)", CONST_INIT_TRANSIENT_PERIOD, MONTH, CONST_INIT_TRANSIENT_PERIOD/MONTH, s+2, (CONST_INIT_TRANSIENT_PERIOD/MONTH > s+2));
        
			printf("\n\n\tCondition for HOUSING_MARKET_PRICE_GROWTH_FACTOR compute:");
			printf("\n\t[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2]: %d >= %d (true=%d)", HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size, s+2, (HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2));

			printf("\n\n\tCondition for RENTAL_MARKET_PRICE_GROWTH_FACTOR compute:");
			printf("\n\t[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2]: %d >= %d (true=%d)", HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size, s+2, (HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2));
        }
        #endif


    // On day 6020: Initialize history for housing market price: set [n-s-1]...[n] equal to HOUSING_MARKET_PRICE_INDEX
    // Due to CONST_INIT_TRANSIENT_PERIOD=6001, we have to use (1+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH = 6020 
    // Ensure that more than s+2 months have passed: we need data for {t, t-1, ...., t-s-1}
    if( (DAY==(2+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH) && (CONST_INIT_TRANSIENT_PERIOD/MONTH > s+2))
	{
		for (i=0; i<=s+1; i++)
			add_double(&HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data, HOUSING_MARKET_PRICE_INDEX);

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
            printf("\n\n\tInitializing HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data:");
            printf("\n\t\t[");
            for(i=0; i<HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size; i++)
                printf("%.2f ",HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[i]);
            printf("]");
        }
        #endif
	}

    /* Catch Exceptions:
     * if p_{t-S-1}==0:  PRICE_GROWTH_FACTOR =  (p_{t-1}/p_{t-S})
     * if p_{t-S}  ==0:  PRICE_GROWTH_FACTOR =  (p_{t-1}/p_{t-S+1}) etc.
     * Look for first non-zero starting from price[S+1] to price[1].
     */
    // Ensure that at least s+2 months have passed: we need data for {t, t-1, ...., t-s-1}

    // For months >=s+2: HOUSING_MARKET_PRICE_GROWTH_FACTOR compute
	if (HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2)
    {
        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
            printf("\n\n\tHOUSING_PRICE_INDEX_DATA.economy.levels.ts.data:");
            printf("\n\t\t[");
            for(i=0; i<HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size; i++)
                printf("%.2f ", HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[i]);
            printf("]");
        }
        #endif

        n = HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-s-1;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
	        printf("\n\t\tStart [t-s-1]: n=%d\tp[t-s-1]=%.2f", n, HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
	        
        	printf("\tp[t-1]/p[n]=%.2f/%.2f=%.2f", HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1], HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n], HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
        }
        #endif


        // If p[t-s-1]==0.0 search for the first non-zero element closer to the last element
        while(n < HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1 && HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]<EPSILON)
        {
       	        #ifdef _DEBUG_MODE    
		        if (PRINT_DEBUG_EUROSTAT)
		        {
		            printf("\n\t\tTrying: n=%d\tp[%d]=%.2f", n, n, HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
		
		        	printf("\tp[t-1]/p[n]=%.2f/%.2f=%.2f", HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1], HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n], HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
		        }
		        #endif

            n++;
        }

        // Set growth factor:
        if (n==HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1)
        {
            // Set growth factor to 1.0 if all prices in histrory are 0.0: p(t-1) = ... = p(t-s-1) = 0.0
            HOUSING_MARKET_PRICE_GROWTH_FACTOR = 1.0; 

            #ifdef _DEBUG_MODE    
            if (PRINT_EUROSTAT_INFO)
            {
                printf("\n\tWARNING: HOUSING_PRICE_INDEX_DATA: n=%d Reached front of backward horizon to compute price growth factor", n);
            }
            #endif
        }
        else
        {
            // Set growth factor using normal formula: p(t-1)/p(t-s-1)
            HOUSING_MARKET_PRICE_GROWTH_FACTOR = HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data.array[n];
        }
        
        #ifdef _DEBUG_MODE    
        if (PRINT_EUROSTAT_INFO)
        {
            printf("\n\tHOUSING_MARKET_PRICE_GROWTH_FACTOR=%.2f", HOUSING_MARKET_PRICE_GROWTH_FACTOR);            
        }
        #endif
    }


    //Growth factor computation using the backward horizon s for rental market price expectations
	s = CONST_RENTAL_MARKET_BACKWARD_HORIZON;
	
    // On day 6020: Initialize history for rental market price : set [n-s-1]...[n] equal to HOUSING_MARKET_PRICE_INDEX
    // Due to CONST_INIT_TRANSIENT_PERIOD=6001, we have to use (1+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH = 6020 
	// Ensure that more than s+2 months have passed: we need data for {t, t-1, ...., t-s-1}
    if( (DAY==(2+CONST_INIT_TRANSIENT_PERIOD/MONTH)*MONTH) && (CONST_INIT_TRANSIENT_PERIOD/MONTH > s+2))
	{
		for (i=0; i<=s+1; i++)
			add_double(&RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data, RENTAL_MARKET_PRICE_INDEX);
        
        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
            printf("\n\tInitializing RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data:");
            printf("\n\t\t[");
            for(i=0; i<RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size; i++)
                printf("%.2f ",RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[i]);
            printf("]");
        }
        #endif
	}

	// For months >=s+2: RENTAL_MARKET_PRICE_GROWTH_FACTOR compute
	if (RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size >= s+2)
    {
        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
            printf("\n\n\tRENTAL_PRICE_INDEX_DATA.economy.levels.ts.data:");
            printf("\n\t\t[");
            for(i=0; i<RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size; i++)
                printf("%.2f ", RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[i]);
            printf("]");
        }
        #endif

        n = RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-s-1;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_EUROSTAT)
        {
	        printf("\n\t\tStart [t-s-1]: n=%d\tp=%.2f", n, RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);

        	printf("\tp[t-1]/p[n]=%.2f/%.2f=%.2f", RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1], RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n], RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
        }
        #endif

        // If p[t-s-1]==0.0 search for the first non-zero element closer to the last element
        while(n < RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1 && RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]<EPSILON)
        {
                #ifdef _DEBUG_MODE    
		        if (PRINT_DEBUG_EUROSTAT)
		        {
		            printf("\n\t\tTrying: n=%d\tp[n]=%.2f", n, RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);

		        	printf("\tp[t-1]/p[n]=%.2f/%.2f=%.2f", RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1], RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n], RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n]);
		        }
		        #endif

            n++;
        }
        
        // Set growth factor:
        if (n==RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1)
        {
            // Set growth factor to 1.0 if all prices in histrory are 0.0: p(t-1) = ... = p(t-s-1) = 0.0
            RENTAL_MARKET_PRICE_GROWTH_FACTOR = 1.0;
            
            #ifdef _DEBUG_MODE    
            if (PRINT_EUROSTAT_INFO)
            {
                printf("\n\tWARNING: RENTAL_PRICE_INDEX_DATA: n=%d Reached front of backward horizon to compute price growth factor", n);
            }
            #endif
        }
        else
        {
            // Set growth factor using normal formula: p(t-1)/p(t-s-1)
            RENTAL_MARKET_PRICE_GROWTH_FACTOR = RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.size-1]/RENTAL_PRICE_INDEX_DATA.economy.levels.ts.data.array[n];
        }

        #ifdef _DEBUG_MODE    
        if (PRINT_EUROSTAT_INFO)
        {
            printf("\n\tRENTAL_MARKET_PRICE_GROWTH_FACTOR %.2f", RENTAL_MARKET_PRICE_GROWTH_FACTOR);
        }
        #endif
    }


	//Add macro indicators to adt
	macro_indicators_adt macro_indicators;

	macro_indicators.credit_to_gdp_ratio  							= CREDIT_TO_GDP_RATIO; // On [0,1] scale
	macro_indicators.credit_minus_gdp_growth 						= CREDIT_MINUS_GDP_GROWTH; // On [0,100] scale
    macro_indicators.credit_growth                                  = TOTAL_CREDIT_DATA.economy.pct_change_yoy_series.current; // On [0,100] scale
    
	macro_indicators.mortgage_credit_growth 						= MORTGAGE_CREDIT_GROWTH_RATE_YOY; // On [0,100] scale
    macro_indicators.housing_market_price_growth                    = HOUSING_PRICE_INFLATION_RATE_YOY; // On [0,100] scale
    macro_indicators.house_price_minus_rent_price_growth            = HOUSE_PRICE_MINUS_RENT_PRICE_GROWTH; // On [0,100] scale
	macro_indicators.mortgage_interest_rate_spread_policy_rate      = WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE; // On [0,1] scale

    macro_indicators.housing_market_price_growth_factor             = HOUSING_MARKET_PRICE_GROWTH_FACTOR; // On [0,1] scale
    macro_indicators.rental_market_price_growth_factor              = RENTAL_MARKET_PRICE_GROWTH_FACTOR; // On [0,1] scale

    macro_indicators.investment_gdp_ratio                           = INVESTMENT_GDP_RATIO; // On [0,1] scale
    macro_indicators.firm_credit_growth 							= FIRM_CREDIT_GROWTH; // On [0,100] scale
	macro_indicators.new_mortgage_to_housing_value_ratio		 	= NEW_MORTGAGE_TO_HOUSING_VALUE_RATIO; // On [0,1] scale
	macro_indicators.new_firm_loans_to_investment_ratio 			= NEW_FIRM_LOANS_TO_INVESTMENT_RATIO; // On [0,1] scale
	macro_indicators.rental_market_price_growth 					= RENTAL_PRICE_INFLATION_RATE_YOY; // On [0,100] scale

	// Send indicators to CB
	add_eurostat_to_ecb_macro_indicators_message(macro_indicators);
}
