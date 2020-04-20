#include "../header.h"
#include "../NSA_agent_header.h"
#include "../my_library_header.h"
#include "NSA_aux_header.h"


/*
 * \fn NSA_agent_empty_networks()
 * \brief 
 */
int NSA_agent_empty_networks()
{
	
	//Cleanup	
	//Clear node_adt_arrays
	reset_node_adt_array( &NETWORK_BANKS );
	
	//Cleanup
	//Clear node_adt_arrays
	reset_node_adt_array( &NETWORK_FIRMS );
	
	//Cleanup	
	//Clear node_adt_arrays
	reset_node_adt_array( &NETWORK );

	return 0;	
}

/*
 * \fn NSA_agent_cleanup_data()
 * \brief 
 */
int NSA_agent_cleanup_data()
{
	int i;
	
	//Cleanup	
	for(i=0; i<NETWORK_BANKS.size; i++)
	{
		//Clear debt_item_arrays
		reset_debt_item_array( &(NETWORK_BANKS.array[i].loan_portfolio) );
	}
	
	//Cleanup
	for(i=0; i<NETWORK_FIRMS.size; i++)
	{
		//Clear debt_item_arrays
		reset_debt_item_array( &(NETWORK_FIRMS.array[i].loan_portfolio) );
	}
	
	return 0;	
}

/*
 * \fn NSA_agent_init_networks()
 * \brief 
 */
int NSA_agent_init_networks()
{
	int i, id, found;
	
	// Datatypes for temporary network items
	financial_data_adt temp_financial_data;
	init_financial_data_adt(&temp_financial_data);
	
  	debt_item_array temp_loan_portfolio;
	init_debt_item_array(&temp_loan_portfolio);

	int_array temp_friend_id_list;
	init_int_array(&temp_friend_id_list);
	

	// 1 BANK_ID
	// 2 Reconstruct entire network (possibly not needed, but defensive programming: if someone creates new banks, firms)
	//XML: Msgs are ordered on bank_id ascending (only need first msg)

	START_BANK_LOAN_PORTFOLIO_MESSAGE_LOOP
		// Set bank ids in network
		id = bank_loan_portfolio_message->id;
			
		// Check if id already used: if not, generate new item
		found=0;
		for (i=0; i<NETWORK_BANKS.size; i++)
		{
			//printf("\n\n For i=%d of %d\n", i, NETWORK_BANKS.size);
			
			if(NETWORK_BANKS.array[i].id == id)
			{
				found=1; break;
			}
		}
		
		if(!found)
		{
			//printf("\n\n Generating new network item.\n");
			
			// Set network data: create new item, set id		  
			add_node_adt(&NETWORK_BANKS, id, &temp_financial_data, &temp_loan_portfolio, &temp_friend_id_list, 'b');
		}
	FINISH_BANK_LOAN_PORTFOLIO_MESSAGE_LOOP
			
	//2. FIRM_ID
	//XML: Msgs are ordered on firm_id, ascending (only need first msg)
	START_FIRM_LOAN_PORTFOLIO_MESSAGE_LOOP
		// Set firm ids in network
		id = firm_loan_portfolio_message->id;

		// Check if id already used: if not, generate new item
		found=0;
		for (i=0; i<NETWORK_FIRMS.size; i++)
		{
			//printf("\n\n For i=%d of %d\n", i, NETWORK_FIRMS.size);
			if(NETWORK_FIRMS.array[i].id == id)
			{
				found=1; break;
			}
		}
		
		if(!found)
		{
			//printf("\n\n Generating new network item.\n");
			
			// Set network data: create new item, set id		  
			add_node_adt(&NETWORK_FIRMS, id, &temp_financial_data, &temp_loan_portfolio, &temp_friend_id_list, 'f');
		}
	FINISH_FIRM_LOAN_PORTFOLIO_MESSAGE_LOOP
	
	//2. FIRM_ID
	//XML: Msgs are ordered on id, ascending (only need first msg)
	START_HH_LOAN_PORTFOLIO_MESSAGE_LOOP
		// Set firm ids in network
		id = hh_loan_portfolio_message->id;

		// Check if id already used: if not, generate new item
		found=0;
		for (i=0; i<NETWORK_HHS.size; i++)
		{
			//printf("\n\n For i=%d of %d\n", i, NETWORK_FIRMS.size);
			if(NETWORK_HHS.array[i].id == id)
			{
				found=1; break;
			}
		}
		
		if(!found)
		{
			//printf("\n\n Generating new network item.\n");
			
			// Set network data: create new item, set id		  
			add_node_adt(&NETWORK_HHS, id, &temp_financial_data, &temp_loan_portfolio, &temp_friend_id_list, 'h');
		}
	FINISH_HH_LOAN_PORTFOLIO_MESSAGE_LOOP	
					
	// free temp adts
	free_financial_data_adt(&temp_financial_data);
	free_debt_item_array(&temp_loan_portfolio);
	free_int_array(&temp_friend_id_list);

	return 0;	
}

