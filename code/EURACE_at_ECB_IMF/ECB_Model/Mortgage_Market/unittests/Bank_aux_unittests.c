#include <CUnit/Basic.h>
#include "../../../header.h"
#include "../../../Bank_agent_header.h"
#include "../../../my_library_header.h"

/************Bank Role: Mortgage Market, Aux functions********************************/

/************ Unit tests ********************************/
/*
 * \fn: void unittest_Bank_set_pd_lgd_daily_firm_loans()
 * \brief: Unit test for: Bank_set_pd_lgd_daily_firm_loans.
 * Status: Tested OK
 */
void unittest_Bank_set_pd_lgd_daily_firm_loans()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_credit = 1;
    
	add_int(&FIRM_PL_DAILY, 1);
	add_int(&FIRM_PL_DAILY, 2);

	add_int(&FIRM_NPL_DAILY, 1);
	add_int(&FIRM_NPL_DAILY, 2);

	add_double(&FIRM_LGD_DAILY, 1.0);	
	add_double(&FIRM_LGD_DAILY, 2.0);	

	//FIRM_BANKRUPTCY_MESSAGE
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_firm_bankruptcy, sizeof(m_firm_bankruptcy));
	
	/***** Messages: pre-conditions **********************************/
    //add_bankruptcy_message(firm_id, bank_id, bad_debt, credit_refunded, writeoff_exposure)
   	add_firm_bankruptcy_message(1,2,100.0,0.0,10.0);
	add_firm_bankruptcy_message(2,2,100.0,0.0,10.0);
	add_firm_bankruptcy_message(3,2,100.0,0.0,10.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_bankruptcy, &i_firm_bankruptcy);

	//FIRM_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_firm_installment, sizeof(m_firm_installment));
	
	/***** Messages: pre-conditions **********************************/
    //add_installment_message(bank_id, installment_amount, interest_amount, exposure_per_installment, total_risk_exposure)
   	add_firm_installment_message(1, 100.0, 1.0, 1.0, 1.0);
	add_firm_installment_message(2, 200.0, 2.0, 2.0, 2.0);
	add_firm_installment_message(3, 200.0, 2.0, 2.0, 2.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_firm_installment, &i_firm_installment);

    /***** Function evaluation ***************************************/
	Bank_set_pd_lgd_daily_firm_loans();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
	int i=FIRM_PL_DAILY.size-1;

	CU_ASSERT_EQUAL(FIRM_PL_DAILY.array[i], 3);
	CU_ASSERT_EQUAL(FIRM_NPL_DAILY.array[i], 3);

	CU_ASSERT_DOUBLE_EQUAL(FIRM_LGD_DAILY.array[i], 300.0, 1e-3);
	

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


void unittest_Bank_set_pd_lgd_quarterly_firm_loans()
{
		int rc;
//	    /************* At start of unit test, add one agent **************/
		unittest_init_Bank_agent();

//		/***** Variables: Memory pre-conditions **************************/
		FLAME_environment_variable_print_debug_credit = 1;

		//ACTIVE FIRMS set: [1,2]
		add_int(&FIRM_PL_DAILY, 1);
		add_int(&FIRM_PL_DAILY, 2);

		//NPL set: [1,2]
		add_int(&FIRM_NPL_DAILY, 1);
		add_int(&FIRM_NPL_DAILY, 2);

		//Losses set: [1.0,2.0]
		add_double(&FIRM_LGD_DAILY, 1.0);	
		add_double(&FIRM_LGD_DAILY, 2.0);	

		//set: [0.0,0.0]
		FIRM_PD_YEAR = 0.0;
		FIRM_LGD_YEAR = 0.0;

		//set: [0.0,2.0] -> [3/(3+3)=0.5]
		add_double(&FIRM_PD_CYCLE, 0.0);
		add_double(&FIRM_PD_CYCLE, 2.0);

		//set: [0.0,2.0] -> [3.0/3= 1.0]
		add_double(&FIRM_LGD_CYCLE, 0.0);	
		add_double(&FIRM_LGD_CYCLE, 2.0);	

//      /***** Messages: pre-conditions **********************************/
				
//	    /***** Function evaluation ***************************************/
		Bank_set_pd_lgd_quarterly_firm_loans();
//	    
//	    /***** Variables: Memory post-conditions *************************/
//		CU_ASSERT_DOUBLE_EQUAL(<var_name1>, <value>, 1e-3);
//		CU_ASSERT_EQUAL(<var_name2>, <value>);
//		
		//FIRM_PD_YEAR = NPL/(PL+NPL)
		printf("\n\t%2.3f\n", FIRM_PD_YEAR);
		CU_ASSERT_DOUBLE_EQUAL(FIRM_PD_YEAR, 0.5, 1e-3);
		
		//FIRM_LGD_YEAR = loss(NPL)/NPL		
		printf("\n\t%2.3f\n", FIRM_LGD_YEAR);
		CU_ASSERT_DOUBLE_EQUAL(FIRM_LGD_YEAR, 1.0, 1e-3);
		
//     /***** Messages: post-conditions **********************************/
//	
//	    /************* At end of unit test, free the agent **************/
	    //free_agent();
		unittest_free_Bank_agent();
//     /************* At end of unit tests, free all Messages **********/
	    free_messages();
}

