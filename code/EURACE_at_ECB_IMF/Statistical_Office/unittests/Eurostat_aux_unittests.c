#include <CUnit/Basic.h>
#include "../../header.h"
#include "../../Eurostat_agent_header.h"
#include "../Eurostat_aux_header.h"
#include "../../Sanity_Checks/sanity_checks_aux_headers.h"
#include "../../my_library_header.h"

/************Eurostat Role: Statistical Office ********************************/

/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: 
 */
void unittest_a()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    //function();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/************ Unit tests ********************************/

/*
 * \fn: void unittest_Eurostat_read_household_data()
 * \brief: Unit test for: Eurostat_read_household_data.
 * Status: OK 21.9.2017
 */
void unittest_Eurostat_read_household_data_2()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();

    FLAME_environment_variable_print_debug_eurostat = 1;

    /***** Variables: Memory pre-conditions **************************/
    add_household_data(&REGION_HOUSEHOLD_DATA,
            1,                       //1 region_id
            0,0,0,0,0,0,             //6 no_households_skill
            0,0,0,0,0,0,             //6 employed_skill
            0,                       //1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0, //6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

    add_household_data(&REGION_HOUSEHOLD_DATA,
            2,                       //1 region_id
            0,0,0,0,0,0,             //6 no_households_skill//no_households
            0,0,0,0,0,0,             //6 employed_skill
            0,                       //1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0, //6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

/*
    for(i = 0; i < REGION_HOUSEHOLD_DATA.size; i++)
    {
            printf("\n\n Unittest");
            printf("\n\t index %d", i);
            printf("\n\t REGION_HOUSEHOLD_DATA.array[%d].region_id %d", i, REGION_HOUSEHOLD_DATA.array[i].region_id);
    }
*/
    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_household_send_data, sizeof(m_household_send_data));
            #ifdef ERRCHECK
            if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create 'household_send_data' board\n");
               switch(rc) {
                   case MB_ERR_INVALID:
                       fprintf(stderr, "\t reason: Invalid message size\n");
                       break;
                   case MB_ERR_MEMALLOC:
                       fprintf(stderr, "\t reason: out of memory\n");
                       break;
                   case MB_ERR_INTERNAL:
                       fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
                       break;
               }
            }
            #endif
    
    /***** Messages: pre-conditions **********************************/
/*  OLD_STYLE:
   
    add_household_send_data_message(
	    household_id, region_id, general_skill, employment_status, double wage, double specific_skill,
		just_employed, just_unemployed, start_employed, start_unemployed, enter_matching, unemployed_duration, consumption_budget_in_month, payment_account, mean_net_income, consumption_expenditure)        

    add_household_send_data_message(0,1,1, 1, 0.0, 0.0, 0,0,0,0,0,0,0,0,0, 1.0);
    add_household_send_data_message(0,1,2,-1, 0.0, 0.0, 0,0,0,0,0,0,0,0,0, 2.0);
    
    add_household_send_data_message(0,2,1, 1, 0.0, 0.0, 0,0,0,0,0,0,0,0,0, 3.0);
    add_household_send_data_message(0,2,2,-1, 0.0, 0.0, 0,0,0,0,0,0,0,0,0, 4.0);
*/  

    household_info_adt household_info;
    init_household_info_adt(&household_info);

    household_info.region_id = 1;
    household_info.general_skill = 1;
    household_info.specific_skill = 1;
    household_info.employment_status = 1; //if -1 then unemployed
    household_info.consumption_expenditure = 1.0;

    // New-style message:
    add_household_send_data_message(ID, household_info);

    
    household_info.region_id = 1;
    household_info.general_skill = 2;
    household_info.specific_skill = 2;
    household_info.employment_status = -1; //if -1 then unemployed
    household_info.consumption_expenditure = 2.0;

    // New-style message:
    add_household_send_data_message(ID, household_info);


    household_info.region_id = 2;
    household_info.general_skill = 1;
    household_info.specific_skill = 1;
    household_info.employment_status = 1; //if -1 then unemployed
    household_info.consumption_expenditure = 3.0;

    // New-style message:
    add_household_send_data_message(ID, household_info);

    
    household_info.region_id = 2;
    household_info.general_skill = 2;
    household_info.specific_skill = 3;
    household_info.employment_status = -1; //if -1 then unemployed
    household_info.consumption_expenditure = 4.0;

    // New-style message:
    add_household_send_data_message(ID, household_info);

    /***** Message: Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_household_send_data, &i_household_send_data);
            
    if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create Iterator for 'household_send_data'\n");
               switch(rc) {
                   case MB_ERR_INVALID:
                       fprintf(stderr, "\t reason: 'household_send_data' board is invalid\n");
                       break;
                   case MB_ERR_LOCKED:
                       fprintf(stderr, "\t reason: 'household_send_data' board is locked\n");
                       break;
                   case MB_ERR_MEMALLOC:
                       fprintf(stderr, "\t reason: out of memory\n");
                       break;
                   case MB_ERR_INTERNAL:
                       fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
                       break;
               }
            }
                
    /***** Function evaluation ***************************************/
    Eurostat_read_household_data();
    //Eurostat_compute_subsistence_level();

    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(NUM_HOUSEHOLDS, 4);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].no_households, 2);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].no_households, 2);
    
    CU_ASSERT_EQUAL(UNEMPLOYED, 2);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].unemployed, 1);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].unemployed, 1);
    
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].no_households_skill_1, 1);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].no_households_skill_1, 1);
    
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].no_households_skill_2, 1);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].no_households_skill_2, 1);

    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].average_s_skill, 3.0);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].average_s_skill, 4.0);

    CU_ASSERT_EQUAL(AVERAGE_S_SKILL, 7.0);


    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Eurostat_compute_subsistence_level()
 * \brief: Unit test for: Eurostat_compute_subsistence_level.
 * Status: OK 21.9.2017
 */