/*
 * \fn NSA_agent_read_messages()
 * \brief 
 */
int NSA_agent_read_messages()
{
	int i, id, found;

	START_BANK_LOAN_PORTFOLIO_MESSAGE_LOOP

		// Get the index
		id = bank_loan_portfolio_message->id;
		
		// Search id in network
		found=0;
		for (i=0; i<NETWORK_BANKS.size; i++)
		{
			//printf("\n\n Searching Bank ID %d For i=%d of %d\n", id, i, NETWORK_BANKS.size);
			if(NETWORK_BANKS.array[i].id == id)
			{
				found=1; 
				
				//Dynamic code: add the new financial_data
				/*
				NETWORK_BANKS.array[i].financial_data.equity 		= bank_loan_portfolio_message->financial_data.equity;
				NETWORK_BANKS.array[i].financial_data.total_assets 	= bank_loan_portfolio_message->financial_data.total_assets;
				NETWORK_BANKS.array[i].financial_data.total_debt 	= bank_loan_portfolio_message->financial_data.total_debt;
				NETWORK_BANKS.array[i].financial_data.equity_asset_ratio= bank_loan_portfolio_message->financial_data.equity_asset_ratio;
				NETWORK_BANKS.array[i].financial_data.debt_equity_ratio = bank_loan_portfolio_message->financial_data.debt_equity_ratio;
				*/				
				
				//copy directly
				copy_financial_data_adt(&(NETWORK_BANKS.array[i].financial_data), &(bank_loan_portfolio_message->financial_data));
				
				break;
			}
		}

		// Code should never reach this point:
		if (!found)
			printf("\n\n NSA_agent_read_messages(): Bank ID=%d not found!\n", id);

	FINISH_BANK_LOAN_PORTFOLIO_MESSAGE_LOOP

	START_FIRM_LOAN_PORTFOLIO_MESSAGE_LOOP

		// Get the index
		id = firm_loan_portfolio_message->id;
	
		// Search id in network
		found=0;
		for (i=0; i<NETWORK_FIRMS.size; i++)
		{
			//printf("\n\n Searching Firm ID %d For i=%d of %d\n", id, i, NETWORK_FIRMS.size);
			if(NETWORK_FIRMS.array[i].id == id)
			{
				found=1; 
				
				//Dynamic code: add new loan_adt in loan_portfolio at the end of the array
				add_debt_item( &(NETWORK_FIRMS.array[i].loan_portfolio), 
					firm_loan_portfolio_message->loan_adt.bank_id,
					firm_loan_portfolio_message->loan_adt.loan_id,
					firm_loan_portfolio_message->loan_adt.loan_value,
					firm_loan_portfolio_message->loan_adt.interest_rate,
					firm_loan_portfolio_message->loan_adt.installment_amount,
					firm_loan_portfolio_message->loan_adt.bad_debt,
					firm_loan_portfolio_message->loan_adt.nr_periods_before_repayment );
				/*
				NETWORK_FIRMS.array[i].financial_data.equity 		= firm_loan_portfolio_message->financial_data.equity;
				NETWORK_FIRMS.array[i].financial_data.total_assets 	= firm_loan_portfolio_message->financial_data.total_assets;
				NETWORK_FIRMS.array[i].financial_data.total_debt 	= firm_loan_portfolio_message->financial_data.total_debt;
				NETWORK_FIRMS.array[i].financial_data.equity_asset_ratio= firm_loan_portfolio_message->financial_data.equity_asset_ratio;
				NETWORK_FIRMS.array[i].financial_data.debt_equity_ratio = firm_loan_portfolio_message->financial_data.debt_equity_ratio;
				*/
				
				//copy directly
				copy_financial_data_adt(&(NETWORK_FIRMS.array[i].financial_data), &(firm_loan_portfolio_message->financial_data));

				
				// Add bank as friend in list (multiple entries: make list unique later, or leave as weights)
				add_int(&NETWORK_FIRMS.array[i].friend_id_list, firm_loan_portfolio_message->loan_adt.bank_id);
								
				//break;
			}
		}

		// Code should never reach this point:
		if (!found)
			printf("\n\n NSA_agent_read_messages(): Firm ID=%d not found!\n", id);
		
	FINISH_FIRM_LOAN_PORTFOLIO_MESSAGE_LOOP


	START_HH_LOAN_PORTFOLIO_MESSAGE_LOOP

		// Get the index
		id = hh_loan_portfolio_message->id;
	
		// Search id in network
		found=0;
		for (i=0; i<NETWORK_HHS.size; i++)
		{
			//printf("\n\n Searching Household ID %d For i=%d of %d\n", id, i, NETWORK_HHS.size);
			if(NETWORK_HHS.array[i].id == id)
			{
				found=1; 
				
				//Dynamic code: add new loan_adt in loan_portfolio at the end of the array
				add_debt_item( &(NETWORK_HHS.array[i].loan_portfolio), 
					hh_loan_portfolio_message->loan_adt.bank_id,
					hh_loan_portfolio_message->loan_adt.loan_id,
					hh_loan_portfolio_message->loan_adt.loan_value,
					hh_loan_portfolio_message->loan_adt.interest_rate,
					hh_loan_portfolio_message->loan_adt.installment_amount,
					hh_loan_portfolio_message->loan_adt.bad_debt,
					hh_loan_portfolio_message->loan_adt.nr_periods_before_repayment );
				/*
				NETWORK_HHS.array[i].financial_data.equity 		= hh_loan_portfolio_message->financial_data.equity;
				NETWORK_HHS.array[i].financial_data.total_assets 	= hh_loan_portfolio_message->financial_data.total_assets;
				NETWORK_HHS.array[i].financial_data.total_debt 	= hh_loan_portfolio_message->financial_data.total_debt;
				NETWORK_HHS.array[i].financial_data.equity_asset_ratio= hh_loan_portfolio_message->financial_data.equity_asset_ratio;
				NETWORK_HHS.array[i].financial_data.debt_equity_ratio = hh_loan_portfolio_message->financial_data.debt_equity_ratio;
				*/
				
				//copy directly
				copy_financial_data_adt(&(NETWORK_HHS.array[i].financial_data), &(hh_loan_portfolio_message->financial_data));

				
				// Add bank as friend in list (multiple entries: make list unique later, or leave as weights)
				add_int(&NETWORK_HHS.array[i].friend_id_list, hh_loan_portfolio_message->loan_adt.bank_id);
								
				//break;
			}
		}

		// Code should never reach this point:
		if (!found)
			printf("\n\n NSA_agent_read_messages(): Firm ID=%d not found!\n", id);
		
	FINISH_HH_LOAN_PORTFOLIO_MESSAGE_LOOP
	
    return 0;	
}

