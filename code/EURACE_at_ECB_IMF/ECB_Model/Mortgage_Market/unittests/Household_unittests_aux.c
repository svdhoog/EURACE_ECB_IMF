#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"

/*
 * 1) initialize agent
 * 2) add memory pre-conditions
 * 3) initialise MBs
 * 4) add message pre-conditions
 * 5) create MB iterators
 * 6) evaluate function to be tested
 * 7) assertions on memory post-conditions
 * 8) assertions on message post-conditions
 * 9) free agent
 * 10) free MBs
 */

/*
 * \fn: void unittest_Household_function()
 * \brief: Unit test for: Household_function.
 * Status: NOT Tested
 */
//void unittest_Household_()
//{
	
    /************* At start of unit test, add one agent **************/
//	unittest_init_Household_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	/***** Messages: initialize message boards **********************************/
/*
	MB_Create(&b_<message>, sizeof(m_<message>));
*/	
	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Message: Adding message iterators ***************************************/
	//MB_Iterator_Create(b_<message>, &i_<message>);

    /***** Function evaluation ***************************************/
//	<Agentname>_function();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop
/*
    START_<message>_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(<message>_message->var, value);
	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
	FINISH_<message>_MESSAGE_LOOP
*/	
    /************* At end of unit test, free the agent **************/
//	unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
//    free_messages();
//}


/*
 * \fn: void unittest_recompute_mortgage_attributes()
 * \brief: Unit test for: unittest_recompute_mortgage_attributes
 * \brief: Status: TO TEST
 */
