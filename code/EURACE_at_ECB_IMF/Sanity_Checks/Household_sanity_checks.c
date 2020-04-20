#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"
#include "sanity_checks_aux_headers.h"

int print_agent_info(int agent_id)
{
	if (ID == agent_id)
	{
		printf("===== Household %d =====\n", ID);
		printf("IS_HOMELESS %d \n", IS_HOMELESS);
		printf("IS_TENANT %d \n", IS_TENANT);
		if (IS_TENANT)
		{
			printf("  MONTHLY_RENT %f \n", MONTHLY_RENT);
			printf("  RENT_PAID %f \n", RENT_PAID);
			printf("  landlord_id %d \n", RENTAL_UNIT.owner_id);
			printf("  monthly_rent %f \n", RENTAL_UNIT.monthly_rent);
			printf("  price_index_base %f \n", RENTAL_UNIT.cpi_base);
			printf("  rent_price_index_base %f \n", RENTAL_UNIT.rpi_base);
		}
		printf("IS_OWNER %d \n", IS_OWNER);
		if (IS_OWNER)
		{
			printf("  object_id %d \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);
			printf("  value %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
			printf("  hpi_base %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base);
		}
		printf("IS_LANDLORD %d \n", IS_LANDLORD);
		printf("PAYMENT_ACCOUNT %f \n", PAYMENT_ACCOUNT);
		printf("  MEAN_NET_INCOME %f \n", MEAN_NET_INCOME);
		printf("  CONSUMPTION_BUDGET_IN_MONTH %f \n", CONSUMPTION_BUDGET_IN_MONTH);
		printf("    CONSUMPTION_BUDGET %f \n", CONSUMPTION_BUDGET);
		printf("    WEEKLY_BUDGET %f \n", WEEKLY_BUDGET);
		printf("    EXPENDITURES %f \n", EXPENDITURES);
		printf("  HOUSING_BUDGET %f \n", HOUSING_BUDGET);
		printf("HAVE_MORTGAGE %d \n", HAVE_MORTGAGE);
		printf("  HAVE_HMR_MORTGAGE %d \n", HAVE_HMR_MORTGAGE);
		if (HAVE_HMR_MORTGAGE)
		{
			printf("    current_duration %d \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration);
			printf("    outstanding_principal %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
			printf("    monthly_principal %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal);
			printf("    monthly_interest %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest);
			printf("    monthly_total_repayment %f \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
			printf("    status %d \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
			printf("      arrears_counter %d \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.arrears_counter);
			printf("      npl_counter %d \n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.npl_counter);
		}
		printf("  HAVE_OP_MORTGAGE %d \n", HAVE_OP_MORTGAGE);

		printf("PLAN_SELL_HMR %d\n", PLAN_SELL_HMR);
		printf("PLAN_SELL_OP %d\n", PLAN_SELL_OP);

		printf("=========\n");



	}

	return 0;
}

int print_agent_info_day(int agent_id, int day)
{
	if (DAY == day)
		print_agent_info(agent_id);

	return 0;
}

int print_agent_info_monthly(int agent_id, int day_in_month)
{
	if ((DAY - (DAY % 20)*20) % 20 == day_in_month)
		print_agent_info(agent_id);

	return 0;
}

int print_agent_object_info()
{
	int i;

	//printf("===================== HOUSING OBJECTS ====================\n");

	//Columns: 
	//Tenant: 	object_id	landlord_id
	//Owner: 	object_id	value 		mortgage object_id 
	//Landlord: object_id	value 		mortgage object_id 

	if (IS_TENANT)
	{
		printf("\n\t Tenant (id, object_id, tenant_id, landlord_id, monthly_rent)");
		printf("\t %d", ID);
		printf("\t %d", RENTAL_UNIT.object_id);
		printf("\t %d", RENTAL_UNIT.tenant_id);
		printf("\t %d", RENTAL_UNIT.owner_id);
		printf("\t %f \n", RENTAL_UNIT.monthly_rent);
	}

	if (IS_OWNER)
	{
		printf("\n\t Owner (id, object_id, value, [mrtg object_id], [mrtg outstanding_principal])");
		printf("\t %d", ID);
		printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);
		printf("\t%f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price);
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price<EPSILON) 
			printf("\t[*]");
		
		if (HAVE_HMR_MORTGAGE)
		{
			printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.object_id);
			printf("\t%f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
			printf("\n");
		}
		else
			printf("\t-- \n");
	}

	if (IS_LANDLORD)
	{
		for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
		{
			printf("\n\t Landlord (id, object_id, tenant_id, landlord_id, value, [mrtg object_id], [mrtg outstanding_principal])");
			printf("\t %d", ID);
			printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id);
			printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id);
			printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].owner_id);
			printf("\t%f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price);				
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price<EPSILON) 
				printf("\t[*]");
			
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active>0)
			{
					printf("\t%d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.object_id);
					printf("\t%f", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal);
			}
			printf("\t[%d/%d objects]", i+1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size);
			printf("\n");
		}
	}
	//printf("=========\n");

	return 0;
}

void print_object_info(int id)
{
	if (DAY % 20 - 2 ==0)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id==id)
		{
			printf("\n--- OBJECT %d ---\n", id);
			printf("type: HMR\n");
			printf("owner: %d\n", ID);
			printf("value_estimated: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value_estimated);
			printf("hpi_base: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.hpi_base);
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active)
			{
				printf("mortgage: ACTIVE\n");
				printf("\tstatus: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status);
				printf("\tduration: %d / %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.current_duration, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.initial_duration);
				printf("\tannuity: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_total_repayment);
				printf("\tinterest_rate: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.interest_rate_annual);
				printf("\toutstanding_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_principal);
				printf("\toutstanding_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.outstanding_interest);
				printf("\tmonthly_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_principal);
				printf("\tmonthly_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.monthly_interest);


			}
			printf("----------------\n");
		}

		int i;
		for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id==id)
			{
				printf("\n--- OBJECT %d ---\n", id);
				printf("type: OP\n");
				printf("owner: %d\n", ID);
				printf("tenant: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id);
				printf("value_estimated: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].value_estimated);
				printf("hpi_base: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].hpi_base);

			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active)
			{
				printf("\tstatus: %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status);
				printf("\tduration: %d / %d\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.current_duration, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.initial_duration);
				printf("\tannuity: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_total_repayment);
				printf("\tinterest_rate: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.interest_rate_annual);
				printf("\toutstanding_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_principal);
				printf("\toutstanding_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.outstanding_interest);
				printf("\tmonthly_principal: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_principal);
				printf("\tmonthly_interest: %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_interest);

			}
			printf("----------------\n");

		}
	}

}

int Household_sanity_checks()
{
	//print_agent_info_monthly(1515, 3);
	//print_agent_info_monthly(319, 3);

	//print_agent_info_monthly(1188,3);

	if(ENABLE_SANITY_CHECKS)
	{
		double eps=1e-4;

		/* General */

		// payment account negative, nan, inf
		if (PAYMENT_ACCOUNT < -eps)
			fprintf(stderr, "\n[!] IT %d ID %d payment_account < 0, payment_account: %f", DAY, ID, PAYMENT_ACCOUNT);

		assert(PAYMENT_ACCOUNT > -eps);

		if (PAYMENT_ACCOUNT != PAYMENT_ACCOUNT)
			fprintf(stderr, "\n[!] IT %d ID %d payment_account NaN, payment_account: %f", DAY, ID, PAYMENT_ACCOUNT);
		
		if (PAYMENT_ACCOUNT == INFINITY)
			fprintf(stderr, "\n[!] IT %d ID %d payment_account INFINITY, payment_account: %f", DAY, ID, PAYMENT_ACCOUNT);

		/*
		 * Housing Specific
		 */

		// No landlord
		if (!DISABLE_HOUSING_AND_RENTAL_MARKET && DAY > INIT_TRANSIENT_PERIOD)
		{
			if (IS_TENANT)
				if (RENTAL_UNIT.owner_id <= 0)
				{
					fprintf(stderr, "\n[!] IT %d ID %d is_tenant, but landlord_id <= 0, landlord_id: %d", DAY, ID, RENTAL_UNIT.owner_id);
					assert(RENTAL_UNIT.owner_id > 0);
				}
				

			if(IS_OWNER)
			{
				if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price < 1e-5)
				{
					//fprintf(stderr, "\nError 42");
					//fprintf(stderr, "[!] IS_OWNER but hmr value == 0, ID: %d (DAY %d)\n", ID, DAY);
					//assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price > 1e-5);
				}

				if(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id < 1)
				{
					fprintf(stderr, "[!] IS_OWNER but object_id 0 or negative, ID: %d (DAY %d)\n", ID, DAY);
					assert(HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id >= 1);
				}
			}
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_RENTAL && DAY > CONST_INIT_TRANSIENT_PERIOD)
		{
			print_agent_object_info();
		}      
		#endif

	}//ENABLE_SANITY_CHECKS

	return 0;
}

void enforce_hh_valid_payment_account(const char * string)
{
	double eps=1e-10;

	// payment account nan, inf
	if (PAYMENT_ACCOUNT != PAYMENT_ACCOUNT || PAYMENT_ACCOUNT == INFINITY || PAYMENT_ACCOUNT == -INFINITY)
	{
		fprintf(stderr, "\nABORTED: Invalid payment account (%s): PAYMENT_ACCOUNT = %f\n", string, PAYMENT_ACCOUNT);
	}

	assert(PAYMENT_ACCOUNT == PAYMENT_ACCOUNT);
	assert(PAYMENT_ACCOUNT != INFINITY);
	assert(PAYMENT_ACCOUNT != -INFINITY);

    // payment account negative
    if (PAYMENT_ACCOUNT < -eps)
        fprintf(stderr, "\nWARNING: Invalid payment account (%s): PAYMENT_ACCOUNT = %f (precision %f)\n", string, PAYMENT_ACCOUNT, eps);
    
    //assert(PAYMENT_ACCOUNT > -eps);
}