/*
 * \fn NSA_agent_construct_bank_network()
 * \brief Function to initialize the loan_portfolios in the bank network.
 * From pre-existing debt_items in the firm loan_portfolio's, add the debt_items to the banks' loan_portfolio's.
 */
int NSA_agent_construct_bank_network()
{

    #ifdef _DEBUG_MODE    
    if (PRINT_DEBUG_NSA)
    {
	printf("\n\nStart: NSA_Credit_Functions.c, NSA_agent_construct_bank_network()\n");
    }
    #endif

	
	int i,j,k;
	int bank_id=0;
	
	// 14.3.2014: Banks do not YET have a portfolio of LOANS: not implemented yet
	// Construct Bank loan portfolio from Firm data
	// Cycle through Firm network, and copy all debt_items from firm network into bank network that correspond to the correct bank_id
	for(i=0; i<NETWORK_FIRMS.size; i++)
	{
		
		//printf("\n\n For Firm i=%d of NETWORK_FIRMS.size %d\n", i+1, NETWORK_FIRMS.size);
		
		for(j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
		{
			//printf("\n\n For debt_item j=%d of loan_portfolio.size %d\n", j+1, NETWORK_FIRMS.array[i].loan_portfolio.size);
			
			// Check bank_id
			bank_id = NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id;

			// Search bank_id in network
			for (k=0; k<NETWORK_BANKS.size; k++)
			{
				//printf("\n\n For Bank k=%d of NETWORK_BANKS.size %d: Searching Bank ID %d \n", k+1, NETWORK_BANKS.size, bank_id);
				if(NETWORK_BANKS.array[k].id == bank_id)
				{
					//printf("\n Found bank ID %d:\n", NETWORK_BANKS.array[k].bank_id);
					
					add_debt_item( &(NETWORK_BANKS.array[k].loan_portfolio),
							  NETWORK_FIRMS.array[i].id, // instead of bank_id, store firm id
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_id,
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value,
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].interest_rate,
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].installment_amount,
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].bad_debt,
							  NETWORK_FIRMS.array[i].loan_portfolio.array[j].nr_periods_before_repayment );
										
					// Add firm as friend in list (multiple entries: make list unique later, or leave as weights)
					add_int(&NETWORK_BANKS.array[k].friend_id_list, NETWORK_FIRMS.array[i].id);


					//int last_index = NETWORK_BANKS.array[k].friend_id_list.size-1;
					//printf("\n\t Added debt_item to loan portfolio\n");
					//printf("\n\t Added Firm %d to friend_list of Bank %d: size=%d, entry=%d\n", NETWORK_FIRMS.array[i].id, NETWORK_BANKS.array[k].bank_id, NETWORK_BANKS.array[k].friend_id_list.size, NETWORK_BANKS.array[k].friend_id_list.array[last_index]);

					break;
				}
			}
		}
	}


	TOTAL_VALUE_LOANS_BANKS=0;
	
	for(i=0; i<NETWORK_BANKS.size; i++)
		for(j=0; j<NETWORK_BANKS.array[i].loan_portfolio.size; j++)
		{
			TOTAL_VALUE_LOANS_BANKS += NETWORK_BANKS.array[i].loan_portfolio.array[j].loan_value;
		}

	// Compute total loan value in firms
	TOTAL_VALUE_LOANS_FIRMS=0;
	for(i=0; i<NETWORK_FIRMS.size; i++)
		for(j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
		{
			TOTAL_VALUE_LOANS_FIRMS += NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value;
		}

		
	// Check consistency:
	if (fabs(TOTAL_VALUE_LOANS_BANKS - TOTAL_VALUE_LOANS_FIRMS)>1.0)
		printf("\n\n Consistency check: TOTAL_VALUE_LOANS_BANKS %2.2f TOTAL_VALUE_LOANS_FIRMS %2.2f DIFF %2.2f\n", TOTAL_VALUE_LOANS_BANKS, TOTAL_VALUE_LOANS_FIRMS, fabs(TOTAL_VALUE_LOANS_BANKS - TOTAL_VALUE_LOANS_FIRMS) );
	
    return 0;	
}