void unittest_Eurostat_compute_subsistence_level()
{
    int rc;
    
    int i;
    double temp;

    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();

    FLAME_environment_variable_print_debug_eurostat = 1;
    FLAME_environment_variable_const_minimum_consumption_of_median = 0.5;

    /***** Variables: Memory pre-conditions **************************/

    add_double(&CONSUMPTION_LEVELS, 1.0);
    add_double(&CONSUMPTION_LEVELS, 2.0);
    add_double(&CONSUMPTION_LEVELS, 3.0);
    add_double(&CONSUMPTION_LEVELS, 4.0);
    add_double(&CONSUMPTION_LEVELS, 5.0);
    add_double(&CONSUMPTION_LEVELS, 6.0);

    //Init 12 values in history array
    init_double_array(&HISTORY_MEDIAN_CONSUMPTION_LEVELS);

    printf("\n\n Init HISTORY_MEDIAN_CONSUMPTION_LEVELS: size=%d\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.size);
        for (i=0; i<HISTORY_MEDIAN_CONSUMPTION_LEVELS.size; i++)
            printf(" %f", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[i]);

    for (i=0; i<12; i++)
      add_double(&HISTORY_MEDIAN_CONSUMPTION_LEVELS, i);

    printf("\n Set HISTORY_MEDIAN_CONSUMPTION_LEVELS: size=%d\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.size);
        for (i=0; i<HISTORY_MEDIAN_CONSUMPTION_LEVELS.size; i++)
            printf(" %f", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[i]);

    temp = 0.5*(HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[5]+HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[6]);

    printf("\n\n Median of history before adding a new value:\n\ttemp = 0.5*(item[5]+item[6])=0.5*(%2.2f+%2.2f)=%2.2f\n\tMEDIAN_MEDIAN_CONSUMPTION_LEVEL = %2.2f\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[5], HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[6], temp, MEDIAN_MEDIAN_CONSUMPTION_LEVEL);


    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    
    Eurostat_compute_subsistence_level();

    printf("\n\n After call: HISTORY_MEDIAN_CONSUMPTION_LEVELS: size=%d\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.size);
        for (i=0; i<HISTORY_MEDIAN_CONSUMPTION_LEVELS.size; i++)
            printf(" %f", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[i]);

    temp = 0.5*(HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[5]+HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[6]);

    printf("\n\n Median of history after adding a new value:\n\ttemp = 0.5*(item[5]+item[6])=0.5*(%2.2f+%2.2f)=%2.2f\n\tMEDIAN_MEDIAN_CONSUMPTION_LEVEL = %2.2f\n", HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[5], HISTORY_MEDIAN_CONSUMPTION_LEVELS.array[6], temp, MEDIAN_MEDIAN_CONSUMPTION_LEVEL);

    /***** Variables: Memory post-conditions *****/
    //  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //  CU_ASSERT_EQUAL(var, result);

    CU_ASSERT_DOUBLE_EQUAL(MEDIAN_CONSUMPTION_LEVEL, 3.5, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(MEDIAN_MEDIAN_CONSUMPTION_LEVEL, 6.5, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(MINIMUM_SUBSISTENCE_LEVEL, 2.75, 1e-3);

    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Eurostat_calc_macro_data()
 * \brief: Unit test for: Eurostat_calc_macro_data.
 * Status: Tested OK 30.09.17
 */
void unittest_Eurostat_calc_macro_data_1()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    MONTHLY_SOLD_QUANTITY=0.0;
    MONTHLY_REVENUE=0.0;
    LABOUR_SHARE_RATIO=0.0;
    
    //Allocate memory
/*  firm_data(REGION_ID, FIRMS, VACANCIES,
    NO_EMPLOYEES, NO_EMPLOYEES_SKILL_1, NO_EMPLOYEES_SKILL_2, NO_EMPLOYEES_SKILL_3, NO_EMPLOYEES_SKILL_4, NO_EMPLOYEES_SKILL_5, 
    AVERAGE_WAGE, AVERAGE_WAGE_SKILL_1, AVERAGE_WAGE_SKILL_2, AVERAGE_WAGE_SKILL_3, AVERAGE_WAGE_SKILL_4, AVERAGE_WAGE_SKILL_5, 
    AVERAGE_S_SKILLS, AVERAGE_S_SKILL_1, AVERAGE_S_SKILL_2, AVERAGE_S_SKILL_3, AVERAGE_S_SKILL_4, AVERAGE_S_SKILL_5,

    CUM_REVENUE, CAPITAL_COSTS, NET_EARNINGS, TOTAL_DEBT, TOTAL_ASSETS, EQUITY,
    PRICE, PRICE_LAST_MONTH, 
    quality,
    TOTAL_SUPPLY, CUM_TOTAL_SOLD_QUANTITY, OUTPUT, PLANNED_OUTPUT, AGE,
    FIRM_PRODUCTIVITY, FIRM_PRODUCTIVITY_PROGRESS,
    value_inventory,
    units_capital_stock, base_wage_offer, dividends, potential_output, capacity_utilization_rate);
*/  
    add_firm_data(&REGION_FIRM_DATA,
            1,0,0,0,                  //4 REGION_ID, NO_FIRMS, no_active_firms, VACANCIES
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 total_mean_wage, average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 average_s_skill
            0.0,0.0,0.0,0.0,          //4 NET_EARNINGS, TOTAL_DEBT, TOTAL_ASSETS, EQUITY,
            0.0,0.0,0.0,              //3 RATIOS 
            0.0,0.0,0.0,0.0,          //4 monthly_sold_quantity, monthly_output, monthly_revenue, monthly_planned_output
            0.0,0.0,                  //2 monthly_investment_value, investment_gdp_ratio, 
            0.0,0.0,0.0,              //3 gdp, cpi, cpi_last_month
            0.0,0.0,                  //2 no_firm_births, no_firm_deaths, 
            0.0,0.0);                 //2 productivity_progress, productivity

    add_firm_data(&REGION_FIRM_DATA,
            2,0,0,0,                  //4 REGION_ID, FIRMS, no_active_firms, VACANCIES
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 total_mean_wage, average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0,  //6 average_s_skill
            0.0,0.0,0.0,0.0,          //4 NET_EARNINGS, TOTAL_DEBT, TOTAL_ASSETS, EQUITY,
            0.0,0.0,0.0,              //3 RATIOS 
            0.0,0.0,0.0,0.0,          //4 monthly_sold_quantity, monthly_output, monthly_revenue, monthly_planned_output
            0.0,0.0,                  //2 monthly_investment_value, investment_gdp_ratio, 
            0.0,0.0,0.0,              //3 gdp, cpi, cpi_last_month
            0.0,0.0,                  //2 no_firm_births, no_firm_deaths, 
            0.0,0.0);                 //2 productivity_progress, productivity

    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_firm_send_data, sizeof(m_firm_send_data));
    rc = MB_Create(&b_bank_send_data, sizeof(m_bank_send_data));
    rc = MB_Create(&b_clearinghouse_to_eurostat_data, sizeof(m_clearinghouse_to_eurostat_data));

    /***** Messages: pre-conditions **********************************/
    //Set 4 firms in 2 regions, using these vars:
    //  firm_send_data_message->region_id;
    //  firm_send_data_message->cum_revenue;
    //  firm_send_data_message->capital_costs;
    //  firm_send_data_message->cum_total_sold_quantity;

    //Fixture 1:
    //1,0,0,10, 0,0,0,0,0, 0,0,1,0,0, 0, 100,100,100,0,0, 0,0,1000,0,0,0, 0.0,0.0, 0, 10.0,0,0,0,0,0.0,0.0, 0,0,0,0

    int region_id=1;
    int no_employees=10;
    double mean_wage=1;
    double net_earnings=100;
    double cum_revenue=100;
    double capital_costs=100;
    double sold_quantity_in_calendar_month=1000;

    firm_info_adt firm_info_1;

    firm_info_1.region_id                                           =    region_id                                       ;
    firm_info_1.mean_wage                                           =    mean_wage                                       ;
    firm_info_1.cum_revenue                                         =    cum_revenue                                     ;
    firm_info_1.capital_costs                                       =    capital_costs                                   ;
    firm_info_1.net_earnings                                        =    net_earnings                                    ;
    firm_info_1.sold_quantity_in_calendar_month                     =    sold_quantity_in_calendar_month                 ;

    //Fixture 2:
    //2,0,0,10, 0,0,0,0,0, 0,0,1,0,0, 0,  10, 10,100,0,0, 0,0, 100,0,0,0, 0.0,0.0, 0, 10.0,0,0,0,0,0.0,0.0, 0,0,0,0
    region_id=2;
    no_employees=10;
    mean_wage=1;
    net_earnings=10;
    cum_revenue=10;
    capital_costs=10;
    sold_quantity_in_calendar_month=100;

    firm_info_adt firm_info_2;
    firm_info_2.region_id                                           =    region_id                                       ;
    firm_info_2.mean_wage                                           =    mean_wage                                       ;
    firm_info_2.cum_revenue                                         =    cum_revenue                                     ;
    firm_info_2.capital_costs                                       =    capital_costs                                   ;
    firm_info_2.net_earnings                                        =    net_earnings                                    ;
    firm_info_2.sold_quantity_in_calendar_month                     =    sold_quantity_in_calendar_month                 ;

    add_firm_send_data_message(1, firm_info_1);
    add_firm_send_data_message(2, firm_info_1);
    add_firm_send_data_message(3, firm_info_2);
    add_firm_send_data_message(4, firm_info_2);

    /***** Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_firm_send_data, &i_firm_send_data);
    rc = MB_Iterator_Create(b_bank_send_data, &i_bank_send_data);
    rc = MB_Iterator_Create(b_clearinghouse_to_eurostat_data, &i_clearinghouse_to_eurostat_data);
                        
    /***** Function evaluation ***************************************/
    Eurostat_calc_macro_data();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    
    //After the message loop, check the aggregate in both regions
    //GDP=cum_revenue+capital_costs
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[0].gdp, 400.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[0].monthly_sold_quantity, 2000.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[0].monthly_revenue, 200.0, 1e-3);
    
    //GDP=cum_revenue+capital_costs
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[1].gdp, 40.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[1].monthly_sold_quantity, 200.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[1].monthly_revenue, 20.0, 1e-3);
    
    //And the economy-wide totals
    CU_ASSERT_DOUBLE_EQUAL(MONTHLY_SOLD_QUANTITY, 2200.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(MONTHLY_REVENUE, 220.0, 1e-3);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}



/*
 * \fn: void unittest_Eurostat_calc_macro_data()
 * \brief: Unit test for: Eurostat_calc_macro_data.
 * Status: Tested OK 30.09.17
 */
void unittest_Eurostat_calc_macro_data_2()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_firm_send_data, sizeof(m_firm_send_data));
    rc = MB_Create(&b_bank_send_data, sizeof(m_bank_send_data));
    rc = MB_Create(&b_clearinghouse_to_eurostat_data, sizeof(m_clearinghouse_to_eurostat_data));

    double equity = 1.0;
    double total_assets = 1.0;
    double ecb_debt = 1.0;
    double equity_asset_ratio = 1.0;
    double debt_equity_ratio = 1.0;
    double rea = 1.0;
    double pl_mortgage_loans = 1.0;
    double npl_mortgage_loans = 1.0;
    double total_credit = 1.0;
    double total_bad_debt_calendar_month = 1.0;

    financial_data_adt financial_data;
    
    //fixture 1:
    financial_data.equity   = equity;
    financial_data.total_assets = total_assets;
    financial_data.total_debt = ecb_debt;
    financial_data.equity_asset_ratio = equity_asset_ratio;
    financial_data.debt_equity_ratio = debt_equity_ratio;
    financial_data.rea = rea;
    financial_data.pl_mortgage_loans = pl_mortgage_loans;
    financial_data.npl_mortgage_loans = npl_mortgage_loans;
    financial_data.pl_firm_loans = total_credit;
    financial_data.npl_firm_loans = total_bad_debt_calendar_month;

    // send financial_data
    add_bank_send_data_message(1, financial_data);

    //fixture 2:    
    financial_data.equity   = equity;
    financial_data.total_assets = total_assets;
    financial_data.total_debt = ecb_debt;
    financial_data.equity_asset_ratio = equity_asset_ratio;
    financial_data.debt_equity_ratio = debt_equity_ratio;
    financial_data.rea = rea;
    financial_data.pl_mortgage_loans = pl_mortgage_loans;
    financial_data.npl_mortgage_loans = npl_mortgage_loans;
    financial_data.pl_firm_loans = total_credit;
    financial_data.npl_firm_loans = total_bad_debt_calendar_month;

    // Send financial_data
    add_bank_send_data_message(2, financial_data);


    /***** Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_firm_send_data, &i_firm_send_data);
    rc = MB_Iterator_Create(b_bank_send_data, &i_bank_send_data);
    rc = MB_Iterator_Create(b_clearinghouse_to_eurostat_data, &i_clearinghouse_to_eurostat_data);

    /***** Function evaluation ***************************************/
    Eurostat_calc_macro_data();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(BANK_EQUITY, 2.0, 1e-3);
         
    CU_ASSERT_DOUBLE_EQUAL(BANK_ASSETS, 2.0, 1e-3);
         
    CU_ASSERT_DOUBLE_EQUAL(BANK_REA, 2.0, 1e-3);

    CU_ASSERT_DOUBLE_EQUAL(BANK_PL_MORTGAGE_LOANS, 2.0, 1e-3);
         
    CU_ASSERT_DOUBLE_EQUAL(BANK_NPL_MORTGAGE_LOANS, 2.0, 1e-3);
         
    CU_ASSERT_DOUBLE_EQUAL(BANK_PL_FIRM_LOANS, 2.0, 1e-3);
          
    CU_ASSERT_DOUBLE_EQUAL(BANK_NPL_FIRM_LOANS, 2.0, 1e-3);
          


    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_sum_array()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double_array A;
    init_double_array(&A);

    for (i=0; i<10; i++)
        add_double(&A, i+1);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);


    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double sum1=0.0;

    sum_array(&A, 1, 2, &sum1);

    printf("\n\nSum1 after call %2.2f", sum1);

    double sum2 = 0.0;
    sum_array(&A, 1, 3, &sum2);

    printf("\n\nSum2 after call %2.2f", sum2);

    double sum3 = 0.0;
    sum_array(&A, 1, 9, &sum3);

    printf("\n\nSum3 after call %2.2f", sum3);

    double sum4 = 0.0;
    sum_array(&A, 1, 10, &sum4);

    double sum5 = 0.0;
    sum_array(&A, 1, 11, &sum5);

    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum1, 5.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum2, 9.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum3, 54.0, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_sum_array_backwards()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    double_array A;
    init_double_array(&A);

    for (i=0; i<10; i++)
        add_double(&A, i+1);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double sum1 = 0.0;
    sum_array_backwards(&A, 1, 2, &sum1);
    printf("\n\nSum1 after call %2.2f", sum1);

    double sum2 = 0.0;
    sum_array_backwards(&A, 1, 3, &sum2);
    printf("\n\nSum2 after call %2.2f", sum2);
    
    double sum3 = 0.0;
    sum_array_backwards(&A, 1, 9, &sum3);
    printf("\n\nSum3 after call %2.2f", sum3);

    double sum4 = 0.0;
    sum_array_backwards(&A, 1, 10, &sum4);
    printf("\n\nSum4 after call %2.2f", sum4);

    double sum5 = 0.0;
    sum_array_backwards(&A, 1, 11, &sum5);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum1, 19.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum2, 27.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum3, 54.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(sum4, 55.0, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_ratio_qoq()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double_array A;
    init_double_array(&A);

    for (i=0; i<30; i++)
        add_double(&A, 1.0);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double ratio = 0.0;
    ratio = ratio_qoq(&A);

    printf("\n\nRatio after call: return value = %2.2f var = %2.2f", ratio_qoq(&A), ratio);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(ratio, 1.0, 1e-3);


    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_ratio_yoy()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double_array A;
    init_double_array(&A);

    for (i=0; i<30; i++)
        add_double(&A, 1.0);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double ratio = 0.0;
    ratio = ratio_yoy(&A);
    
    printf("\n\nRatio after call %2.2f %2.2f", ratio_yoy(&A), ratio);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(ratio, 1.0, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_pct_change_qoq()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double_array A;
    init_double_array(&A);

    for (i=0; i<3; i++)
        add_double(&A, 1.0);
    for (i=3; i<6; i++)
        add_double(&A, 2.0);
    for (i=6; i<9; i++)
        add_double(&A, 3.0);
    for (i=9; i<12; i++)
        add_double(&A, 4.0);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double growth = 0.0;
    growth = pct_change_qoq(&A);
    
    printf("\nAfter call: return value = %2.2f, var=%2.2f", pct_change_qoq(&A), growth);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(growth, (4.0/3.0-1)*100, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_pct_change_yoy()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double_array A;
    init_double_array(&A);

    for (i=0; i<12; i++)
        add_double(&A, 1.0);
    for (i=12; i<24; i++)
        add_double(&A, 2.0);
    for (i=24; i<36; i++)
        add_double(&A, 3.0);

    printf("\n\nBefore Size %d;", A.size);
    for (i=0; i<A.size; i++)
      printf(" %2.2f", A.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    double growth = 0.0;
    growth = pct_change_yoy(&A);
    
    printf("\nAfter call: return value = %2.2f, var=%2.2f", pct_change_yoy(&A), growth);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(growth, (3.0/2.0-1)*100, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_compute_statistics_economy()
{
    int i;  
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    data_adt DATA;
    init_data_adt(&DATA);

/*
    DATA.economy.levels.ts.data.array[i]

    DATA.economy.levels.ts.mean
    DATA.economy.levels.ts.median
    DATA.economy.levels.ts.sd
    DATA.economy.levels.ts.cv

    DATA.economy.pct_change_qoq_series.ts.data.array[0]
    
    DATA.economy.pct_change_qoq_series.current
    DATA.economy.pct_change_qoq_series.ts.mean
    DATA.economy.pct_change_qoq_series.ts.median
    DATA.economy.pct_change_qoq_series.ts.sd
    DATA.economy.pct_change_qoq_series.ts.cv

    DATA.economy.pct_change_yoy_series.ts.data.array[0]
    
    DATA.economy.pct_change_yoy_series.current
    DATA.economy.pct_change_yoy_series.ts.mean
    DATA.economy.pct_change_yoy_series.ts.median
    DATA.economy.pct_change_yoy_series.ts.sd
    DATA.economy.pct_change_yoy_series.ts.cv

*/
    for (i=0; i<12; i++)
        add_double(&DATA.economy.levels.ts.data, 1.0);
    for (i=12; i<24; i++)
        add_double(&DATA.economy.levels.ts.data, 2.0);
    for (i=24; i<36; i++)
        add_double(&DATA.economy.levels.ts.data, 3.0);

    printf("\n\nBefore Size %d;", DATA.economy.levels.ts.data.size);

    for (i=0; i<DATA.economy.levels.ts.data.size; i++)
      printf(" %2.2f", DATA.economy.levels.ts.data.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    //void compute_statistics_economy(data_adt * x)

    compute_statistics_economy(&DATA);

    printf("\nAfter call");

    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    
    printf("\nDATA.economy.levels.ts.mean: %2.2f", DATA.economy.levels.ts.mean);
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.levels.ts.mean, 2.0, 1e-3);

    printf("\nDATA.economy.levels.ts.median: %2.2f", DATA.economy.levels.ts.median);
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.levels.ts.median, 2.0, 1e-3);

    printf("\nDATA.economy.levels.ts.sd: %2.2f", DATA.economy.levels.ts.sd);
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.levels.ts.sd, 0.830, 1e-2);

    printf("\nDATA.economy.levels.ts.cv: %2.2f", DATA.economy.levels.ts.cv);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.levels.ts.cv, 0.410, 1e-2);


    
    printf("\nDATA.economy.pct_change_qoq_series.current: %2.2f", DATA.economy.pct_change_qoq_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.pct_change_qoq_series.current, 0.0, 1e-3);

    printf("\nDATA.economy.pct_change_yoy_series.current: %2.2f", DATA.economy.pct_change_yoy_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.economy.pct_change_yoy_series.current, 50.0, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_compute_statistics_regions()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    data_adt DATA;
    init_data_adt(&DATA);

    history_adt history;
    init_history_adt(&history);

    int total_regions=2;

    for (i=0; i<total_regions; i++)
        add_history_adt(&DATA.regions,
                        &(history.levels), 
                        &(history.pct_change_qoq_series),
                        &(history.pct_change_yoy_series));

/*
    DATA.regions.array[0].levels.ts.data.array[i]

    DATA.regions.array[0].levels.ts.mean
    DATA.regions.array[0].levels.ts.median
    DATA.regions.array[0].levels.ts.sd
    DATA.regions.array[0].levels.ts.cv

    DATA.regions.array[0].pct_change_qoq_series.ts.data.array[0]
    
    DATA.regions.array[0].pct_change_qoq_series.current
    DATA.regions.array[0].pct_change_qoq_series.ts.mean
    DATA.regions.array[0].pct_change_qoq_series.ts.median
    DATA.regions.array[0].pct_change_qoq_series.ts.sd
    DATA.regions.array[0].pct_change_qoq_series.ts.cv

    DATA.regions.array[0].pct_change_yoy_series.ts.data.array[0]
    
    DATA.regions.array[0].pct_change_yoy_series.current
    DATA.regions.array[0].pct_change_yoy_series.ts.mean
    DATA.regions.array[0].pct_change_yoy_series.ts.median
    DATA.regions.array[0].pct_change_yoy_series.ts.sd
    DATA.regions.array[0].pct_change_yoy_series.ts.cv

*/

/* Region 0 */
    for (i=0; i<12; i++)
        add_double(&DATA.regions.array[0].levels.ts.data, 1.0);
    for (i=12; i<24; i++)
        add_double(&DATA.regions.array[0].levels.ts.data, 2.0);
    for (i=24; i<36; i++)
        add_double(&DATA.regions.array[0].levels.ts.data, 3.0);

    printf("\n\nBefore Size %d;", DATA.regions.array[0].levels.ts.data.size);

    for (i=0; i<DATA.regions.array[0].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[0].levels.ts.data.array[i]);

/* Region 1 */
    for (i=0; i<12; i++)
        add_double(&DATA.regions.array[1].levels.ts.data, 1.0);
    for (i=12; i<24; i++)
        add_double(&DATA.regions.array[1].levels.ts.data, 2.0);
    for (i=24; i<36; i++)
        add_double(&DATA.regions.array[1].levels.ts.data, 3.0);

    printf("\n\nBefore Size %d;", DATA.regions.array[1].levels.ts.data.size);

    for (i=0; i<DATA.regions.array[1].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[1].levels.ts.data.array[i]);
    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    //void compute_statistics_regions(data_adt * x, int i)

  /* Region 0 */

    compute_statistics_regions(&DATA, 0);

    printf("\nAfter call");
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    printf("\nDATA.regions.array[0].levels.ts.mean: %2.2f", DATA.regions.array[0].levels.ts.mean);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].levels.ts.mean, 2.0, 1e-3);

    printf("\nDATA.regions.array[0].levels.ts.median: %2.2f", DATA.regions.array[0].levels.ts.median);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].levels.ts.median, 2.0, 1e-3);

    printf("\nDATA.regions.array[0].levels.ts.sd: %2.2f", DATA.regions.array[0].levels.ts.sd);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].levels.ts.sd, 0.83, 1e-2);

    printf("\nDATA.regions.array[0].levels.ts.cv: %2.2f", DATA.regions.array[0].levels.ts.cv);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].levels.ts.cv, 0.41, 1e-2);
    
    printf("\nDATA.regions.array[0].pct_change_qoq_series.current: %2.2f", DATA.regions.array[0].pct_change_qoq_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].pct_change_qoq_series.current, 0.0, 1e-3);

    printf("\nDATA.regions.array[0].pct_change_yoy_series.current: %2.2f", DATA.regions.array[0].pct_change_yoy_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[0].pct_change_yoy_series.current, 50.0, 1e-3);

