#include "../../../header.h"
#include "../../../ClearingHouse_agent_header.h"
#include "../../../my_library_header.h"

/***************** TEMPLATE ****************/
/** \fn ClearingHouse()
 * \brief Function to
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: TODO
 */
/*
int ClearingHouse()
{

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_RENTAL)
    {
        printf("\n\n ID: %d ClearingHouse", ID);
    }      
    #endif
 
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

/** \fn ClearingHouse_init_rent_matching()
 * \brief Function to initialize the relationship between tenants and landlords
 * \brief Condition: Only once, after initial transient period.
 * \brief UNITTEST: 
 */
int ClearingHouse_init_rent_matching()
{
	int i;
	int count_objects=0;
	int count_owners=0;
	int count_tenants=0;
	int count_landlords=0;
	int count_tenants_positive_rent=0;

	double sum = 0.0;
	double mean_rent = 0.0;

	START_OWNER_INFO_MESSAGE_LOOP
		add_int(&INIT_OWNERS, owner_info_message->id);
		count_owners++;
	FINISH_OWNER_INFO_MESSAGE_LOOP

	START_TENANT_INFO_MESSAGE_LOOP
		add_tenant_adt(&INIT_TENANTS, tenant_info_message->id, tenant_info_message->monthly_rent);
		count_tenants++;
		sum +=  tenant_info_message->monthly_rent;
		if (tenant_info_message->monthly_rent > EPSILON) count_tenants_positive_rent++;
	FINISH_TENANT_INFO_MESSAGE_LOOP

	if (count_tenants_positive_rent > 0)
		mean_rent = sum/((double) count_tenants_positive_rent);
	else
		mean_rent = 0.0;

	START_LANDLORD_INFO_MESSAGE_LOOP
		add_int(&INIT_LANDLORDS, landlord_info_message->id);
		count_landlords++;
	FINISH_LANDLORD_INFO_MESSAGE_LOOP

	int no_tenants = INIT_TENANTS.size;
	int no_landlords = INIT_LANDLORDS.size;

	#ifdef _DEBUG_MODE    
	if (PRINT_RENTAL_INFO)
	{
		printf("\n\n ClearingHouse_init_rent_matching:");
		printf("\n\n Count tenant_info mesgs: %d Size INIT_TENANTS %d", count_tenants, INIT_TENANTS.size);
		printf("\n\n Count landlord_info msgs: %d Size INIT_LANDLORDS %d", count_landlords, INIT_LANDLORDS.size);
		printf("\n\n Count owner_info msgs: %d Size INIT_OWNERS %d", count_owners, INIT_OWNERS.size);

		printf("\n\n No. Tenants in array: %d\n", no_tenants);
		printf("\n No. Landlords in array: %d\n", no_landlords);
		printf("\n Avg. no. Tenants per Landlord: ");

		if(no_landlords>0) printf("%.3f\n", ((double) no_tenants)/no_landlords);
		else 	printf("0 (no landlords)\n");
		
		printf("\n Mean rent: %f\n", mean_rent);
		printf("\n Tenants with positive rent: %d\n", count_tenants_positive_rent);
		printf("\n Tenants w/o positive rent: %d  [will get mean_rent]\n", count_tenants-count_tenants_positive_rent);
	}      
	#endif

	
	int tenant=0;
	int landlord=0;
	count_objects=0;
	
	//Match tenants to landlords using round-robin algorithm (j=i mod m): j=landlord, i=tenant m=no_landlords n=no_tenants
	for (tenant=0; tenant<no_tenants; tenant++)
	{
		landlord = tenant % no_landlords; //landlord index = tenant index mod (no. landlords)

		//Exclude landlord being self-tenant
		if (INIT_LANDLORDS.array[landlord] == INIT_TENANTS.array[tenant].id)
			landlord = (tenant+1) % no_landlords;
			
		real_estate_adt rental_object;
		init_real_estate_adt(&rental_object);
		
		count_objects++;
		rental_object.object_id = count_objects;
		
		rental_object.tenant_id  = INIT_TENANTS.array[tenant].id;
		rental_object.owner_id = INIT_LANDLORDS.array[landlord];
		
		rental_object.cpi_base = 0.0;		// Has to be set later
		
		//HFCS DATA
		//Catch rent=0.0 and set to mean_rent
		if (INIT_TENANTS.array[tenant].monthly_rent > EPSILON)
			rental_object.monthly_rent = INIT_TENANTS.array[tenant].monthly_rent;
		else
			rental_object.monthly_rent = mean_rent;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_RENTAL_UNITTEST)
		{			
			printf("\n Matching TENANT [%4d] (ID %4d) to LANDLORDS [%4d] (ID %4d)", tenant, INIT_TENANTS.array[tenant].id, landlord, INIT_LANDLORDS.array[landlord]);
			if (INIT_TENANTS.array[tenant].monthly_rent > EPSILON)
				printf("\tRent: %f\n", rental_object.monthly_rent);
			else
				printf("\tRent: %f (rent info from tenant overridden: %f)\n", rental_object.monthly_rent, INIT_TENANTS.array[tenant].monthly_rent);
		}      
		#endif
		
		/* Set rental_object.value
		 * 1. This is set based on the house_price_index PRICE_INDEX_HMR in Household_read_rent_matching_info() 
		 * 2. Set to 1/n of the sampled value
		 */
		rental_object.purchase_price		= 0.0;  

		add_tenant_rent_matching_info_message(INIT_TENANTS.array[tenant].id, INIT_LANDLORDS.array[landlord], rental_object);
		add_landlord_rent_matching_info_message(INIT_TENANTS.array[tenant].id, INIT_LANDLORDS.array[landlord], rental_object);

		//free
		free_real_estate_adt(&rental_object);
	}

	#ifdef _DEBUG_MODE    
	if (PRINT_RENTAL_INFO)
	{
		printf("\n Attributed rental objects (matched tenants): %d\n", count_objects);
	}      
	#endif

	//Allocate object_ids to all owners for their HMR
	count_owners=0;
	for (i=0; i<INIT_OWNERS.size; i++)
	{
		count_objects++;
		count_owners++;
		add_hmr_matching_info_message(INIT_OWNERS.array[i], count_objects);
	}

	#ifdef _DEBUG_MODE    
	if (PRINT_RENTAL_INFO)
	{
		printf("\n Attributed owner objects: %d\n", count_owners);
		printf("\n Attributed total objects: %d\n", count_objects);
	}      
	#endif


	return 0;   
}

