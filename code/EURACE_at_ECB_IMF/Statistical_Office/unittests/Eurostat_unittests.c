#include <CUnit/Basic.h>
#include "../../header.h"
#include "../../Eurostat_agent_header.h"
#include "../../my_library_header.h"

/************Eurostat Role: Statistical Office ********************************/

/************ Unit tests ********************************/

/*
 * \fn: void unittest_()
 * \brief: Unit test for: .
 * Status: 
 */
void unittest_asserts()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    double x = 9.0;
    int n = 9;

    ID = 1;

    /***** Messages: initialize message boards **********************************/
    //rc = MB_Create(&b_<name>, sizeof(m_<name>));


    /***** Adding message iterators ***************************************/
    //rc = MB_Iterator_Create(b_<name>, &i_<name>);
                        
    /***** Function evaluation ***************************************/
   
   int test_larger = 0; 
  
    if(test_larger)
    {
      //These should fail on the 1st assert:
      assert(x > 1e+1);
      assert(n > 1e+1);
      assert(ID > 2);
    }
    else
    {
      //These should not raise any errors:
        assert(x < 1e+1);
        assert(n < 1e+1);
        assert(ID < 2);
    }   

    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(, result, 1e-3);

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Eurostat_compute_house_price_index()
 * \brief: Unit test for: Eurostat_compute_house_price_index.
 * \brief: - input tenant_info_message
 * \brief: - input landlord_info_message
 * \brief: - input owner_info_message
 * \brief: - output house_price_index_message(HMR_PRICE_INDEX, OP_PRICE_INDEX, RENT_PRICE_INDEX);
 * Status: NOT Tested
 */
