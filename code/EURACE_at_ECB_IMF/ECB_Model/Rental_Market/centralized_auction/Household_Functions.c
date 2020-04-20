#include "../../../header.h"
#include "../../../Household_agent_header.h"
#include "../../../my_library_header.h"
#include "../../Housing_Market/centralized_auction/Household_aux_header.h"
#include <assert.h>

/***************** TEMPLATE ****************/
/** \fn Household_TEMPLATE()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: 
 */
/*
int Household_TEMPLATE()
{
    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_MORTGAGE)
    {
        printf("\n\n Household_ ID: %d", ID);
    }      
    #endif
      
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

int Household_read_subsistence_level()
{
	
    START_SUBSISTENCE_LEVEL_MESSAGE_LOOP
        PRICE_INDEX = subsistence_level_message->cpi;
        MINIMUM_SUBSISTENCE_LEVEL = subsistence_level_message->minimum_subsistence_level;
        HOUSING_MARKET_PRICE_GROWTH_FACTOR = subsistence_level_message->housing_market_price_growth_factor;
        RENTAL_MARKET_PRICE_GROWTH_FACTOR = subsistence_level_message->rental_market_price_growth_factor;
        VARIABLE_RATE_LOANS_BASE_RATE = subsistence_level_message->variable_rate_loans_base_rate;
    FINISH_SUBSISTENCE_LEVEL_MESSAGE_LOOP
	
	return 0;
}


/** \fn Household_init_rent()
 * \brief Function to initialize the rental price from empirical data input
 * \brief Condition: (iteration_loop >= a.init_transient_period) && (a.switch_real_estate_sector == 1)
 * \brief UNITTEST: 
 */
int Household_init_rent()
{
	if (IS_TENANT)
	{
		MONTHLY_RENT = (1/12.0) * TOTAL_GROSS_ANNUAL_INCOME * CONST_INIT_HSTI_RATIO; //CONST_INIT_HSTI_RATIO: Rent-to-income ratio (housing service costs)

		// Quick and Dirty Fix to enable Rent payment before Budget determination
		HOUSING_BUDGET = MONTHLY_RENT;

		if (DISABLE_HOUSING_AND_RENTAL_MARKET)
			MONTHLY_RENT = 0.0;

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_RENTAL_UNITTEST)
		{
			if ( (MONTHLY_RENT != MONTHLY_RENT) )
				fprintf(stderr, "\n ID %d monthly_rent=%f", ID, MONTHLY_RENT);
			else if (MONTHLY_RENT<EPSILON)
				fprintf(stderr, "\nRENT ID %d sampled_monthly_rent: %f MONTHLY_RENT: %f HSTI ratio: %f \n",
				 ID, SAMPLE_INPUTS.sampled_monthly_rent, MONTHLY_RENT, CONST_INIT_HSTI_RATIO);
		}      
		#endif
	}

	return 0;
}

/** \fn Household_init_housing_states()
 * \brief Function to initialize housing_states: homeless, tenant, owner, landlord
 * \brief Condition: None
 * \brief UNITTEST: 
 */
int Household_init_housing_states()
{	
	double hmr_value=0.0;
	double op_value = 0.0;

//********** NEW DEFINITIONS OF IS_TENANT, IS_OWNER_IS_LANDLORD BASED ON CONDITIONS IN PROPERTY AND MORTGAGES
	
	IS_LANDLORD = 0;
	IS_OWNER = 0;
	IS_TENANT = 0;
	BTL_GENE = 0;

	hmr_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.purchase_price;
	op_value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.purchase_price;

	//Condition: value>0, duration>0, annuity>0, principal>0
	if(op_value>EPSILON)
	{
		IS_LANDLORD = 1;
		BTL_GENE = 1;
		add_landlord_info_message(ID, op_value);
	}

	if (hmr_value>EPSILON)
	{
		IS_OWNER = 1;
		add_owner_info_message(ID, hmr_value);
	}
	else
	{
		IS_TENANT = 1;
		add_tenant_info_message(ID, MONTHLY_RENT);
	}

	
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_init_housing_states (0):", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;
}

