#include "../header.h"
#include "../Mall_agent_header.h"
#include "../my_library_header.h"
#include "Mall_aux_header.h"



/********************Mall agent functions*****************/

int Mall_idle()
{
	return 0;
}

/** \fn Mall_update_mall_stock()
 * \brief Malls receive the goods deliveries  
 */
int Mall_update_mall_stock()
{
	int i,j;
	
	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_MALL)
	{ 
		printf("\n\n IT %d ID: %d Mall_update_mall_stock", DAY, ID);
		printf("\n\t Mall processes update_mall_stock_message content:");
		printf("\n\t Firm_id\t quantity\t price\t previous_price");
	}
	#endif
	
	START_UPDATE_MALL_STOCK_MESSAGE_LOOP
	
	//Filter: 
	if(ID==update_mall_stock_message->mall_id)
	{
		for(j=0; j < CURRENT_STOCK.size; j++)
		{   
			
			if(update_mall_stock_message->firm_id==
				CURRENT_STOCK.array[j].firm_id)
			{
				CURRENT_STOCK.array[j].stock=CURRENT_STOCK.array[j].
				stock + update_mall_stock_message->quantity;
				
				CURRENT_STOCK.array[j].firm_id=
				update_mall_stock_message->firm_id;
				
				CURRENT_STOCK.array[j].quality=
				update_mall_stock_message->quality;
				
				CURRENT_STOCK.array[j].price=
				update_mall_stock_message->price;
				
				CURRENT_STOCK.array[j].previous_price=
				update_mall_stock_message->previous_price;
				
				
				#ifdef _DEBUG_MODE
				if (PRINT_DEBUG_MALL)
				{ 
					printf("\n\t %03d \t %f\t %f\t %f", 
						   update_mall_stock_message->firm_id, update_mall_stock_message->quantity,
							update_mall_stock_message->price, update_mall_stock_message->previous_price);
				}
				#endif
				
			}
		}
	}
	FINISH_UPDATE_MALL_STOCK_MESSAGE_LOOP
	
	/*This reads the bankruptcy message of firms in a illiquitity bankruptcy*/
	START_FIRM_BANKRUPTCY_ILLIQUIDITY_MESSAGE_LOOP
	for(i=0; i< CURRENT_STOCK.size; i++)
	{   
		
		if(firm_bankruptcy_illiquidity_message->firm_id==
			CURRENT_STOCK.array[i].firm_id)
		{
			CURRENT_STOCK.array[i].stock=0;
			
			//CURRENT_STOCK.array[i].firm_id=update_mall_stock_message->firm_id;
			
			CURRENT_STOCK.array[i].quality=0;
			
			CURRENT_STOCK.array[i].price=0;
			
			CURRENT_STOCK.array[i].previous_price=0;
		}
	}
	FINISH_FIRM_BANKRUPTCY_ILLIQUIDITY_MESSAGE_LOOP 
	
	return 0;
}

/** \fn Mall_send_quality_price_info_1()
 * \brief Malls send message with quality and price information. 
 */
int Mall_send_quality_price_info_1()
{
	
	int i;
	int available;
	
	for(i=0;i<CURRENT_STOCK.size;i++)
	{
		if(CURRENT_STOCK.array[i].stock > 0)
		{
			available= 1;
		}else
		{
			available= 0;
		}   
		
		add_quality_price_info_1_message(ID,REGION_ID,
										 CURRENT_STOCK.array[i].firm_id,         
								   CURRENT_STOCK.array[i].quality, 
								   CURRENT_STOCK.array[i].price,available);
		
	}
	
	return 0;
}


/** \fn Mall_update_mall_stocks_sales_rationing_1()
 * \brief Mall reads the consumption requests and satisfies the demand if possible (otherwise rationing). 
 */
