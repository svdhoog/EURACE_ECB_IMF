#include <stdio.h>

#include "../header.h"
#include "../Bank_agent_header.h"
#include "../my_library_header.h"

#include "balance_sheet_operations.h"


/** \fn asset_liability_add
 *  \brief Usage: asset_liability_add(&ASSET, &LIABILITY, value)
 */          
void asset_liability_add(double * asset, double * liability, double value)
{
	*(asset)     += value;
	*(liability) += value;
}

/** \fn  asset_liability_subtract
 *  \brief Usage: asset_liability_subtract(&ASSET, &LIABILITY, value)
 */          
void asset_liability_subtract(double * asset, double * liability, double value)
{
	*(asset)     -= value;
	*(liability) -= value;
}

/** \fn  asset_transformation
 *  \brief Usage: asset_transformation(&LIABILITY1, &LIABILITY2, value)
 */
void asset_transformation(double * from, double * to, double value)
{
	*(from) -= value;
	*(to)   += value;
}

/** \fn  liability_transformation
 *  \brief Usage: liability_transformation(&LIABILITY1, &LIABILITY2, value)
 */
void liability_transformation(double * from, double * to, double value)
{
	*(from) -= value;
	*(to)   += value;
}


void recompute_mortgage_stocks()
{
	int i;
	
	PL_MORTGAGE_LOANS = 0.0;
	NPL_MORTGAGE_LOANS = 0.0;

	PL_MORTGAGE_LOANS_OP = 0.0;
	PL_MORTGAGE_LOANS_HMR = 0.0;
	
	NPL_MORTGAGE_LOANS_OP = 0.0;
	NPL_MORTGAGE_LOANS_HMR = 0.0;

	TOTAL_MORTGAGE_CREDIT = 0.0;


	for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
	{

		assert(MORTGAGE_PORTFOLIO.array[i].type > 0);
		
		switch(MORTGAGE_PORTFOLIO.array[i].status)
		{
			//paying
			case 0:  PL_MORTGAGE_LOANS += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

					if (MORTGAGE_PORTFOLIO.array[i].type==1) PL_MORTGAGE_LOANS_HMR += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
					if (MORTGAGE_PORTFOLIO.array[i].type==2) PL_MORTGAGE_LOANS_OP  += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

			break;
			
			//arrears
			case 1:  PL_MORTGAGE_LOANS += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

					if (MORTGAGE_PORTFOLIO.array[i].type==1) PL_MORTGAGE_LOANS_HMR += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
					if (MORTGAGE_PORTFOLIO.array[i].type==2) PL_MORTGAGE_LOANS_OP  += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

			break;
			
			//npl
			case 2: NPL_MORTGAGE_LOANS += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

					if (MORTGAGE_PORTFOLIO.array[i].type==1) NPL_MORTGAGE_LOANS_HMR += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
					if (MORTGAGE_PORTFOLIO.array[i].type==2) NPL_MORTGAGE_LOANS_OP  += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

			break;
		}
	}

	// NPL mortgages for seized houses, that have not yet been sold
	for(i=0; i<SEIZED_COLLATERAL.size; i++)
	{
		assert(SEIZED_COLLATERAL.array[i].seized_house.mortgage.type > 0);
		NPL_MORTGAGE_LOANS += SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;

		if (SEIZED_COLLATERAL.array[i].seized_house.mortgage.type==1) NPL_MORTGAGE_LOANS_HMR += SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;
		if (SEIZED_COLLATERAL.array[i].seized_house.mortgage.type==2) NPL_MORTGAGE_LOANS_OP += SEIZED_COLLATERAL.array[i].seized_house.mortgage.outstanding_principal;
	}

	//NPL mortgage ratios
	NPL_RATIO_MORTGAGE_LOANS_OP = 0.0;
	NPL_RATIO_MORTGAGE_LOANS_HMR = 0.0;
	NPL_RATIO_MORTGAGE_LOANS = 0.0;
	
	//OP
	if(NPL_MORTGAGE_LOANS_OP+PL_MORTGAGE_LOANS_OP>0.0)
	  NPL_RATIO_MORTGAGE_LOANS_OP		= NPL_MORTGAGE_LOANS_OP/(NPL_MORTGAGE_LOANS_OP+PL_MORTGAGE_LOANS_OP);
	
	//HMR
	if(NPL_MORTGAGE_LOANS_HMR+PL_MORTGAGE_LOANS_HMR>0.0) 
		NPL_RATIO_MORTGAGE_LOANS_HMR 	= NPL_MORTGAGE_LOANS_HMR/(NPL_MORTGAGE_LOANS_HMR+PL_MORTGAGE_LOANS_HMR);
	
	//Total
	if(NPL_MORTGAGE_LOANS+PL_MORTGAGE_LOANS>0.0)
	 	  NPL_RATIO_MORTGAGE_LOANS 		= NPL_MORTGAGE_LOANS/(NPL_MORTGAGE_LOANS+PL_MORTGAGE_LOANS);

	 TOTAL_MORTGAGE_CREDIT = PL_MORTGAGE_LOANS + NPL_MORTGAGE_LOANS;

}