/** \fn Household_read_rent_matching_info()
 * \brief Function to initialize:
   - the relationship between tenants and landlords
   - for owners: attribute object_ids to HMRs 
   - for landlords: create rental units and attribute object_ids
   - for tenants: create rental units 
 * \brief Condition: If (IS_TENANT==1) OR (IS_LANDLORD==1)
 * \brief UNITTEST: PASS 12.1.17
 */
int Household_read_rent_matching_info()
{
	real_estate_adt rental;
	init_real_estate_adt(&rental);


	//All households store the price index info:
	START_HOUSE_PRICE_INDEX_MESSAGE_LOOP
		copy_price_index_adt(&(house_price_index_message->hmr_price_index), &HMR_PRICE_INDEX);
		copy_price_index_adt(&(house_price_index_message->op_price_index), &OP_PRICE_INDEX);
		copy_price_index_adt(&(house_price_index_message->rent_price_index), &RENT_PRICE_INDEX);
	FINISH_HOUSE_PRICE_INDEX_MESSAGE_LOOP


/*
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_RENTAL)
	{
		printf("\n\n ITS: %d ID: %d Household_read_rent_matching_info:", DAY, ID);
		printf("\n HMR_PRICE_INDEX.mean %f HMR_PRICE_INDEX.sd %f", HMR_PRICE_INDEX.mean, HMR_PRICE_INDEX.sd);
		printf("\n OP_PRICE_INDEX.mean %f OP_PRICE_INDEX.sd %f", OP_PRICE_INDEX.mean, OP_PRICE_INDEX.sd);
		printf("\n RENT_PRICE_INDEX.mean %f RENT_PRICE_INDEX.sd %f", RENT_PRICE_INDEX.mean, RENT_PRICE_INDEX.sd);
	}      
	#endif
*/
	
	if (IS_OWNER)
	{
		START_HMR_MATCHING_INFO_MESSAGE_LOOP
			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id = hmr_matching_info_message->object_id;
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_RENTAL_UNITTEST)
			{
				//printf("\n\n ITS: %d ID: %d Household_read_rent_matching_info: Owner\n", DAY, ID);
				printf("\n\t Owner object_id:\t %d", hmr_matching_info_message->object_id);
			}      
			#endif
		
		FINISH_HMR_MATCHING_INFO_MESSAGE_LOOP
		
		/*
		 *		START_HOUSE_PRICE_INDEX_MESSAGE_LOOP
		 *			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.value = house_price_index_message->hmr_price_index.mean + random_unif_interval(-1.0,1.0) * house_price_index_message->hmr_price_index.sd;
		 *		FINISH_HOUSE_PRICE_INDEX_MESSAGE_LOOP
		 */
	}
	
	if (IS_TENANT)
	{
		START_TENANT_RENT_MATCHING_INFO_MESSAGE_LOOP
		
			copy_real_estate_adt(&(tenant_rent_matching_info_message->rental_unit), &RENTAL_UNIT);
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_RENTAL_UNITTEST)
			{
				//printf("\n\n ITS: %d ID: %d Household_read_rent_matching_info: Tenant matched\n", DAY, ID);
				printf("\n\t Tenant object_id:\t\t  %d", tenant_rent_matching_info_message->rental_unit.object_id);
			}      
			#endif

		FINISH_TENANT_RENT_MATCHING_INFO_MESSAGE_LOOP


		//This sets values of the rental unit from the tenants perspective, see below for value from Landlords perspective (different value)
		RENTAL_UNIT.cpi_base = PRICE_INDEX;
		RENTAL_UNIT.rpi_base = 0;
		RENTAL_UNIT.purchase_price = 0.0;	// Here we set just a value to have it initialized
	}
	
	if (IS_LANDLORD)
	{
		//Start with an empty rental_property_list (was filled at initialization)
		init_real_estate_adt_array(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list);
		
		int count=0;
		
		START_LANDLORD_RENT_MATCHING_INFO_MESSAGE_LOOP
		
			count++;
			
			copy_real_estate_adt(&(landlord_rent_matching_info_message->rental_unit), &rental);
			
			rental.cpi_base = PRICE_INDEX;
			rental.rpi_base = 0;
						
			//Add new rental_property to list:
			//add_rental_adt(&VAR, object_id, tenant_id, landlord_id, monthly_rent)
			add_real_estate_adt(&HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list,
					rental.object_id,
					rental.owner_id,
					rental.tenant_id,
					rental.quality,
					rental.monthly_rent,
					DAY,
					rental.purchase_price,
					rental.hpi_base,
					rental.rpi_base,
					rental.cpi_base,
					rental.purchase_price,
					rental.monthly_rent,
					0,
					0,
					0.0,
					0.0,
					&rental.mortgage);
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_RENTAL_UNITTEST)
			{
				//if (count==1) printf("\n\n ITS: %d ID: %d Household_read_rent_matching_info: Landlord matched\n", DAY, ID);
				printf("\n\t Landlord object_id:\t\t\t %d", landlord_rent_matching_info_message->rental_unit.object_id);
			}      
			#endif
			
		FINISH_LANDLORD_RENT_MATCHING_INFO_MESSAGE_LOOP
	}

	free_real_estate_adt(&rental);
	
    
    
    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_receive_rental_matching_as_tenant (0): After RENT_MATCHING_INFO_MESSAGE_LOOP", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
    
	return 0;
}

