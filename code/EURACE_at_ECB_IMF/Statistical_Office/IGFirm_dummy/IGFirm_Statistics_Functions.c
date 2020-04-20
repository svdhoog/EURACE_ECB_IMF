#include "../../header.h"
#include "../../IGFirm_agent_header.h"
#include "../../my_library_header.h"


int IGFirm_reset_variables()
{

		RECEIVED_INTEREST_IN_CALENDAR_MONTH=0;
		
		IGFIRM_STOCKS_CALENDAR.payment_account_day_1 = PAYMENT_ACCOUNT;
		
		
		
		IGFIRM_OUTFLOWS_CALENDAR.energy_costs = 0.0;
		IGFIRM_OUTFLOWS_CALENDAR.tax_payment = 0.0;
		IGFIRM_OUTFLOWS_CALENDAR.total_dividend_payment = 0.0;
		IGFIRM_OUTFLOWS_CALENDAR.value_of_repurchased_shares = 0.0;
		IGFIRM_OUTFLOWS_CALENDAR.total_expenses = 0.0;
		
		IGFIRM_INFLOWS_CALENDAR.cum_revenue = 0.0;
		IGFIRM_INFLOWS_CALENDAR.value_of_issued_shares = 0.0;
		IGFIRM_INFLOWS_CALENDAR.subsidy = 0.0;
		IGFIRM_INFLOWS_CALENDAR.total_income = 0.0;
		IGFIRM_INFLOWS_CALENDAR.deposit_interest = 0.0;
		IGFIRM_INFLOWS_CALENDAR.net_inflow = 0.0;
		
		
		int i;

	double total_assets = 0.0;
	double equity = 0.0;
	
	if (DAY==1)
	{
		//Set initial capital goods price in relation to the wage
		CAPITAL_GOOD_PRICE  = INITIAL_CAPITAL_PRICE_WAGE_RATIO * WAGE_OFFER;

		//Enter vintage array
		for (i=0; i<VINTAGES.size; i++)	
			VINTAGES.array[i].price	= INITIAL_CAPITAL_PRICE_WAGE_RATIO * WAGE_OFFER;
		
		//UNIT COSTS
		UNIT_COSTS = INITIAL_CAPITAL_PRICE_WAGE_RATIO * WAGE_OFFER;
	}
	
   
  
    total_assets = PAYMENT_ACCOUNT ;

    equity = total_assets ;
    
	
    
    IGFIRM_STOCKS_CALENDAR.equity = equity;
    IGFIRM_STOCKS_CALENDAR.total_liabilities = equity;
   	IGFIRM_STOCKS_CALENDAR.total_assets = total_assets;
    IGFIRM_STOCKS_CALENDAR.payment_account = PAYMENT_ACCOUNT;
  

	return 0;
}


/** \fn IGFirm_read_policy_announcements()
 * \brief IGFirms reads the policy_announcement_messages from Governments
 */
int IGFirm_read_policy_announcements()
{
    //Message send by Government:
    START_POLICY_ANNOUNCEMENT_MESSAGE_LOOP          
        //Filter: 
        if(policy_announcement_message->gov_id == GOV_ID)
        {
            TAX_RATE_CORPORATE = policy_announcement_message->tax_rate_corporate;
            TAX_RATE_VAT = policy_announcement_message->tax_rate_vat;
            TRANSFER_PAYMENT = policy_announcement_message->firm_transfer_payment;
            SUBSIDY_PCT = policy_announcement_message->firm_subsidy_pct;
        }
    FINISH_POLICY_ANNOUNCEMENT_MESSAGE_LOOP
    
    return 0;
}


/** \fn Firm_receive_data()
 * \brief Firms receive the data messages from the Eurostat*/
int IGFirm_receive_data()
{
				
	/* If day of month to act... */



		START_MSG_SKILLS_EUROSTAT_TO_IGFIRM_MESSAGE_LOOP

		ECONOMY_WIDE_GENERAL_SKILLS_IN_FIRMS=msg_skills_eurostat_to_igfirm_message->mean_general_skills;
		ECONOMY_WIDE_SPECIFIC_SKILLS_IN_FIRMS=msg_skills_eurostat_to_igfirm_message->mean_specific_skills;
		PRODUCTIVITY_PROGRESS_IN_ECONOMY=msg_skills_eurostat_to_igfirm_message->productivity_progress;
		ECONOMY_WIDE_CAPITAL_STOCK = msg_skills_eurostat_to_igfirm_message->total_capital_stock_units;

		FINISH_MSG_SKILLS_EUROSTAT_TO_IGFIRM_MESSAGE_LOOP

		UNIT_COSTS = UNIT_COSTS*(1+PRODUCTIVITY_PROGRESS_IN_ECONOMY);
		
		
		
		return 0;
}


