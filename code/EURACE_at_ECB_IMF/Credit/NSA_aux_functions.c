#include "../header.h"
#include "../my_library_header.h"
#include "../NSA_agent_header.h"
#include "NSA_aux_header.h"

//Network agent initializes the Network array with initial data
void initialize_network(void)
{
    int i,j,k;

    // Prepare empty items
    int tmp_id=0;
    
    financial_data_adt tmp_financial_data;
    init_financial_data_adt(&tmp_financial_data);
    
    debt_item_array tmp_loan_portfolio;
    init_debt_item_array(&tmp_loan_portfolio);
    
    int_array tmp_friend_id_list;
    init_int_array(&tmp_friend_id_list);

    //Empty the network
    reset_node_adt_array(&NETWORK);
    
    //Network Agent receives data from nodes in order to initialize the Network
  	for (i=0; i<NETWORK_FIRMS.size; i++)
	{
/*
	  //simple copy
	  add_node_adt(&NETWORK,
			    NETWORK_FIRMS.array[i].id,
			    &(NETWORK_FIRMS.array[i].financial_data),
			    &(NETWORK_FIRMS.array[i].loan_portfolio),
			    &(NETWORK_FIRMS.array[i].friend_id_list),
			    'f');
*/
	  //add empty node to copy data into:
	  add_node_adt(&NETWORK,
			    tmp_id,
			    &tmp_financial_data,
			    &tmp_loan_portfolio,
			    &tmp_friend_id_list,
			    'f');
	  
	  //use build-in functionality of FLAME:
	  //void copy_node_adt(node_adt * from, node_adt * to)
	  copy_node_adt(&NETWORK_FIRMS.array[i], &NETWORK.array[i]);

	  //Copy element-wise:
	 
	  //copy_financial_data_adt(&NETWORK_FIRMS.array[i].financial_data, &NETWORK.array[i].financial_data);
	  //copy_debt_item_adt_array(&NETWORK_FIRMS.array[i].loan_portfolio, &NETWORK.array[i].loan_portfolio);
  	  //copy_int_array(&NETWORK_FIRMS.array[i].friend_id_list, &NETWORK.array[i].friend_id_list);
	} 

	//Print all friend_lists
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	    printf("\n\nInside initialize_network(): \n");
	    
	    //For each node i
	    for(i = 0; i < NETWORK_FIRMS.size; i++)
	    {
		  printf("\nFIRM NETWORK ID %d FRIEND LIST [ ", NETWORK_FIRMS.array[i].id);
		  for(k = 0; k < NETWORK_FIRMS.array[i].friend_id_list.size; k++)
		      printf("%d ", NETWORK_FIRMS.array[i].friend_id_list.array[k]);
		  printf("]");
	    }	
	}
	#endif

	// use index i as starting index for adding banks to network
	i = NETWORK_FIRMS.size;

	for (j=i; j<(i+NETWORK_BANKS.size); j++)
	{

/*
	  //simple copy
	  add_node_adt(&NETWORK,
			    NETWORK_BANKS.array[j].id,
			    &(NETWORK_BANKS.array[j].financial_data),
			    &(NETWORK_BANKS.array[j].loan_portfolio),
			    &(NETWORK_BANKS.array[j].friend_id_list),
			    'b');
*/
	  //add empty node to copy data into:
	  add_node_adt(&NETWORK,
			    tmp_id,
			    &tmp_financial_data,
			    &tmp_loan_portfolio,
			    &tmp_friend_id_list,
			    'f');
	  //use build-in functionality of FLAME:
	  //void copy_node_adt(node_adt * from, node_adt * to)
	  copy_node_adt(&NETWORK_BANKS.array[j-i], &NETWORK.array[j]);

	  //Copy element-wise:
	  //copy_financial_data_adt(&NETWORK_BANKS.array[j-1].financial_data, &NETWORK.array[j].financial_data);
	  //copy_debt_item_adt_array(&NETWORK_BANKS.array[j-1].loan_portfolio, &NETWORK.array[j].loan_portfolio);
  	  //copy_int_array(&NETWORK_BANKS.array[j-1].friend_id_list, &NETWORK.array[j].friend_id_list);
	}

	//Print all friend_lists
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	    //For each node i
	    for(i = 0; i < NETWORK_BANKS.size; i++)
	    {
		  printf("\nBANK NETWORK ID %d FRIEND LIST [ ", NETWORK_BANKS.array[i].id);
		  for(k = 0; k < NETWORK_BANKS.array[i].friend_id_list.size; k++)
		      printf("%d ", NETWORK_BANKS.array[i].friend_id_list.array[k]);
		  printf("]");
	    }	
	}
	#endif
	
    	// free temp adts
	free_financial_data_adt(&tmp_financial_data);
	free_debt_item_array(&tmp_loan_portfolio);
	free_int_array(&tmp_friend_id_list);
}