int Household_check_eviction() 
{
	START_LANDLORD_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP

		#ifdef _DEBUG_MODE    
		if (PRINT_RENTAL_INFO)
		{
			printf("\nIT %d ID %d LANDLORD CANCEL RENTAL CONTRACT RECEIVED (ID: %d | T: %d L: %d, O: %d)", DAY, ID, ID, landlord_cancel_rental_contract_message->tenant_id, landlord_cancel_rental_contract_message->landlord_id, landlord_cancel_rental_contract_message->object_id);
		}
		#endif

		if (IS_TENANT && ID == landlord_cancel_rental_contract_message->tenant_id) 
		{
				IS_TENANT = 0;
				IS_HOMELESS = 1;
				init_real_estate_adt(&RENTAL_UNIT);
				PLAN_RENT_HMR = 1;
				EVICTED_AS_TENANT = 1;
		}
		else if (IS_OWNER && ID == landlord_cancel_rental_contract_message->tenant_id )
		{
			#ifdef _DEBUG_MODE    
			if (PRINT_RENTAL_INFO)
			{
				fprintf(stderr, "\nIT %d ID %d SPECIAL CASE: landlord_cancel_rental_contract message received as former tenant, but just already purchased an HMR so I am an owner! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, ID, landlord_cancel_rental_contract_message->tenant_id, landlord_cancel_rental_contract_message->landlord_id, landlord_cancel_rental_contract_message->object_id);
			}
			#endif
		}
		else if (IS_TENANT==0 && IS_OWNER==0 && ID == landlord_cancel_rental_contract_message->tenant_id) 
		{
			fprintf(stderr, "\nIT %d ID %d BUG: landlord_cancel_rental_contract message received as tenant, but not a tenant! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, ID, landlord_cancel_rental_contract_message->tenant_id, landlord_cancel_rental_contract_message->landlord_id, landlord_cancel_rental_contract_message->object_id);
		}

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
        {
            printf("\n\nIT %d ID %d Household_check_rental_contract (0): After LANDLORD_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP", DAY, ID);
            Household_print_housing_states();
        }
        #endif
        
    FINISH_LANDLORD_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP
    
    return 0;
}


int Household_pay_rent()
{
	int landlord_id = 0;

	//Tenants pay their rent
	RENT_PAID = 0;

	double effective_budget = min(HOUSING_BUDGET, PAYMENT_ACCOUNT);
	
	if (IS_TENANT) 
	{
		if (RENTAL_UNIT.owner_id > 0)
		{
            //Ensure this tenant has a valid rental unit with non-zero landlord_id: New tenants (owners who just sold their house) do not yet have a rental unit.
			if (MONTHLY_RENT > effective_budget || random_unif() < RANDOM_HMR_PROPENSITY) //Random moving
			{
				// Default on rent
				DEFAULTED_ON_RENT = 1;
				IS_TENANT = 0;
				IS_HOMELESS = 1;

				add_tenant_cancel_rental_contract_message(ID, RENTAL_UNIT.owner_id, RENTAL_UNIT.object_id);
                
				#ifdef _DEBUG_MODE
				if (PRINT_HOUSING_INFO)
				{
					printf("INFO ID %4d: 'default on rent' monthly_rent %f payment_account %f housing_budget %f\n", ID, MONTHLY_RENT, PAYMENT_ACCOUNT, HOUSING_BUDGET);
				}
				#endif
			}
			else
			{
				// Make payment
				
				//MONTHLY_RENT determined when HH budget calculated
				landlord_id = RENTAL_UNIT.owner_id;
				PAYMENT_ACCOUNT -= MONTHLY_RENT;
				log_outflow("household" , "payment_account", "net_worth", "rent", ID, MONTHLY_RENT);

				RENT_PAID = MONTHLY_RENT;
				
				add_rent_payment_message(ID, landlord_id, MONTHLY_RENT);

		   		if (PAYMENT_ACCOUNT<0.0)
					fprintf(stderr, "IT %d ID %d Household_pay_rent(): PAYMENT_ACCOUNT %f<0.0", DAY, ID, PAYMENT_ACCOUNT);
			}
		}
	}
	
	return 0;
}

int Household_receive_rent()
{
	
	//Filter: ID==rent_payment_message->landlord_id
	int count=0;
	OP_INCOME = 0.0;
	START_RENT_PAYMENT_MESSAGE_LOOP
		assert(IS_LANDLORD);

		PAYMENT_ACCOUNT += rent_payment_message->monthly_rent;
		log_inflow("household", "payment_account", "net_worth", "rent", ID, rent_payment_message->monthly_rent);

		OP_INCOME += rent_payment_message->monthly_rent;
		count++;

	FINISH_RENT_PAYMENT_MESSAGE_LOOP

	OP_PROFIT = OP_INCOME - OP_EXPENSE;

	assert(count<=HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size);

	return 0;
}

/** \fn Household_check_rental_contract()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: PASS 12.1.17 
 */
int Household_check_rental_contract()
{
	int i;
	int found = 0;


	START_TENANT_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP

        if (IS_LANDLORD && ID == tenant_cancel_rental_contract_message->landlord_id)
		{
				#ifdef _DEBUG_MODE    
				if (PRINT_RENTAL_INFO)
		        {
					printf("\nIT %d ID %d TENANT CANCEL RENTAL CONTRACT RECEIVED (ID: %d | T: %d L: %d, O: %d)", DAY, ID, ID, tenant_cancel_rental_contract_message->tenant_id, tenant_cancel_rental_contract_message->landlord_id, tenant_cancel_rental_contract_message->object_id);
		        }
		        #endif

				found=0;
				//Check which rental unit is concerned and set tenant == -1 (unoccupied)
				for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
				{
					if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id == tenant_cancel_rental_contract_message->object_id)
					{
						found = 1;
						HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id = -1;
						PLAN_LET_OP = 1;
					}
				}

				if (found==0)
				{
					if (!CHECK_SPECIAL_CASE)
					{
						if (IS_TENANT==0 && IS_LANDLORD==0 && ID==tenant_cancel_rental_contract_message->landlord_id)
							fprintf(stderr, "\nIT %d ID %d BUG (CHECK_SPECIAL_CASE=%d SOLD_OBJECT_ID %d): tenant_cancel_rental_contract message received as non-tenant + non-landlord! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, CHECK_SPECIAL_CASE, SOLD_OBJECT_ID, ID, tenant_cancel_rental_contract_message->tenant_id, tenant_cancel_rental_contract_message->landlord_id, tenant_cancel_rental_contract_message->object_id);
						
						if (IS_TENANT==1 && IS_LANDLORD==0 && ID==tenant_cancel_rental_contract_message->landlord_id)
							fprintf(stderr, "\nIT %d ID %d BUG: tenant_cancel_rental_contract message received as tenant + non-landlord! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, ID, tenant_cancel_rental_contract_message->tenant_id, tenant_cancel_rental_contract_message->landlord_id, tenant_cancel_rental_contract_message->object_id);
					}

					// Check for special case: just sold OP with same object_id as the rental unit that tenant wants to cancel
					if(CHECK_SPECIAL_CASE && SOLD_OBJECT_ID==tenant_cancel_rental_contract_message->object_id)
					{
						//Only check this
					    SOLD_OBJECT_ID = -1;
					    CHECK_SPECIAL_CASE = 0;

						#ifdef _DEBUG_MODE    
						if (PRINT_DEBUG_HOUSING_BRANCHES)
				        {
							fprintf(stderr, "\nIT %d ID %d Not a BUG: object in tenant_cancel_rental_contract not found but this is OK! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, ID, tenant_cancel_rental_contract_message->tenant_id, tenant_cancel_rental_contract_message->landlord_id, tenant_cancel_rental_contract_message->object_id);
				        }
				        #endif
					}
					else
					{
						if(HH_MORTGAGES_OP_WRITEOFF==0)	// Surpress warning if OP was seized
						{
							fprintf(stderr, "\nIT %d ID %d BUG: object in tenant_cancel_rental_contract not found! (ID: %d | T: %d L: %d, O: %d)\n", DAY, ID, ID, tenant_cancel_rental_contract_message->tenant_id, tenant_cancel_rental_contract_message->landlord_id, tenant_cancel_rental_contract_message->object_id);
							fprintf(stderr, "\n Rental objects of landlord %d", ID);

							for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
								fprintf(stderr, "\n %d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id);
						}
					}
				}
			}	

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_HOUSING_BRANCHES)
        {
            printf("\n\nIT %d ID: %d Household_check_rental_contract (0): After TENANT_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP", DAY, ID);
            Household_print_housing_states();
        }
        #endif
    FINISH_TENANT_CANCEL_RENTAL_CONTRACT_MESSAGE_LOOP
	
	return 0;
}


/** \fn Household_offer_rental_units()
 * \brief Function to offer a rental unit.
 * \brief Household creates new agents of type: House
 * \brief Message input: 
 * \brief Message output: 
 * \brief UNITTEST: PASS 12.1.17 
 */
int Household_offer_rental_units()
{
	
	ask_adt ask;
	int i, object_id;
	double last_rent __attribute__((unused)) = 0.0;
	double rent_index_base __attribute__((unused)) = 0.0;
	int units_offered = 0;
	int count =0;

	//Check which rental units have a tenant (occupied)

	// This prints out the list of Occupied Rental Units for each Landlord
	#ifdef _DEBUG_MODE    
	if (PRINT_RENTAL_INFO)
	{
		for (i=0;i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
		{
			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id != -1)
			{
				count++;
				if(count==1)
				{
					printf("\n\n ITS %d ID %d Household_offer_rental_units", DAY, ID);
					printf("\n\tOccupied Rental Units");
					printf("\n\tLandlord %d objects: %d", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size);
				}

				printf("\n\tobject %d [%2d/%2d] Tenant ID %4d", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id,
					 i+1, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id);
					
				if (ID == HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id)
					printf(" <-- self-tenant");
			}
		}
	}      
	#endif

	UNITS_VACANT = 0;
	UNITS_FILLED = 0;

	//Check which rental units have tenant == -1 (unoccupied)
	count=0;
	for (i=0;i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size;i++)
	{
		if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id == -1)
		{
			UNITS_VACANT++;
			count++;
			if(count==1)
			{
				#ifdef _DEBUG_MODE    
				if (PRINT_RENTAL_INFO)
				{
					printf("\n\n ITS %d ID %d Household_offer_rental_units", DAY, ID);
					printf("\n\tUnoccupied Rental Units");
					printf("\n\tLandlord %d objects: %d", ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size);
				}      
				#endif
			}

			if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market == 0)
				HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].renting_init_price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated;

			ask.price = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].renting_init_price;

			if (CONST_RENT_ASK_PRICE_ADJUSTMENT>0)
				ask.price = ask.price  * pow(1.0 - CONST_RENT_ASK_PRICE_ADJUSTMENT, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market);

			//printf("~R %f %d %f\n", HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market, ask.price);


	 		object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;
			
			ask.object_id = object_id;
			ask.quality = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].quality;
			ask.id = ID;
			ask.months_on_market = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market;


			HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market +=1;

			#ifdef _DEBUG_MODE    
			if (PRINT_RENTAL_INFO)
			{
				printf("\n\tobject_id %d Rent ASK %f", object_id, ask.price);
			}      
			#endif

			//Offering on rental market:
			add_rental_price_ask_message(ask.price, ask);

			units_offered = 1;
	
			//free
			free_ask_adt(&ask);
			//printf("OK \n");
		}
		else
		{
			UNITS_FILLED++;
		}
	}

	UNITS_TOTAL = UNITS_VACANT + UNITS_FILLED;

	if (units_offered == 1)
		MONTHS_VACANT_UNITS += 1;
	else
		MONTHS_VACANT_UNITS = 0;
	
	return 0;
}