/** \fn IGFirm_send_subsidy_notification()
 * \brief Function to send a subsidy_notification_message to the government. 
 */
int IGFirm_send_subsidy_notification()
{
      
    return 0;       
}

/** \fn IGFirm_send_transfer_notification()
 * \brief Function to send a transfer_notification_message to the government. 
 */
int IGFirm_send_transfer_notification()
{
    
        
    return 0;       
}



/** \fn Firm_send_data_to_Eurostat()
 * \brief Firms send data to Market Research: controlling results and creating macro data
 */
int IGFirm_send_data_to_Eurostat()
{
	//Increase the age of the firm in months
	AGE++;

	//Set data to be send to Eurostat using firm_info_adt
	firm_info_adt firm_info;

	//Init to set all values to 0
	init_firm_info_adt(&firm_info);

	firm_info.region_id                                           =    REGION_ID                                       ;
	/*
	firm_info.vacancies                                           =    VACANCIES                                       ;
	firm_info.posted_vacancies                                    =    POSTED_VACANCIES                                ;
	*/
	
	firm_info.no_employees                                        =    0			                                   ;
	
	/*
	firm_info.no_employees_skill_1                                =    NO_EMPLOYEES_SKILL_1                            ;
	firm_info.no_employees_skill_2                                =    NO_EMPLOYEES_SKILL_2                            ;
	firm_info.no_employees_skill_3                                =    NO_EMPLOYEES_SKILL_3                            ;
	firm_info.no_employees_skill_4                                =    NO_EMPLOYEES_SKILL_4                            ;
	firm_info.no_employees_skill_5                                =    NO_EMPLOYEES_SKILL_5                            ;

	firm_info.no_employees_production                             =    NO_EMPLOYEES_PRODUCTION                         ;
	firm_info.no_employees_innovation                             =    NO_EMPLOYEES_INNOVATION                         ;
	firm_info.total_mean_wage                                     =    TOTAL_MEAN_WAGE                                 ;
	firm_info.mean_wage                                           =    MEAN_WAGE                                       ;
	firm_info.mean_wage_r_and_d                                   =    MEAN_WAGE_R_AND_D                               ;
	*/

	firm_info.mean_specific_skills                                =    1					                           ;
	
	/*
	firm_info.average_s_skill_of_1                                =    AVERAGE_S_SKILL_OF_1                            ;
	firm_info.average_s_skill_of_2                                =    AVERAGE_S_SKILL_OF_2                            ;
	firm_info.average_s_skill_of_3                                =    AVERAGE_S_SKILL_OF_3                            ;
	firm_info.average_s_skill_of_4                                =    AVERAGE_S_SKILL_OF_4                            ;
	firm_info.average_s_skill_of_5                                =    AVERAGE_S_SKILL_OF_5                            ;

	firm_info.cum_revenue                   		              =    FIRM_INFLOWS_CALENDAR.cum_revenue               ;
	firm_info.capital_costs                                       =    CAPITAL_COSTS                                   ;
	firm_info.net_earnings                                        =    NET_EARNINGS                                    ;
	firm_info.total_debt                                          =    TOTAL_DEBT                                      ;
	firm_info.total_assets                                        =    TOTAL_ASSETS                                    ;
	firm_info.equity                                              =    EQUITY                                          ;
	firm_info.price                                               =    PRICE                                           ;
	firm_info.price_last_month                                    =    PRICE_LAST_MONTH                                ;
	firm_info.quality                                             =    QUALITY                                         ;
	firm_info.total_supply                                        =    TOTAL_SUPPLY                                    ;
	firm_info.sold_quantity_in_calendar_month                     =    SOLD_QUANTITY_IN_CALENDAR_MONTH                 ;
	firm_info.output                                              =    OUTPUT                                          ;
	firm_info.planned_output                                      =    PLANNED_OUTPUT                                  ;
	*/

	firm_info.age                                                 =    AGE                                             ;

	/*
	firm_info.firm_productivity                                   =    FIRM_PRODUCTIVITY                               ;
	firm_info.firm_productivity_progress                          =    FIRM_PRODUCTIVITY_PROGRESS                      ;
	firm_info.total_value_local_inventory    					  =    FIRM_STOCKS_CALENDAR.total_value_local_inventory;
	firm_info.total_units_capital_stock                           =    TOTAL_UNITS_CAPITAL_STOCK                       ;
	firm_info.wage_offer                                          =    WAGE_OFFER                                      ;
	*/

	firm_info.total_dividend_payment						      =    IGFIRM_OUTFLOWS_CALENDAR.total_dividend_payment ;

	/*
	firm_info.potential_output                                    =    POTENTIAL_OUTPUT                                ;
	firm_info.capacity_utilization_rate                           =    UTILIZATION_CAPACITY                            ;
	*/

	// Initialize variables!
	firm_info.no_employees = 0;
	firm_info.mean_specific_skills = 1;

	add_firm_send_data_message(ID, firm_info);
		
	//printf("In Firm_send_data: Firm %d OUTPUT: %.2f\n", ID, OUTPUT);
	//printf("In Firm_send_data: Firm %d PLANNED_OUTPUT: %.2f\n", ID, PLANNED_OUTPUT);
        
    return 0;       
}