/*
 * \fn NSA_agent_compute_adjacency_matrix()
 * \brief 
 */
int NSA_agent_compute_adjacency_matrix()
{
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: NSA_Credit_Functions.c, NSA_agent_compute_adjacency_matrix()\n");
	}
	#endif

	
	int i,j,k;
	int bank_id;

	/* 
		DEBT_ITEM: loan_adt
		bank_id
		loan_value
		interest_rate
		installment_amount
		rea_per_installment
		loan_rea
		bad_debt
		nr_periods_before_repayment
	*/

	// Adjacency Matrix of Firm-Bank network.
	// Let A be NxM: A[n][m]
	// N: FLAME_environment_variable_NO_FIRMS
	// M: FLAME_environment_variable_NO_BANKS

	// 1. 	NUMBER of Loans of each firm with each bank
	// A[i][k]: firm i, bank k, A[i][k]= total number of loans of firm i with bank k
	// For all firms, check all debts: Set A[i,k]++ where k= index in BANK NETWORK
	//int A[FLAME_environment_variable_no_firms][FLAME_environment_variable_no_banks];
	//int A[NO_FIRMS][NO_BANKS];
	int A[ROWS][COLS];
	
	
	// 2. VALUE of Loans of each firm with each bank
	// B[i][k]: firm i, bank k, B[i][k]= total value of all loans of firm i with bank k
	// For all firms, check all debts: Set B[i,k] += loan_value where k = index in BANK NETWORK
	//double B[FLAME_environment_variable_no_firms][FLAME_environment_variable_no_banks];
	//double B[NO_FIRMS][NO_BANKS];
	double B[ROWS][COLS];
	
	for(i=0;i<NETWORK_FIRMS.size;i++)
		for(j=0;j<NETWORK_FIRMS.array[i].loan_portfolio.size;j++)
		{
			bank_id = NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id;
			
			// Search index of bank_id in network: k is the bank's index
			for (k=0; k<NETWORK_BANKS.size; k++)
			{
				//printf("\n\n Searching Bank ID %d For k=%d of %d\n", bank_id, k, NETWORK_BANKS.size);
				if(NETWORK_BANKS.array[k].id == bank_id)
					break;
			}
			//printf("\n\n After for-loop:  k=%d\n", k);
			
			//NUMBER of Loans of each firm with each bank
			A[i][k]++;

			//Value of Loans of each firm with each bank
			B[i][k] += NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value;
			
			if (abs(B[i][k])>1e+9) fprintf(stderr, "\n\nERROR in NSA_agent_compute_adjacency_matrix(): B[%d][%d]>1e+9 (%f) \n", i, k, B[i][k]);
		}

		
	// Print adjacency_matrix_DAY to a file: 
	//print_adjacency_matrix_data(A, B, NETWORK_FIRMS.size, NETWORK_BANKS.size);
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  print_adjacency_matrix_data(A, B, ROWS, COLS);
	}
	#endif