/** \fn Household_send_rental_bid()
 * \brief Function to
 * \brief Message input:
 * \brief Message output:
 * \brief UNITTEST: PASS 12.1.17
 */
int Household_send_rental_bid()
{
	
	//printf("\n Household_send_rental_bid START \n");
	bid_adt bid;
	bid.id = ID;

	if (RENTAL_MARKET_USE_DSTI_FOR_BID)
		bid.price = MORTGAGE_SAMPLING_DRAW.dsti * MEAN_NET_INCOME;
	else
		bid.price = RENTAL_MARKET_BID_FRACTION_INCOME*MEAN_NET_INCOME;

	bid.min_quality = 0;

	if (MONTHS_HOME_SEEKING > 0)
		bid.price = bid.price  * pow(1.0 + CONST_RENT_BID_PRICE_ADJUSTMENT, MONTHS_HOME_SEEKING);

	if (bid.price > 0.5 * MEAN_NET_INCOME)
		bid.price = 0.5 * MEAN_NET_INCOME;
	
	if (bid.price > 0.0)
		add_rental_price_bid_message(bid.price, bid);

	//free
	free_bid_adt(&bid);

	MONTHS_HOME_SEEKING += 1;


	//printf("\n Household_send_rental_bid END \n");
	return 0;
}


