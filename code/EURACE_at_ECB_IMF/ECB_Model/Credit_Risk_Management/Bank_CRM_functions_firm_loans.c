/*********************************
 * Bank_CRM_functions_firm_loans.c 
 * Bank auxiliary functions for Credit Risk Management.
 * *********************************
 * History:
 * 28.05.2014 first version
 * 12.06.2016 Added PD and LGD
 * 14.10.2016 Updated PD and LGD computations
 * 27.10.2017 Fixed LLR updating and IRB shortfall
 * For firms: 
 * - PD  based on backward-looking realized default rates
 * 	(Note: all non-performing loans are in fact defaulted loans: 
 * 	no deliquency period of 60 days/3 months is taken into account yet in code below)
 * - LGD based on backward-looking realized loss rates
 * Added firm-specific vars:
 * - FIRM_PD_PL_YEAR, FIRM_LGD_PL_YEAR, 
 * - FIRM_PD_CYCLE, FIRM_LGD_CYCLE,
 * - FIRM_PD_PL_REG, FIRM_LGD_PL_REG
 *********************************/
//For Main model
#include <gsl/gsl_sort.h>

#include "../../header.h"
#include "../../Bank_agent_header.h"
#include "../../my_library_header.h"
#include "Bank_aux_headers.h"
#include "../../Statistical_Office/balance_sheet_operations.h"

//For ECB sub-model
//#include "../header.h"
//#include "../Bank_agent_header.h"

/** \fn int Bank_set_pd_lgd_daily_firm_loans(void)
 * \Brief Function to set the daily data for PD and LGD
 * \Brief Use: to compute the quarterly pd_point_in_time and lgd_point_in_time
 * \Brief Frequency: daily
 * \Brief After Function: 
 * \Brief Called in Function: Bank_receive_installment()
 * \Brief Before Function: 
 * \Brief Inputs: bankruptcy, installment message
 * \Brief Expected: FIRM_NPL_DAILY, FIRM_LGD_PL_DAILY, FIRM_PL_DAILY: dynamic int array (all length 60)
 * \Brief FIRM_NPL_DAILY: firm non-performing loans
 * \Brief FIRM_LGD_PL_DAILY: loss given default
 * \Brief FIRM_PL_DAILY:  firm performing loans
 */
int Bank_set_pd_lgd_daily_firm_loans(void)
{
    //DAILY ACTIVITY
	//outstanding loans that are still performing: COUNT installment_messages.
	//outstanding loans that are non-performing (defaulted): COUNT bankruptcy_messages.	
	int npl_count=0;
	int pl_count=0;
	double pl,npl=0.0;
	double ll_total=0.0;
	double ld_total=0.0;

	// Count performing loans today
	START_FIRM_INSTALLMENT_MESSAGE_LOOP
	    pl_count++;
	FINISH_FIRM_INSTALLMENT_MESSAGE_LOOP
	
	//STUB: Firm loan NPL
	//We do not yet have NPL messages for firm loans, assume firm loans are either PL or in default
/*
	START_FIRM_LOAN_NPL_MESSAGE_LOOP
	  npl_count++;
	  ld_total += firm_loan_npl_message->loan_default;	//loan_default = outstanding principal
	  ll_total += firm_loan_npl_message->loan_loss;		//loan_loss = loan_default - collateral_value;
	FINISH_FIRM_LOAN_NPL_MESSAGE_LOOP
*/

	// Count non-performing loans today
	// Assume: LD and LL are the same, because NPL and default are the same (for now)
	START_FIRM_BANKRUPTCY_MESSAGE_LOOP
	  npl_count++;	  
	  ld_total += firm_bankruptcy_message->bad_debt;
	  ll_total += firm_bankruptcy_message->bad_debt;
	FINISH_FIRM_BANKRUPTCY_MESSAGE_LOOP

	//Performing (pl) and non-performing (npl) volume of firm loans
	pl = FIRM_CREDIT - ll_total;
	npl = ll_total;

	// Add item at end of array
	add_double(&FIRM_PL_DAILY, pl);
	add_double(&FIRM_NPL_DAILY, npl);
	add_double(&FIRM_LL_DAILY, ll_total);
	add_double(&FIRM_LD_DAILY, ld_total);	


	// Remove item from head index 0
	remove_double(&FIRM_PL_DAILY, 0);
	remove_double(&FIRM_NPL_DAILY, 0);
	remove_double(&FIRM_LL_DAILY, 0);
	remove_double(&FIRM_LD_DAILY, 0);

	return 0;
}	