/*
#ifdef _DEBUG_MODE    
if (PRINT_DEBUG_NSA)
{

// DEBUG ======================================
i=11;k=16;
	printf("\n i=%d\t k=%d\n", i,k);
	printf("\nA[%d][%d]=%d\tB[%d][%d]=%f\n", i,k,i,k, A[i][k], B[i][k]);

//which firm? which bank? firm_id=79, bank_id = 1704
	printf("\n which firm?: NETWORK_FIRMS.array[%d].id =%d\n", i, NETWORK_FIRMS.array[i].id);
	printf("\n which bank?: NETWORK_BANKS.array[%d].bank_id=%d\n",  k, NETWORK_BANKS.array[k].bank_id);
			

//value of loan is huge
// search j such that: bank_id of firm's debt == bank_id in bank network at node k:
for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
	if ( (NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id) == (NETWORK_BANKS.array[k].bank_id) )
	{
		printf("\n Max_j=%d, Break at j=%d\tbank_id=%d\n", NETWORK_FIRMS.array[i].loan_portfolio.size, j, NETWORK_BANKS.array[k].bank_id);
		printf("\n B[%d][%d]=%f\tLoan value in Network: %f\n", i,k, B[i][k], NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value);
		printf("\n which bank?: NETWORK_FIRMS.array[%d].loan_portfolio.array[%d].bank_id =%d\n", i, j, NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id);

		//break;
	}



//which firm in bank loan portfolio? which bank? 
// search j such that: firm_id of bank's loan == firm_id in firm network at node i:
for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
	// NOTE: In debt_item (NETWORK_BANKS.array[k].loan_portfolio.array[j].firm_id) is called .bank_id
	if ( (NETWORK_BANKS.array[k].loan_portfolio.array[j].bank_id) == (NETWORK_FIRMS.array[i].id) )
	{
	  printf("\n Max_j=%d, Break at j=%d\n", NETWORK_BANKS.array[k].loan_portfolio.size, j);
	  printf("\n which firm?: NETWORK_BANKS.array[%d].loan_portfolio.array[%d].firm_id =%d\n", k, j, NETWORK_BANKS.array[k].loan_portfolio.array[j].bank_id);

	  //break;
	}
}
#endif
*/
	