/* Region 1 */

    compute_statistics_regions(&DATA, 1);

    printf("\nAfter call");
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    printf("\nDATA.regions.array[1].levels.ts.mean: %2.2f", DATA.regions.array[1].levels.ts.mean);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].levels.ts.mean, 2.0, 1e-3);

    printf("\nDATA.regions.array[1].levels.ts.median: %2.2f", DATA.regions.array[1].levels.ts.median);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].levels.ts.median, 2.0, 1e-3);

    printf("\nDATA.regions.array[1].levels.ts.sd: %2.2f", DATA.regions.array[1].levels.ts.sd);
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].levels.ts.sd, 0.83, 1e-2);

    printf("\nDATA.regions.array[1].levels.ts.cv: %2.2f", DATA.regions.array[1].levels.ts.cv);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].levels.ts.cv, 0.41, 1e-2);


    
    printf("\nDATA.regions.array[1].pct_change_qoq_series.current: %2.2f", DATA.regions.array[1].pct_change_qoq_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].pct_change_qoq_series.current, 0.0, 1e-3);

    printf("\nDATA.regions.array[1].pct_change_yoy_series.current: %2.2f", DATA.regions.array[1].pct_change_yoy_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(DATA.regions.array[1].pct_change_yoy_series.current, 50.0, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_update_timeseries_economy()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    data_adt DATA;
    init_data_adt(&DATA);

    for (i=0; i<2; i++)
        add_double(&DATA.economy.levels.ts.data, 1.0);

    printf("\n\nBefore Size %d;", DATA.economy.levels.ts.data.size);

    for (i=0; i<DATA.economy.levels.ts.data.size; i++)
      printf(" %2.2f", DATA.economy.levels.ts.data.array[i]);
    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    //void update_timeseries_economy(data_adt * x, double value)
    update_timeseries_economy(&DATA, 100.0);
    
    printf("\nAfter call");

    for (i=0; i<DATA.economy.levels.ts.data.size; i++)
      printf(" %2.2f", DATA.economy.levels.ts.data.array[i]);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 */
void unittest_update_timeseries_regions()
{
    int i;
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    data_adt DATA;
    init_data_adt(&DATA);

/* Region 0 */
    for (i=0; i<2; i++)
        add_double(&DATA.regions.array[0].levels.ts.data, 1.0);

    printf("\n\nBefore Size %d;", DATA.regions.array[0].levels.ts.data.size);
    printf("\nRegion 0:");
    for (i=0; i<DATA.regions.array[0].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[0].levels.ts.data.array[i]);

/* Region 1 */
    for (i=0; i<2; i++)
        add_double(&DATA.regions.array[1].levels.ts.data, 1.0);

    printf("\n\nBefore Size %d;", DATA.regions.array[1].levels.ts.data.size);
    printf("\nRegion 1:");
    for (i=0; i<DATA.regions.array[1].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[1].levels.ts.data.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    //void update_timeseries_regions(data_adt * x, int i, double value)
    update_timeseries_regions(&DATA, 0, 100.0);

    update_timeseries_regions(&DATA, 1, 100.0);
    
    printf("\nAfter call");

    printf("\nRegion 0:");
    for (i=0; i<DATA.regions.array[0].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[0].levels.ts.data.array[i]);

    printf("\nRegion 1:");
    for (i=0; i<DATA.regions.array[1].levels.ts.data.size; i++)
      printf(" %2.2f", DATA.regions.array[1].levels.ts.data.array[i]);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: TESTED OK 8.10.17
 * NOTE: This unittest only tests for one variable, GDP_DATA.
 * For this unittest to pass, you have to comment out in the main function
 * Eurostat_compute_timeseries_statistics(): all other variables that are updated (ie *_DATA).
 */
void unittest_Eurostat_compute_timeseries_statistics()
{
    int i;  
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    GDP = 10.0;

    //data_adt GDP_DATA;
    init_data_adt(&GDP_DATA);

/*
    DATA.economy.levels.ts.data.array[i]

    DATA.economy.levels.ts.mean
    DATA.economy.levels.ts.median
    DATA.economy.levels.ts.sd
    DATA.economy.levels.ts.cv

    DATA.economy.pct_change_qoq_series.ts.data.array[0]
    
    DATA.economy.pct_change_qoq_series.current
    DATA.economy.pct_change_qoq_series.ts.mean
    DATA.economy.pct_change_qoq_series.ts.median
    DATA.economy.pct_change_qoq_series.ts.sd
    DATA.economy.pct_change_qoq_series.ts.cv

    DATA.economy.pct_change_yoy_series.ts.data.array[0]
    
    DATA.economy.pct_change_yoy_series.current
    DATA.economy.pct_change_yoy_series.ts.mean
    DATA.economy.pct_change_yoy_series.ts.median
    DATA.economy.pct_change_yoy_series.ts.sd
    DATA.economy.pct_change_yoy_series.ts.cv

*/
    for (i=0; i<12; i++)
        add_double(&GDP_DATA.economy.levels.ts.data, 1.0);
    for (i=12; i<24; i++)
        add_double(&GDP_DATA.economy.levels.ts.data, 2.0);
    for (i=24; i<36; i++)
        add_double(&GDP_DATA.economy.levels.ts.data, 3.0);

    printf("\n\nBefore Size %d;", GDP_DATA.economy.levels.ts.data.size);

    for (i=0; i<GDP_DATA.economy.levels.ts.data.size; i++)
      printf(" %2.2f", GDP_DATA.economy.levels.ts.data.array[i]);

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    Eurostat_compute_timeseries_statistics();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    printf("\nAfter call");
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    
    printf("\nGDP_DATA.economy.levels.ts.mean: %2.2f", GDP_DATA.economy.levels.ts.mean);
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.levels.ts.mean, 2.0, 1e-3);

    printf("\nGDP_DATA.economy.levels.ts.median: %2.2f", GDP_DATA.economy.levels.ts.median);
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.levels.ts.median, 2.0, 1e-3);

    printf("\nGDP_DATA.economy.levels.ts.sd: %2.2f", GDP_DATA.economy.levels.ts.sd);
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.levels.ts.sd, 0.830, 1e-2);

    printf("\nGDP_DATA.economy.levels.ts.cv: %2.2f", GDP_DATA.economy.levels.ts.cv);    
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.levels.ts.cv, 0.410, 1e-2);
    
    printf("\nGDP_DATA.economy.pct_change_qoq_series.current: %2.2f", GDP_DATA.economy.pct_change_qoq_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.pct_change_qoq_series.current, 0.0, 1e-3);

    printf("\nGDP_DATA.economy.pct_change_yoy_series.current: %2.2f", GDP_DATA.economy.pct_change_yoy_series.current);    
    CU_ASSERT_DOUBLE_EQUAL(GDP_DATA.economy.pct_change_yoy_series.current, 50.0, 1e-3);


    printf("\nAfter UPDATE");

    for (i=0; i<GDP_DATA.economy.levels.ts.data.size; i++)
      printf(" %2.2f", GDP_DATA.economy.levels.ts.data.array[i]);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}




