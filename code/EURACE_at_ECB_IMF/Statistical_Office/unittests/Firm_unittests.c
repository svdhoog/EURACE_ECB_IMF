#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "../../header.h"
#include "../../Firm_agent_header.h"
#include "../../my_library_header.h"

/************Firm Role: Statistical Office ********************************/

/************ Unit tests ********************************/

/*
 * \fn: void unittest_Firm_send_data_to_Eurostat()
 * \brief: Unit test for: Firm_send_data_to_Eurostat.
 * Status: Tested OK
 */
void unittest_Firm_send_data_to_Eurostat()
{
    int rc;
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Firm_agent();
    
    /***** Variables: Memory pre-conditions **************************/
    printf("\n\t Setting data for firm_send_data_message\n");

/*	
    //NOTE: lower case have been added later on
	ID, REGION_ID, VACANCIES, posted_vacancies, NO_EMPLOYEES,
	NO_EMPLOYEES_SKILL_1, NO_EMPLOYEES_SKILL_2, NO_EMPLOYEES_SKILL_3, NO_EMPLOYEES_SKILL_4, NO_EMPLOYEES_SKILL_5,
	employees_production, employees_r_and_d, average_wage, average_wage_production, average_wage_r_and_d,
	average_s_skill,
	AVERAGE_S_SKILL_1, AVERAGE_S_SKILL_2, AVERAGE_S_SKILL_3, AVERAGE_S_SKILL_4, AVERAGE_S_SKILL_5,
	CUM_REVENUE, CAPITAL_COSTS, NET_EARNINGS, TOTAL_DEBT, TOTAL_ASSETS, EQUITY,
	PRICE, PRICE_LAST_MONTH, 
	quality,
	TOTAL_SUPPLY, CUM_TOTAL_SOLD_QUANTITY, OUTPUT, PLANNED_OUTPUT, AGE, FIRM_PRODUCTIVITY, FIRM_PRODUCTIVITY_PROGRESS,
	value_inventory, units_capital_stock, base_wage_offer, dividends
*/

	ID = 1;
	QUALITY=2.0;

    
    /***** Messages: initialize message boards **********************************/
    rc = MB_Create(&b_firm_send_data, sizeof(m_firm_send_data));
//            #ifdef ERRCHECK
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
//            #endif
    
    /***** Messages: pre-conditions **********************************/

    /***** Function evaluation ***************************************/
    Firm_send_data_to_Eurostat();

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
                    
    
    /***** Variables: Memory post-conditions *****/
    
    /***** Messages: Message post-conditions *****/
	int count=0;
	START_FIRM_SEND_DATA_MESSAGE_LOOP
		count++;
		printf("\n\t Read message %d\n", count);
		CU_ASSERT_EQUAL(firm_send_data_message->firm_id, 1);
		CU_ASSERT_DOUBLE_EQUAL(firm_send_data_message->quality, 2.0, 1e-3);
	FINISH_FIRM_SEND_DATA_MESSAGE_LOOP

    /************* At end of unit test, free the agent **************/
    unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