/*
//firm_id=49, bank_id =1689
i=16;k=0;
			printf("\n%d\t%f\n", A[i][k], B[i][k]);

			
//value of loan is -nan
// search j such that: bank_id of firm's debt == bank_id in bank network at node k:
for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
	if (NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id==NETWORK_BANKS.array[k].bank_id)
	{
		break;
	}
	printf("\n Max_j=%d, Break at j=%d\tbank_id=%d\n", NETWORK_FIRMS.array[i].loan_portfolio.size, j, NETWORK_BANKS.array[k].bank_id);
	printf("\nB[%d][%d]=%f\t%f\n", i,k, B[i][k], NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value);
	printf("\n which bank?: NETWORK_FIRMS.array[%d].loan_portfolio.array[%d].bank_id =%d\n", i, j, NETWORK_FIRMS.array[i].loan_portfolio.array[j].bank_id);


//which firm in bank loan portfolio? which bank? 
// search j such that: firm_id of bank's loan == firm_id in firm network at node i:
for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++)
	// NOTE: In debt_item (NETWORK_BANKS.array[k].loan_portfolio.array[j].firm_id) is called .bank_id
	if ( (NETWORK_BANKS.array[k].loan_portfolio.array[j].bank_id) == (NETWORK_FIRMS.array[i].id) )
	{
		break;
	}
	printf("\n Max_j=%d, Break at j=%d\n", NETWORK_BANKS.array[k].loan_portfolio.size, j);
	printf("\n which firm?: NETWORK_BANKS.array[%d].loan_portfolio.array[%d].firm_id =%d\n", k, j, NETWORK_BANKS.array[k].loan_portfolio.array[j].bank_id);
*/
// END OF DEBUG ======================================


/*
	FILE *file1;
	char *filename;

	filename = malloc(40*sizeof(char));
	filename[0]=0;
	
	printf("Opening file: adj_matrix_%d.csv", DAY);

	sprintf(filename, "adj_matrix_%d.csv", DAY);
		 
	file1 = fopen(filename,"w");

	for(i = 0; i < ROWS; i++)
	{
		for(k = 0; k < COLS; k++)
		{
			fprintf(file1,"%d\t", A[i][k]);
			
			//Pointer equivalent: *A is pointer to first element *A[0], so *(A+i) points to A[i] and for A[i][k] we need:
//			fprintf(file1,"%d\t", (*(*(A+i) + k)));
		}
		fprintf(file1,"\n");
	}

	fprintf(file1,"\n");
	for(i = 0; i < ROWS; i++)
	{
		for(k = 0; k < COLS; k++)
		{
			fprintf(file1,"%.2f\t", B[i][k]);

			//Pointer equivalent:
//			fprintf(file1,"%.2f\t", (*(*(B+i) + k)));
		}
		fprintf(file1,"\n");
	}

	fclose(file1);
	free(filename);
*/

    return 0;	
}

/*
 * \fn NSA_agent_compute_aggregates()
 * \brief 
 */