/** \fn int Bank_set_pd_lgd_quarterly_firm_loans(void)
 * \Brief Function to set the quarterly data for pd_point_in_time and lgd_point_in_time
 * \brief REF: Model Specifications, 2.10.2017
 * \Brief Frequency: runs daily
 * \Brief After Function: 
 * \Brief Called in Function: Bank_receive_installment()
 * \Brief Before Function: 
 * \Brief Inputs: None
 * \Brief Expected: 
 */
int Bank_set_pd_lgd_quarterly_firm_loans(void)
{
    int i;

	double sum_loan_loss = 0.0;
	double sum_loan_default = 0.0;
	
	// sum daily Loan Loss flow across past quarter (loan volume in default - value collateral)
	for (i=0; i<FIRM_LL_DAILY.size; i++)
		sum_loan_loss += FIRM_LL_DAILY.array[i];
	  
	// sum daily Loan Default flow  across past quarter (loan volume in default)
	for (i=0; i<FIRM_LD_DAILY.size; i++)
	    sum_loan_default += FIRM_LD_DAILY.array[i];
	
	//Point-in-time PD	
    FIRM_PD_PL_QTR = 0.0;
    if( FIRM_PL_DAILY.array[0] > EPSILON)
           FIRM_PD_PL_QTR = sum_loan_default/FIRM_PL_DAILY.array[0];

    #ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_FLOORS_AND_CEILINGS)
    {            
        //Warning for floor-level:
        if(FIRM_PD_PL_QTR < CONST_MIN_PD_CORP)
            fprintf(stderr, "\nIT %d ID %d WARNING in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_QTR<CONST_MIN_PD_CORP: FIRM_PD_PL_QTR = %f\nsum_loan_default = %f\nFIRM_PL_DAILY.array[0] = %f\nSetting FIRM_PD_PL_QTR = CONST_MIN_PD_CORP = %f", DAY, ID, FIRM_PD_PL_QTR, sum_loan_default, FIRM_PL_DAILY.array[0], CONST_MIN_PD_CORP);

        //Warning for Ceiling-level    
        if(FIRM_PD_PL_QTR > CONST_MAX_PD_CORP)
            fprintf(stderr, "\nIT %d ID %d WARNING in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_QTR>CONST_MAX_PD_CORP: FIRM_PD_PL_QTR = %f\nsum_loan_default = %f\nFIRM_PL_DAILY.array[0] = %f\nSetting FIRM_PD_PL_QTR = CONST_MAX_PD_CORP = %f", DAY, ID, FIRM_PD_PL_QTR, sum_loan_default, FIRM_PL_DAILY.array[0], CONST_MAX_PD_CORP);
    }              
    #endif

    //Floor-level
    FIRM_PD_PL_QTR = max(FIRM_PD_PL_QTR, CONST_MIN_PD_CORP);
    FIRM_PD_NPL_YEAR = 1;
    
	//Ceiling-level
	FIRM_PD_PL_QTR = min(FIRM_PD_PL_QTR, CONST_MAX_PD_CORP);

	//During initial transient period, set firm_pd to a low constant (default 0.25%, i.e. 0.0025)
	if (DAY<CONST_INIT_TRANSIENT_PERIOD)
		FIRM_PD_PL_QTR = CONST_INIT_PD_CORP;

	//Point-in-time PD, scaled-up to one year
	FIRM_PD_PL_YEAR = 1 - pow(1-FIRM_PD_PL_QTR, 4);
	
	//Point-in-time LGD: No collateral means LGD=1
	//FIRM_LGD_PL_YEAR = 1;

	//Effective bad debt/write-down ratio over a 1-year moving window with daily update
	//See Credit/Bank_Credit_Functions.c, line 1200: FIRM_WD_FACTOR_MA = sum_bad_debt / sum_wd_principal;
	FIRM_LGD_PL_YEAR = FIRM_WD_FACTOR_MA;

	//Safety checks:
	if(FIRM_PD_PL_QTR > 1.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_QTR>1.0: FIRM_PD_PL_QTR = %f\nsum_loan_default = %f\nFIRM_PL_DAILY.array[0] = %f", DAY, ID, FIRM_PD_PL_QTR, sum_loan_default, FIRM_PL_DAILY.array[0]);

	if(FIRM_PD_PL_QTR < 0.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_QTR<0.0: FIRM_PD_PL_QTR = %f\nsum_loan_default = %f\nFIRM_PL_DAILY.array[0] = %f", DAY, ID, FIRM_PD_PL_QTR, sum_loan_default, FIRM_PL_DAILY.array[0]);

	if(FIRM_PD_PL_YEAR > 1.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_YEAR>1.0: FIRM_PD_PL_YEAR = %f", DAY, ID, FIRM_PD_PL_YEAR);

	if(FIRM_PD_PL_YEAR < 0.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_PD_PL_YEAR<0.0: FIRM_PD_PL_YEAR = %f", DAY, ID, FIRM_PD_PL_YEAR);

	if(FIRM_LGD_PL_YEAR > 1.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_LGD_PL_YEAR>1.0: FIRM_LGD_PL_YEAR = %f", DAY, ID, FIRM_LGD_PL_YEAR);

	if(FIRM_LGD_PL_YEAR < 0.0)
		fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_firm_loans()\nFIRM_LGD_PL_YEAR<0.0: FIRM_LGD_PL_YEAR = %f", DAY, ID, FIRM_LGD_PL_YEAR);

	assert(FIRM_PD_PL_QTR<1+1e-7);
	assert(FIRM_PD_PL_QTR>-1e-7);

	assert(FIRM_PD_PL_YEAR<1+1e-7);
	assert(FIRM_PD_PL_YEAR>-1e-7);

	assert(FIRM_LGD_PL_YEAR<1+1e-7);
	assert(FIRM_LGD_PL_YEAR>-1e-7);
	

	// Add item at end of array FIRM_PD_CYCLE and FIRM_LGD_CYCLE
	add_double(&FIRM_PD_CYCLE, FIRM_PD_PL_YEAR);
	add_double(&FIRM_LGD_CYCLE, FIRM_LGD_PL_YEAR);

	// Remove item from head index 0
	remove_double(&FIRM_PD_CYCLE, 0);
	remove_double(&FIRM_LGD_CYCLE, 0);

		// Calculate MA's
	int n=0;
	double sum=0;
	for(i=0;i<4;i++)
	{
		if (FIRM_PD_CYCLE.size-i-1>0)
		{
			sum+=FIRM_PD_CYCLE.array[FIRM_PD_CYCLE.size-i-1];
			n++;
		}
	}
	FIRM_PD_PL_YEAR_MA = sum/n;

	n=0;
	sum=0;
	for(i=0;i<4;i++)
	{
		if (FIRM_LGD_CYCLE.size-i-1>0)
		{
			sum+=FIRM_LGD_CYCLE.array[FIRM_LGD_CYCLE.size-i-1];
			n++;
		}
	}
	FIRM_LGD_PL_YEAR_MA = sum/n;

    return 0;
}	

 /** \fn int Bank_set_pd_lgd_reg_firm_loans(void)
 * \Brief Function to compute PD and LGD across a time window of 5 years
 * \Brief Aggregate [5 year/20 quarters/60 months] of FIRM_PD_PL_YEAR and FIRM_LGD_PL_YEAR in a moving average.
 * \Brief Frequency: monthly, on last day of calendar month
 * \Brief After Function: 
 * \Brief Called in Function: Bank_receive_installment()
 * \Brief Before Function: 
 * \Brief Expected: FIRM_PD_CYCLE, FIRM_LGD_CYCLE: dynamic double array, length 20 (20 quarters, 5 years)
 * \Brief Expected: FIRM_PD_PL_REG, FIRM_LGD_PL_REG: double
 */
int Bank_set_pd_lgd_reg_firm_loans(void)
{
	int i;

	// On last day of calendar year: sum 20 quarterly PD and LGD across 5 years
	double sum1=0,sum2=0;

	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n\n Bank_set_pd_lgd_yearly_firm_loans: BEFORE summing cycle arrays\n");
	
		printf("\n FIRM_PD_CYCLE = [");
		for (i=0; i<FIRM_PD_CYCLE.size; i++)
			printf(" %.3f",FIRM_PD_CYCLE.array[i]);
		printf("]\n");
	
		printf("\n FIRM_LGD_CYCLE = [");
		for (i=0; i<FIRM_LGD_CYCLE.size; i++)
			printf(" %.3f",FIRM_LGD_CYCLE.array[i]);
		printf("]\n");
	}
	#endif

  // sum 20 quarters of PD ratio data
	for (i=0; i<FIRM_PD_CYCLE.size; i++)
		sum1 += FIRM_PD_CYCLE.array[i];
	
	// PD through-the-cycle: FIRM_PD_PL_REG = MA(FIRM_PD_PL_YEAR)
	//Mean:
	if(FIRM_PD_CYCLE.size>0) 
		FIRM_PD_PL_REG = sum1/FIRM_PD_CYCLE.size;
		
	/* 75th percentile of array FIRM_LGD_CYCLE*/
	int n = FIRM_LGD_CYCLE.size;
	
	double sorted_data[n];
		
	for (i=0; i<n; i++)
		sorted_data[i] = FIRM_LGD_CYCLE.array[i];

	gsl_sort(sorted_data, 1, n);

	//GSL function for percentiles
	//out = gsl_stats_quantile_from_sorted_data (sorted_data, 1, n, 0.75);
	
	//Simple: 75th percentile of 60 items is at element [44]
	FIRM_LGD_PL_REG = sorted_data[44];

	//Regulatory expected loss (Eq 19, 2nd term, and multiplied by principle outstanding)
	FIRM_EL_PL_REG = FIRM_PD_PL_REG * FIRM_LGD_PL_REG * FIRM_CREDIT;

	//Regulatory LGD downturn
	FIRM_LGD_NPL_REG = FIRM_LGD_PL_REG;
	
	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n\n Bank_set_pd_lgd_yearly_firm_loans: AFTER summing cycle arrays\n");

		printf("\n sum1 = [");
			printf(" %.3f", sum1);
		printf("]\n");
	
		printf("\n sum2 = [");
			printf(" %.3f", sum2);
		printf("]\n");

		printf("\n FIRM_PD_PL_REG = [");
			printf(" %.3f", FIRM_PD_PL_REG);
		printf("]\n");
	
		printf("\n FIRM_LGD_PL_REG = [");
			printf(" %.3f", FIRM_LGD_PL_REG);
		printf("]\n");
	}
	#endif
    
    return 0;
}

/** \fn Bank_set_risk_weight_firm_loans()
 * \brief Function to compute the Basel 2 risk-weight formula for firm loans.
 * \Brief Frequency: monthly, on first day of month
 * \Brief After Function: 
 * \Brief Called in Function:
 * \Brief Before Function: 
 * \brief Message input: None
 * \brief Message output: None
 * \brief UNITTEST: TODO
 */
int Bank_set_risk_weight_firm_loans()
{
	double pd,lgd;

	// During initial transient period: set to initial values, later on set to endogeneous values
	if (DAY < CONST_INIT_TRANSIENT_PERIOD)
	{
		pd = CONST_INIT_PD_CORP;
		lgd = CONST_INIT_LGD_CORP;
	}
	else
	{
		//Eq(3): use last 20 quarters: MA_5(PD_YEAR), re-computed at end of each year
		pd = FIRM_PD_PL_REG;

		//Eq(8): use last 20 quarters: 75th quantile of array (LGD_YEAR), re-computed at end of each year
		lgd = FIRM_LGD_PL_REG;
	}


	/* convert array [1369] to matrix [37][37] */
	int i,j,k;
	//int const n=37;
	int n=sqrt(LOOKUPTABLE_RISKWEIGHTS_FIRM_LOANS.size);


	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n Bank ID %d RW firm loans =", ID);
		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
			{
				k= i*n + j;
				printf("%f ", LOOKUPTABLE_RISKWEIGHTS_FIRM_LOANS.array[k]); 
			}
			printf("\n");
		}
	}
	#endif

	//Look-up Basel II risk weight from table:

	i=0; //Search PD on rows, stop on exceeding cell value:
	while (pd+EPSILON > LOOKUPTABLE_RISKWEIGHTS_FIRM_LOANS.array[i*n] && i<(n-1) ) i++;
	
	j=0; //Search LGD on columns, stop on exceeding cell value:
	while (lgd+EPSILON > LOOKUPTABLE_RISKWEIGHTS_FIRM_LOANS.array[j] && j<(n-1) ) j++;

	k = i*n + j;
	
	BASEL_RISK_WEIGHT_FIRM_LOANS_PL = LOOKUPTABLE_RISKWEIGHTS_FIRM_LOANS.array[k];

	BASEL_RISK_WEIGHT_FIRM_LOANS_NPL = 0.74;		//NOTE: NOT 0 (IRB) NOR 1 (STA), but reflecting historical weights of IRB/STA

	#ifdef _DEBUG_MODE	
	if (PRINT_DEBUG_CRM_RISK_WEIGHT_LOOKUP)
	{
		printf("\n [CRM_RISK_WEIGHT_LOOKUP: firm loan PL] Bank ID %d PD %f LGD %f RW %f", ID, pd, lgd, BASEL_RISK_WEIGHT_FIRM_LOANS_PL);
	}
	#endif

	return 0;   
}

/** \fn Bank_update_LLR_firm_loans()
 * \brief Function to compute the FIRM_EL_PL_YEAR
 * \Brief Frequency: 
 * \Brief After Function: 
 * \Brief Called in Function: Bank_issue_loans, Bank_receive_installment
 * \Brief Before Function: 
 * \brief UNITTEST: TODO
 */
int Bank_update_LLR_firm_loans()
{
	double delta;
	
	//For now assume all firm loans have the same PD
	//Old rule, before 13.03.2020
	//FIRM_EL_PL_YEAR = FIRM_PD_PL_YEAR_MA * FIRM_LGD_PL_YEAR_MA * FIRM_CREDIT;

	//New rule, 13.03.2020
	FIRM_EL_PL_YEAR = FIRM_PD_PL_YEAR_MA * FIRM_LGD_PL_YEAR_MA * FIRM_CREDIT;

	//Update LLR delta:
	delta = FIRM_EL_PL_YEAR - LLR_FIRM_LOANS;

	//Transfer delta from equity to LLR:
	EQUITY 			-= delta;
	LLR_FIRM_LOANS 	+= delta;		

	PROFITS[0] 		-= delta;		

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\n IT %d ID: %d Bank_update_loan_loss_reserve_firm_loans(): UPDATE OF LLR BASED ON FIRM_EL_PL_YEAR", DAY,ID);
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\t FIRM_EL_PL_YEAR: %f Loan loss reserves: %f", FIRM_EL_PL_YEAR, LLR_FIRM_LOANS);
	#endif   

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\t FIRM_EL_PL_YEAR: %f Loan loss reserves: %f delta: %f", FIRM_EL_PL_YEAR, LLR_FIRM_LOANS, delta);
	#endif 

	//Check loan_loss_reserve never becomes negative:
	if (LLR_FIRM_LOANS < 0.0)
	{
		#ifdef _DEBUG_MODE     
		if (PRINT_DEBUG_CREDIT) 
		{
				printf("\n\nCredit/Bank_Credit_Functions.c, Bank_update_loan_loss_reserve_firm_loans(), line 615: loan_loss_reserve is negative, transferring to equity: %2.2f\n", LLR_FIRM_LOANS);
		}
		#endif

		//Transfer complete (negative) LLR to equity
		EQUITY += fabs(LLR_FIRM_LOANS);
		LLR_FIRM_LOANS = 0.0;
	}

	if(CONST_SWITCH_IRB_SHORTFALL_ADJUSTMENT)
		Bank_update_IRB_shortfall_firm_loans();

	return 0;   
}

/** \fn Bank_update_IRB_shortfall_firm_loans()
 * \brief Function to adjust the IRB shortfall for Firm loans
 * \Brief Frequency: 
 * \Brief After Function: 
 * \Brief Called in Function: Bank_update_LLR_firm_loans
 * \Brief Before Function:  
 * \brief UNITTEST: TODO
 */
int Bank_update_IRB_shortfall_firm_loans()
{
	double dE = 0.0;
	double net_transfer = 0.0;

	/* Capital adjustment : IRB shortfall
	 * dE can be >0 or <0 and reflects the amount added to residual equity from CAPITAL_ADJ_(N)PL_FIRM_LOANS
	 * Eq (20)+(21)
	 */

	//First transfer the current IRB shortfall stock back to the residual equity, such that it starts from 0
	liability_transformation(&CAPITAL_ADJ_PL_FIRM_LOANS, &EQUITY, CAPITAL_ADJ_PL_FIRM_LOANS);

	//int code = 1;
	int code = 2;

	/* Code A*/
	if(code==1)
	{
		//Eq (20)
		dE = min( FIRM_EL_PL_YEAR - FIRM_EL_PL_REG, 6e-3 * REA_FIRM_LOANS);
		net_transfer = (-dE) - CAPITAL_ADJ_PL_FIRM_LOANS;
/*
		printf("\n\nIn function:");
		printf("\n PL FIRM LOANS");
*/
		liability_transformation(&EQUITY, &CAPITAL_ADJ_PL_FIRM_LOANS, net_transfer);
/*
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
		printf("\n net_transfer = %.2f", net_transfer);
*/
	}

	/* Code B*/
	if(code==2)
	{
		//Eq (20)
		dE = min( FIRM_EL_PL_YEAR - FIRM_EL_PL_REG, 6e-3 * REA_FIRM_LOANS);
		liability_transformation(&CAPITAL_ADJ_PL_FIRM_LOANS, &EQUITY, dE);
/*
		printf("\n\nIn function:");
		printf("\n PL FIRM LOANS");
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
*/
	}

	//Eq (21)
	//By design CAPITAL_ADJ_NPL_FIRM_LOANS = 0.0 due to: FIRM_LGD_NPL_YEAR = FIRM_LGD_NPL_REG = 1.0 (firm loans are without collateral)
	//dE = (FIRM_LGD_NPL_YEAR - FIRM_LGD_NPL_REG) * NPL_FIRM_LOANS;
	//net_transfer = (-dE) - CAPITAL_ADJ_NPL_FIRM_LOANS;
	//liability_transformation(&EQUITY, &CAPITAL_ADJ_NPL_FIRM_LOANS, net_transfer);	

	return 0;   
}
