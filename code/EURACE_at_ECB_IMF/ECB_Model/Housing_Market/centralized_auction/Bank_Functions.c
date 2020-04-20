#include "../../../header.h"
#include "../../../Bank_agent_header.h"
#include "../../../my_library_header.h"
#include "../../../Statistical_Office/balance_sheet_operations.h"
#include "../../../Sanity_Checks/sanity_checks_aux_headers.h"

/***************** TEMPLATE ****************/
/** \fn Bank_TEMPLATE()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
/*
int Bank_TEMPLATE()
{

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING)
    {
        printf("\n\n Bank_ ID: %d", ID);
    }      
    #endif
 
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

/** \fn Bank_sell_real_estate()
 * \brief Function to sell seized collateral due to defaulted mortgage loans.
 * \brief Bank creates new agents of tpe: House
 * \brief Message input: 
 * \brief Message output: 
 * Condition: if (SEIZED_COLLATERAL.size>0)  PLAN_SELL_REAL_ESTATE=1
 * \brief UNITTEST: PASS 5.1.17, Updated: 7.6.17 PASS
 */
int Bank_sell_real_estate()
{
	int i;
	double housing_market_price_index __attribute__((unused));
	housing_market_price_index = 0.0;
	ask_adt ask;

	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING_UNITTEST)
	{
		printf("\n\nITS %d ID %d Bank_sell_real_estate:", DAY, ID);
	}      
	#endif

	//Read housing_market_price_index for re-evaluating sale value of collateral
 	//NOTE: real_estate_market_data_message only send by ClearingHouse on day 2 of month
 	//But this function is active every day, conditional on having something to sell
	START_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP
		housing_market_price_index = real_estate_market_data_message->housing_market_price_index;
	FINISH_REAL_ESTATE_MARKET_DATA_MESSAGE_LOOP


	//Selling on housing market:
	for (i=0; i<SEIZED_COLLATERAL.size; i++)
	{
		ask.object_id = SEIZED_COLLATERAL.array[i].seized_house.object_id;
		ask.quality = SEIZED_COLLATERAL.array[i].seized_house.quality;
		ask.id = ID;

		if (ask.object_id==0) fprintf(stderr, "\nITS %d ID %d WARNING object_id=0 in seized collateral", DAY, ID);

		//Price gets re-evaluated with price growth rate since day of acquisition
		// Catch housing_market_price_index_base==-1 if there was no housing market transactions (ClearingHouse sets housing_market_price_index=-1 in this case)
		ask.price = SEIZED_COLLATERAL.array[i].seized_house.value_estimated *(1-CONST_HOUSE_ASK_PRICE_INITIAL_MARKDOWN_BANK);

		
		// Price of object for sale depreciates with duration on the market
		int months_on_market = ((int)(DAY - SEIZED_COLLATERAL.array[i].seizure_day)) / MONTH;
		if (months_on_market>0)
			ask.price = ask.price  * pow(1.0 - CONST_HOUSE_ASK_PRICE_ADJUSTMENT_BANK, months_on_market);

		ask.months_on_market = months_on_market;			

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_UNITTEST)
		{
			printf("\nOP ask: SEIZED_COLLATERAL.array[%d] ask.object_id %d ask.id %d ask.price %f\n", i, ask.object_id, ask.id, ask.price);
			printf("\n\thousing_market_price_index %f\n\thousing_market_price_index_base %f",	housing_market_price_index, SEIZED_COLLATERAL.array[i].seized_house.hpi_base);
		}      
		#endif
		
		//Selling on housing market:
		add_real_estate_price_ask_message(ask.price, ask);
	}
    
	//free
	free_ask_adt(&ask);

	return 0;   
}

/** \fn Bank_receive_real_estate_transaction()
 * \brief Function to process housing transactions
 * \brief Message input: real_estate_transaction_message
 * \brief Message output: seized_liquidity_recompensation
 * \brief Mesg from Bank to former owner of housing unit, to inform about a windfall profit due to selling the seized collateral at a price above the outstanding principal and interest on the mortgage.
 * \brief UNITTEST: PASS 5.1.17
 */