int NSA_agent_compute_aggregates()
{
  	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: NSA_agent_compute_aggregates()\n");
	}
	#endif

	int i,j;
	double sum;

	/* Aggregates for banks: */

	//  Count no. of total loans for single banks:
	//	NETWORK_BANKS.array[0].loan_portfolio.size

	//  Total no. of loans at banks:  
		sum=0;
		for (i=0;i<NETWORK_BANKS.size;i++) 
			sum += NETWORK_BANKS.array[i].loan_portfolio.size;
		TOTAL_NO_LOANS_BANKS = sum;
			   
	//  count size of total loans:
		sum=0;
		for (i=0; i<NETWORK_BANKS.size; i++) 
			for (j=0; j<NETWORK_BANKS.array[i].loan_portfolio.size; j++) 
				sum += NETWORK_BANKS.array[i].loan_portfolio.array[j].loan_value;
		TOTAL_VALUE_LOANS_BANKS = sum;

	/* Aggregates for firms: */

	//  Count no. of total loans for single firms:
	//  Total no. of loans at firms:  
		sum=0;
		for (i=0;i<NETWORK_FIRMS.size;i++) 
			sum += NETWORK_FIRMS.array[i].loan_portfolio.size;
		TOTAL_NO_LOANS_FIRMS = sum;

	//  Count size of total loans at firms:
		sum=0.0;
		for (i=0; i<NETWORK_FIRMS.size; i++) 
			for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++) 
				sum += NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value;
		TOTAL_VALUE_LOANS_FIRMS = sum;
		
		#ifdef _DEBUG_MODE    
		if (PRINT_DEBUG_NSA)
		{
			printf("\n\n IT %d, NSA Agent: TOTAL_NO_LOANS_BANKS=%d TOTAL_NO_LOANS_FIRMS=%d\n",
			       DAY, TOTAL_NO_LOANS_BANKS, TOTAL_NO_LOANS_FIRMS);
			printf("\n\n IT %d, NSA Agent: TOTAL_VALUE_LOANS_BANKS=%8.2f TOTAL_VALUE_LOANS_FIRMS=%8.2f DIFF=%8.2f\n",
			       DAY, TOTAL_VALUE_LOANS_BANKS, TOTAL_VALUE_LOANS_FIRMS, TOTAL_VALUE_LOANS_BANKS - TOTAL_VALUE_LOANS_FIRMS);
		}
		#endif
		
	// Set distributions
	// FIRM_LOAN_NO_DIST   : Firm Loans (number) distribution - number of loans in single firms
	// FIRM_LOAN_VALUE_DIST : Firm Loans (value) distribution - value of loans in single firms
	
	reset_int_array(&FIRM_LOAN_NO_DIST);
	for (i=0; i<NETWORK_FIRMS.size; i++)
	  add_int(&FIRM_LOAN_NO_DIST, NETWORK_FIRMS.array[i].loan_portfolio.size);

	reset_double_array(&FIRM_LOAN_VALUE_DIST);
	for (i=0; i<NETWORK_FIRMS.size; i++) 
	  for (j=0; j<NETWORK_FIRMS.array[i].loan_portfolio.size; j++) 
	    add_double(&FIRM_LOAN_VALUE_DIST, NETWORK_FIRMS.array[i].loan_portfolio.array[j].loan_value);
		
		
		
	/* Consistency checks */
	//Check consistency no. of loans
		if (TOTAL_NO_LOANS_BANKS != TOTAL_NO_LOANS_FIRMS)
			printf("\n\n IT %d, NSA Agent: Consistency error: TOTAL_NO_LOANS_BANKS=%d TOTAL_NO_LOANS_FIRMS=%d\n",
			  DAY, TOTAL_NO_LOANS_BANKS, TOTAL_NO_LOANS_FIRMS);

	//Check consistency size of loans
		if (fabs(TOTAL_VALUE_LOANS_BANKS - TOTAL_VALUE_LOANS_FIRMS)>1.0)
			printf("\n\n IT %d, NSA Agent: Consistency error: TOTAL_VALUE_LOANS_BANKS=%8.2f TOTAL_VALUE_LOANS_FIRMS=%8.2f\n",
			  DAY, TOTAL_VALUE_LOANS_BANKS, TOTAL_VALUE_LOANS_FIRMS);

    return 0;	
}

/*
 * \fn NSA_agent_compute_network_statistics()
 * \brief 
 */
int NSA_agent_compute_network_statistics()
{
    	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: NSA_agent_compute_network_statistics()\n");
	}
	#endif

	//Function: Initialize network structure with node types 'f' and 'b'
	initialize_network();
	
	//Function: Check the consistency of the network structur
	network_structure_consistency_check();
	
	//Write data to .cxf-File for graphical Network Analysis
	// Write-out is done at end of the month: 0, 20, ...
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  if(DAY%MONTH == 0)
		  cuttlefish_data();
	}
	#endif
	
	
	return 0;
}
