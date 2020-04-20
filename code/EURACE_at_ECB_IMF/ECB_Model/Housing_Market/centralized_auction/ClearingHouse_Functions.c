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
    if (PRINT_DEBUG_HOUSING)
    {
        printf("\n\n ID: %d ClearingHouse", ID);
    }      
    #endif
 
 return 0;   
}
*/
/***************** END OF TEMPLATE ****************/

/** \fn Clearinghouse_Send_Market_Data()
 * \brief Function for ClearingHouse to send price means / indices for rental and housing markets
 * \brief Freq: Monthly, phase 2 (on 2nd day of month)
 * \brief Message input: 
 * \brief Message output: real_estate_market_data
 * \brief 
 */
int Clearinghouse_Send_Market_Data()
{
	add_real_estate_market_data_message(RENTAL_MARKET_PRICE_MEAN, RENTAL_MARKET_PRICE_INDEX, HOUSING_MARKET_PRICE_MEAN, HOUSING_MARKET_PRICE_INDEX);

	return 0;
}


/** \fn ClearingHouse_recycle_unprocessed_real_estate_bids()
 * \brief Function for ClearingHouse to send itself messages with unprocessed_bids/asks to recycle
 * \brief Msgs to be sorted in ascending order by function ClearingHouse_receive_real_estate_bids().
 * \brief Message input: 
 * \brief Message output: real_estate_price_bid, real_estate_price_ask
 * \brief UNITTEST:  PASS 4.1.17
 */
int ClearingHouse_recycle_unprocessed_real_estate_bids()
{
/*
	int i;
	bid_adt bid;
	ask_adt ask;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING)
	{
		printf("\n\n ID: %d ClearingHouse_recycle_unprocessed_real_estate_bids\n", ID);
	}      
	#endif
	
	//Send a msg for each unprocessed ask:
	for (i = 0; i<HOUSE_AUCTION.unprocessed.asks.size; i++)
	{

		ask.id = HOUSE_AUCTION.unprocessed.asks.array[i].id;
		ask.price = HOUSE_AUCTION.unprocessed.asks.array[i].price;

		if (ask.price < eps )
			fprintf(stderr, "\nERROR ClearingHouse_recycle_unprocessed_real_estate_bids, real_estate_price_ask_message: ask.price=%f < %f\n", ask.price, eps);

		add_real_estate_price_ask_message(ask.price, ask);
	}

	//Send a msg for each unprocessed bid:
	for (i = 0; i<HOUSE_AUCTION.unprocessed.bids.size; i++)
	{

		bid.id = HOUSE_AUCTION.unprocessed.bids.array[i].id;
		bid.price = HOUSE_AUCTION.unprocessed.bids.array[i].price;

		if (bid.price < eps )
			fprintf(stderr, "\nERROR ClearingHouse_recycle_unprocessed_real_estate_bids, real_estate_price_bid_message: bid.price=%f < %f\n", bid.price, eps);

		add_real_estate_price_bid_message(bid.price, bid);
	}

 	//free
	free_bid_adt(&bid);
	free_ask_adt(&ask);
*/
	return 0;   
}

int transaction_rank_quality_function(const void *x, const void *y)
{
    if( ((real_estate_transaction_data_adt *)x)->quality > ((real_estate_transaction_data_adt *)y)->quality) return 1;
    else if( ((real_estate_transaction_data_adt *)x)->quality < ((real_estate_transaction_data_adt *)y)->quality) return -1;
    else return 0;
}

int transaction_rank_price_function(const void *x, const void *y)
{
    if( ((real_estate_transaction_data_adt *)x)->price > ((real_estate_transaction_data_adt *)y)->price) return 1;
    else if( ((real_estate_transaction_data_adt *)x)->price < ((real_estate_transaction_data_adt *)y)->price) return -1;
    else return 0;
}

int bid_rank_price_function(const void *x, const void *y)
{
    if( ((real_estate_bid_data_adt *)x)->price > ((real_estate_bid_data_adt *)y)->price) return 1;
    else if( ((real_estate_bid_data_adt *)x)->price < ((real_estate_bid_data_adt *)y)->price) return -1;
    else return 0;
}

int ask_rank_quality_function(const void *x, const void *y)
{
    if( ((real_estate_ask_data_adt *)x)->quality > ((real_estate_ask_data_adt *)y)->quality) return 1;
    else if( ((real_estate_ask_data_adt *)x)->quality < ((real_estate_ask_data_adt *)y)->quality) return -1;
    else return 0;
}