void unittest_Eurostat_compute_house_price_index()
{
    int rc;
  
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    FLAME_environment_variable_print_debug_rental = 1;
    FLAME_environment_variable_print_debug_rental_unittest = 1;

    /***** Variables: Memory pre-conditions **************************/
    DAY=2;

  /***** Messages: initialize message boards **********************************/
  rc = MB_Create(&b_tenant_info, sizeof(m_tenant_info));
  rc = MB_Create(&b_landlord_info, sizeof(m_landlord_info));
  rc = MB_Create(&b_owner_info, sizeof(m_owner_info));
  rc = MB_Create(&b_homeless_info, sizeof(m_homeless_info));

  /***** Messages: pre-conditions **********************************/
 
  //add_landlord_info_message(ID, hmr_value, op_value);
  add_landlord_info_message(0, 100.0);
  add_landlord_info_message(0, 100.0);
  
  //add_owner_info_message(ID, hmr_value);
  add_owner_info_message(0, 100.0);

  //add_tenant_info_message(ID, monthly_rent);
  add_tenant_info_message(0, 1.0);
  add_tenant_info_message(0, 2.0);

  //add_tenant_info_message(ID);
  add_homeless_info_message(1);
  add_homeless_info_message(2);

  rc = MB_Iterator_Create(b_tenant_info, &i_tenant_info);
  rc = MB_Iterator_Create(b_landlord_info, &i_landlord_info);
  rc = MB_Iterator_Create(b_owner_info, &i_owner_info);
  rc = MB_Iterator_Create(b_homeless_info, &i_homeless_info);

    /***** Function evaluation ***************************************/
    Eurostat_compute_house_price_index();
    
    /***** Variables: Memory post-conditions *****/
    //CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


    /***** Message: Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_house_price_index, &i_house_price_index);

    /***** Messages: Message post-conditions *****/
    //add_house_price_index_message(hmr_price_index, op_price_index, rent_price_index);

    START_HOUSE_PRICE_INDEX_MESSAGE_LOOP
       CU_ASSERT_DOUBLE_EQUAL(house_price_index_message->rent_price_index.mean, 1.5, 1e-3);
       
       CU_ASSERT_DOUBLE_EQUAL(house_price_index_message->hmr_price_index.mean, 33.33, 1e-3);
       
       CU_ASSERT_DOUBLE_EQUAL(house_price_index_message->op_price_index.mean, 200.0, 1e-3);
       
    FINISH_HOUSE_PRICE_INDEX_MESSAGE_LOOP
  
       CU_ASSERT_DOUBLE_EQUAL(RENT_PRICE_INDEX.mean, 1.5, 1e-3);
       CU_ASSERT_DOUBLE_EQUAL(HMR_PRICE_INDEX.mean, 33.33, 1e-3);
       CU_ASSERT_DOUBLE_EQUAL(OP_PRICE_INDEX.mean, 200.0, 1e-3);


    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_reset_data()
 * \brief: Unit test for: Eurostat_reset_data.
 * Status: Tested OK
 */
void unittest_Eurostat_reset_data()
{   
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FLAME_environment_variable_total_regions = 2;

    add_firm_data(&REGION_FIRM_DATA,
            1,100,0,0,               //4 region_id, no_firms, no_active_firms, vacancies 
            0,0,0,0,0,0,             //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);                //2 productivity_progress, productivity
    
    add_firm_data(&REGION_FIRM_DATA,
            2,200,0,0,               //4 region_id, no_firms, no_active_firms, vacancies 
            0,0,0,0,0,0,             //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);               //2 productivity_progress, productivity

    add_household_data(&REGION_HOUSEHOLD_DATA,
            1,                          //1 region_id
            10,0,0,0,0,0,		//6 no_households_skill
            0,0,0,0,0,0,		//6 employed_skill
            0,				//1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0,	//6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0,	//6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

    add_household_data(&REGION_HOUSEHOLD_DATA,
            2,                         //1 region_id
            20,0,0,0,0,0,              //6 no_households_skill
            0,0,0,0,0,0,               //6 employed_skill
            0,                         //1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0,   //6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0,   //6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

    
    /***** Messages: initialize message boards **********************************/

    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
   
    /***** Function evaluation ***************************************/
    Eurostat_reset_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[0].no_firms, 0);
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[1].no_firms, 0);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[0].no_households, 0);
    CU_ASSERT_EQUAL(REGION_HOUSEHOLD_DATA.array[1].no_households, 0);   
    
    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_compute_mean_price()
 * \brief: Unit test for: Eurostat_compute_mean_price.
 * Status: Tested OK
 */
void unittest_Eurostat_compute_mean_price()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_firm_send_data, sizeof(m_firm_send_data));
            #ifdef ERRCHECK
            if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create 'firm_send_data' board\n");
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

    //NOTE: lower case have been added later on
/*  add_firm_send_data_message(ID, firm_info)
firm_info:

firm_info.ID;
firm_info.REGION_ID;
firm_info.VACANCIES;
firm_info.posted_vacancies
firm_info.NO_EMPLOYEES,
firm_info.NO_EMPLOYEES_SKILL_1
firm_info.NO_EMPLOYEES_SKILL_2
firm_info.NO_EMPLOYEES_SKILL_3
firm_info.NO_EMPLOYEES_SKILL_4
firm_info.NO_EMPLOYEES_SKILL_5,
firm_info.employees_production
firm_info.employees_r_and_d
firm_info.average_wage
firm_info.average_wage_production
firm_info.average_wage_r_and_d,
firm_info.average_s_skill,
firm_info.AVERAGE_S_SKILL_1
firm_info.AVERAGE_S_SKILL_2
firm_info.AVERAGE_S_SKILL_3
firm_info.AVERAGE_S_SKILL_4
firm_info.AVERAGE_S_SKILL_5,
firm_info.CUM_REVENUE
firm_info.CAPITAL_COSTS
firm_info.NET_EARNINGS
firm_info.TOTAL_DEBT
firm_info.TOTAL_ASSETS
firm_info.EQUITY,
firm_info.PRICE
firm_info.PRICE_LAST_MONTH
firm_info.quality,
firm_info.TOTAL_SUPPLY
firm_info.CUM_TOTAL_SOLD_QUANTITY
firm_info.OUTPUT
firm_info.PLANNED_OUTPUT
firm_info.AGE
firm_info.FIRM_PRODUCTIVITY
firm_info.FIRM_PRODUCTIVITY_PROGRESS,
firm_info.value_inventory
firm_info.units_capital_stock
firm_info.base_wage_offer
firm_info.dividends );
*/  

//Fixture:
/*
    ID = 1,2;
    REGION_ID = 1;
    PRICE=1;
    TOTAL_SUPPLY=10;
*/
    firm_info_adt firm_info;
    init_firm_info_adt(&firm_info);

    firm_info.region_id = 1;
    firm_info.price = 1.0;
    firm_info.total_supply = 10.0;

    add_firm_send_data_message(1, firm_info);

    firm_info.region_id = 1;
    firm_info.price = 1.0;
    firm_info.total_supply = 10.0;
    
    add_firm_send_data_message(2, firm_info);
    
    /***** Message: Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_firm_send_data, &i_firm_send_data);
            
    if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create Iterator for 'firm_send_data'\n");
               switch(rc) {
                   case MB_ERR_INVALID:
                       fprintf(stderr, "\t reason: 'firm_send_data' board is invalid\n");
                       break;
                   case MB_ERR_LOCKED:
                       fprintf(stderr, "\t reason: 'firm_send_data' board is locked\n");
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
    Eurostat_compute_mean_price();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(PRICE_INDEX, 1.5, 1e-3);

    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_read_firm_data()
 * \brief: Unit test for: Eurostat_read_firm_data.
 * Status: Tested OK
 */
void unittest_Eurostat_read_firm_data()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    add_firm_data(&REGION_FIRM_DATA,
            1,0,0,0,                 //4 region_id, no_firms, no_active_firms, vacancies 
            0,0,0,0,0,0,             //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);               //2 productivity_progress, productivity

    add_firm_data(&REGION_FIRM_DATA,
            2,0,0,0,                 //4 region_id, no_firms, no_active_firms, vacancies 
            0,0,0,0,0,0,             //6 employees_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);               //2 productivity_progress, productivity

    
    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_firm_send_data, sizeof(m_firm_send_data));
            #ifdef ERRCHECK
            if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create 'firm_send_data' board\n");
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
    //NOTE: lower case have been added later on
/*  add_firm_send_data_message(ID, firm_info)
firm_info:

firm_info.REGION_ID;
firm_info.VACANCIES;
firm_info.posted_vacancies
firm_info.NO_EMPLOYEES,
firm_info.NO_EMPLOYEES_SKILL_1
firm_info.NO_EMPLOYEES_SKILL_2
firm_info.NO_EMPLOYEES_SKILL_3
firm_info.NO_EMPLOYEES_SKILL_4
firm_info.NO_EMPLOYEES_SKILL_5,
firm_info.employees_production
firm_info.employees_r_and_d
firm_info.average_wage
firm_info.average_wage_production
firm_info.average_wage_r_and_d,
firm_info.average_s_skill,
firm_info.AVERAGE_S_SKILL_1
firm_info.AVERAGE_S_SKILL_2
firm_info.AVERAGE_S_SKILL_3
firm_info.AVERAGE_S_SKILL_4
firm_info.AVERAGE_S_SKILL_5,
firm_info.CUM_REVENUE
firm_info.CAPITAL_COSTS
firm_info.NET_EARNINGS
firm_info.TOTAL_DEBT
firm_info.TOTAL_ASSETS
firm_info.EQUITY,
firm_info.PRICE
firm_info.PRICE_LAST_MONTH
firm_info.quality,
firm_info.TOTAL_SUPPLY
firm_info.CUM_TOTAL_SOLD_QUANTITY
firm_info.OUTPUT
firm_info.PLANNED_OUTPUT
firm_info.AGE
firm_info.FIRM_PRODUCTIVITY
firm_info.FIRM_PRODUCTIVITY_PROGRESS,
firm_info.value_inventory
firm_info.units_capital_stock
firm_info.base_wage_offer
firm_info.dividends
*/  
    //Fixture:
    //PRICE=1;
    //TOTAL_SUPPLY=10;

    printf("\n\t Adding firm_send_data_message\n");

    firm_info_adt firm_info;
    init_firm_info_adt(&firm_info);

    firm_info.region_id = 1;

    firm_info.vacancies = 100;
    firm_info.posted_vacancies = 100;
    firm_info.no_employees = 50;

    firm_info.no_employees_skill_1 = 10;
    firm_info.no_employees_skill_2 = 10;
    firm_info.no_employees_skill_3 = 10;
    firm_info.no_employees_skill_4 = 10;
    firm_info.no_employees_skill_5 = 10;

    firm_info.price = 1.0;
    firm_info.total_supply = 10.0;

    add_firm_send_data_message(1, firm_info);

    firm_info.region_id = 1;

    firm_info.vacancies = 100;
    firm_info.posted_vacancies = 100;
    firm_info.no_employees = 100;

    firm_info.no_employees_skill_1 = 20;
    firm_info.no_employees_skill_2 = 20;
    firm_info.no_employees_skill_3 = 20;
    firm_info.no_employees_skill_4 = 20;
    firm_info.no_employees_skill_5 = 20;

    firm_info.price = 2.0;
    firm_info.total_supply = 10.0;

    add_firm_send_data_message(2, firm_info);
            
    firm_info.region_id = 2;

    firm_info.vacancies = 300;
    firm_info.posted_vacancies = 300;
    firm_info.no_employees = 150;

    firm_info.no_employees_skill_1 = 30;
    firm_info.no_employees_skill_2 = 30;
    firm_info.no_employees_skill_3 = 30;
    firm_info.no_employees_skill_4 = 30;
    firm_info.no_employees_skill_5 = 30;

    firm_info.price = 1.0;
    firm_info.total_supply = 10.0;

    add_firm_send_data_message(3, firm_info);

    firm_info.region_id = 2;

    firm_info.vacancies = 300;
    firm_info.posted_vacancies = 300;
    firm_info.no_employees = 200;

    firm_info.no_employees_skill_1 = 40;
    firm_info.no_employees_skill_2 = 40;
    firm_info.no_employees_skill_3 = 40;
    firm_info.no_employees_skill_4 = 40;
    firm_info.no_employees_skill_5 = 40;

    firm_info.price = 1.0;
    firm_info.total_supply = 10.0;

    add_firm_send_data_message(4, firm_info);

    
    
    /***** Message: Adding message iterators ***************************************/
    rc = MB_Iterator_Create(b_firm_send_data, &i_firm_send_data);
            
    if (rc != MB_SUCCESS)
            {
               fprintf(stderr, "ERROR: Could not create Iterator for 'firm_send_data'\n");
               switch(rc) {
                   case MB_ERR_INVALID:
                       fprintf(stderr, "\t reason: 'firm_send_data' board is invalid\n");
                       break;
                   case MB_ERR_LOCKED:
                       fprintf(stderr, "\t reason: 'firm_send_data' board is locked\n");
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
    Eurostat_read_firm_data();
    
    /***** Variables: Memory post-conditions *****/

    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[0].no_firms, 2); 
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[1].no_firms, 2);
    
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[0].vacancies, 200);
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[1].vacancies, 600);
    CU_ASSERT_EQUAL(NO_VACANCIES, 800);
    
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[0].no_employees, 150);
    CU_ASSERT_EQUAL(REGION_FIRM_DATA.array[1].no_employees, 350);
    CU_ASSERT_EQUAL(NO_EMPLOYEES, 500);
    
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_compute_region_firm_data()
 * \brief: Unit test for: Eurostat_compute_region_firm_data.
 * Status: Tested OK
 */