/** \fn Household_receive_rental_matching_as_landlord()
 * \brief Function to read rental matchings
 * \brief Message input: rental_matching, filtered on m.landlord_id == a.id
 * \brief Message output:
 * \brief UNITTEST: PASS 12.1.17 
 */
int Household_receive_rental_matching_as_landlord()
{
	int i, count;

	mortgage_contract_adt mortgage;
	init_mortgage_contract_adt(&mortgage);
    
    count=0;
   	START_RENTAL_MATCHING_MESSAGE_LOOP
	if(rental_matching_message->landlord_id == ID)
	{


		count++;

 		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_RENTAL)
		{
			printf("\nID=%d RENTAL_MATCHING_MESSAGE %d\n", ID, count);
		}      
		#endif
		
		//If letting/rental out Other Property:
			//Single:
			//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.object_id = rental_matching_message->rental_unit.object_id;
			//HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.initial_other_property.rent = rental_matching_message->rental_unit.monthly_rent;
			
			//Multiple:
			//Find correct array index i for which object_id matches:
		int found=0;
			for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
			{
				if (rental_matching_message->rental_unit.object_id == HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id)
				{
					found=1;
					
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id = rental_matching_message->rental_unit.tenant_id;
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].monthly_rent = rental_matching_message->rental_unit.monthly_rent;
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rent_estimated = rental_matching_message->rental_unit.monthly_rent;
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].months_on_rental_market = 0;

					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].cpi_base = PRICE_INDEX;
					HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].rpi_base = RENTAL_MARKET_PRICE_INDEX;
	
					#ifdef _DEBUG_MODE    
					if (PRINT_DEBUG_RENTAL)
					{
						printf("\n Landlord rental matching: MEM : Tenant ID=%d Landlord ID=%d rent=%.3f", 
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].tenant_id,
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].owner_id,
							HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].monthly_rent);

						printf("\n Landlord rental matching: MSG : Tenant ID=%d Landlord ID=%d rent=%.3f\n", 
							rental_matching_message->rental_unit.tenant_id,
							rental_matching_message->rental_unit.owner_id,
							rental_matching_message->rental_unit.monthly_rent);
					}      
					#endif

					break;
				}
			}
		assert(found);
	}
	FINISH_RENTAL_MATCHING_MESSAGE_LOOP

	free_mortgage_contract_adt(&mortgage);

	if(count>0 && PLAN_LET_OP)
		PLAN_LET_OP = 0;

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_receive_rental_matching_as_landlord (0): After RENTAL_MATCHING_MESSAGE_LOOP", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;   
}