int auction_ask_rank_quality_function(const void *x, const void *y)
{
    if( ((ask_adt *)x)->quality > ((ask_adt *)y)->quality) return 1;
    else if( ((ask_adt *)x)->quality < ((ask_adt *)y)->quality) return -1;
    else return 0;
}

int ask_rank_price_function(const void *x, const void *y)
{
    if( ((real_estate_ask_data_adt *)x)->price > ((real_estate_ask_data_adt *)y)->price) return 1;
    else if( ((real_estate_ask_data_adt *)x)->price < ((real_estate_ask_data_adt *)y)->price) return -1;
    else return 0;
}

int sort_transaction_data_by_quality(real_estate_transaction_data_adt_array data)
{
	qsort(data.array, data.size, sizeof(real_estate_transaction_data_adt), transaction_rank_quality_function);

	return 0;
}

int sort_transaction_data_by_price(real_estate_transaction_data_adt_array data)
{
	qsort(data.array, data.size, sizeof(real_estate_transaction_data_adt), transaction_rank_price_function);

	return 0;
}

int sort_bid_data_by_price(real_estate_bid_data_adt_array data)
{
	qsort(data.array, data.size, sizeof(real_estate_bid_data_adt), bid_rank_price_function);

	return 0;
}

int sort_ask_data_by_quality(real_estate_ask_data_adt_array data)
{
	qsort(data.array, data.size, sizeof(real_estate_ask_data_adt), ask_rank_quality_function);

	return 0;
}

int sort_ask_data_by_price(real_estate_ask_data_adt_array data)
{
	qsort(data.array, data.size, sizeof(real_estate_ask_data_adt), ask_rank_price_function);

	return 0;
}

double get_highest_potential_price(real_estate_ask_data_adt_array data, double q)
{
	int i;
	for (i=data.size-1;i>=0;i--)
		if (data.array[i].quality <= q && data.array[i].matched_to_bid_price > 0.0)
			return data.array[i].matched_to_bid_price;

	if (data.size>0)
		return data.array[0].matched_to_bid_price;
	else
		return -1.0;
}

double get_lowest_potential_price(real_estate_ask_data_adt_array data, double q)
{
	int i;

	for (i=0;i<data.size;i++)
		if (data.array[i].quality >= q)
		{
			if (data.array[i].matched_to_bid_price > 0)
			{
				return data.array[i].matched_to_bid_price;
			}
			else
			{
				return data.array[i].price;
			}
		}

	return INFINITY;
}

double get_highest_potential_quality(real_estate_ask_data_adt_array data, double p)
{
	int i;

	for (i=data.size-1;i>=0;i--)
		if (data.array[i].price <= p)
			return data.array[i].quality;

	return 0.0;
}

int get_transaction_data_closest_index_by_quality(real_estate_transaction_data_adt_array data, double q)
{
	int a=0, b=data.size-1, m=0,c=0;

	if (data.size>0)
	{
		while ((b-a)>1)
		{
			m = a + (b-a)/2;

			if (data.array[m].quality>q)
				b=m;
			else
				a=m;
		}

		if (fabs(data.array[a].quality-q) < fabs(data.array[b].quality-q))
			c=a;
		else
			c=b;
	}
	else
		c = -1;

	return c;
}

int get_transaction_data_closest_index_by_price(real_estate_transaction_data_adt_array data, double p)
{
	int a=0, b=data.size-1, m=0,c=0;

	if (data.size>0)
	{
		while ((b-a)>1)
		{
			m = a + (b-a)/2;

			if (data.array[m].price>p)
				b=m;
			else
				a=m;
		}

		if (fabs(data.array[a].price-p) < fabs(data.array[b].price-p))
			c=a;
		else
			c=b;
	}
	else
		c=-1;

	return c;
}

double get_transaction_data_avg_price_in_neighborhood(real_estate_transaction_data_adt_array data, int index, int r)
{
	double sum = data.array[index].price;
	int dist=0;
	int il=0,ir=0;
	int n=1;

	for (dist=1;dist<=r;dist++)
	{
		il = index - dist;
		ir = index + dist;

		if (il>=0)
		{
			sum += data.array[il].price;
			n++;
		}

		if (ir<data.size)
		{
			sum += data.array[ir].price;
			n++;
		}
	}

	return sum / n;
}