//Find the position/index of node id in the NETWORK
int find_node_index(int id)
{
	int i;

	//For each node
	for(i = 0; i < NETWORK.size; i++)
	{
		//If node is found in the Network
		if(NETWORK.array[i].id == id)
		{
		    //Return position/Network.array_index
		    return i;
		}
	}

	//Not found: Return error value
	return -1;
}

/*
 * \fn: int find_node_index_of_friend(int f, int h)
 * \brief: Node ID1=f checks if node ID2=h is on his friendlist
 * Return value: 
 * index1: index of h on the friend list of f
 * index2: index of h on the general network array
 */
int find_node_index_of_friend(int f, int h)
{
	int i,j;
	int index_f=-1;
	int index_h=-1;
	
	int f_index = find_node_index(f);
	
	//printf("\n Consider: NETWORK.array[%d].friend_id_list.size=%d", f_index, NETWORK.array[f_index].friend_id_list.size);
	
	//For each Friend i on friendlist of f
	for(i = 0; i<NETWORK.array[f_index].friend_id_list.size; i++)
	{
		//printf("\n Consider: NETWORK.array[%d].friend_id_list.array[%d]=%d", f_index, i, NETWORK.array[f_index].friend_id_list.array[i]);
		
		//If node h is a friend of f: IDs below are equal, and returns index1
		if(h == NETWORK.array[f_index].friend_id_list.array[i])
		{
			//printf("\n Found: h=%d NETWORK.array[%d].friend_id_list.array[%d]=%d at index i=%d", h, f_index, i, NETWORK.array[f_index].friend_id_list.array[i], i);
		  
			//Store position of: Friend_id_list.array[i]
			index_f = i;

			//Search general network for index2:
			for(j=0; j<NETWORK.size; j++)
			  if(h == NETWORK.array[j].id)
			  {
			    index_h = j; //Store position of NETWORK.array[j]
			    break;
			  }
		  break;
		}
	}
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\n find_node_index_of_friend: ID %d searches for ID %d in NETWORK array\n", f, h);
	  printf("\n NETWORK index of ID %d: index_f=%d; NETWORK index of ID %d: index_h=%d\n", f, index_f, h, index_h);
	}
	#endif

	// return value is the NETWORK array index of h
	return index_h;
}


//Network Agent checks the consistency of the network structure
void network_structure_consistency_check(void)
{
        #ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: network_structure_consistency_check()\n");
	}
	#endif

	int i,j,k, index;
	int link_i_j;

	//Print all friend_lists
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	    //For each node i
	    for(i = 0; i < NETWORK.size; i++)
	    {
		  printf("\nNETWORK ID %d FRIEND LIST [ ", NETWORK.array[i].id);
		  for(k = 0; k < NETWORK.array[i].friend_id_list.size; k++)
		      printf("%d ", NETWORK.array[i].friend_id_list.array[k]);
		  printf("]");
	    }	
	}
	#endif
	
	//Check consistency of friend_lists
	//For each node i
	for(i = 0; i < NETWORK.size; i++)
	{
		//For each friend j of node i 
		for(j = 0; j < NETWORK.array[i].friend_id_list.size; j++)
		{
			//Check if node i has a link with himself
			if(NETWORK.array[i].friend_id_list.array[j] ==  NETWORK.array[i].id)
				fprintf(stderr,"\nERROR: node %d is on his own friendlist %d\n", NETWORK.array[i].id,NETWORK.array[i].friend_id_list.array[j]);

			//Find Friend at index j in Network array
			index = find_node_index_of_friend(NETWORK.array[i].id, NETWORK.array[i].friend_id_list.array[j]);

/*
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_NSA)
			{
			  printf("\nFound node ID=%d on the friendlist of node ID=%d",
					NETWORK.array[i].friend_id_list.array[j], NETWORK.array[i].id);

				//print friendlist of i
				  printf(" [ ");
				  for(k = 0; k < NETWORK.array[i].friend_id_list.size; k++)
				      printf("%d ", NETWORK.array[i].friend_id_list.array[k]);
				  printf("]\n");

			  printf("Found node ID=%d in the general network as index=%d: %d\n",
					NETWORK.array[i].friend_id_list.array[j], index, NETWORK.array[index].id);
			}
			#endif
*/			
			link_i_j = 0;

			//For each Friend k of Friend j of node i
			for(k = 0; k < NETWORK.array[index].friend_id_list.size; k++)
			{
				//If friend k is node i: i and j are on one another's friendlist
				if(NETWORK.array[i].id == NETWORK.array[index].friend_id_list.array[k])
					link_i_j++;
			}
			
			#ifdef _DEBUG_MODE    
			if (PRINT_DEBUG_NSA)
			{
				//Check: If i and j are not on one another's friendlist: -> directed Link! (produces an error message)
				if(link_i_j == 0)
				{
				  fprintf(stderr,"\nERROR: node i:%d ID=%d is not on the friend list of node j:%d ID_on_network %d ID_on_i_list %d. ",
					i, NETWORK.array[i].id, index, NETWORK.array[index].id, NETWORK.array[i].friend_id_list.array[j]);
				
				  fprintf(stderr,"Friends of j:%d ID %d: [ ", index, NETWORK.array[index].id);
				  for(k = 0; k < NETWORK.array[index].friend_id_list.size; k++)
				      fprintf(stderr,"%d ", NETWORK.array[index].friend_id_list.array[k]);
				  fprintf(stderr,"]\n");
				}

				//Check: If i and j are multiple times on eachother's friend list: -> multiple links! (produces an error message)
				if(link_i_j > 1)
				{
					fprintf(stderr,"\nERROR: node i:%d ID=%d is %d times on the friendlist of node j:%d ID %d",
					i, NETWORK.array[i].id, link_i_j, NETWORK.array[i].friend_id_list.array[j], NETWORK.array[index].id);

					fprintf(stderr,"Friends of j:%d ID %d: [ ", index, NETWORK.array[index].id);
					for(k = 0; k < NETWORK.array[index].friend_id_list.size; k++)
					    fprintf(stderr,"%d ", NETWORK.array[index].friend_id_list.array[k]);
					fprintf(stderr,"]\n");
				}
			}
			#endif
			
		}//END: for(j = 0; j < NETWORK.array[i].friend_id_list.size; j++)
	}//END: for(i = 0; i < NETWORK.size; i++)
}