void unittest_Eurostat_compute_region_firm_data()
{   
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    add_firm_data(&REGION_FIRM_DATA,
            1,100,0,0,               //4 region_id, no_firms, no_active_firms, vacancies 
            10,0,0,0,0,0,             //6 employees, employees_skill_1 5
            100.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage, average_wage_skill_1 5
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);               //2 productivity_progress, productivity

    add_firm_data(&REGION_FIRM_DATA,
            2,200,0,0,               //4 region_id, no_firms, no_active_firms, vacancies 
            20,0,0,0,0,0,             //6 employees, employees_skill_1 5
            200.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage, average_wage_skill_1 5
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_s_skill
            0.0,0.0,0.0,0.0,0.0,     //5 total_earnings -> average_debt_earnings_ratio
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_debt_equity_ratio -> monthly_planned_output
            0.0,0.0,                 //monthly_investment_value, investment_gdp_ratio
            0.0,0.0,0.0,             //3 gdp, cpi, cpi_last_month 
            0,0,                     //2 no_firm_births, no_firm_deaths
            0.0,0.0);               //2 productivity_progress, productivity

    /***** Messages: initialize message boards **********************************/

    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
  
    /***** Function evaluation ***************************************/
    Eurostat_compute_region_firm_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[0].total_mean_wage, 100.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_FIRM_DATA.array[1].total_mean_wage, 200.0, 1e-3);
    
    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_compute_global_firm_data()
 * \brief: Unit test for: Eurostat_compute_global_firm_data.
 * Status: Tested OK
 */