void print_bank_balance_sheet()
{
	int i;
    double sum;
    
	SEIZED_COLLATERAL_VALUE = 0.0;
	
	for (i=0; i< SEIZED_COLLATERAL.size; i++)
		SEIZED_COLLATERAL_VALUE += SEIZED_COLLATERAL.array[i].seized_house.purchase_price;
	
	//balance sheet differential
	double total_liabilities = 	DEPOSITS + ECB_DEBT 
								+ LLR_FIRM_LOANS 
								+ LLR_PL_MORTGAGE_LOANS 
								+ LLR_NPL_MORTGAGE_LOANS 
								+ CAPITAL_ADJ_PL_MORTGAGE_LOANS
								+ CAPITAL_ADJ_NPL_MORTGAGE_LOANS	
								+ CAPITAL_ADJ_PL_FIRM_LOANS 
								+ CAPITAL_ADJ_NPL_FIRM_LOANS 
								+ EQUITY;
		
	double total_assets = RESERVES + FIRM_CREDIT + PL_MORTGAGE_LOANS + NPL_MORTGAGE_LOANS;
	
	double diff = total_assets - total_liabilities;
	
	printf("\nIT %d ID: %d Bank Balance Sheet", DAY,ID);
	//printf("\n[HH_PD_PL_YEAR %2.2f\tHH_LGD_PL_YEAR %2.2f\tHH_PD_NPL_YEAR %2.2f\tHH_LGD_NPL_QTR %2.2f]", HH_PD_PL_YEAR, HH_LGD_PL_YEAR, HH_PD_NPL_YEAR, HH_LGD_NPL_QTR);
	//printf("\nStocks:");
	printf("\n\t\t\t\t\t\tDEPOSITS\t\t%8.2f\nRESERVES\t\t%8.2f\t\tECB DEBT\t\t%8.2f\nFIRM LOANS\t\t%8.2f\t\tLLR FIRM LOANS\t\t%8.2f\nPL MORTGAGES\t\t%8.2f\t\tLLR PL MORTGAGES\t%8.2f\nNPL MORTGAGES\t\t%8.2f\t\tLLR NPL MORTGAGES\t%8.2f\n\t\t\t\t\t\tCAPCHARGE MORT.LOANS\t\t\t%8.2f\n\t\t\t\t\t\tCAPCHARGE FIRM LOANS\t\t\t%8.2f\nSEIZED COLL.\t\t%8.2f\t\tEQUITY\t\t\t%8.2f",
			DEPOSITS,
			RESERVES, ECB_DEBT,
			FIRM_CREDIT, LLR_FIRM_LOANS,
			PL_MORTGAGE_LOANS, LLR_PL_MORTGAGE_LOANS,
			NPL_MORTGAGE_LOANS, LLR_NPL_MORTGAGE_LOANS,
			CAPITAL_ADJ_PL_MORTGAGE_LOANS + CAPITAL_ADJ_NPL_MORTGAGE_LOANS,
			CAPITAL_ADJ_PL_FIRM_LOANS + CAPITAL_ADJ_NPL_FIRM_LOANS,
			SEIZED_COLLATERAL_VALUE, EQUITY);
    
    double eq_rea_ratio = 0.0;
    double eq_rea_pct = 0.0;
    if(REA>1e-6)
    {   eq_rea_ratio = EQUITY/REA;
        eq_rea_pct = 100*EQUITY/REA;
    }
    printf("\tE/TA  %8.2f (%2.2f%%)\t\tE/REA  %8.2f (%2.2f%%)", EQUITY/total_assets, 100*EQUITY/total_assets, eq_rea_ratio, eq_rea_pct);
	printf("\nAssets\t\t\t%8.2f\t\tLiabilities\t\t%8.2f\tDiff  %8.2f (%2.2f%%)", total_assets, total_liabilities, diff, 100*diff/total_liabilities);
	printf("\nMORTGAGE CLAIM\t\t%8.2f", MORTGAGE_CLAIM);
	
	//Raise warning if diff > 0.01 percent of balance sheet size
	if (100*fabs(diff)/total_liabilities > 1e-6)
	{
		fprintf(stderr, "\n\nIT %d ID: %d WARNING Bank Balance Sheet: Diff %2.16f (%2.16f%%)", DAY, ID, diff, 100*diff/total_liabilities);
	}	

	//Print output to files: agent-specific
	FILE *file1;
	char *filename;

	#ifdef _DEBUG_MODE        
	if (PRINT_DEBUG_FILES_BANKS)
	{                       
		filename = malloc(40*sizeof(char));
		filename[0]=0;
		
		if (ID>1e4) fprintf(stderr, "\n\nWARNING: In print output to files, agent ID>1e4 requires longer string buffer for filenames.");

		sprintf(filename, "Bank_balance_sheet_%4d.txt", ID);
		//strcpy(filename, "its/Bank_balance_sheet_%d.txt");      
		file1 = fopen(filename,"a");
		
		//PRINT STATEMENTS
		//format: fprintf(file1,"\nIT %d %f %f", DAY,RESERVES,DEPOSITS);


		fprintf(file1,"\n\nIT %d ID: %d Bank Balance Sheet", DAY,ID);
		fprintf(file1,"\n\t\t\t\t\t\tDEPOSITS\t\t%8.2f\nRESERVES\t\t%8.2f\t\tECB DEBT\t\t%8.2f\nFIRM LOANS\t\t%8.2f\t\tLLR FIRM LOANS\t\t%8.2f\nPL MORTGAGES\t\t%8.2f\t\tLLR PL MORTGAGES\t%8.2f\nNPL MORTGAGES\t\t%8.2f\t\tLLR NPL MORTGAGES\t%8.2f\n\t\t\t\t\t\tCAPCHARGE MORT.LOANS\t\t\t%8.2f\n\t\t\t\t\t\tCAPCHARGE FIRM LOANS\t\t\t%8.2f\nSEIZED COLL.\t\t%8.2f\t\tEQUITY\t\t\t%8.2f",
			DEPOSITS,
			RESERVES, ECB_DEBT,
			FIRM_CREDIT, LLR_FIRM_LOANS,
			PL_MORTGAGE_LOANS, LLR_PL_MORTGAGE_LOANS,
			NPL_MORTGAGE_LOANS, LLR_NPL_MORTGAGE_LOANS,
			CAPITAL_ADJ_PL_MORTGAGE_LOANS + CAPITAL_ADJ_NPL_MORTGAGE_LOANS,
			CAPITAL_ADJ_PL_FIRM_LOANS + CAPITAL_ADJ_NPL_FIRM_LOANS,
			SEIZED_COLLATERAL_VALUE, EQUITY);

	    fprintf(file1,"\tE/TA  %8.2f (%2.2f%%)\t\tE/REA  %8.2f (%2.2f%%)", EQUITY/total_assets, 100*EQUITY/total_assets, eq_rea_ratio, eq_rea_pct);
		fprintf(file1,"\nAssets\t\t\t%8.2f\t\tLiabilities\t\t%8.2f\tDiff  %8.2f (%2.2f%%)", total_assets, total_liabilities, diff, 100*diff/total_liabilities);
		fprintf(file1,"\nMORTGAGE CLAIM\t\t%8.2f", MORTGAGE_CLAIM);
		
		//Raise warning if diff > 0.01 percent of balance sheet size
		if (100*fabs(diff)/total_liabilities > 1e-6)
		{
            fprintf(file1, "\n\nIT %d ID: %d WARNING Bank Balance Sheet: Diff %2.16f (%2.16f%%)", DAY, ID, diff, 100*diff/total_liabilities);
		}	


		//PRINT OUT MORTGAGE PORTFOLIO
		fprintf(file1,"\n\nIT %d ID: %d print_bank_balance_sheet output to file", DAY, ID);
        fprintf(file1,"\n\t\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
        fprintf(file1,"\n\tNo\tactive\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 
        
        sum = 0.0;
        for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
        {
            sum += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
        }

        for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
		{
			fprintf(file1, "\n\t%2d/%2d\t%4d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
				i+1,
				MORTGAGE_PORTFOLIO.size,
				MORTGAGE_PORTFOLIO.array[i].active,
				MORTGAGE_PORTFOLIO.array[i].household_id,
				MORTGAGE_PORTFOLIO.array[i].object_id,
				MORTGAGE_PORTFOLIO.array[i].monthly_principal,
				MORTGAGE_PORTFOLIO.array[i].monthly_interest,
				MORTGAGE_PORTFOLIO.array[i].outstanding_principal,
				MORTGAGE_PORTFOLIO.array[i].outstanding_interest,
				MORTGAGE_PORTFOLIO.array[i].monthly_total_repayment,
				MORTGAGE_PORTFOLIO.array[i].initial_duration,
				MORTGAGE_PORTFOLIO.array[i].current_duration,
				MORTGAGE_PORTFOLIO.array[i].status,			
				MORTGAGE_PORTFOLIO.array[i].arrears_counter,
				MORTGAGE_PORTFOLIO.array[i].npl_counter,
				(MORTGAGE_PORTFOLIO.array[i].active==0) );
		} 
		fprintf(file1,"\nSum outstanding principal:\t%f", sum);
        
		fclose(file1);
		free(filename);
	}                
    #endif

}


/** \fn mortgage_portfolio_diff()
 * \brief Function to compute the diff between two mortgage portfolios (dynamic arrays of mortgage_contract_adts)
 * \brief Used in: Bank_reconstruct_mortgage_portfolio
 * \brief mortgage_contract_adt_array * m1: pointer to a mortgage portfolio before change
 * \brief mortgage_contract_adt_array * m2: pointer to a mortgage portfolio after change
 * \brief int_array exclusive_in_m1: index list of all m1 mortgages that are only present in m1
 * \brief int_array exclusive_in_m2: index list of all m2 mortgages that are only present in m2
 * \brief int_array present_in_both_using_m1_index: index list of all mortgages that are present in both portfolios (using indexes in m1)
 * \brief int_array present_in_both_using_m2_index: index list of all mortgages that are present in both portfolios (using indexes in m2)
 */
void mortgage_portfolio_diff(mortgage_contract_adt_array * m1, mortgage_contract_adt_array * m2)
{
	int i,j,n1,n2;
    double sum1,sum2,diff;

	int_array exclusive_in_m1; init_int_array(&exclusive_in_m1);
	int_array exclusive_in_m2; init_int_array(&exclusive_in_m2);
	int_array present_in_both_using_m1_index; init_int_array(&present_in_both_using_m1_index);
	int_array present_in_both_using_m2_index; init_int_array(&present_in_both_using_m2_index);

	// Construct inclusive lists
	// Double loop to compare all (i,j) tuples
	// Matrix (m1[i],m2[j]) of mortgage tuples: compare on attributes: household_id, object_id
	// BUT: only compare Upper Triangle of the matrix, plus diagonal elements
	// Due to symmetry: n(n-1)/2 compares needed
    for (j=0; j<(*m2).size; j++)
    {
        for (i=j; i<(*m1).size; i++)
        {
            // Compare mortgage m1[i] and m2[j]
            if ((*m1).array[i].household_id == (*m2).array[j].household_id && (*m1).array[i].object_id == (*m2).array[j].object_id)
            {   add_int(&present_in_both_using_m1_index, i);
                add_int(&present_in_both_using_m2_index, j);
            }
        }
    }
    
    // Print comparison matrix (Right-Upper-Triangular)
/*
    printf("\n");
	for (j=0; j<(*m2).size; j++)
	{
		printf("\n", i,j);
		for (i=0; i<j; i++)
			printf("| o o ");
		for (i=j; i<(*m1).size; i++)
		{
			// Diagnostics
			printf("|%2d %2d", j,i);
		}
		printf("|");
	}
	printf("\n");
*/

    // Construct exclusive lists: complements of inclusive lists
    // Start from full list of indices, and remove items on the inclusive list, starting from the end
    for (i=0; i<(*m1).size; i++)
        add_int(&exclusive_in_m1, i);
    for (i=0; i<(*m2).size; i++)
        add_int(&exclusive_in_m2, i);
    
    // Ensure inclusive lists are sorted lists in ascending/increasing order
    //Sort m1
    n1 = present_in_both_using_m1_index.size;
    double sorted_data_m1[n1];      //type is double for gsl_sort 
    for (i=0; i<n1; i++)
        sorted_data_m1[i] = (double) present_in_both_using_m1_index.array[i];
    gsl_sort(sorted_data_m1, 1, n1);
    
    //Sort m2
    n2 = present_in_both_using_m2_index.size;
    double sorted_data_m2[n2];      //type is double for gsl_sort
    for (i=0; i<n2; i++)
        sorted_data_m2[i] = (double) present_in_both_using_m2_index.array[i];
    gsl_sort(sorted_data_m2, 1, n2);
    
/*
    printf("\n\n\tsorted_data_m1:\t%2d", n1);
    printf("\n[");
    for (i=0; i<n1; i++)
        printf("%2.1f|", sorted_data_m1[i]);
    printf("]");
    
    printf("\n\n\tsorted_data_m2:\t%2d", n2);
    printf("\n[");
    for (i=0; i<n2; i++)
        printf("%2.1f|", sorted_data_m2[i]);
    printf("]");
*/
    
    // Traverse inclusive list from end, remove indices that appear in inclusive list from the full list, to end up with the exclusive list
    for (i=n1-1; i>=0; i--)
        remove_int(&exclusive_in_m1, (int) sorted_data_m1[i]);
    for (i=n2-1; i>=0; i--)
        remove_int(&exclusive_in_m2, (int) sorted_data_m2[i]);

    sum1 = 0.0;
    for (j=0; j<exclusive_in_m1.size; j++)
    {
        i = exclusive_in_m1.array[j];
        sum1 += (*m1).array[i].outstanding_principal + (*m1).array[i].monthly_principal;
    }
        
    sum2 = 0.0;
    for (j=0; j<exclusive_in_m2.size; j++)
    {
        i = exclusive_in_m2.array[j];
        sum2 += (*m2).array[i].outstanding_principal + (*m2).array[i].monthly_principal;
    }
    
    diff = sum2 - sum1;


	//Print output to stdout
	int print_output_to_stdout = 0;

	if (print_output_to_stdout)
	{
		// Summary:
		//PRINT OUT MORTGAGE PORTFOLIO DIFF RESULTS
		printf("\n\nIT %d ID: %d mortgage_portfolio_diff()", DAY, ID);
		printf("\nInputs:\n\tm1 size %d\n\tm2 size %d\n\tTotal %d", (*m1).size, (*m2).size, (*m1).size + (*m2).size);

		printf("\nSummary:");
		printf("\n\tpresent_in_both_using_m1_index:\t%d", present_in_both_using_m1_index.size);
		printf("\n\tpresent_in_both_using_m2_index:\t%d", present_in_both_using_m2_index.size);
        printf("\n");
        
		if ( present_in_both_using_m1_index.size != present_in_both_using_m2_index.size )
			printf(" <-- FAIL PRESENT IN BOTH SIZES INCONSISTENT");

        printf("\n\texclusive_in_m1:\t\t%d", exclusive_in_m1.size);
		if ( present_in_both_using_m1_index.size + exclusive_in_m1.size != (*m1).size )
			printf(" <-- FAIL M1 SIZES INCONSISTENT");

        printf("\n\texclusive_in_m2:\t\t%d", exclusive_in_m2.size);
		if ( present_in_both_using_m2_index.size + exclusive_in_m2.size != (*m2).size )
			printf(" <-- FAIL M2 SIZES INCONSISTENT");

        printf("\n");

        if ( present_in_both_using_m1_index.size + exclusive_in_m1.size != (*m1).size )
            printf(" <-- FAIL TOTAL SIZES INCONSISTENT");
        
        printf("\n");

        if ( present_in_both_using_m2_index.size + exclusive_in_m2.size != (*m2).size )
            printf(" <-- FAIL TOTAL SIZES INCONSISTENT");
        
        printf("\n\tSum Total:\t\t\t%d", present_in_both_using_m1_index.size + present_in_both_using_m2_index.size + exclusive_in_m1.size + exclusive_in_m2.size);
		if ( present_in_both_using_m1_index.size + present_in_both_using_m2_index.size + exclusive_in_m1.size + exclusive_in_m2.size != (*m1).size + (*m2).size)
			printf(" <-- FAIL TOTAL SIZES INCONSISTENT");

		printf("\n\nDetailed output:");

		printf("\n\n\tpresent_in_both_using_m1_index:\t%2d", present_in_both_using_m1_index.size );
		printf("\n[");
		for (i=0; i<present_in_both_using_m1_index.size; i++)
			printf("%2d|", present_in_both_using_m1_index.array[i]);		
		printf("]");

        printf("\n\n\tpresent_in_both_using_m2_index:\t%2d", present_in_both_using_m2_index.size );
        printf("\n[");
        for (i=0; i<present_in_both_using_m2_index.size; i++)
            printf("%2d|", present_in_both_using_m2_index.array[i]);
        printf("]");

        
        printf("\n\n\tsorted_data_m1:\t%2d", n1);
        printf("\n[");
        for (i=0; i<n1; i++)
            printf("%2.0f|", sorted_data_m1[i]);
        printf("]");
        
        printf("\n\n\tsorted_data_m2:\t%2d", n2);
        printf("\n[");
        for (i=0; i<n2; i++)
            printf("%2.0f|", sorted_data_m2[i]);
        printf("]");
        
        
		printf("\n\n\texclusive_in_m1:\t%2d", exclusive_in_m1.size);
		printf("\n[");
		for (i=0; i<exclusive_in_m1.size; i++)
			printf("%2d|", exclusive_in_m1.array[i]);
		printf("]");

		printf("\n\n\texclusive_in_m2:\t%2d", exclusive_in_m2.size);
		printf("\n[");
		for (i=0; i<exclusive_in_m2.size; i++)
			printf("%2d|", exclusive_in_m2.array[i]);
		printf("]");


        //PRINT OUT MORTGAGE PORTFOLIO m1
        printf("\n\nIT %d ID: %d mortgage_portfolio_diff: Items found *ONLY* in portfolio 1", DAY, ID);
        printf("\n\t\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
        printf("\n\tNo\tactive\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 


        for (j=0; j<exclusive_in_m1.size; j++)
        {
            i = exclusive_in_m1.array[j];
            
            printf("\n\t%2d/%2d\t%4d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
                    i+1,
                    (*m1).size,
                    (*m1).array[i].active,
                    (*m1).array[i].household_id,
                    (*m1).array[i].object_id,
                    (*m1).array[i].monthly_principal,
                    (*m1).array[i].monthly_interest,
                    (*m1).array[i].outstanding_principal,
                    (*m1).array[i].outstanding_interest,
                    (*m1).array[i].monthly_total_repayment,
                    (*m1).array[i].initial_duration,
                    (*m1).array[i].current_duration,
                    (*m1).array[i].status,         
                    (*m1).array[i].arrears_counter,
                    (*m1).array[i].npl_counter,
                    ((*m1).array[i].active==0));
        }    
        printf("\nSum outstanding principal:\t%f", sum1);
        
        //PRINT OUT MORTGAGE PORTFOLIO m2
        printf("\n\nIT %d ID: %d mortgage_portfolio_diff: Items found *ONLY* in portfolio 2", DAY, ID);
        printf("\n\t\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
        printf("\n\tNo\tactive\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 
     

        for (j=0; j<exclusive_in_m2.size; j++)
        {
            i = exclusive_in_m2.array[j];
            
            printf("\n\t%2d/%2d\t%4d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
                    i+1,
                    (*m2).size,
                    (*m2).array[i].active,
                    (*m2).array[i].household_id,
                    (*m2).array[i].object_id,
                    (*m2).array[i].monthly_principal,
                    (*m2).array[i].monthly_interest,
                    (*m2).array[i].outstanding_principal,
                    (*m2).array[i].outstanding_interest,
                    (*m2).array[i].monthly_total_repayment,
                    (*m2).array[i].initial_duration,
                    (*m2).array[i].current_duration,
                    (*m2).array[i].status,         
                    (*m2).array[i].arrears_counter,
                    (*m2).array[i].npl_counter,
                    ((*m2).array[i].active==0));
        }    
        printf("\nSum outstanding principal:\t%f (%f)", sum2, diff);
	} //print_output_to_stdout


	int print_output_to_files = 1;

	if(print_output_to_files)
	{
        //Print output to files: agent-specific
        FILE *file1;
        char *filename;

		#ifdef _DEBUG_MODE        
		if (PRINT_DEBUG_FILES_BANKS)
		{                       
			filename = malloc(40*sizeof(char));
			filename[0]=0;
			
			if (ID>1e4) fprintf(stderr, "\n\nWARNING: In print output to files, agent ID>1e4 requires longer string buffer for filenames.");

			sprintf(filename, "Bank_balance_sheet_%4d.txt", ID);
			//strcpy(filename, "its/Bank_balance_sheet_%d.txt");      
			file1 = fopen(filename,"a");
			
			//PRINT STATEMENTS
			//format: fprintf(file1,"\nIT %d %f %f", DAY,RESERVES,DEPOSITS);

			// Summary:
			//PRINT OUT MORTGAGE PORTFOLIO DIFF RESULTS
			fprintf(file1,"\n\nIT %d ID: %d mortgage_portfolio_diff", DAY, ID);
			fprintf(file1,"\nInputs:\n\tm1 size %d\n\tm2 size %d\n\tTotal %d", (*m1).size, (*m2).size, (*m1).size + (*m2).size);

			fprintf(file1,"\nSummary:");
			fprintf(file1,"\n\tpresent_in_both_using_m1_index:\t%d", present_in_both_using_m1_index.size);
			fprintf(file1,"\n\tpresent_in_both_using_m2_index:\t%d", present_in_both_using_m2_index.size);
	        fprintf(file1,"\n");
			if (present_in_both_using_m1_index.size != present_in_both_using_m2_index.size)
				fprintf(file1," <-- FAIL PRESENT IN BOTH SIZES INCONSISTENT");

	        fprintf(file1,"\n\texclusive_in_m1:\t\t%d", exclusive_in_m1.size);
			if ( present_in_both_using_m1_index.size + exclusive_in_m1.size != (*m1).size )
				fprintf(file1," <-- FAIL M1 SIZES INCONSISTENT");

	        fprintf(file1,"\n\texclusive_in_m2:\t\t%d", exclusive_in_m2.size);
			if ( present_in_both_using_m2_index.size + exclusive_in_m2.size != (*m2).size )
				fprintf(file1," <-- FAIL M2 SIZES INCONSISTENT");

	        fprintf(file1,"\n\tTotal:\t\t\t\t%d", exclusive_in_m1.size + exclusive_in_m2.size);

	        fprintf(file1,"\n");
	        if ( present_in_both_using_m1_index.size + exclusive_in_m1.size != (*m1).size )
	            fprintf(file1," <-- FAIL TOTAL SIZES INCONSISTENT");
	        
	        fprintf(file1,"\n");
	        if ( present_in_both_using_m2_index.size + exclusive_in_m2.size != (*m2).size )
	            fprintf(file1," <-- FAIL TOTAL SIZES INCONSISTENT");
	        
	        fprintf(file1,"\n\tSum Total:\t\t\t%d", present_in_both_using_m1_index.size + present_in_both_using_m2_index.size + exclusive_in_m1.size + exclusive_in_m2.size);
			if ( present_in_both_using_m1_index.size + present_in_both_using_m2_index.size + exclusive_in_m1.size + exclusive_in_m2.size != (*m1).size + (*m2).size )
				fprintf(file1," <-- FAIL TOTAL SIZES INCONSISTENT");

	        
			fprintf(file1,"\n\nDetailed output:");

			fprintf(file1,"\n\n\tpresent_in_both_using_m1_index:\t%2d", present_in_both_using_m1_index.size);
			fprintf(file1,"\n[");
			for (i=0; i<present_in_both_using_m1_index.size; i++)
				fprintf(file1,"%2d|", present_in_both_using_m1_index.array[i]);		
			fprintf(file1,"]");

			fprintf(file1,"\n\n\tpresent_in_both_using_m2_index:\t%2d", present_in_both_using_m2_index.size);
			fprintf(file1,"\n[");
			for (i=0; i<present_in_both_using_m2_index.size; i++)
				fprintf(file1,"%2d|", present_in_both_using_m2_index.array[i]);
			fprintf(file1,"]");
	        
	        
	        fprintf(file1,"\n\n\tsorted_data_m1:\t%2d", n1);
	        fprintf(file1,"\n[");
	        for (i=0; i<n1; i++)
	            fprintf(file1,"%2.0f|", sorted_data_m1[i]);
	        fprintf(file1,"]");
	        
	        fprintf(file1,"\n\n\tsorted_data_m2:\t%2d", n2);
	        fprintf(file1,"\n[");
	        for (i=0; i<n2; i++)
	            fprintf(file1,"%2.0f|", sorted_data_m2[i]);
	        fprintf(file1,"]");
	        
	        
			fprintf(file1,"\n\n\texclusive_in_m1:\t%2d", exclusive_in_m1.size);
			fprintf(file1,"\n[");
			for (i=0; i<exclusive_in_m1.size; i++)
				fprintf(file1,"%2d|", exclusive_in_m1.array[i]);
			fprintf(file1,"]");

			fprintf(file1,"\n\n\texclusive_in_m2:\t%2d", exclusive_in_m2.size);
			fprintf(file1,"\n[");
			for (i=0; i<exclusive_in_m2.size; i++)
				fprintf(file1,"%2d|", exclusive_in_m2.array[i]);
			fprintf(file1,"]");

	        
	        //PRINT OUT MORTGAGE PORTFOLIO m1
	        fprintf(file1,"\n\nIT %d ID: %d mortgage_portfolio_diff: Items found *ONLY* in portfolio 1 -- file output", DAY, ID);
	        fprintf(file1,"\n\t\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
	        fprintf(file1,"\n\tNo\tactive\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 

	        sum1 = 0.0;
	        for (j=0; j<exclusive_in_m1.size; j++)
	        {
	            i = exclusive_in_m1.array[j];
	            sum1 += (*m1).array[i].outstanding_principal;
	        }

	        for (j=0; j<exclusive_in_m1.size; j++)
	        {
	            i = exclusive_in_m1.array[j];
	            
	            fprintf(file1, "\n\t%2d/%2d\t%4d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
	                    i+1,
	                    (*m1).size,
	                    (*m1).array[i].active,
	                    (*m1).array[i].household_id,
	                    (*m1).array[i].object_id,
	                    (*m1).array[i].monthly_principal,
	                    (*m1).array[i].monthly_interest,
	                    (*m1).array[i].outstanding_principal,
	                    (*m1).array[i].outstanding_interest,
	                    (*m1).array[i].monthly_total_repayment,
	                    (*m1).array[i].initial_duration,
	                    (*m1).array[i].current_duration,
	                    (*m1).array[i].status,         
	                    (*m1).array[i].arrears_counter,
	                    (*m1).array[i].npl_counter,
	                    ((*m1).array[i].active==0));
	        }    
	        fprintf(file1,"\nSum outstanding principal:\t%f", sum1);
	        
	        //PRINT OUT MORTGAGE PORTFOLIO m2
	        fprintf(file1,"\n\nIT %d ID: %d mortgage_portfolio_diff: Items found *ONLY* in portfolio 2 -- file output", DAY, ID);
	        fprintf(file1,"\n\t\t\t\t\t\tmonthly\t\t\tmonthly\t\t\toutstanding\t\toutstanding\t\tmonthly\t\t\tinitial\t\tcurrent\t\tcurrent\tarrears\tnpl");
	        fprintf(file1,"\n\tNo\tactive\tHH ID\tobject\t\tprincipal\t\tinterest\t\tprincipal\t\tinterest\t\tannuity\t\t\tduration\tduration\tstatus\tcounter\tcounter\tpaid off"); 
	        
	        
	        for (j=0; j<exclusive_in_m2.size; j++)
	        {
	            i = exclusive_in_m2.array[j];
	            
	            fprintf(file1, "\n\t%2d/%2d\t%4d\t%4d\t%4d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t%d\t%d\t%d",
	                    i+1,
	                    (*m2).size,
	                    (*m2).array[i].active,
	                    (*m2).array[i].household_id,
	                    (*m2).array[i].object_id,
	                    (*m2).array[i].monthly_principal,
	                    (*m2).array[i].monthly_interest,
	                    (*m2).array[i].outstanding_principal,
	                    (*m2).array[i].outstanding_interest,
	                    (*m2).array[i].monthly_total_repayment,
	                    (*m2).array[i].initial_duration,
	                    (*m2).array[i].current_duration,
	                    (*m2).array[i].status,         
	                    (*m2).array[i].arrears_counter,
	                    (*m2).array[i].npl_counter,
	                    ((*m2).array[i].active==0));
	        }    
	        fprintf(file1,"\nSum outstanding principal:\t%f\t(%f)", sum2, diff);
	        
			fclose(file1);
			free(filename);
		}                
	    #endif
	} //print_output_to_files

	free_int_array(&exclusive_in_m1);
	free_int_array(&exclusive_in_m2);
	free_int_array(&present_in_both_using_m1_index);
	free_int_array(&present_in_both_using_m2_index);

}

/* \fn: int compute_mean_interest_rates_all_mortgages(void)
 * \brief: Function to compute the weighted and non-weighted mean interest rates for all mortgages in stock.
 */
int compute_mean_interest_rates_all_mortgages(void)
{
	/** NEW MORTGAGES ***/
	MEAN_INTEREST_RATE_NEW_MORTGAGES = 0.0;
	WEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES = 0.0;

	//Set MEAN_INTEREST_RATE for NEW_MORTGAGES
	if (BANK_NO_MORTGAGES_NEW>0)
	{	
		//Non-weighted mean interest rate:
		MEAN_INTEREST_RATE_NEW_MORTGAGES = SUM_INTEREST_RATES_NEW_MORTGAGES/BANK_NO_MORTGAGES_NEW;

		//Volume-weighted mean interest rate:
		WEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES = WEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES/BANK_MORTGAGES_NEW_BUSINESS;

		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_FUNDING_COSTS/BANK_MORTGAGES_NEW_BUSINESS;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_POLICY_RATE/BANK_MORTGAGES_NEW_BUSINESS;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_MORTGAGES_WACF/BANK_MORTGAGES_NEW_BUSINESS;
	}


	/** ALL MORTGAGES ***/
	int i;
	double interest_rate_annual;

	MEAN_INTEREST_RATE_ALL_MORTGAGES = 0.0;
	WEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES = 0.0;
	SUM_INTEREST_RATES_ALL_MORTGAGES = 0.0;
	WEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES = 0.0;
	TOTAL_MORTGAGE_CREDIT = 0.0;

	//Non-weighted and volume-weighted sums of interest rates
	for(i=0; i<MORTGAGE_PORTFOLIO.size; i++)
	{
		//Using annualized interest
		interest_rate_annual = pow(1+MORTGAGE_PORTFOLIO.array[i].monthly_interest, 12) - 1;

/*		#ifdef _DEBUG_MODE        
	    if (PRINT_DEBUG_MACROPRU)
	    {
			printf("\n\tIT %d ID %d interest_rate_annual: %f", DAY, ID, interest_rate_annual);
	    }              
	    #endif
*/

		SUM_INTEREST_RATES_ALL_MORTGAGES += interest_rate_annual;
		
		WEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES += interest_rate_annual * MORTGAGE_PORTFOLIO.array[i].outstanding_principal;

		TOTAL_MORTGAGE_CREDIT	 += MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
	}

	BANK_NO_MORTGAGES = MORTGAGE_PORTFOLIO.size;

	//Set MEAN_INTEREST_RATE for ALL MORTGAGES
	if (BANK_NO_MORTGAGES>0)
	{	
		//Non-weighted mean interest rate:
		MEAN_INTEREST_RATE_ALL_MORTGAGES = SUM_INTEREST_RATES_ALL_MORTGAGES/BANK_NO_MORTGAGES;

		//Volume-weighted mean interest rate:
		WEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES = WEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES/TOTAL_MORTGAGE_CREDIT;
	}


	#ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_MACROPRU)
    {
        printf("\n\n\tIT %d ID %d compute_mean_interest_rates_all_mortgages", DAY, ID);

		printf("\n\n-- Non-weighted MORTGAGES interest_rate: ALL");
		printf("\n\tMEAN_INTEREST_RATE_ALL_MORTGAGES: %f", MEAN_INTEREST_RATE_ALL_MORTGAGES);
		printf("\n\tSUM_INTEREST_RATES_ALL_MORTGAGES: %f", SUM_INTEREST_RATES_ALL_MORTGAGES);
		printf("\n\tBANK_NO_MORTGAGES: %d", BANK_NO_MORTGAGES);

		printf("\n\n-- Volume-weighted MORTGAGES interest_rate: ALL");
		printf("\n\tWEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES: %f", WEIGHTED_MEAN_INTEREST_RATE_ALL_MORTGAGES);
		printf("\n\tWEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES: %f", WEIGHTED_SUM_INTEREST_RATES_ALL_MORTGAGES);
		printf("\n\tTOTAL_MORTGAGE_CREDIT: %f", TOTAL_MORTGAGE_CREDIT);

		printf("\n\n-- Non-weighted MORTGAGES interest_rate: NEW");
		printf("\n\tMEAN_INTEREST_RATE_NEW_MORTGAGES: %f", MEAN_INTEREST_RATE_NEW_MORTGAGES);
		printf("\n\tSUM_INTEREST_RATES_NEW_MORTGAGES: %f", SUM_INTEREST_RATES_NEW_MORTGAGES);
		printf("\n\tBANK_NO_MORTGAGES_NEW: %d", BANK_NO_MORTGAGES_NEW);

		printf("\n\n-- Volume-weighted MORTGAGES interest_rate: NEW");
		printf("\n\tWEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES: %f", WEIGHTED_MEAN_INTEREST_RATE_NEW_MORTGAGES);
		printf("\n\tWEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES: %f", WEIGHTED_SUM_INTEREST_RATES_NEW_MORTGAGES);
		printf("\n\tBANK_MORTGAGES_NEW_BUSINESS: %f", BANK_MORTGAGES_NEW_BUSINESS);
    }              
    #endif

	return 0;
}

/* \fn: int compute_mean_interest_rates_all_firm_loans(void)
 * \brief: Function to compute the weighted and non-weighted mean interest rates for all firm loans in stock.
 */
int compute_mean_interest_rates_all_firm_loans(void)
{
	/** NEW FIRM LOANS ***/
	MEAN_INTEREST_RATE_NEW_FIRM_LOANS = 0.0;
	WEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS = 0.0;
    
	if (BANK_NO_FIRM_LOANS_NEW>0)
	{	
		//Non-weighted mean interest rate:
		MEAN_INTEREST_RATE_NEW_FIRM_LOANS = SUM_INTEREST_RATES_NEW_FIRM_LOANS/BANK_NO_FIRM_LOANS_NEW;

		//Volume-weighted mean interest rate:
		WEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS = WEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS/BANK_FIRM_LOANS_NEW_BUSINESS;        

		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_FUNDING_COSTS/BANK_FIRM_LOANS_NEW_BUSINESS;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_POLICY_RATE/BANK_FIRM_LOANS_NEW_BUSINESS;
		WEIGHTED_MEAN_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF = WEIGHTED_SUM_INTEREST_RATE_SPREADS_NEW_FIRM_LOANS_WACF/BANK_FIRM_LOANS_NEW_BUSINESS;
	}

	/** ALL FIRM LOANS ***/
	MEAN_INTEREST_RATE_ALL_FIRM_LOANS = 0.0;
	WEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS = 0.0;

	SUM_INTEREST_RATES_ALL_FIRM_LOANS = 0.0;
	WEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS = 0.0;
	FIRM_CREDIT = 0.0;

	int i;
	double interest_rate_annual;

	//Non-weighted and volume-weighted sums of interest rates
	for(i=0; i<FIRM_LOAN_PORTFOLIO.size; i++)
	{
		//Using annualized interest
		interest_rate_annual = FIRM_LOAN_PORTFOLIO.array[i].interest_rate;

/*		#ifdef _DEBUG_MODE        
	    if (PRINT_DEBUG_MACROPRU)
	    {
			printf("\n\tIT %d ID %d interest_rate_annual: %f", DAY, ID, interest_rate_annual);
	    }              
	    #endif
*/

		SUM_INTEREST_RATES_ALL_FIRM_LOANS += interest_rate_annual;

		WEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS += interest_rate_annual * FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal;

		FIRM_CREDIT += FIRM_LOAN_PORTFOLIO.array[i].outstanding_principal;
	}

	BANK_NO_FIRM_LOANS = FIRM_LOAN_PORTFOLIO.size;

	//Set MEAN_INTEREST_RATE for ALL_FIRM_LOANS
	if (BANK_NO_FIRM_LOANS>0)
	{	
		//Non-weighted mean interest rate:
		MEAN_INTEREST_RATE_ALL_FIRM_LOANS = SUM_INTEREST_RATES_ALL_FIRM_LOANS/BANK_NO_FIRM_LOANS;

		//Volume-weighted mean interest rate:
		WEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS = WEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS/FIRM_CREDIT;
	}

 	/* Aggregate Expected Loss Ratio:
	 * (EL/L). This is computed as a ratio of two sums.
	 * First we sum ELs across 4 categories, firm loans and mortgage loans, for PL and NPl.
	 * The 2nd sum is the total loan stock across the same 4 categories.
	 */
	EL_RATIO = 0.0;
	if ((FIRM_CREDIT + TOTAL_MORTGAGE_CREDIT) > EPSILON)
	 	EL_RATIO = (FIRM_EL_PL_YEAR + HH_EL_PL_YEAR + HH_EL_NPL_YEAR) / (FIRM_CREDIT + TOTAL_MORTGAGE_CREDIT);

/*
	#ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_MACROPRU)
    {
        printf("\n\n\tIT %d ID %d compute_mean_interest_rates_all_firm_loans", DAY, ID);

		printf("\n\n-- Non-weighted FIRM_LOANS interest_rate: ALL");
		printf("\n\tMEAN_INTEREST_RATE_ALL_FIRM_LOANS: %f", MEAN_INTEREST_RATE_ALL_FIRM_LOANS);
		printf("\n\tSUM_INTEREST_RATES_ALL_FIRM_LOANS: %f", SUM_INTEREST_RATES_ALL_FIRM_LOANS);
		printf("\n\tBANK_NO_FIRM_LOANS: %d", BANK_NO_FIRM_LOANS);

		printf("\n\n-- Volume-weighted FIRM_LOANS interest_rate: ALL");
		printf("\n\tWEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS: %f", WEIGHTED_MEAN_INTEREST_RATE_ALL_FIRM_LOANS);
		printf("\n\tWEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS: %f", WEIGHTED_SUM_INTEREST_RATES_ALL_FIRM_LOANS);
		printf("\n\tFIRM_CREDIT: %f", FIRM_CREDIT);

		printf("\n\n-- Non-weighted FIRM_LOANS interest_rate: NEW");
		printf("\n\tMEAN_INTEREST_RATE_NEW_FIRM_LOANS: %f", MEAN_INTEREST_RATE_NEW_FIRM_LOANS);
		printf("\n\tSUM_INTEREST_RATES_NEW_FIRM_LOANS: %f", SUM_INTEREST_RATES_NEW_FIRM_LOANS);
		printf("\n\tBANK_NO_FIRM_LOANS_NEW: %d", BANK_NO_FIRM_LOANS_NEW);

		printf("\n\n-- Volume-weighted FIRM_LOANS interest_rate: NEW");
		printf("\n\tWEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS: %f", WEIGHTED_MEAN_INTEREST_RATE_NEW_FIRM_LOANS);
		printf("\n\tWEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS: %f", WEIGHTED_SUM_INTEREST_RATES_NEW_FIRM_LOANS);
		printf("\n\tBANK_FIRM_LOANS_NEW_BUSINESS: %f", BANK_FIRM_LOANS_NEW_BUSINESS);
    }              
    #endif
*/
	
	return 0;
}


/* \fn: int compute_acceptance_rates_mortgage_loans(void)
 * \brief: Function to compute the ratios of mortgages 
 * \brief: accepted/offered, fixed/offered, fixed/accepted, variable/offered, variable/accepted.
 */
int compute_acceptance_rates_mortgage_loans(void)
{

	MRTG_VARIABLE_PCT_OFFERED_FLOW_RATIO   = 0.0;
	MRTG_FIXED_PCT_OFFERED_FLOW_RATIO      = 0.0;
	MRTG_VARIABLE_PCT_ACCEPTED_FLOW_RATIO  = 0.0;
	MRTG_FIXED_PCT_ACCEPTED_FLOW_RATIO     = 0.0;
	MRTG_VARIABLE_ACCEPTED_PCT_FLOW_RATIO  = 0.0;
	MRTG_FIXED_ACCEPTED_PCT_FLOW_RATIO     = 0.0;
	MRTG_ACCEPTED_PCT_FLOW_RATIO           = 0.0;
                                               
	MRTG_VARIABLE_PCT_OFFERED_CUM_RATIO    = 0.0;
	MRTG_FIXED_PCT_OFFERED_CUM_RATIO       = 0.0;
	MRTG_VARIABLE_PCT_ACCEPTED_CUM_RATIO   = 0.0;
	MRTG_FIXED_PCT_ACCEPTED_CUM_RATIO      = 0.0;
	MRTG_FIXED_ACCEPTED_PCT_CUM_RATIO      = 0.0;
	MRTG_VARIABLE_ACCEPTED_PCT_CUM_RATIO   = 0.0;
	MRTG_ACCEPTED_PCT_CUM_RATIO            = 0.0;

	//Flow counters:
	//Subclass/Offered:
	if(MRTG_TOTAL_OFFERED_FLOW_COUNT>0) MRTG_VARIABLE_PCT_OFFERED_FLOW_RATIO = MRTG_VARIABLE_OFFERED_FLOW_COUNT/(double)MRTG_TOTAL_OFFERED_FLOW_COUNT;
	if(MRTG_TOTAL_OFFERED_FLOW_COUNT>0) MRTG_FIXED_PCT_OFFERED_FLOW_RATIO = MRTG_FIXED_OFFERED_FLOW_COUNT/(double)MRTG_TOTAL_OFFERED_FLOW_COUNT;

	//Subclass/Accepted:
	if(MRTG_TOTAL_ACCEPTED_FLOW_COUNT>0) MRTG_VARIABLE_PCT_ACCEPTED_FLOW_RATIO = MRTG_VARIABLE_ACCEPTED_FLOW_COUNT/(double)MRTG_TOTAL_ACCEPTED_FLOW_COUNT;
	if(MRTG_TOTAL_ACCEPTED_FLOW_COUNT>0) MRTG_FIXED_PCT_ACCEPTED_FLOW_RATIO = MRTG_FIXED_ACCEPTED_FLOW_COUNT/(double)MRTG_TOTAL_ACCEPTED_FLOW_COUNT;

	//Functional Accepted/Offered:
	if(MRTG_VARIABLE_OFFERED_FLOW_COUNT>0) MRTG_VARIABLE_ACCEPTED_PCT_FLOW_RATIO = MRTG_VARIABLE_ACCEPTED_FLOW_COUNT/(double)MRTG_VARIABLE_OFFERED_FLOW_COUNT;
	if(MRTG_FIXED_OFFERED_FLOW_COUNT>0) MRTG_FIXED_ACCEPTED_PCT_FLOW_RATIO = MRTG_FIXED_ACCEPTED_FLOW_COUNT/(double)MRTG_FIXED_OFFERED_FLOW_COUNT;

	//Total Accepted/Offered:
	if(MRTG_TOTAL_OFFERED_FLOW_COUNT>0) MRTG_ACCEPTED_PCT_FLOW_RATIO = MRTG_TOTAL_ACCEPTED_FLOW_COUNT/(double)MRTG_TOTAL_OFFERED_FLOW_COUNT;

	//Cumulative counters:
	//Subclass/offered:
	if(MRTG_TOTAL_OFFERED_CUM_COUNT>0) MRTG_VARIABLE_PCT_OFFERED_CUM_RATIO = MRTG_VARIABLE_OFFERED_CUM_COUNT/(double)MRTG_TOTAL_OFFERED_CUM_COUNT;
	if(MRTG_TOTAL_OFFERED_CUM_COUNT>0) MRTG_FIXED_PCT_OFFERED_CUM_RATIO = MRTG_FIXED_OFFERED_CUM_COUNT/(double)MRTG_TOTAL_OFFERED_CUM_COUNT;

	//Subclass/Accepted:
	if(MRTG_TOTAL_ACCEPTED_CUM_COUNT>0) MRTG_VARIABLE_PCT_ACCEPTED_CUM_RATIO = MRTG_VARIABLE_ACCEPTED_CUM_COUNT/(double)MRTG_TOTAL_ACCEPTED_CUM_COUNT;
	if(MRTG_TOTAL_ACCEPTED_CUM_COUNT>0) MRTG_FIXED_PCT_ACCEPTED_CUM_RATIO = MRTG_FIXED_ACCEPTED_CUM_COUNT/(double)MRTG_TOTAL_ACCEPTED_CUM_COUNT;

	//Functional Accepted/Offered:
	if(MRTG_FIXED_OFFERED_CUM_COUNT>0) MRTG_FIXED_ACCEPTED_PCT_CUM_RATIO = MRTG_FIXED_ACCEPTED_CUM_COUNT/(double)MRTG_FIXED_OFFERED_CUM_COUNT;
	if(MRTG_VARIABLE_OFFERED_CUM_COUNT>0) MRTG_VARIABLE_ACCEPTED_PCT_CUM_RATIO = MRTG_VARIABLE_ACCEPTED_CUM_COUNT/(double)MRTG_VARIABLE_OFFERED_CUM_COUNT;

	//Total Accepted/Offered:
	if(MRTG_TOTAL_OFFERED_CUM_COUNT>0) MRTG_ACCEPTED_PCT_CUM_RATIO = MRTG_TOTAL_ACCEPTED_CUM_COUNT/(double)MRTG_TOTAL_OFFERED_CUM_COUNT;


	return 0;
}