double get_transaction_data_avg_quality_in_neighborhood(real_estate_transaction_data_adt_array data, int index, int r)
{
	double sum = data.array[index].quality;
	int dist=0;
	int il=0,ir=0;
	int n=1;

	for (dist=1;dist<=r;dist++)
	{
		il = index - dist;
		ir = index + dist;

		if (il>=0)
		{
			sum += data.array[il].quality;
			n++;
		}

		if (ir<data.size)
		{
			sum += data.array[ir].quality;
			n++;
		}
	}

	return sum / n;
}

double get_average_highest_price_in_past(real_estate_ask_data_adt_array data, double q, int m)
{
	double sum __attribute__((unused)) = 0;
	int day;
	int n __attribute__((unused)) = 0;
	int i,j;
	real_estate_ask_data_adt_array asks;
	double p=0.0;

	init_real_estate_ask_data_adt_array(&asks);

	for (i=1;i<=m;i++)
	{
		day = DAY-i*20;
		for (j=0;j<data.size;j++)
			if (data.array[j].day==day)
				add_real_estate_ask_data_adt(&asks, data.array[j].day, data.array[j].price, data.array[j].quality, data.array[j].matched_to_bid_price);
	}

	if (asks.size>0)
		p = get_highest_potential_price(asks, q);

	free_real_estate_ask_data_adt_array(&asks);

	if (p>0.0)
		return p;
	else
		return -1;
}

double get_average_lowest_price_in_auctions(real_estate_ask_data_adt_array data, double q, int m)
{
	double sum=0;
	int day;
	int n __attribute__((unused)) = 0;
	int i,j;
	real_estate_ask_data_adt_array asks;

	for (i=1;i<=m;i++)
	{
		day = DAY-i*20;
		init_real_estate_ask_data_adt_array(&asks);
		for (j=0;j<data.size;j++)
			if (data.array[j].day==day)
				add_real_estate_ask_data_adt(&asks, data.array[j].day, data.array[j].price, data.array[j].quality, data.array[j].matched_to_bid_price);

		if (asks.size>0)
		{
			n++;
			sum+=get_lowest_potential_price(asks, q);
		}

		free_real_estate_ask_data_adt_array(&asks);

	}

	if (n>0)
		return sum/n;
	else
		return INFINITY;
}

double get_average_highest_quality_in_past(real_estate_ask_data_adt_array data, double p, int m)
{
	double sum __attribute__((unused)) = 0;
	int day;
	int n __attribute__((unused)) = 0;
	int i,j;
	real_estate_ask_data_adt_array asks;
	double q=0.0;

	init_real_estate_ask_data_adt_array(&asks);

	for (i=1;i<=m;i++)
	{
		day = DAY-i*20;
		
		for (j=0;j<data.size;j++)
			if (data.array[j].day==day)
				add_real_estate_ask_data_adt(&asks, data.array[j].day, data.array[j].price, data.array[j].quality, data.array[j].matched_to_bid_price);
	}

	if (asks.size>0)
		q = get_highest_potential_quality(asks, p);

	free_real_estate_ask_data_adt_array(&asks);		

	if (q>0.0)
		return q;
	else
		return -1.0;

}

int Clearinghouse_send_real_estate_market_info()
{
	int id=0;
	int for_op __attribute__((unused)) = 0;
	double desired_expenditure=0.0;
	double estimated_home_quality = -1.0, estimated_rental_price = -1.0, estimated_home_price=-1.0, estimated_rental_quality=-1.0;

	sort_ask_data_by_quality(HOUSING_MARKET_PAST_ASKS);
	sort_ask_data_by_price(RENTAL_MARKET_PAST_ASKS);

	START_REAL_ESTATE_MARKET_INFO_REQUEST_MESSAGE_LOOP

		id = real_estate_market_info_request_message->id;
		for_op = real_estate_market_info_request_message->for_op;
		desired_expenditure = real_estate_market_info_request_message->expenditure;

		estimated_home_quality = get_average_highest_quality_in_past(HOUSING_MARKET_PAST_ASKS, desired_expenditure, CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS);
		estimated_rental_price = get_average_highest_price_in_past(RENTAL_MARKET_PAST_ASKS, estimated_home_quality, CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS);

		if (estimated_home_quality<0.0)
			estimated_home_quality = 0.0;

		if (estimated_rental_price<0.0)
			estimated_rental_price = INFINITY;


		add_real_estate_market_info_response_message(id, estimated_home_quality, estimated_home_price, estimated_rental_quality, estimated_rental_price);

	FINISH_REAL_ESTATE_MARKET_INFO_REQUEST_MESSAGE_LOOP

	return 0;
}