/** \fn IGFirm_compute_stock_flows()
 * \brief IGFirms compute stock and flows for blanace sheets.
 */
int IGFirm_compute_stock_flows()
{
	
	IGFIRM_OUTFLOWS_CALENDAR.total_expenses =
   
    IGFIRM_OUTFLOWS_CALENDAR.energy_costs+
    IGFIRM_OUTFLOWS_CALENDAR.tax_payment+
    IGFIRM_OUTFLOWS_CALENDAR.total_dividend_payment+
    IGFIRM_OUTFLOWS_CALENDAR.value_of_repurchased_shares;
    
    IGFIRM_INFLOWS_CALENDAR.total_income =
    IGFIRM_INFLOWS_CALENDAR.cum_revenue+
    IGFIRM_INFLOWS_CALENDAR.value_of_issued_shares+
    IGFIRM_INFLOWS_CALENDAR.subsidy + 
    IGFIRM_INFLOWS_CALENDAR.deposit_interest;
    
    IGFIRM_STOCKS_CALENDAR.payment_account_day_20 = PAYMENT_ACCOUNT;
    
    IGFIRM_INFLOWS_CALENDAR.net_inflow = 
	IGFIRM_INFLOWS_CALENDAR.total_income -
	IGFIRM_OUTFLOWS_CALENDAR.total_expenses;

	double total_value_local_inventory = 0.0;
	double total_assets = 0.0;
	double equity = 0.0;
	double total_debt = 0.0;
	
 /* In Dummy: IGFirm as no assets and no debt */
 /*
    for (i=LOANS.size-1; i>-1; i--)
    {
    	total_debt += LOANS.array[i].loan_value;
    }

    total_assets = PAYMENT_ACCOUNT + TOTAL_VALUE_CAPITAL_STOCK;
*/

    total_assets = PAYMENT_ACCOUNT;

    equity = total_assets - total_debt;
    
    IGFIRM_STOCKS_CALENDAR.equity = equity;
    IGFIRM_STOCKS_CALENDAR.total_debt = total_debt;
    IGFIRM_STOCKS_CALENDAR.total_liabilities = total_debt+equity;
    IGFIRM_STOCKS_CALENDAR.total_assets = total_assets;
    IGFIRM_STOCKS_CALENDAR.payment_account = PAYMENT_ACCOUNT;
    IGFIRM_STOCKS_CALENDAR.total_value_local_inventory = total_value_local_inventory;

	return 0;
}


int IGFirm_send_payments_to_bank()
{
	add_bank_account_update_message(ID, BANK_ID, PAYMENT_ACCOUNT);

	log_stock("ig_firm", "payment_account", ID, PAYMENT_ACCOUNT);
	log_stock("ig_firm", "net_worth", ID, PAYMENT_ACCOUNT);
	
	return 0;
}