int Bank_receive_real_estate_transaction()
{
	int i, object_id;
	double price;
	double sum_sales_revenue=0.0;
	double sum_claim_flow=0.0;
	double recompensation = 0.0;

	double lgd = 0.0;
	double sum_sale_value_capped = 0.0;
	double sum_principal_wo = 0.0;
	
	
	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_receive_real_estate_transaction(): Before REAL_ESTATE_TRANSACTION_MESSAGE_LOOP", DAY, ID);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif

	int count=0;
	int found=0;

	START_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP
		count++;
		if(count==1)
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING)
			{
				printf("\n\nIT %d ID: %d Bank_receive_real_estate_transaction", DAY, ID);
				printf("\n\nID\tobject_id");
			}      
			#endif
		}

		price = real_estate_transaction_message->house.purchase_price;
		object_id = real_estate_transaction_message->house.object_id;

		if (object_id==0) fprintf(stderr, "\n\nIT %d ID: %d WARNING object_id=0 in real_estate_transaction_message", DAY, ID);

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING)
		{
			printf("\n\n%4d\t%4d", ID, object_id);
		}
		#endif

		found=0;
		for (i=0; i<SEIZED_COLLATERAL.size; i++)
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING)
				printf("\nSEIZED_COLLATERAL.array[%d].object_id=%d", i, SEIZED_COLLATERAL.array[i].seized_house.object_id);
			#endif

			if(SEIZED_COLLATERAL.array[i].seized_house.object_id == object_id)
			{
				found=1;

				#ifdef _DEBUG_MODE    
				if (PRINT_HOUSING_INFO)
				{
					printf("\n\nSelling SEIZED_COLLATERAL Bank ID: %d found object at index %d with object_id=%d", ID, i, object_id);

				}      
				#endif

				// Recompensate household
				recompensation = price - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;

				if (recompensation>0.0)
					add_seized_collateral_recompensation_message(SEIZED_COLLATERAL.array[i].seized_house.owner_id, recompensation);
				else
					recompensation = 0.0;

				// Accounting
				NPL_MORTGAGE_LOANS -= SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;
				RESERVES += (price - recompensation);
				EQUITY += (price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);
				PROFITS[0] += (price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);

				log_outflow_single("bank", "mortgages_defaulted", "house_sold", ID, SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);
				log_inflow_single("bank", "reserves", "real_estate_transaction", ID, price - recompensation);

				if (price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal > 0)
				{
					log_inflow_single("bank", "equity", "mortgage_extra_profit", ID, price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);
					MORTGAGE_EXTRA_PROFIT_CALENDAR_MONTH += price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;
				}
				else
				{
					log_outflow_single("bank", "equity", "mortgage_writeoff", ID, -(price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal));
					MORTGAGE_WRITEOFF_CALENDAR_MONTH +=  -(price - recompensation - SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);
				}

				//Ratio of collateral sale price vs. outstanding claim
				add_double(&COLLATERAL_PRICE_TO_CLAIM_RATIO, price / (SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal + SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_interest));


				// LGD
				sum_principal_wo += SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;
				sum_sale_value_capped += min(price, SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal);

				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_HOUSING)
				{
					printf("\n\n Removing adt from SEIZED_COLLATERAL array; Size before: %d", SEIZED_COLLATERAL.size);
				}      
				#endif

				remove_foreclosure_adt(&SEIZED_COLLATERAL, i);

				#ifdef _DEBUG_MODE    
				if (PRINT_DEBUG_HOUSING)
				{
					printf("\t Size after %d\n", SEIZED_COLLATERAL.size);
				}      
				#endif

				break;
			}
		}
		assert(found);

	FINISH_REAL_ESTATE_TRANSACTION_MESSAGE_LOOP

	if (sum_principal_wo>0.0)
		lgd = 1 - sum_sale_value_capped / sum_principal_wo;
	assert(lgd>=0.0);
	assert(lgd<=1.0);

	add_double(&SUM_SALE_VALUE_CAPPED_MONTHLY, sum_sale_value_capped);
	add_double(&SUM_MORTGAGE_PRINCIPAL_WRITTEN_OFF_MONTHLY, sum_principal_wo);

	if(SUM_SALE_VALUE_CAPPED_MONTHLY.size>3)
		remove_double(&SUM_SALE_VALUE_CAPPED_MONTHLY, 0);
	if(SUM_MORTGAGE_PRINCIPAL_WRITTEN_OFF_MONTHLY.size>3)
		remove_double(&SUM_MORTGAGE_PRINCIPAL_WRITTEN_OFF_MONTHLY, 0);

	enforce_bank_balanced_balance_sheet("Bank_receive_real_estate_transaction");

	#ifdef _DEBUG_MODE    
	if (PRINT_BANK_INFO)
	{
		printf("\n\nIT %d ID: %d Bank_receive_real_estate_transaction(): After REAL_ESTATE_TRANSACTION_MESSAGE_LOOP", DAY, ID);
		printf("\nFlows:");
		printf("\nSales revenue\t\t%f\t(RESERVES+,SEIZED COLL-)", sum_sales_revenue);
		printf("\nMortgage claim flow\t%f\t(MORTGAGE_CLAIM+)", sum_claim_flow);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		print_bank_balance_sheet();
	}
	#endif
	
	return 0;   
}