void unittest_Bank_set_pd_lgd_yearly_firm_loans()
{
		int rc;

//	    /************* At start of unit test, add one agent **************/
		unittest_init_Bank_agent();

//		/***** Variables: Memory pre-conditions **************************/
		FLAME_environment_variable_print_debug_credit = 1;
		DAY = 240;

		add_double(&FIRM_PD_CYCLE, 20.0);
		add_double(&FIRM_PD_CYCLE, 40.0);

		add_double(&FIRM_LGD_CYCLE, 60.0);	
		add_double(&FIRM_LGD_CYCLE, 80.0);	

		FIRM_PD_REG = 0.0;
		FIRM_LGD_REG = 0.0;

//      /***** Messages: pre-conditions **********************************/
				
//	    /***** Function evaluation ***************************************/
		Bank_set_pd_lgd_yearly_firm_loans();
//	    
//	    /***** Variables: Memory post-conditions *************************/
//		CU_ASSERT_DOUBLE_EQUAL(<var_name1>, <value>, 1e-3);
//		CU_ASSERT_EQUAL(<var_name2>, <value>);
		CU_ASSERT_DOUBLE_EQUAL(FIRM_PD_REG, 3.0, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(FIRM_LGD_REG, 7.0, 1e-3);
		
//     /***** Messages: post-conditions **********************************/
//	
//	    /************* At end of unit test, free the agent **************/
	    //free_agent();
		unittest_free_Bank_agent();
//     /************* At end of unit tests, free all Messages **********/
	    free_messages();
}


/*
 * \fn: void unittest_Bank_set_pd_lgd_monthly_firm_loans()
 * \brief: Unit test for: Bank_set_pd_lgd_monthly_firm_loans.
 * Status: Tested OK
 */
void unittest_Bank_set_pd_lgd_monthly_household_loans()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Bank_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FLAME_environment_variable_print_debug_credit = 1;
    
	add_int(&HH_PL_MONTHLY, 1);
	add_int(&HH_PL_MONTHLY, 2);

	add_int(&HH_NPL_MONTHLY, 1);
	add_int(&HH_NPL_MONTHLY, 2);

	add_double(&HH_LGD_MONTHLY, 1.0);	
	add_double(&HH_LGD_MONTHLY, 2.0);	

	//HH_BANKRUPTCY_MESSAGE
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_default, sizeof(m_mortgage_default));
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_default_message(int household_id, int bank_id, double bad_debt)
   	add_mortgage_default_message(1,2,10.0);
	add_mortgage_default_message(2,2,10.0);
	add_mortgage_default_message(3,2,10.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_default, &i_mortgage_default);

	//HH_ID_MESSAGE
	/***** Messages: initialize message boards **********************************/
	MB_Create(&b_mortgage_payment, sizeof(m_mortgage_payment));
	
	/***** Messages: pre-conditions **********************************/
        //add_mortgage_payment_message(int household_id, int bank_id, double principal, double interest)
   	add_mortgage_payment_message(1, 0, 1.0, 1.0);
	add_mortgage_payment_message(2, 0, 1.0, 1.0);
	add_mortgage_payment_message(3, 0, 1.0, 1.0);

    /***** Message: Adding message iterators ***************************************/
	MB_Iterator_Create(b_mortgage_payment, &i_mortgage_payment);

    /***** Function evaluation ***************************************/
	Bank_set_pd_lgd_monthly_household_loans();
    
    /***** Variables: Memory post-conditions *****/
    //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
	int i=HH_PL_MONTHLY.size-1;

	CU_ASSERT_EQUAL(HH_PL_MONTHLY.array[i], 3);
	CU_ASSERT_EQUAL(HH_NPL_MONTHLY.array[i], 3);

	CU_ASSERT_DOUBLE_EQUAL(HH_LGD_MONTHLY.array[i], 30.0, 1e-3);
	

    /***** Messages: Message post-conditions *****/
	//start a reading loop
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Bank_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