/*
 * \fn: void unittest_Eurostat_compute_macro_indicators()
 * \brief: Unit test for: Eurostat_compute_macro_indicators.
 * Status: TODO
 * NOTE: This unittest tests if 
 */
void unittest_Eurostat_compute_macro_indicators()
{
    int i;  
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
	DAY = 6040;	
	FLAME_environment_variable_const_init_transient_period = 6001;
	FLAME_environment_variable_const_housing_market_backward_horizon = 12;
	FLAME_environment_variable_const_rental_market_backward_horizon = 12;
	FLAME_environment_variable_print_eurostat_info = 1;
	FLAME_environment_variable_print_debug_eurostat = 1;

	HOUSING_MARKET_PRICE_MEAN = 1.20;
	RENTAL_MARKET_PRICE_MEAN = 1.30;

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
    Eurostat_compute_macro_indicators();
    
    	DAY = 6060;	

		add_double(&HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data, HOUSING_MARKET_PRICE_MEAN+1);
		add_double(&RENTAL_MARKET_PRICE_INDEX_DATA.economy.levels.ts.data, RENTAL_MARKET_PRICE_MEAN+1);

	    CU_ASSERT_DOUBLE_EQUAL(HOUSING_MARKET_PRICE_GROWTH_FACTOR, 1.00, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(RENTAL_MARKET_PRICE_GROWTH_FACTOR, 1.00, 1e-3);

    Eurostat_compute_macro_indicators();

	    CU_ASSERT_DOUBLE_EQUAL(HOUSING_MARKET_PRICE_GROWTH_FACTOR, 1.83, 1e-2);
		CU_ASSERT_DOUBLE_EQUAL(RENTAL_MARKET_PRICE_GROWTH_FACTOR, 1.77, 1e-2);

    	DAY = 6080;	
		
		add_double(&HOUSING_PRICE_INDEX_DATA.economy.levels.ts.data, HOUSING_MARKET_PRICE_MEAN+2);
		add_double(&RENTAL_MARKET_PRICE_INDEX_DATA.economy.levels.ts.data, RENTAL_MARKET_PRICE_MEAN+2);

    Eurostat_compute_macro_indicators();
     
    CU_ASSERT_DOUBLE_EQUAL(HOUSING_MARKET_PRICE_GROWTH_FACTOR, 2.67, 1e-2);
	CU_ASSERT_DOUBLE_EQUAL(RENTAL_MARKET_PRICE_GROWTH_FACTOR, 2.54, 1e-2);

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    //printf("\nAfter call");
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