/** \fn ClearingHouse_recycle_unprocessed_rental_bids()
 * \brief Function for ClearingHouse to send itself messages with unprocessed_bids/asks to recycle
 * \brief Msgs to be sorted in ascending order by function ClearingHouse_receive_rental_bids().
 * \brief Message input: 
 * \brief Message output: rental_price_bid, rental_price_ask
 * \brief UNITTEST:  
 */
int ClearingHouse_recycle_unprocessed_rental_bids()
{
/*
	int i;
	bid_adt bid;
	ask_adt ask;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_RENTAL)
	{
		printf("\n\n ID: %d ClearingHouse_recycle_unprocessed_rental_bids", ID);
	}      
	#endif
	
	//Send a msg for each unprocessed ask:
	for (i = 0; i<RENTAL_AUCTION.unprocessed.asks.size; i++)
	{

		ask.id = RENTAL_AUCTION.unprocessed.asks.array[i].id;
		ask.price = RENTAL_AUCTION.unprocessed.asks.array[i].price;

		add_rental_price_ask_message(ask.price, ask);
	}

	//Send a msg for each unprocessed bid:
	for (i = 0; i<RENTAL_AUCTION.unprocessed.bids.size; i++)
	{

		bid.id = RENTAL_AUCTION.unprocessed.bids.array[i].id;
		bid.price = RENTAL_AUCTION.unprocessed.bids.array[i].price;

		add_rental_price_bid_message(bid.price, bid);
	}

	free_bid_adt(&bid);
	free_ask_adt(&ask);
*/
	return 0;   
}