void unittest_Bank_set_pd_lgd_quarterly_household_loans()
{
		int rc;
//	    /************* At start of unit test, add one agent **************/
		unittest_init_Bank_agent();

//		/***** Variables: Memory pre-conditions **************************/
		FLAME_environment_variable_print_debug_credit = 1;

		//ACTIVE FIRMS set: [1,2]
		add_int(&HH_PL_MONTHLY, 1);
		add_int(&HH_PL_MONTHLY, 2);

		//NPL set: [1,2]
		add_int(&HH_NPL_MONTHLY, 1);
		add_int(&HH_NPL_MONTHLY, 2);

		//Losses set: [1.0,2.0]
		add_double(&HH_LGD_MONTHLY, 1.0);	
		add_double(&HH_LGD_MONTHLY, 2.0);	

		//set: [0.0,0.0]
		HH_PD_YEAR = 0.0;
		HH_LGD_YEAR = 0.0;

		//set: [0.0,2.0] -> [3/(3+3)=0.5]
		add_double(&HH_PD_CYCLE, 0.0);
		add_double(&HH_PD_CYCLE, 2.0);

		//set: [0.0,2.0] -> [3.0/3= 1.0]
		add_double(&HH_LGD_CYCLE, 0.0);	
		add_double(&HH_LGD_CYCLE, 2.0);	

//      /***** Messages: pre-conditions **********************************/
				
//	    /***** Function evaluation ***************************************/
		Bank_set_pd_lgd_quarterly_household_loans();
//	    
//	    /***** Variables: Memory post-conditions *************************/
//		CU_ASSERT_DOUBLE_EQUAL(<var_name1>, <value>, 1e-3);
//		CU_ASSERT_EQUAL(<var_name2>, <value>);
//		
		//HH_PD_YEAR = NPL/(PL+NPL)
		printf("\n\t%2.3f\n", HH_PD_YEAR);
		CU_ASSERT_DOUBLE_EQUAL(HH_PD_YEAR, 0.5, 1e-3);
		
		//HH_LGD_YEAR = loss(NPL)/NPL		
		printf("\n\t%2.3f\n", HH_LGD_YEAR);
		CU_ASSERT_DOUBLE_EQUAL(HH_LGD_YEAR, 1.0, 1e-3);
		
//     /***** Messages: post-conditions **********************************/
//	
//	    /************* At end of unit test, free the agent **************/
	    //free_agent();
		unittest_free_Bank_agent();
//     /************* At end of unit tests, free all Messages **********/
	    free_messages();
}

void unittest_Bank_set_pd_lgd_yearly_household_loans()
{
		int rc;

//	    /************* At start of unit test, add one agent **************/
		unittest_init_Bank_agent();

//		/***** Variables: Memory pre-conditions **************************/
		FLAME_environment_variable_print_debug_credit = 1;
		DAY = 240;

		add_double(&HH_PD_CYCLE, 20.0);
		add_double(&HH_PD_CYCLE, 40.0);

		add_double(&HH_LGD_CYCLE, 60.0);	
		add_double(&HH_LGD_CYCLE, 80.0);	

		HH_PD_REG = 0.0;
		HH_LGD_REG = 0.0;

//      /***** Messages: pre-conditions **********************************/
				
//	    /***** Function evaluation ***************************************/
		Bank_set_pd_lgd_yearly_household_loans();
//	    
//	    /***** Variables: Memory post-conditions *************************/
//		CU_ASSERT_DOUBLE_EQUAL(<var_name1>, <value>, 1e-3);
//		CU_ASSERT_EQUAL(<var_name2>, <value>);
		CU_ASSERT_DOUBLE_EQUAL(HH_PD_REG, 3.0, 1e-3);
		CU_ASSERT_DOUBLE_EQUAL(HH_LGD_REG, 7.0, 1e-3);
		
//     /***** Messages: post-conditions **********************************/
//	
//	    /************* At end of unit test, free the agent **************/
	    //free_agent();
		unittest_free_Bank_agent();
//     /************* At end of unit tests, free all Messages **********/
	    free_messages();
}