int Clearinghouse_asses_houses()
{
	HOUSING_MARKET_STORED_OBSERVATIONS = HOUSING_MARKET_PAST_TRANSACTIONS.size;
	RENTAL_MARKET_STORED_OBSERVATIONS = RENTAL_MARKET_PAST_TRANSACTIONS.size;

	int i;
	for (i=0;i<HOUSING_MARKET_PAST_TRANSACTIONS.size; i++) 
		vlog_data("ch_history", 2, HOUSING_MARKET_PAST_TRANSACTIONS.array[i].quality,HOUSING_MARKET_PAST_TRANSACTIONS.array[i].price);

	sort_ask_data_by_price(HOUSING_MARKET_PAST_ASKS);
	sort_ask_data_by_price(RENTAL_MARKET_PAST_ASKS);


	double q=0;
	int id=0;
	int oid=0;

	double house_price = -1.0;
	double rent_price = -1.0;

	HOUSING_MARKET_FAILED_ASSESMENTS = 0;
	RENTAL_MARKET_FAILED_ASSESMENTS = 0;

	START_ASSES_HOUSE_REQUEST_MESSAGE_LOOP
		id = asses_house_request_message->id;
		oid = asses_house_request_message->object_id;
		q = asses_house_request_message->quality;

		house_price = get_average_highest_price_in_past(HOUSING_MARKET_PAST_ASKS, q, CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS);
		rent_price = get_average_highest_price_in_past(RENTAL_MARKET_PAST_ASKS, q, CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS);


		add_asses_house_response_message(id, oid, house_price, rent_price);

	FINISH_ASSES_HOUSE_REQUEST_MESSAGE_LOOP

	return 0;
}



/** \fn ClearingHouse_receive_real_estate_bids()
 * \brief Function to
 * \brief Message input: real_estate_price_bid, real_estate_price_ask
 * \brief Message output: 
 * \brief UNITTEST:  PASS 4.1.17
 */
int ClearingHouse_receive_real_estate_bids()
{
    double eps __attribute__((unused)) = 1e-3;
    
	bid_adt bid;
	ask_adt ask;

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING_UNITTEST)
	{
		printf("\n\n ID: %d ClearingHouse_receive_real_estate_bids\n", ID);
	}      
	#endif
	HOUSING_MARKET_MEAN_ASK = -1;
	HOUSING_MARKET_MEAN_BID = -1;
	HOUSING_MARKET_MEAN_ASK_QUALITY = -1;

	double sum_bid=0.0;
	double sum_ask=0.0;
	double sum_q=0.0;

	HOUSING_MARKET_BIDS = 0;
	
    START_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
    	assert(real_estate_price_bid_message->bid.price > -EPSILON);
    	assert(isfinite(real_estate_price_bid_message->bid.price));
	
        copy_bid_adt(&(real_estate_price_bid_message->bid), &bid);
        add_bid_adt(&HOUSE_AUCTION.pending.bids, bid.id, bid.price, bid.min_quality, bid.for_op);
	    sum_bid += bid.price;
	    HOUSING_MARKET_BIDS++;
			vlog_data("ch_bid", 2, bid.price, bid.min_quality);

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING_UNITTEST)
			{
				printf("b ID %d P %f Q_min %f\n", bid.id, bid.price, bid.min_quality);
			}      
			#endif


        
	FINISH_REAL_ESTATE_PRICE_BID_MESSAGE_LOOP
 
	int sum_months_on_market = 0;
	HOUSING_MARKET_SLOW_SELLER = 0;

 	HOUSING_MARKET_ASKS = 0;
    
	START_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP

		assert(real_estate_price_ask_message->ask.price > -EPSILON);
		assert(isfinite(real_estate_price_ask_message->ask.price));

       	copy_ask_adt(&(real_estate_price_ask_message->ask), &ask);
       		add_ask_adt(&HOUSE_AUCTION.pending.asks, ask.object_id, ask.quality, ask.id, ask.price, ask.months_on_market);
	        sum_ask += ask.price;
	        HOUSING_MARKET_ASKS++;
	        sum_q   += ask.quality;
	        sum_months_on_market += ask.months_on_market;
	        if (ask.months_on_market > 24)
	        	HOUSING_MARKET_SLOW_SELLER++;
	        vlog_data("ch_ask", 2, ask.price, ask.quality);

			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_HOUSING_UNITTEST)
			{
				printf("a ObjectID %4d ID %4d Price %f Q %f\n", ask.object_id, ask.id, ask.price, ask.quality);
			}      
			#endif

       	
        
	FINISH_REAL_ESTATE_PRICE_ASK_MESSAGE_LOOP

	if (HOUSING_MARKET_ASKS>0)
	{
		HOUSING_MARKET_MEAN_MONTHS_ON_MARKET = ((float) sum_months_on_market) / HOUSING_MARKET_ASKS;
		HOUSING_MARKET_MEAN_ASK = sum_ask / HOUSING_MARKET_ASKS;
		HOUSING_MARKET_MEAN_ASK_QUALITY = sum_q / HOUSING_MARKET_ASKS;
	}
	else
		HOUSING_MARKET_MEAN_MONTHS_ON_MARKET=-1;

	if (HOUSING_MARKET_BIDS>0)
		HOUSING_MARKET_MEAN_BID = sum_bid / HOUSING_MARKET_BIDS;

  	//free
	free_bid_adt(&bid);
	free_ask_adt(&ask);

	return 0;   
}