/** \fn ClearingHouse_receive_rental_bids()
 * \brief Function to
 * \brief Message input: rental_price_bid, rental_price_ask
 * \brief Message output: 
 * \brief UNITTEST:  
 */
int ClearingHouse_receive_rental_bids()
{
	//printf("\nClearingHouse_receive_rental_bids START\n");
	bid_adt bid;
	ask_adt ask;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_RENTAL)
	{
		printf("\n\n ID: %d ClearingHouse_receive_rental_bids", ID);
	}      
	#endif
	//printf("\n");

	RENTAL_MARKET_MEAN_ASK = -1;
	RENTAL_MARKET_MEAN_BID = -1;
	RENTAL_MARKET_MEAN_ASK_QUALITY = -1;

	double sum_bid = 0.0;
	double sum_ask = 0.0;
	double sum_q = 0.0;

	RENTAL_MARKET_BIDS = 0;
	START_RENTAL_PRICE_BID_MESSAGE_LOOP
		assert(rental_price_bid_message->bid.price > -EPSILON);
		assert(isfinite(rental_price_bid_message->bid.price));

    	copy_bid_adt(&(rental_price_bid_message->bid), &bid);
        add_bid_adt(&RENTAL_AUCTION.pending.bids, bid.id, bid.price, bid.min_quality, bid.for_op);
        sum_bid+=bid.price;
        RENTAL_MARKET_BIDS++;


	FINISH_RENTAL_PRICE_BID_MESSAGE_LOOP

	int sum_months_on_market = 0;
	RENTAL_MARKET_ASKS = 0;
	START_RENTAL_PRICE_ASK_MESSAGE_LOOP
		assert(rental_price_ask_message->ask.price > -EPSILON);
		assert(isfinite(rental_price_ask_message->ask.price));
		
        copy_ask_adt(&(rental_price_ask_message->ask), &ask);
        add_ask_adt(&RENTAL_AUCTION.pending.asks, ask.object_id, ask.quality, ask.id, ask.price,  ask.months_on_market);

        
        RENTAL_MARKET_ASKS++;
        sum_ask += ask.price;
        sum_q += ask.quality;
            sum_months_on_market += ask.months_on_market;
        
	FINISH_RENTAL_PRICE_ASK_MESSAGE_LOOP

	if (RENTAL_MARKET_ASKS>0)
	{
		RENTAL_MARKET_MEAN_MONTHS_ON_MARKET = ((float) sum_months_on_market) / RENTAL_MARKET_ASKS;
		RENTAL_MARKET_MEAN_ASK = sum_ask / RENTAL_MARKET_ASKS;
		RENTAL_MARKET_MEAN_ASK_QUALITY = sum_q / RENTAL_MARKET_ASKS;
	}
	else
		RENTAL_MARKET_MEAN_MONTHS_ON_MARKET = 0;

	if (RENTAL_MARKET_BIDS>0)
	{
		RENTAL_MARKET_MEAN_BID = sum_bid / RENTAL_MARKET_BIDS;
	}

	free_bid_adt(&bid);
	free_ask_adt(&ask);


	return 0;   
}