//Write data for Network analysis tool "Cuttlefish" to a file
void cuttlefish_data(void)
{
      	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: cuttlefish_data()\n");
	}
	#endif

	int i,j,k;

	FILE *file1;
	char *filename;

	filename = malloc(40*sizeof(char));
	filename[0]=0;
	
	sprintf(filename, "cuttlefish_%d.cxf", DAY);
		 
	file1 = fopen(filename,"w");

	fprintf(file1,"configuration: undirected\n");
	
	
	int node_size;
	double node_color;
	
	int edge_weight;
	
	int count;
	
	// CONFIGURATION FOR NODES
	for(i = 0; i < NETWORK.size; i++)
	{
		//printf("\n Node %d is %c\n", i, NETWORK.array[i].type_of_node);
		
		//select bank feature to filter on
		if(NETWORK.array[i].type_of_node == 'b')
		{
		    
			//Default setting: fixed size and color
			node_size=5;
			node_color = 0.0;
			
			//color proportional to the equity/asset ratio (financial fragility) (a in [0,1])
			//node_size=5;
			//node_color = NETWORK.array[i].financial_data.equity_asset_ratio;
			
			//size  proportional to the equity
			//color proportional to the equity/asset ratio (financial fragility) (a in [0,1])
			node_size  = min(5,NETWORK.array[i].financial_data.equity);
			node_color = NETWORK.array[i].financial_data.equity_asset_ratio;
			
			//printf("\n node: (%d) size{%d} color{%f,%f,%f}\n", NETWORK.array[i].id, node_size, node_color, node_color, node_color);
			fprintf(file1,"node: (%d) size{%d} color{%f,%f,%f}\n", NETWORK.array[i].id, node_size, node_color, node_color, node_color);
		}
		//select firm feature to filter on
		if(NETWORK.array[i].type_of_node == 'f')
		{

			//Default setting: fixed size and color
			node_size=5;
			node_color = 1.0;
			
			//color proportional to the equity/asset ratio (financial fragility) (a in [0,1])
			//node_size=5;
			//node_color = NETWORK.array[i].financial_data.equity_asset_ratio;
			
			//size  proportional to the equity
			//color proportional to the equity/asset ratio (financial fragility) (a in [0,1])
			node_size  = min(5,NETWORK.array[i].financial_data.equity);
			node_color = NETWORK.array[i].financial_data.equity_asset_ratio;

			//printf("\n node: (%d) size{%d} color{%f,%f,%f}\n", NETWORK.array[i].id, node_size, node_color, node_color, node_color);
			fprintf(file1,"node: (%d) size{%d} color{%f,%f,%f}\n", NETWORK.array[i].id, node_size, node_color, node_color, node_color);
		}
	}

	
	// CONFIGURATION FOR EDGES
	for(i = 0; i < NETWORK.size; i++)
	{
		//printf("\n i %d of NETWORK.size %d \n", i, NETWORK.size);
		//printf("\n NETWORK.array[i].friend_id_list.size %d \n", NETWORK.array[i].friend_id_list.size);
				
		for(j = 0; j < NETWORK.array[i].friend_id_list.size; j++)
		{

			//printf("\n NETWORK.array[i].id %d <=> NETWORK.array[i].friend_id_list.array[j] %d\n", NETWORK.array[i].id, NETWORK.array[i].friend_id_list.array[j]);
			
			if(NETWORK.array[i].id > NETWORK.array[i].friend_id_list.array[j])
			{
				//Default setting: fixed  color for edges
				edge_weight=1;
				
				//printf("edge: (%d,%d) color{%f,%f,%f}\n", NETWORK.array[i].id,NETWORK.array[i].friend_id_list.array[j],0.5,0.5,0.5);
				//fprintf(file1,"edge: (%d,%d) color{%f,%f,%f}\n", NETWORK.array[i].id,NETWORK.array[i].friend_id_list.array[j], edge_color, edge_color, edge_color);
				
				/*
				sum=0.0;
				if (NETWORK.array[i].node_type=='b')
				  for (k=0;k<NETWORK.array[i].loan_portfolio.size; k++)
				    if (NETWORK.array[i].loan_portfolio.array[k].id == NETWORK.array[i].friend_id_list.array[j])
				      sum += NETWORK.array[i].loan_portfolio.array[k].loan_value;

				*/
				

				//color of edge proportional to loan_value/total_equity of bank
				//edge_color = sum/NETWORK.array[i].financial_data.equity;
				//fprintf(file1,"edge: (%d,%d) color{%f,%f,%f}\n", NETWORK.array[i].id,NETWORK.array[i].friend_id_list.array[j], edge_color, edge_color, edge_color);

				// Edge weight: how often does a friend apear on the frined list; use this as weight for the connection
				count=0;
				
				//How often is j on the list?
				  for (k=0; k<NETWORK.array[i].friend_id_list.size; k++)
				    if (j==NETWORK.array[i].friend_id_list.array[k])
				      count++;
				
				
				//weight of edge proportional to no.times on friend_list
				edge_weight = count;
				fprintf(file1,"edge: (%d,%d) weight{%d} color{%f,%f,%f}\n", NETWORK.array[i].id,NETWORK.array[i].friend_id_list.array[j], edge_weight, 0.5,0.5,0.5);
			}
		}	
	}

	fclose(file1);
	free(filename);
}