void unittest_recompute_mortgage_attributes()
{
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    FLAME_environment_variable_print_mortgage_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    mortgage_contract_adt mortgage;
    
    init_mortgage_contract_adt(&mortgage);
/*  
    mortgage.object_id
    mortgage.household_id
    mortgage.bank_id
*/  
    mortgage.initial_duration = 12*20;
    mortgage.interest_rate_annual = 0.02;
    mortgage.current_duration = 240;
    mortgage.outstanding_principal = 296511.052269838;
    mortgage.outstanding_interest = 63488.9477301617;
    mortgage.monthly_total_repayment = 1500.0;

/*
s=1:
monthly_interest        494.185087116409
monthly_principal       1005.81491288359
monthly_total_repayment 1500
outstanding_principal   295505.237356955
*/

/*  
    mortgage.monthly_principal
    mortgage.monthly_interest
    
    mortgage.status 
    mortgage.arrears_counter
    mortgage.npl_counter
*/
        printf("\n\nInitial mortgage data");
        printf("\n interest\t %.2f", mortgage.interest_rate_annual);
        printf("\n duration\t %d", mortgage.current_duration);
        printf("\n annuity\t %.3f", mortgage.monthly_total_repayment);
        printf("\n balance\t %.3f", mortgage.outstanding_principal+mortgage.outstanding_interest);
        printf("\n outstanding_principal\t %.3f", mortgage.outstanding_principal);
        printf("\n outstanding_interest\t %.3f", mortgage.outstanding_interest);
        printf("\n");

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    int t;
    
    int T=mortgage.initial_duration+10;
    //int T=10;

    //printf("\n\n\t\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    //printf("\ntime\ts\tdur\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    printf("\nt\t\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    for (t=0; t<T; t++)
    {
        printf("\n %d", t);

        recompute_mortgage_attributes(&mortgage);

        /* Update mortgage */
        mortgage.current_duration--;
        mortgage.outstanding_principal -= mortgage.monthly_principal;
        mortgage.outstanding_interest -= mortgage.monthly_interest;
    }   

    /***** Variables: Memory post-conditions *****/
//  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

    

//  CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
//  CU_ASSERT_EQUAL(avg_initial_duration, 33);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_recompute_mortgage_attributes()
 * \brief: Unit test for: unittest_recompute_mortgage_attributes
 * \brief: Status: TO TEST
 */
void unittest_recompute_mortgage_attributes_2()
{
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    FLAME_environment_variable_print_mortgage_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    mortgage_contract_adt mortgage;
    
    init_mortgage_contract_adt(&mortgage);
/*  
    mortgage.object_id
    mortgage.household_id
    mortgage.bank_id
*/  
    mortgage.initial_duration = 12*20;
    mortgage.interest_rate_annual = 0.02;
    mortgage.current_duration = 240;
    mortgage.outstanding_principal = 0.0;
    mortgage.outstanding_interest = 0.0;
    mortgage.monthly_total_repayment = 1500.0;

/*
s=1:
monthly_interest        494.185087116409
monthly_principal       1005.81491288359
monthly_total_repayment 1500
outstanding_principal   295505.237356955
*/

/*  
    mortgage.monthly_principal
    mortgage.monthly_interest
    
    mortgage.status 
    mortgage.arrears_counter
    mortgage.npl_counter
*/
        printf("\n\nInitial mortgage data");
        printf("\n interest\t %.2f", mortgage.interest_rate_annual);
        printf("\n duration\t %d", mortgage.current_duration);
        printf("\n annuity\t %.3f", mortgage.monthly_total_repayment);
        printf("\n balance\t %.3f", mortgage.outstanding_principal+mortgage.outstanding_interest);
        printf("\n outstanding_principal\t %.3f", mortgage.outstanding_principal);
        printf("\n outstanding_interest\t %.3f", mortgage.outstanding_interest);
        printf("\n");

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    int t;
    
    int T=mortgage.initial_duration+10;
    //int T=10;

    //printf("\n\n\t\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    //printf("\ntime\ts\tdur\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    printf("\nt\t\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\tbalance\n");
    for (t=0; t<T; t++)
    {
        printf("%d", t);

        recompute_mortgage_attributes(&mortgage);

        /* Update mortgage */
        mortgage.current_duration--;
        mortgage.outstanding_principal -= mortgage.monthly_principal;
        mortgage.outstanding_interest  -= mortgage.monthly_interest;
    }   

    /***** Variables: Memory post-conditions *****/
//  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

    

//  CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
//  CU_ASSERT_EQUAL(avg_initial_duration, 33);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_recompute_mortgage_attributes_including_arrears()
 * \brief: Unit test for: unittest_recompute_mortgage_attributes_including_arrears
 * \brief: Output: Same as unittest_recompute_mortgage_attributes(); normal repayment schedule
 * \brief: Status: PASSED 2.10.2018
 */
void unittest_recompute_mortgage_attributes_including_arrears()
{
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    FLAME_environment_variable_print_mortgage_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    mortgage_contract_adt mortgage;
    
    init_mortgage_contract_adt(&mortgage);
/*  
    mortgage.object_id
    mortgage.household_id
    mortgage.bank_id
*/  
    mortgage.initial_duration = 12*20;
    mortgage.interest_rate_annual = 0.02;
    mortgage.current_duration = 240;
    mortgage.outstanding_principal = 0.0;
    mortgage.outstanding_interest = 0.0;
    mortgage.monthly_total_repayment = 1500.0;

/*
s=1:
monthly_interest        494.185087116409
monthly_principal       1005.81491288359
monthly_total_repayment 1500
outstanding_principal   295505.237356955
*/

/*  
    mortgage.monthly_principal
    mortgage.monthly_interest
    
    mortgage.status 
    mortgage.arrears_counter
    mortgage.npl_counter
*/
        printf("\n\nInitial mortgage data");
        printf("\n interest\t %.2f", mortgage.interest_rate_annual);
        printf("\n duration\t %d", mortgage.current_duration);
        printf("\n annuity\t %.3f", mortgage.monthly_total_repayment);
        printf("\n balance\t %.3f", mortgage.outstanding_principal+mortgage.outstanding_interest);
        printf("\n outstanding_principal\t %.3f", mortgage.outstanding_principal);
        printf("\n outstanding_interest\t %.3f", mortgage.outstanding_interest);
        printf("\n");

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    int t;
    
    int T=mortgage.initial_duration+10;
    //int T=10;

    //printf("\n\n\t\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    //printf("\ntime\ts\tdur\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    printf("\nt\t\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding");
    printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\n");
    for (t=0; t<T; t++)
    {
        printf("%d", t);

        recompute_mortgage_attributes_including_arrears(&mortgage);

        /* Update mortgage */
        mortgage.current_duration--;
        
        mortgage.outstanding_principal -= mortgage.monthly_principal;
        mortgage.outstanding_interest  -= mortgage.monthly_interest;
    }   

    /***** Variables: Memory post-conditions *****/
//  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

    

//  CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
//  CU_ASSERT_EQUAL(avg_initial_duration, 33);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_recompute_mortgage_attributes_including_arrears_1()
 * \brief: Unit test for: unittest_recompute_mortgage_attributes_including_arrears
 * \brief: Test 1: same as unittest_recompute_mortgage_attributes(); normal repayment schedule
 * \brief: Status: PASSED 2.10.2018
 */
void unittest_recompute_mortgage_attributes_including_arrears_1()
{
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    FLAME_environment_variable_print_mortgage_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    mortgage_contract_adt mortgage;
    
    init_mortgage_contract_adt(&mortgage);
/*  
    mortgage.object_id
    mortgage.household_id
    mortgage.bank_id
*/  
    mortgage.initial_duration = 12*20;
    mortgage.interest_rate_annual = 0.02;
    mortgage.current_duration = 240;
    mortgage.outstanding_principal = 0.0;
    mortgage.outstanding_interest = 0.0;
    mortgage.monthly_total_repayment = 1500.0;

/*
s=1:
monthly_interest        494.185087116409
monthly_principal       1005.81491288359
monthly_total_repayment 1500
outstanding_principal   295505.237356955
*/

/*  
    mortgage.monthly_principal
    mortgage.monthly_interest
    
    mortgage.status 
    mortgage.arrears_counter
    mortgage.npl_counter
*/
        printf("\n\nInitial mortgage data");
        printf("\n interest\t %.2f", mortgage.interest_rate_annual);
        printf("\n duration\t %d", mortgage.current_duration);
        printf("\n annuity\t %.3f", mortgage.monthly_total_repayment);
        printf("\n balance\t %.3f", mortgage.outstanding_principal+mortgage.outstanding_interest);
        printf("\n outstanding_principal\t %.3f", mortgage.outstanding_principal);
        printf("\n outstanding_interest\t %.3f", mortgage.outstanding_interest);
        printf("\n");

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    int t;
    
    int T=mortgage.initial_duration+10;
    //int T=10;

    //printf("\n\n\t\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    //printf("\ntime\ts\tdur\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    printf("\nt\t\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding");
    printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\n");
    for (t=0; t<15; t++)
    {
        if (t==10)
            mortgage.arrears_counter = 1;

        printf("%d", t);

        recompute_mortgage_attributes_including_arrears(&mortgage);

        /* Update mortgage */
        mortgage.current_duration--;

        if (t==9)              
            mortgage.current_duration++; //do not update current_duration in months that are in arrears

        mortgage.outstanding_principal -= mortgage.monthly_principal;
        mortgage.outstanding_interest  -= mortgage.monthly_interest;

        if (t==10)
            mortgage.arrears_counter = 0;
    }   

    /***** Variables: Memory post-conditions *****/
//  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

    

//  CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
//  CU_ASSERT_EQUAL(avg_initial_duration, 33);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_recompute_mortgage_attributes_including_arrears_2()
 * \brief: Unit test for: unittest_recompute_mortgage_attributes_including_arrears
 * \brief: Test 2: repayment schedule with arrears_counter=1
 * \brief: Status:  PASSED 2.10.2018
 */
void unittest_recompute_mortgage_attributes_including_arrears_2()
{
    
    /************* At start of unit test, add one agent **************/
    unittest_init_Household_agent();
    
    FLAME_environment_variable_print_mortgage_info = 1;

    /***** Variables: Memory pre-conditions **************************/
    mortgage_contract_adt mortgage;
    
    init_mortgage_contract_adt(&mortgage);
/*  
    mortgage.object_id
    mortgage.household_id
    mortgage.bank_id
*/  
    mortgage.initial_duration = 12*20;
    mortgage.interest_rate_annual = 0.02;
    mortgage.current_duration = 240;
    mortgage.outstanding_principal = 0.0;
    mortgage.outstanding_interest = 0.0;
    mortgage.monthly_total_repayment = 1500.0;
    

/*
s=1:
monthly_interest        494.185087116409
monthly_principal       1005.81491288359
monthly_total_repayment 1500
outstanding_principal   295505.237356955
*/

/*  
    mortgage.monthly_principal
    mortgage.monthly_interest
    
    mortgage.status 
    mortgage.arrears_counter
    mortgage.npl_counter
*/
        printf("\n\nInitial mortgage data");
        printf("\n interest\t %.2f", mortgage.interest_rate_annual);
        printf("\n duration\t %d", mortgage.current_duration);
        printf("\n annuity\t %.3f", mortgage.monthly_total_repayment);
        printf("\n balance\t %.3f", mortgage.outstanding_principal+mortgage.outstanding_interest);
        printf("\n outstanding_principal\t %.3f", mortgage.outstanding_principal);
        printf("\n outstanding_interest\t %.3f", mortgage.outstanding_interest);
        printf("\n arrears_counter\t %d", mortgage.arrears_counter);
        printf("\n");

    /***** Messages: initialize message boards **********************************/
    
    /***** Messages: pre-conditions **********************************/

    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
    int t;
    
    int T=mortgage.initial_duration+10;
    //int T=10;

    //printf("\n\n\t\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding\toutstanding");
    //printf("\ntime\ts\tdur\tinterest\tprincipal\tinterest\tprincipal\tbalance");
    printf("\nt\t\t\t\tinitial\t\tcurrent\t\tmonthly\t\tmonthly\t\toutstanding\toutstanding");
    printf("\n\tID\tobject\ts\tduration\tduration\tinterest\tprincipal\tinterest\tprincipal\n");
    for (t=0; t<15; t++)
    {
        if (t==10)
            mortgage.arrears_counter = 2;

        printf("%d", t);

        recompute_mortgage_attributes_including_arrears(&mortgage);

        /* Update mortgage */
        mortgage.current_duration--; 

        if (t==8 || t==9)              
            mortgage.current_duration++; //do not update current_duration in months that are in arrears

        mortgage.outstanding_principal -= mortgage.monthly_principal;
        mortgage.outstanding_interest  -= mortgage.monthly_interest;
        mortgage.arrears_counter = 0;

        if (t==10)
            mortgage.arrears_counter = 0; //reset arrears counter when arrears are paid
    }   

    /***** Variables: Memory post-conditions *****/
//  CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/

    

//  CU_ASSERT_DOUBLE_EQUAL(avg_duration, 33.333, 1e-3);
//  CU_ASSERT_EQUAL(avg_initial_duration, 33);
    
    /************* At end of unit test, free the agent **************/
    unittest_free_Household_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