/** \fn ClearingHouse_run_real_estate_auction()
 * \brief Function to
* Data structure:	auction_adt HOUSE_AUCTION
* Bids sorted from lowest to highest: [0]=lowest, [n]=highest, start from end and remove from end, until HOUSE_AUCTION.pending.bids.size==0
*  HOUSE_AUCTION.pending.bids.array[i].id
*  HOUSE_AUCTION.pending.bids.array[i].price
*  HOUSE_AUCTION.unprocessed.bids: after auction process, bids that remain
*
* Asks sorted from lowest to highest: [0]=lowest, [n]=highest, start from end and remove from end, until HOUSE_AUCTION.pending.asks.size==0
*  HOUSE_AUCTION.pending.asks.array[i].id
*  HOUSE_AUCTION.pending.asks.array[i].price
*  HOUSE_AUCTION.unprocessed.asks: after auction process, asks that remain
 * \brief Message input: None
 * \brief Message output: 
 * \brief UNITTEST: 5 TESTS PASSED 5.1.2017
 */

int ClearingHouse_run_real_estate_auction()
{
	#define MAX_LOOPS 1600	//cirquit breaker for auctions

	int n __attribute__((unused));
	int m __attribute__((unused));
	double best_bid __attribute__((unused));
	double best_ask __attribute__((unused));
	int best_bid_id __attribute__((unused));
	int best_ask_id __attribute__((unused));

	real_estate_adt transaction;

	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_HOUSING)
	{
		printf("\n\n ID: %d ClearingHouse_run_real_estate_auction", ID);
        printf("\n\n No.\t\tnBids\t\tnAsks\t\tBestbid\t\tID\t\tBestAsk\t\tID\t\tObject\t\tBuyer\t\tSeller\t\tPrice");
	}      
	#endif