void unittest_Eurostat_compute_global_firm_data()
{   
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    FIRM_AVERAGE_WAGE = 100.0;
    NO_EMPLOYEES = 10;
    
    /***** Messages: initialize message boards **********************************/

    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
        
    /***** Function evaluation ***************************************/
    Eurostat_compute_global_firm_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(FIRM_AVERAGE_WAGE, 10.0, 1e-3);

    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_read_household_data()
 * \brief: Unit test for: Eurostat_read_household_data.
 * Status: Tested OK
 */
void unittest_Eurostat_read_household_data()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    add_household_data(&REGION_HOUSEHOLD_DATA,
            1,                       //1 region_id
            0,0,0,0,0,0,             //6 no_households_skill//no_households
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
/*    
  add_household_send_data_message(id, household_info)

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

  household_info.is_homeless  = IS_HOMELESS;
  household_info.is_tenant  = IS_TENANT;
  household_info.is_owner   = IS_OWNER;
  household_info.is_landlord  = IS_LANDLORD;
  household_info.has_btl_gene = BTL_GENE;

  household_info.hh_no_mortgages       = HH_NO_MORTGAGES;

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

  Old-style:    
	household_id, region_id, general_skill, employment_status, double wage, double specific_skill,
		just_employed, just_unemployed, start_employed, start_unemployed, enter_matching, unemployed_duration, consumption_budget_in_month, payment_account, mean_net_income)        
*/  

  household_info_adt household_info;
  init_household_info_adt(&household_info);
  
  // New-style message:
  //add_household_send_data_message(ID, household_info);

  household_info.region_id = 1;
  household_info.general_skill = 1;
  household_info.employment_status = 1; //if -1 then unemployed
  household_info.hh_mortgages_op_paidoff   = 1.0;
  add_household_send_data_message(0, household_info);


  household_info.region_id = 1;
  household_info.general_skill = 2;
  household_info.employment_status = -1; //if -1 then unemployed
  household_info.hh_mortgages_op_paidoff   = 1.0;
  add_household_send_data_message(0, household_info);
    
  household_info.region_id = 2;
  household_info.general_skill = 1;
  household_info.employment_status = 1; //if -1 then unemployed
  household_info.hh_mortgages_op_paidoff   = 1.0;
  add_household_send_data_message(0, household_info);

  household_info.region_id = 2;
  household_info.general_skill = 2;
  household_info.employment_status = -1; //if -1 then unemployed
  household_info.hh_mortgages_op_paidoff   = 1.0;
  add_household_send_data_message(0, household_info);
    
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

    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_compute_region_household_data()
 * \brief: Unit test for: Eurostat_compute_region_household_data.
 * Status: Tested OK
 */
void unittest_Eurostat_compute_region_household_data()
{   
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    add_household_data(&REGION_HOUSEHOLD_DATA,
            1,                       //region_id
            10,0,0,0,0,0,            //6 no_households_skill
            10,0,0,0,0,0,            //6 employed_skill
            0,                       //1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0, //6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

    add_household_data(&REGION_HOUSEHOLD_DATA,
            2,                       //region_id
            20,0,0,0,0,0,            //6 no_households_skill
            0,0,0,0,0,0,             //6 employed_skill
            20,                      //1 unemployed
            0.0,0.0,0.0,0.0,0.0,0.0, //6 unemployment_rate_skill
            0.0,0.0,0.0,0.0,0.0,0.0, //6 average_wage_skill
            0.0,1.0,1.0,1.0,1.0,1.0, //6 average_s_skill
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //9 attributes added 30.10.17 (payment_account etc.)
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);     //8 attributes added 30.10.17 (is_owner etc.)

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
            
    /***** Function evaluation ***************************************/
    Eurostat_compute_region_household_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(REGION_HOUSEHOLD_DATA.array[0].unemployment_rate, 0.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(REGION_HOUSEHOLD_DATA.array[1].unemployment_rate, 100.0, 1e-3);
    
    /***** Messages: Message post-conditions *****/
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Eurostat_compute_global_household_data()
 * \brief: Unit test for: Eurostat_compute_global_household_data.
 * Status: Tested OK
 */
void unittest_Eurostat_compute_global_household_data()
{   
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    EMPLOYED = 75;
    NUM_HOUSEHOLDS = 100;
    AVERAGE_WAGE = 7500.0;
    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
            
    /***** Function evaluation ***************************************/
    Eurostat_compute_global_household_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(UNEMPLOYMENT_RATE, 25.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(AVERAGE_WAGE, 100.0, 1e-3);
    
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Eurostat_compute_global_household_data()
 * \brief: Unit test for: Eurostat_compute_global_household_data.
 * Status: Tested TODO
 */
void unittest_Eurostat_compute_global_household_data_2()
{   
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/

    NO_HOUSEHOLD_CLASS_1 = 10;
    NO_HOUSEHOLD_CLASS_2 = 10;
    NO_HOUSEHOLD_CLASS_3 = 25;
    NO_HOUSEHOLD_CLASS_4 = 75;

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/
            
    /***** Message: Adding message iterators ***************************************/
            
    /***** Function evaluation ***************************************/
    Eurostat_compute_global_household_data();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_PROPENSITY_1, 0.5, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_PROPENSITY_2, 0.5, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_PROPENSITY_3, 0.25, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(HOUSEHOLD_PROPENSITY_4, 0.75, 1e-3);

    printf("\n NO_HOUSEHOLD_CLASS_1: %d", NO_HOUSEHOLD_CLASS_1);
    printf("\n NO_HOUSEHOLD_CLASS_2: %d", NO_HOUSEHOLD_CLASS_2);
    printf("\n NO_HOUSEHOLD_CLASS_3: %d", NO_HOUSEHOLD_CLASS_3);
    printf("\n NO_HOUSEHOLD_CLASS_4: %d", NO_HOUSEHOLD_CLASS_4);

    printf("\nHOUSEHOLD_PROPENSITY_1: %f", HOUSEHOLD_PROPENSITY_1);
    printf("\nHOUSEHOLD_PROPENSITY_2: %f", HOUSEHOLD_PROPENSITY_2);
    printf("\nHOUSEHOLD_PROPENSITY_3: %f", HOUSEHOLD_PROPENSITY_3);
    printf("\nHOUSEHOLD_PROPENSITY_4: %f", HOUSEHOLD_PROPENSITY_4);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest1_Eurostat_measure_recession()
 * \brief: Unit test for: Eurostat_measure_recession.
 * Status: Not tested
 */
void unittest1_Eurostat_measure_recession()
{
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    RECESSION_STARTED=0;
    HISTORY_QUARTERLY[0].gdp=90;
    HISTORY_QUARTERLY[1].gdp=100;
    HISTORY_QUARTERLY[2].gdp=110;
    
    /***** Messages: pre-conditions **********************************/
    
    /***** Function evaluation ***************************************/
    Eurostat_measure_recession();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RECESSION_STARTED, 1);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest2_Eurostat_measure_recession()
 * \brief: Unit test for: Eurostat_measure_recession.
 * Case: RECESSION_STARTED=1 and after updating RECESSION_DURATION from 0 to 1, the recession continues: RECESSION_STARTED=1
 * Status: Not tested
 */
void unittest2_Eurostat_measure_recession()
{
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    RECESSION_STARTED=1;
    RECESSION_DURATION=0;
    HISTORY_QUARTERLY[0].gdp=90;
    HISTORY_QUARTERLY[1].gdp=100;
    
    /***** Messages: pre-conditions **********************************/
    
    /***** Function evaluation ***************************************/
    Eurostat_measure_recession();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RECESSION_STARTED, 1);
    CU_ASSERT_EQUAL(RECESSION_DURATION, 1); 
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest3_Eurostat_measure_recession()
 * \brief: Unit test for: Eurostat_measure_recession.
 * Case: RECESSION_STARTED=1 and after updating RECESSION_DURATION from 0 to 1, the recession is ended: RECESSION_STARTED=0.
 * Status: Not tested
 */
void unittest3_Eurostat_measure_recession()
{
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    RECESSION_STARTED=1;
    RECESSION_DURATION=0;
    HISTORY_QUARTERLY[0].gdp=110;
    HISTORY_QUARTERLY[1].gdp=100;
    
    /***** Messages: pre-conditions **********************************/
    
    /***** Function evaluation ***************************************/
    Eurostat_measure_recession();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RECESSION_STARTED, 0);
    CU_ASSERT_EQUAL(RECESSION_DURATION, 1);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest4_Eurostat_measure_recession()
 * \brief: Unit test for: Eurostat_measure_recession.
 * Case: Recession does not occur: before RECESSION_STARTED=0, and after RECESSION_STARTED=0.
 * Status: Not tested
 */
void unittest4_Eurostat_measure_recession()
{
    /************* At start of unit test, add one agent **************/
    unittest_init_Eurostat_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    RECESSION_STARTED=0;
    HISTORY_QUARTERLY[0].gdp=90;
    HISTORY_QUARTERLY[1].gdp=100;
    HISTORY_QUARTERLY[2].gdp=90;
    
    /***** Messages: pre-conditions **********************************/
    
    /***** Function evaluation ***************************************/
    Eurostat_measure_recession();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(RECESSION_STARTED, 0);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Eurostat_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