int Mall_update_mall_stocks_sales_rationing_1()
{
	int i,j,k,l;
	double aggregated_demand;
	double rationing_rate;
	double sold_quantity_to_consumer;
	
	consumption_request_array consumption_request_list;
	init_consumption_request_array(&consumption_request_list);

	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_MALL)
	{ 
		printf("\n\n IT %d ID: %d Mall_update_mall_stocks_sales_rationing_1", DAY, ID);
		printf("\n\t Mall processes consumption_request_1_message content: ONLY NEGATIVE CONSUMPTION REQUESTS ARE SHOWN!!");
		printf("\n\t HH ID\t Firm ID\t region ID\t quantity");
	}
	#endif
	
	/* Read the consumption request message */
	START_CONSUMPTION_REQUEST_1_MESSAGE_LOOP
	
	//Filter: (a.id==m.mall_id)
	if(ID==consumption_request_1_message->mall_id)
	{
		add_consumption_request(&consumption_request_list,
								consumption_request_1_message->worker_id,
								consumption_request_1_message->region_id, 
								consumption_request_1_message->firm_id, 
								consumption_request_1_message->quantity );
	
		#ifdef _DEBUG_MODE
		if (PRINT_DEBUG_MALL)
		{ 
			if (consumption_request_1_message->quantity <0 )
			{
				printf("\n\t %02d \t %02d\t %d\t %f", 
							consumption_request_1_message->worker_id, consumption_request_1_message->firm_id,
							consumption_request_1_message->region_id, consumption_request_1_message->quantity );

				fprintf(stderr, "\n\n IT %d ID: %d Mall_update_mall_stocks_sales_rationing_1", DAY, ID);
				fprintf(stderr, "\n\t NEGATIVE CONSUMPTION REQUESTS in consumption_request_1_message!!");
				fprintf(stderr, "\n\t HH ID\t Firm ID\t region ID\t quantity");
				fprintf(stderr, "\n\t %02d \t %02d\t %d\t %f", 
							consumption_request_1_message->worker_id, consumption_request_1_message->firm_id,
							consumption_request_1_message->region_id, consumption_request_1_message->quantity );
			}
		}
		#endif
	}
	FINISH_CONSUMPTION_REQUEST_1_MESSAGE_LOOP
	
	
	/*Aggregation of demand per firm*/
	for(i = 0; i < CURRENT_STOCK.size;i++)
	{
		aggregated_demand=0.0;
		for(j = 0; j < consumption_request_list.size; j++)  
		{
			if(CURRENT_STOCK.array[i].firm_id == 
				consumption_request_list.array[j].firm_id)
			{
				aggregated_demand+= consumption_request_list.array[j].quantity;
			}
		}
		/*If aggregated demand > current stock . Rationing*/
		if(aggregated_demand> CURRENT_STOCK.array[i].stock)
		{
			
			rationing_rate= CURRENT_STOCK.array[i].stock/ aggregated_demand;
			
			
			for(k=0; k<consumption_request_list.size;k++)
			{
				if(CURRENT_STOCK.array[i].firm_id == 
					consumption_request_list.array[k].firm_id)      
				{
					sold_quantity_to_consumer  = consumption_request_list.array[k].quantity*
					rationing_rate;
					
					/*Send accepted consumption volume*/
					add_accepted_consumption_1_message(ID,
													   consumption_request_list.array[k].worker_id, 
										sold_quantity_to_consumer, 1);
					
				}
			}
			/*Calc and store revenues per firm*/
			for(k=0; k< CURRENT_STOCK.size;k++)
			{
				
				if(CURRENT_STOCK.array[i].firm_id==
					FIRM_REVENUES.array[k].firm_id)
				{
					FIRM_REVENUES.array[k].sales = CURRENT_STOCK.array[i]
					.stock*CURRENT_STOCK.array[i].price;
					
					/*mall stock completely sold out*/
					CURRENT_STOCK.array[i].stock=0.0;
				}
			}
		}
		else /*Otherwise no rationing*/
		{
			
			for(k=0; k<consumption_request_list.size;k++)
			{
				if(CURRENT_STOCK.array[i].firm_id == 
					consumption_request_list.array[k].firm_id)
				{   /*Send accepted consumption volume*/
					
					sold_quantity_to_consumer  = consumption_request_list.array[k].quantity;
					
					add_accepted_consumption_1_message(ID,
													   consumption_request_list.array[k].worker_id,
										consumption_request_list.array[k].quantity, 0);
				}   
			}
			
			for(l=0; l< CURRENT_STOCK.size;l++)
			{
				/*Calc and store revenues per firm*/
				if(CURRENT_STOCK.array[i].firm_id==
					FIRM_REVENUES.array[l].firm_id)
				{
					FIRM_REVENUES.array[l].sales = 
					aggregated_demand *CURRENT_STOCK.array[i].price ;
					/*remaining mall stock*/
					CURRENT_STOCK.array[i].stock-=aggregated_demand;
					
				}
			}
		}
	}
	
	free_consumption_request_array(&consumption_request_list);
	
	/*Send second price info*/
	
	int available;
	for(i=0;i<CURRENT_STOCK.size;i++)
	{
		
		if(CURRENT_STOCK.array[i].stock > 0)
		{
			available= 1;
		}else
		{
			available= 0;
		}
		
		add_quality_price_info_2_message(ID,REGION_ID,
										 CURRENT_STOCK.array[i].firm_id,         
										CURRENT_STOCK.array[i].quality, 
										CURRENT_STOCK.array[i].price,available);
	}
	return 0;
}