/*
	while bids not empty and asks not empty:
	{
		update best bid with highest price
		move all asks with a price above the best bid to unprocessed.asks
		match best bid to first ask below best bid
	}
	remaining bids: move bids to unprocessed.bids
	remaining asks: move askss to unprocessed.asks
*/
	double sum_relative_prices=0.0;

	HOUSING_MARKET_PRICE_MEAN = 0.0;
	HOUSING_MARKET_MATCHES = 0;

    int loop_count __attribute__((unused)) = 0;
	int done = 0;

	double sum_diff = 0.0;

	HOUSING_MARKET_UNMET_ASKS = HOUSE_AUCTION.pending.asks.size;
	HOUSING_MARKET_UNMET_BIDS = HOUSE_AUCTION.pending.bids.size;

	double_array rel_prices;
	init_double_array(&rel_prices);

	int i=0;

	double price=0.0;

	// Sort asks by quality
	qsort(HOUSE_AUCTION.pending.asks.array, HOUSE_AUCTION.pending.asks.size, sizeof(ask_adt), auction_ask_rank_quality_function);

	int ask_index;
	int bid_index;

	int do_transaction;

	int seller_id;
	int buyer_id;

	double expected_rent;
	double_array expected_profits;
	init_double_array(&expected_profits);

	sort_bid_data_by_price(RENTAL_MARKET_PAST_BIDS);

	for (i=0;i<HOUSE_AUCTION.pending.asks.size;i++)
	{
		expected_rent = get_average_highest_price_in_past(RENTAL_MARKET_PAST_ASKS, HOUSE_AUCTION.pending.asks.array[i].quality, CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS);
		add_double(&expected_profits, expected_rent / HOUSE_AUCTION.pending.asks.array[i].price);
	}


	while (HOUSE_AUCTION.pending.bids.size > 0 && HOUSE_AUCTION.pending.asks.size > 0)
	{
		// Select random bid
		bid_index = random_int(0, HOUSE_AUCTION.pending.bids.size-1);
		do_transaction = 0;


		if (!HOUSE_AUCTION.pending.bids.array[bid_index].for_op)
		{
			// For HMR
			// Find best ask
			ask_index = HOUSE_AUCTION.pending.asks.size-1;
			done = 0;
			while (!done && HOUSE_AUCTION.pending.asks.size>0)
			{				
				if (HOUSE_AUCTION.pending.bids.array[bid_index].price >= HOUSE_AUCTION.pending.asks.array[ask_index].price && HOUSE_AUCTION.pending.bids.array[bid_index].min_quality <= HOUSE_AUCTION.pending.asks.array[ask_index].quality)
				{
					done = 1;
					do_transaction = 1;
				}
				else
				{
					ask_index--;
					if (ask_index<0)
						done = 1;
				}
			}

		}
		else
		{
			// For OP
			int max_index=0;
			double max_value=-1.0;
			for (i=0;i<HOUSE_AUCTION.pending.asks.size;i++)
			{
				if (expected_profits.array[i] > max_value && HOUSE_AUCTION.pending.bids.array[bid_index].price >= HOUSE_AUCTION.pending.asks.array[i].price)
				{
					max_index = i;
					max_value = expected_profits.array[i];
				}
			}

			if (max_value > 0)
			{
				do_transaction = 1;
				ask_index = max_index;
			}
		}

		if (do_transaction)
		{
			// Transaction!     
            init_real_estate_adt(&transaction);
            
            transaction.object_id = HOUSE_AUCTION.pending.asks.array[ask_index].object_id;
            transaction.quality = HOUSE_AUCTION.pending.asks.array[ask_index].quality;
            seller_id = HOUSE_AUCTION.pending.asks.array[ask_index].id;
            buyer_id  = HOUSE_AUCTION.pending.bids.array[bid_index].id;
            
			price = HOUSE_AUCTION.pending.asks.array[ask_index].price; // set transaction price to ask price

            transaction.purchase_price = price;
            transaction.value_estimated = price;                
            transaction.hpi_base = HOUSING_MARKET_PRICE_INDEX;
            
            HOUSING_MARKET_MATCHES++;
			
            add_real_estate_transaction_message(seller_id, buyer_id, transaction);

            vlog_data("ch_transaction", 4, price, transaction.quality, HOUSE_AUCTION.pending.bids.array[bid_index].price, HOUSE_AUCTION.pending.bids.array[bid_index].min_quality);

			add_real_estate_transaction_data_adt(&HOUSING_MARKET_PAST_TRANSACTIONS, DAY, transaction.quality, transaction.purchase_price);
			add_real_estate_bid_data_adt(&HOUSING_MARKET_PAST_BIDS, DAY, HOUSE_AUCTION.pending.bids.array[bid_index].price, HOUSE_AUCTION.pending.bids.array[bid_index].min_quality, transaction.quality);
			add_real_estate_ask_data_adt(&HOUSING_MARKET_PAST_ASKS, DAY, HOUSE_AUCTION.pending.asks.array[ask_index].price, HOUSE_AUCTION.pending.asks.array[ask_index].quality, HOUSE_AUCTION.pending.bids.array[bid_index].price);

			add_double(&rel_prices, transaction.purchase_price);

			// Remove ask
			remove_ask_adt(&HOUSE_AUCTION.pending.asks, ask_index);
			
			remove_double(&expected_profits, ask_index);			

			assert(HOUSE_AUCTION.pending.asks.size == expected_profits.size);
		}

		remove_bid_adt(&HOUSE_AUCTION.pending.bids, bid_index);
	}

	HOUSING_MARKET_UNMET_BIDS = HOUSING_MARKET_UNMET_BIDS - HOUSING_MARKET_MATCHES;
	HOUSING_MARKET_UNMET_ASKS = HOUSING_MARKET_UNMET_ASKS - HOUSING_MARKET_MATCHES;

	if (HOUSING_MARKET_MATCHES > 0)
	{
		HOUSING_MARKET_PRICE_MEAN = sum_relative_prices / HOUSING_MARKET_MATCHES;
		HOUSING_MARKET_MEAN_BID_ASK_DIFF = sum_diff / HOUSING_MARKET_MATCHES;

		HOUSING_MARKET_PRICE_MEDIAN = my_stats_median(&rel_prices);
	}
    
	/* Update history and calculate index */
	int t;
	for (t=11; t>0; t--)
	{
		HOUSING_MARKET_PRICE_MEDIAN_HISTORY[t] = HOUSING_MARKET_PRICE_MEDIAN_HISTORY[t-1];
	}

	// Add current to [0]:
	HOUSING_MARKET_PRICE_MEDIAN_HISTORY[0] = HOUSING_MARKET_PRICE_MEDIAN;

	double sum=0;
	int c=0;
	for (t=0; t<12; t++)
	{
		if (HOUSING_MARKET_PRICE_MEDIAN_HISTORY[t] > 0)
		{
			sum += HOUSING_MARKET_PRICE_MEDIAN_HISTORY[t];
			c++;
		}
	}

	if (c>0)
		HOUSING_MARKET_PRICE_MOV_MED = sum/c;

	HOUSING_MARKET_PRICE_INDEX = HOUSING_MARKET_PRICE_MOV_MED;

	vlog_data("ch_hpi", 1, HOUSING_MARKET_PRICE_INDEX);

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_HOUSING)
    {
        printf("\n--------------------------------------------------------");
        printf("\n Summary:");
        printf("\n No. matches: %d" ,HOUSING_MARKET_MATCHES);
        printf("\n Mean price: %.2f", HOUSING_MARKET_PRICE_MEAN);
        printf("\n History: [");
			for (t=11; t>0; t--)
				printf("%.2f ", HOUSING_MARKET_PRICE_MEDIAN_HISTORY[t]);
		printf("]");
        printf("\n--------------------------------------------------------\n");
    }      
    #endif

	for (i=0;i<HOUSE_AUCTION.pending.bids.size;i++)
		add_real_estate_bid_data_adt(&HOUSING_MARKET_PAST_BIDS, DAY, HOUSE_AUCTION.pending.bids.array[i].price, HOUSE_AUCTION.pending.bids.array[i].min_quality, -1.0);
	for (i=0;i<HOUSE_AUCTION.pending.asks.size;i++)
		add_real_estate_ask_data_adt(&HOUSING_MARKET_PAST_ASKS, DAY, HOUSE_AUCTION.pending.asks.array[i].price, HOUSE_AUCTION.pending.asks.array[i].quality, -1.0);

	//Reset bid array and ask array
	init_bid_adt_array(&HOUSE_AUCTION.pending.bids);
	init_ask_adt_array(&HOUSE_AUCTION.pending.asks);

	//free
	free_real_estate_adt(&transaction);

	free_double_array(&rel_prices);

	// Remove too old transactions
	for(i=HOUSING_MARKET_PAST_TRANSACTIONS.size-1;i>=0;i--)
		if(DAY-HOUSING_MARKET_PAST_TRANSACTIONS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_transaction_data_adt(&HOUSING_MARKET_PAST_TRANSACTIONS, i);


	// Remove too old bids
	for(i=HOUSING_MARKET_PAST_BIDS.size-1;i>=0;i--)
		if(DAY-HOUSING_MARKET_PAST_BIDS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_bid_data_adt(&HOUSING_MARKET_PAST_BIDS, i);

	// Remove too old asks
	for(i=HOUSING_MARKET_PAST_ASKS.size-1;i>=0;i--)
		if(DAY-HOUSING_MARKET_PAST_ASKS.array[i].day>CLEARING_HOUSE_STORE_MONTHS_OF_TRANSACTIONS*20)
			remove_real_estate_ask_data_adt(&HOUSING_MARKET_PAST_ASKS, i);

	return 0;
}