//Write data for Adjacency Matrix to a file "adj_matrix_DAY"
	// 1. 	NUMBER of Loans of each firm with each bank
	// A[i][k]: firm i, bank k, A[i][k]= total number of loans of firm i with bank k
	//int A[80][20];
	
	
	// 2. VALUE of Loans of each firm with each bank
	// B[i][k]: firm i, bank k, B[i][k]= total value of all loans of firm i with bank k
	//double B[80][20];
	
void print_adjacency_matrix_data(int A[][COLS], double B[][COLS], int nr_rows, int nr_cols)
{
        #ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("\n\nStart: print_adjacency_matrix_data()\n");
	}
	#endif

	
	int i,k;

	FILE *file1;
	char *filename;

	filename = malloc(40*sizeof(char));
	filename[0]=0;
	
	#ifdef _DEBUG_MODE    
	if (PRINT_DEBUG_NSA)
	{
	  printf("Opening file: adj_matrix_%d.csv", DAY);
	}
	#endif

	sprintf(filename, "adj_matrix_%d.csv", DAY);
		 
	file1 = fopen(filename,"w");

	for(i = 0; i < nr_rows; i++)
	{
		for(k = 0; k < nr_cols; k++)
		{
			fprintf(file1,"%d\t", A[i][k]);
			
			//Pointer equivalent: *A is pointer to first element *A[0], so *(A+i) points to A[i] and for A[i][k] we need:
//			fprintf(file1,"%d\t", (*(*(A+i) + k)));
		}
		fprintf(file1,"\n");
	}

	fprintf(file1,"\n");
	for(i = 0; i < nr_rows; i++)
	{
		for(k = 0; k < nr_cols; k++)
		{
			fprintf(file1,"%.2f\t", B[i][k]);

			//Pointer equivalent:
//			fprintf(file1,"%.2f\t", (*(*(B+i) + k)));
			if (fabs(B[i][k])>1e+9) fprintf(stderr, "\n\nERROR in print_adjacency_matrix_data(): B[%d][%d]>1e+9 (%f) \n", i, k, B[i][k]);
		}
		fprintf(file1,"\n");
	}

	fclose(file1);
	free(filename);
}