/** \fn ClearingHouse_run_rental_auction()
 * \brief Function to
* Data structure:	auction_adt RENTAL_AUCTION
* Bids sorted from lowest to highest: [0]=lowest, [n]=highest, start from end and remove from end, until RENTAL_AUCTION.pending.bids.size==0
*  RENTAL_AUCTION.pending.bids.array[i].id
*  RENTAL_AUCTION.pending.bids.array[i].price
*  RENTAL_AUCTION.unprocessed.bids: after auction process, bids that remain
*
* Asks sorted from lowest to highest: [0]=lowest, [n]=highest, start from end and remove from end, until RENTAL_AUCTION.pending.asks.size==0
*  RENTAL_AUCTION.pending.asks.array[i].id
*  RENTAL_AUCTION.pending.asks.array[i].price
*  RENTAL_AUCTION.unprocessed.asks: after auction process, asks that remain
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: 5 TESTS PASSED 5.1.2017
 */

int ClearingHouse_run_rental_auction()
{
	int i;

	int done=0;
	int bid_index, ask_index;

	double sum_relative_prices = 0.0, sum_diff = 0.0;

	double_array rel_prices;
	init_double_array(&rel_prices);

	RENTAL_MARKET_MATCHES = 0;
	RENTAL_MARKET_UNMET_ASKS = RENTAL_AUCTION.pending.asks.size;
	RENTAL_MARKET_UNMET_BIDS = RENTAL_AUCTION.pending.bids.size;

	while (RENTAL_AUCTION.pending.bids.size > 0 && RENTAL_AUCTION.pending.asks.size > 0)
	{
		// Select random bid
		bid_index = random_int(0, RENTAL_AUCTION.pending.bids.size-1);

		// Find best ask
		ask_index = RENTAL_AUCTION.pending.asks.size-1;
		done = 0;
		while (!done && RENTAL_AUCTION.pending.asks.size>0)
		{				
			if (RENTAL_AUCTION.pending.bids.array[bid_index].price >= RENTAL_AUCTION.pending.asks.array[ask_index].price && RENTAL_AUCTION.pending.bids.array[bid_index].min_quality <= RENTAL_AUCTION.pending.asks.array[ask_index].quality)
			{
				done = 1;
				// Transaction!		
				real_estate_adt transaction;
				init_real_estate_adt(&transaction);

				transaction.object_id 	= RENTAL_AUCTION.pending.asks.array[ask_index].object_id;
				transaction.quality 	= RENTAL_AUCTION.pending.asks.array[ask_index].quality;
				transaction.owner_id = RENTAL_AUCTION.pending.asks.array[ask_index].id;
				transaction.tenant_id   = RENTAL_AUCTION.pending.bids.array[bid_index].id;
				transaction.monthly_rent = RENTAL_AUCTION.pending.asks.array[ask_index].price;
				transaction.cpi_base = 0.0;		// Has to be set later
				transaction.rpi_base = 0.0;		// Has to be set later

				RENTAL_MARKET_MATCHES++;
				//sum_relative_prices += (transaction.monthly_rent / hask_quality);
				sum_relative_prices += (transaction.monthly_rent);
				sum_diff += (RENTAL_AUCTION.pending.bids.array[bid_index].price - RENTAL_AUCTION.pending.asks.array[ask_index].price) / RENTAL_AUCTION.pending.bids.array[bid_index].price * 100;
				add_rental_matching_message(transaction.tenant_id, transaction.owner_id, transaction);

				add_real_estate_bid_data_adt(&RENTAL_MARKET_PAST_BIDS, DAY, RENTAL_AUCTION.pending.bids.array[bid_index].price, RENTAL_AUCTION.pending.bids.array[bid_index].min_quality, transaction.quality);
				add_real_estate_ask_data_adt(&RENTAL_MARKET_PAST_ASKS, DAY, RENTAL_AUCTION.pending.asks.array[ask_index].price, RENTAL_AUCTION.pending.asks.array[ask_index].quality, RENTAL_AUCTION.pending.bids.array[bid_index].price);
				add_real_estate_transaction_data_adt(&RENTAL_MARKET_PAST_TRANSACTIONS, DAY, transaction.quality, transaction.monthly_rent);

				add_double(&rel_prices, transaction.monthly_rent);


				// Remove ask
				remove_ask_adt(&RENTAL_AUCTION.pending.asks, ask_index);
				
			}
			else
			{
				ask_index--;
				if (ask_index<0)
					done = 1;
			}
		}

		remove_bid_adt(&RENTAL_AUCTION.pending.bids, bid_index);

	}

	RENTAL_MARKET_UNMET_BIDS = RENTAL_MARKET_UNMET_BIDS - RENTAL_MARKET_MATCHES;
	RENTAL_MARKET_UNMET_ASKS = RENTAL_MARKET_UNMET_ASKS - RENTAL_MARKET_MATCHES;


	if (RENTAL_MARKET_MATCHES > 0)
	{
		RENTAL_MARKET_PRICE_MEAN = sum_relative_prices / RENTAL_MARKET_MATCHES;
		RENTAL_MARKET_MEAN_BID_ASK_DIFF = sum_diff / RENTAL_MARKET_MATCHES;

		RENTAL_MARKET_PRICE_MEDIAN = my_stats_median(&rel_prices);
	}

/* End of version 1*/

	/* Update history and calculate index */
	int t;
	for (t=11; t>0; t--)
	{
		RENTAL_MARKET_PRICE_MEDIAN_HISTORY[t] = RENTAL_MARKET_PRICE_MEDIAN_HISTORY[t-1];
	}

	// Add current to [0]:
	RENTAL_MARKET_PRICE_MEDIAN_HISTORY[0] = RENTAL_MARKET_PRICE_MEDIAN;

	double sum=0;
	int c=0;
	for (t=0; t<12; t++)
	{
		if (RENTAL_MARKET_PRICE_MEDIAN_HISTORY[t] > 0)
		{
			sum += RENTAL_MARKET_PRICE_MEDIAN_HISTORY[t];
			c++;
		}
	}

	if (c>0)
		RENTAL_MARKET_PRICE_MOV_MED = sum/c;

	RENTAL_MARKET_PRICE_INDEX = RENTAL_MARKET_PRICE_MOV_MED;


	HOUSE_VS_RENT_PRICE_INDEX = HOUSING_MARKET_PRICE_INDEX / RENTAL_MARKET_PRICE_INDEX;
	
	for (i=0;i<RENTAL_AUCTION.pending.bids.size;i++)
		add_real_estate_bid_data_adt(&RENTAL_MARKET_PAST_BIDS, DAY, RENTAL_AUCTION.pending.bids.array[i].price, RENTAL_AUCTION.pending.bids.array[i].min_quality, -1.0);
	for (i=0;i<RENTAL_AUCTION.pending.asks.size;i++)
		add_real_estate_ask_data_adt(&RENTAL_MARKET_PAST_ASKS, DAY, RENTAL_AUCTION.pending.asks.array[i].price, RENTAL_AUCTION.pending.asks.array[i].quality, -1.0);

	//Reset bid array and ask array
	init_bid_adt_array(&RENTAL_AUCTION.pending.bids);
	init_ask_adt_array(&RENTAL_AUCTION.pending.asks);

	free_double_array(&rel_prices);

	// Remove too old transactions
	for(i=RENTAL_MARKET_PAST_TRANSACTIONS.size-1;i>=0;i--)
		if(DAY-RENTAL_MARKET_PAST_TRANSACTIONS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_transaction_data_adt(&RENTAL_MARKET_PAST_TRANSACTIONS, i);

	for(i=RENTAL_MARKET_PAST_BIDS.size-1;i>=0;i--)
		if(DAY-RENTAL_MARKET_PAST_BIDS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_bid_data_adt(&RENTAL_MARKET_PAST_BIDS, i);

	for(i=RENTAL_MARKET_PAST_ASKS.size-1;i>=0;i--)
		if(DAY-RENTAL_MARKET_PAST_ASKS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_ask_data_adt(&RENTAL_MARKET_PAST_ASKS, i);

	return 0;
}