/** \fn Mall_update_mall_stocks_sales_rationing_2()
 * \brief After the second request round the mall satisfies the demand if possible, otherwise rationing.
 */
int Mall_update_mall_stocks_sales_rationing_2()
{
	int i,j,k,l;
	double aggregated_demand;
	double rationing_rate;
	double sold_quantity_to_consumer;
	
	consumption_request_array consumption_request_list;
	init_consumption_request_array(&consumption_request_list);
	
	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_MALL)
	{ 
		printf("\n\n IT %d ID: %d Mall_update_mall_stocks_sales_rationing_2", DAY, ID);
		printf("\n\t Mall processes consumption_request_2_message content: ONLY NEGATIVE CONSUMPTION REQUESTS ARE SHOWN!!");
		printf("\n\t HH ID\t Firm ID\t region ID\t quantity");
	}
	#endif
	
	/*Read the request*/
	START_CONSUMPTION_REQUEST_2_MESSAGE_LOOP
	//Filter: (a.id==m.mall_id)
	if(ID==consumption_request_2_message->mall_id)
	{
		add_consumption_request(&consumption_request_list,
								consumption_request_2_message->worker_id, 
								consumption_request_2_message->region_id, 
								consumption_request_2_message->firm_id, 
								consumption_request_2_message->quantity );
		
		#ifdef _DEBUG_MODE
		if (PRINT_DEBUG_MALL)
		{
			if (consumption_request_2_message->quantity <0 )
			{
				printf("\n\t %02d \t %02d\t %d\t %f", 
							consumption_request_2_message->worker_id, consumption_request_2_message->firm_id,
							consumption_request_2_message->region_id, consumption_request_2_message->quantity );
				
				fprintf(stderr, "\n\n IT %d ID: %d Mall_update_mall_stocks_sales_rationing_2", DAY, ID);
				fprintf(stderr, "\n\t NEGATIVE CONSUMPTION REQUESTS in consumption_request_2_message!!");
				fprintf(stderr, "\n\t HH ID\t Firm ID\t region ID\t quantity");
				fprintf(stderr, "\n\t %02d \t %02d\t %d\t %f", 
							consumption_request_2_message->worker_id, consumption_request_2_message->firm_id,
							consumption_request_2_message->region_id, consumption_request_2_message->quantity );
			}
		}
		#endif
		
	}
	FINISH_CONSUMPTION_REQUEST_2_MESSAGE_LOOP
	
	/*Aggregation of demand*/
	for(i = 0; i < CURRENT_STOCK.size;i++)
	{
		aggregated_demand=0;
		for(j = 0; j < consumption_request_list.size; j++)
		{
			if(CURRENT_STOCK.array[i].firm_id == 
				consumption_request_list.array[j].firm_id)
			{
				aggregated_demand+= consumption_request_list.array[j].quantity;
			}
		}
		/*If agg demand > mall stocks . Rationing*/
		if(aggregated_demand > CURRENT_STOCK.array[i].stock)
		{
			
			
			rationing_rate= CURRENT_STOCK.array[i].stock/ aggregated_demand;
			
			for(k=0; k<consumption_request_list.size;k++)
			{
				if(CURRENT_STOCK.array[i].firm_id == 
					consumption_request_list.array[k].firm_id)
				{
					
					sold_quantity_to_consumer  = consumption_request_list.array[k].quantity*
					rationing_rate;
					
					add_accepted_consumption_2_message(ID,
													   consumption_request_list.array[k].worker_id,
										sold_quantity_to_consumer, 1);
				}
			}
			/*Revenues and final mall stock*/
			for(k=0; k< CURRENT_STOCK.size;k++)
			{
				if(CURRENT_STOCK.array[i].firm_id==
					FIRM_REVENUES.array[k].firm_id)
				{
					FIRM_REVENUES.array[k].sales += CURRENT_STOCK.array[i]
					.stock*CURRENT_STOCK.array[i].price;
					
					CURRENT_STOCK.array[i].stock=0; 
				}
			}
		}
		else /*Otherwise no rationing*/
		{
			
			for(k=0; k<consumption_request_list.size;k++)
			{
				if(CURRENT_STOCK.array[i].firm_id == 
					consumption_request_list.array[k].firm_id)
				{
					sold_quantity_to_consumer  = consumption_request_list.array[k].quantity;
					
					add_accepted_consumption_2_message(ID,
													   consumption_request_list.array[k].worker_id, 
										consumption_request_list.array[k].quantity, 0);
				}   
			}
			/*revenues and final stocks*/
			for(l=0; l< CURRENT_STOCK.size;l++)
			{
				
				if(CURRENT_STOCK.array[i].firm_id==
					FIRM_REVENUES.array[l].firm_id)
				{
					
					FIRM_REVENUES.array[l].sales += aggregated_demand *
					CURRENT_STOCK.array[i].price;
					
					CURRENT_STOCK.array[i].stock-=aggregated_demand;
				}
			}
		}
	}
	free_consumption_request_array(&consumption_request_list);
	
	return 0;
}