/** \fn Household_receive_rental_matching_as_tenant()
 * \brief Function to read rental matchings
 * \brief Message input: rental_matching, filtered on m.buyer_id == a.id
 * \brief Message output:
 * \brief UNITTEST: PASS 12.1.17 
 */
int Household_receive_rental_matching_as_tenant()
{
	int count=0;

   	START_RENTAL_MATCHING_MESSAGE_LOOP
   	if(rental_matching_message->tenant_id == ID)
	{
		count++;

		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_RENTAL)
		{
			printf("\nID=%d RENTAL_MATCHING_MESSAGE %d", ID, count);
		}      
		#endif

		//If rental:
		if(PLAN_RENT_HMR)
		{
			//Register data from rental_unit
			copy_real_estate_adt(&(rental_matching_message->rental_unit), &RENTAL_UNIT);
			MONTHS_HOME_SEEKING = 0;

			RENTAL_UNIT.cpi_base =  PRICE_INDEX;
			RENTAL_UNIT.rpi_base =  0.0;
			
			IS_TENANT=1;
			IS_HOMELESS=0;

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_RENTAL)
			{
				printf("\n Tenant rental matching: MEM : Tenant ID=%d Landlord ID=%d rent=%.3f", RENTAL_UNIT.tenant_id, RENTAL_UNIT.owner_id, RENTAL_UNIT.monthly_rent);

				printf("\n Tenant rental matching: MSG : Tenant ID=%d Landlord ID=%d rent=%.3f\n", rental_matching_message->rental_unit.tenant_id, rental_matching_message->rental_unit.owner_id, rental_matching_message->rental_unit.monthly_rent);
			}      
			#endif
		}
	}
	FINISH_RENTAL_MATCHING_MESSAGE_LOOP

	if(count>0 && PLAN_RENT_HMR)
	{
		PLAN_RENT_HMR = 0;
	}

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING_BRANCHES)
    {
        printf("\n\nIT %d ID: %d Household_receive_rental_matching_as_tenant(0): After RENTAL_MATCHING_MESSAGE_LOOP", DAY, ID);
        Household_print_housing_states();
    }
    #endif
    
	return 0;   
}