/** \fn Mall_pay_firm()
 * \brief Mall transfers the revenues to the firm
 */
int Mall_pay_firm()
{
	int i,j;
	
	TOTAL_SUPPLY=0;
	int stock_empty;
	
	for(i=0; i<CURRENT_STOCK.size;i++)
	{
		TOTAL_SUPPLY += CURRENT_STOCK.array[i].stock;
	}
	
	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_MALL)
	{ 
		printf("\n\n IT %d ID: %d Mall_pay_firm", DAY, ID);
		printf("\n\t Mall sales_message content:");
		printf("\n\t Firm_id \t sales \t stock_empty \t stock");
	}
	#endif
	
	
	for(i=0; i< FIRM_REVENUES.size;i++)
	{
		
		
		for(j=0; j<CURRENT_STOCK.size;j++)
		{
			
			if(FIRM_REVENUES.array[i].firm_id == CURRENT_STOCK.array[j].firm_id)
			{
				
				if(CURRENT_STOCK.array[j].stock ==0)
				{
					stock_empty =1;
				}else
				{
					stock_empty =0;
				}
				
				add_sales_message(ID, FIRM_REVENUES.array[i].firm_id,
								  FIRM_REVENUES.array[i].sales, stock_empty, CURRENT_STOCK.array[j].stock);
				
				
				#ifdef _DEBUG_MODE
				if (PRINT_DEBUG_MALL)
				{ 
					printf("\n\t %03d \t %f \t %d \t %f",
						   FIRM_REVENUES.array[i].firm_id, FIRM_REVENUES.array[i].sales, stock_empty, CURRENT_STOCK.array[j].stock);
				}
				#endif
			}
		}
	}
	
	return 0;
}

/* \fn: int Mall_read_insolvency_bankruptcy()
 * \brief: Function to read insolvency_bankruptcy messages from firms in insolvency. The mall has to set the current mall stocks equal 0.
 */
int Mall_read_insolvency_bankruptcy()
{
	int i;
	
	START_FIRM_BANKRUPTCY_INSOLVENCY_MESSAGE_LOOP
	for(i=0; i< CURRENT_STOCK.size; i++)
	{   
		
		if(firm_bankruptcy_insolvency_message->firm_id==
			CURRENT_STOCK.array[i].firm_id)
		{
			CURRENT_STOCK.array[i].stock=0;
			
			//CURRENT_STOCK.array[i].firm_id=update_mall_stock_message->firm_id;
			
			CURRENT_STOCK.array[i].quality=0;
			
			CURRENT_STOCK.array[i].price=0;
			
			CURRENT_STOCK.array[i].previous_price=0;
		}
	}
	
	FINISH_FIRM_BANKRUPTCY_INSOLVENCY_MESSAGE_LOOP 
	
	return 0;
}
