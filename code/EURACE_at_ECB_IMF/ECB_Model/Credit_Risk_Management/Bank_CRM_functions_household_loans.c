/*********************************
 * Bank_CRM_functions_HH_loans.c 
 * Bank auxiliary functions for Credit Risk Management.
 * *********************************
 * History:
 * 28.05.2014 first version
 * 12.06.2016 Added PD and LGD
 * 14.10.2016 Updated PD and LGD computations
 * For households: 
 * - PD  based on backward-looking realized default rates
 * 	(Note: all non-performing loans are in fact defaulted loans: 
 * 	no deliquency period of 60 days/3 months is taken into account yet in code below)
 * - LGD based on backward-looking realized loss rates
 * Added household-specific vars:
 * - HH_PD_YEAR, HH_LGD_YEAR, 
 * - HH_PD_CYCLE, HH_LGD_CYCLE,
 * - HH_PD_REG, HH_LGD_PL_REG
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
//#define DAY iteration_loop
//#define MONTH 20


/** \fn int Bank_set_pd_lgd_monthly_household_loans(void)
 * \Brief Function to set the monthly data for PD and LGD
 * \Brief Use: to compute the quarterly pd_point_in_time and lgd_point_in_time
 * \Brief Frequency: monthly, on DAY 2
 * \Brief After Function:
 * \Brief Called in Function: ECB_Model/Mortgage_Market/Bank_Functions.c: Bank_set_pd_lgd_mortgage_loans()
 * \Brief Before Function: 
 * \Brief Inputs: mortgage_bankruptcy, mortgage_installment message
 * \Brief Expected: HH_PL_MONTHLY, HH_NPL_MONTHLY, HH_LD_MONTHLY, HH_LL_MONTHLY: dynamic int array (all length 3)
 * \Brief HH_PL_MONTHLY:  HOUSEHOLD performing loans
 * \Brief HH_NPL_MONTHLY: HOUSEHOLD non-performing loans
 * \Brief HH_LD_MONTHLY: loan default flow
 * \Brief HH_LL_MONTHLY: loan loss := loan default - value collateral
 */
int Bank_set_pd_lgd_monthly_household_loans(void)
{
    //MONTHLY ACTIVITY
	//outstanding loans that are still performing: COUNT installment_messages.
	//outstanding loans that are in arrears: COUNT arrears_messages.
	//outstanding loans that are non-performing: COUNT npl_messages.
	//outstanding loans that have defaulted: COUNT default_messages.		
	int pl_count=0;
	int arrears_count=0;

	int npl_count=0;
	int default_count=0;

	double loan_loss_flow=0.0;
	double loan_default_flow=0.0;
	
	//Count performing loans today
	START_MORTGAGE_PAYMENT_MESSAGE_LOOP	  
	    pl_count++;

	    //mortgage_payment_message->mortgage.loan_default	
	    //mortgage_payment_message->mortgage.loan_loss
	FINISH_MORTGAGE_PAYMENT_MESSAGE_LOOP

	//Count arrears loans today
	START_MORTGAGE_ARREARS_MESSAGE_LOOP
	    arrears_count++;

	    //mortgage_arrears_message->mortgage.loan_default;
	    //mortgage_arrears_message->mortgage.loan_loss
	FINISH_MORTGAGE_ARREARS_MESSAGE_LOOP


	//Count non-performing loans today
	loan_loss_flow=0.0;
	loan_default_flow=0.0;
	START_MORTGAGE_NPL_MESSAGE_LOOP
	  npl_count++;
	  
	  if (mortgage_npl_message->mortgage.npl_counter==0)
	  		loan_default_flow += mortgage_npl_message->mortgage.loan_default;

	FINISH_MORTGAGE_NPL_MESSAGE_LOOP


	//Count defaulting loans today
	START_MORTGAGE_WRITEOFF_MESSAGE_LOOP
	  default_count++;

	  loan_loss_flow += mortgage_writeoff_message->house.seized_house.mortgage.loan_loss;
	FINISH_MORTGAGE_WRITEOFF_MESSAGE_LOOP

	//Performing (PL+Arrears) and non-performing (NPL) volume of mortgage loans to households
	recompute_mortgage_stocks();

	// Add item at end of array
	add_double(&HH_PL_MONTHLY, PL_MORTGAGE_LOANS);
	add_double(&HH_NPL_MONTHLY, NPL_MORTGAGE_LOANS);
	add_double(&HH_LL_MONTHLY, loan_loss_flow);
	add_double(&HH_LD_MONTHLY, loan_default_flow);	

	// Remove item from head index 0
	remove_double(&HH_PL_MONTHLY, 0);
	remove_double(&HH_NPL_MONTHLY, 0);
	remove_double(&HH_LL_MONTHLY, 0);
	remove_double(&HH_LD_MONTHLY, 0);

	return 0;
}	


/** \fn int Bank_set_pd_lgd_quarterly_household_loans(void)
 * \Brief Function to set the quarterly data for pd_point_in_time and lgd_point_in_time
 * \brief REF: Model Specifications, 2.10.2017
 * \Brief Frequency: runs monthly, add monthly data to a 60-array: 20 quarters of 3 months, or 5 years
 * \Brief After Function: 
 * \Brief Called in Function: ECB_Model/Mortgage_Market/Bank_Functions.c: Bank_set_pd_lgd_mortgage_loans()
 * \Brief Before Function: 
 * \Brief Inputs: none
 * \Brief Expected:
 */
int Bank_set_pd_lgd_quarterly_household_loans(void)
{
	int i;
	double sum_loan_loss = 0.0;
	double sum_loan_default = 0.0;
	double sum_sales_value_capped = 0.0;
	double sum_principal_wo = 0.0;

	// sum daily Loan Loss flow across past quarter (loan volume in default - value collateral)
	for (i=0; i<HH_LL_MONTHLY.size; i++)
	    sum_loan_loss += HH_LL_MONTHLY.array[i];
	  
	// sum daily Loan Default flow  across past quarter (loan volume in default)
	for (i=0; i<HH_LD_MONTHLY.size; i++)
	    sum_loan_default += HH_LD_MONTHLY.array[i];
	
	// PD point-in-time: VOLUME BASED using one quarter of data
	// Eq (7): PD = LD/PL[t-3]
    HH_PD_PL_QTR = 0.0;
    if( HH_PL_MONTHLY.array[0] > EPSILON)
    	HH_PD_PL_QTR = sum_loan_default/HH_PL_MONTHLY.array[0];
	
    #ifdef _DEBUG_MODE        
    if (PRINT_DEBUG_FLOORS_AND_CEILINGS)
    {   //Warning for floor-level:
        if (HH_PD_PL_QTR < CONST_INIT_PD_MORTGAGE)
            fprintf(stderr, "\nIT %d ID %d WARNING in Bank_set_pd_lgd_quarterly_household_loans: HH_PD_PL_QTR below minimum level HH_PD_PL_QTR = %f CONST_INIT_PD_MORTGAGE = %f", DAY, ID, HH_PD_PL_QTR, CONST_INIT_PD_MORTGAGE);
    }              
    #endif
    
	//Floor-level
    HH_PD_PL_QTR = max(HH_PD_PL_QTR, CONST_INIT_PD_MORTGAGE);

    if (HH_PD_PL_QTR > 1.0)
    {
    	HH_PD_PL_QTR = 1.0;
        fprintf(stderr, "\nIT %d ID %d WARNING HH_PD_PL_QTR > 1 (HH_PD_PL_QTR: %f, sum_loan_default: %f, HH_PL_MONTHLY.array[0]: %f). Setting HH_PD_PL_QTR=1.0", DAY, ID, HH_PD_PL_QTR, sum_loan_default, HH_PL_MONTHLY.array[0]);

    }
    
	//Point-in-time PD, scaled-up to one year:
	HH_PD_PL_YEAR = (1 - pow(1-HH_PD_PL_QTR, 4));

	HH_PD_PL_YEAR = min(max(CONST_MIN_PD_MORTGAGES, HH_PD_PL_YEAR), CONST_MAX_PD_MORTGAGES);
	
	//Point-in-time LGD: No rescaling of LGD because it is a ratio of stock measures!
	// Eq(9): LGD = LL/LD
	HH_LGD_PL_YEAR = CONST_MIN_LGD_MORTGAGES;
	for(i=0; i<SUM_SALE_VALUE_CAPPED_MONTHLY.size;i++)
	{
		sum_sales_value_capped += SUM_SALE_VALUE_CAPPED_MONTHLY.array[i];
		sum_principal_wo += SUM_MORTGAGE_PRINCIPAL_WRITTEN_OFF_MONTHLY.array[i];
	}
	if (sum_principal_wo>0.0)
		HH_LGD_PL_YEAR = min(max(CONST_MIN_LGD_MORTGAGES, (1 - sum_sales_value_capped / sum_principal_wo)), CONST_MAX_LGD_MORTGAGES);

	//Safety checks:
	if(HH_PD_PL_QTR > 1.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_PD_PL_QTR>1.0: HH_PD_PL_QTR = %f\n sum_loan_default = %f\n HH_PL_MONTHLY.array[0] = %f", DAY, ID, HH_PD_PL_QTR, sum_loan_default, HH_PL_MONTHLY.array[0]);

	if(HH_PD_PL_QTR < 0.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_PD_PL_QTR<0.0: HH_PD_PL_QTR = %f\n sum_loan_default = %f\n HH_PL_MONTHLY.array[0] = %f", DAY, ID, HH_PD_PL_QTR, sum_loan_default, HH_PL_MONTHLY.array[0]);

	if(HH_PD_PL_YEAR > 1.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_PD_PL_YEAR>1.0: HH_PD_PL_YEAR = %f", DAY, ID, HH_PD_PL_YEAR);

	if(HH_PD_PL_YEAR < 0.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_PD_PL_YEAR<0.0: HH_PD_PL_YEAR = %f", DAY, ID, HH_PD_PL_YEAR);

	if(HH_LGD_PL_YEAR > 1.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_LGD_PL_YEAR>1.0: HH_LGD_PL_YEAR = %f\n sum_loan_loss = %f\n sum_loan_default = %f", DAY, ID, HH_LGD_PL_YEAR, sum_loan_loss, sum_loan_default);

	if(HH_LGD_PL_YEAR < 0.0)
        fprintf(stderr, "\nIT %d ID %d ERROR in Bank_set_pd_lgd_quarterly_household_loans()\nHH_LGD_PL_YEAR<0.0: HH_LGD_PL_YEAR = %f\n sum_loan_loss = %f\n sum_loan_default = %f", DAY, ID, HH_LGD_PL_YEAR, sum_loan_loss, sum_loan_default);

    //Cap LGD to lie in [0,1]
    if(HH_LGD_PL_YEAR > 1.0)
        fprintf(stderr, "\nIT %d ID %d WARNING in Bank_set_pd_lgd_quarterly_household_loans()\nHH_LGD_PL_YEAR exceeds 1.0: HH_LGD_PL_YEAR = %f (forcing to 1.0)\n sum_loan_loss = %f sum_loan_default = %f", DAY, ID, HH_PD_PL_QTR, sum_loan_loss, sum_loan_default);
    
    
	assert(HH_PD_PL_QTR<1+1e-7);
	assert(HH_PD_PL_QTR>-1e-7);

	assert(HH_PD_PL_YEAR<1+1e-7);
	assert(HH_PD_PL_YEAR>-1e-7);

	assert(HH_LGD_PL_YEAR<1+1e-7);
	assert(HH_LGD_PL_YEAR>-1e-7);
	
	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)
	{
		printf("\n\n Bank_set_pd_lgd_quarterly_household_loans: AFTER computing sums, pd_YEAR, lgd_YEAR\n");
		printf("\n sum_loan_loss=%.3f sum_loan_default=%.3f\n", sum_loan_loss, sum_loan_default);
		printf("\n HH_PD_PL_YEAR = %.3f\n", HH_PD_PL_YEAR);
		printf("\n HH_LGD_PL_YEAR = %.3f\n", HH_LGD_PL_YEAR);
	}
	#endif

	// Add item at end of array HH_PD_CYCLE and HH_LGD_CYCLE
	add_double(&HH_PD_CYCLE, HH_PD_PL_YEAR);
	add_double(&HH_LGD_CYCLE, HH_LGD_PL_YEAR);	

	// Remove item from head index 0
	remove_double(&HH_PD_CYCLE, 0);
	remove_double(&HH_LGD_CYCLE, 0);

	int n=0;
	double sum=0;
	for(i=0;i<4;i++)
	{
		if (HH_PD_CYCLE.size-i-1>0)
		{
			sum+=HH_PD_CYCLE.array[HH_PD_CYCLE.size-i-1];
			n++;
		}
	}
	HH_PD_PL_YEAR_MA = sum/n;

	n=0;
	sum=0;
	for(i=0;i<4;i++)
	{
		if (HH_LGD_CYCLE.size-i-1>0)
		{
			sum+=HH_LGD_CYCLE.array[HH_LGD_CYCLE.size-i-1];
			n++;
		}
	}
	HH_LGD_PL_YEAR_MA = sum/n;

    return 0;
}	

/** \fn int Bank_set_pd_lgd_reg_household_loans(void)
 * \Brief Function to compute PD and LGD across a time window of 5 years
 * \Brief Aggregate [5 year/20 quarters/60 months] of HH_PD_YEAR and HH_LGD_YEAR in a moving average.
 * \Brief Frequency: runs monthly, on DAY 2 of calendar month (mortgage market is open only on DAY 2)
 * \Brief After Function: 
 * \Brief Called in Function: ECB_Model/Mortgage_Market/Bank_Functions.c: Bank_set_pd_lgd_mortgage_loans()
 * \Brief Before Function: 
 * \Brief Expected: HH_PD_CYCLE, HH_LGD_CYCLE: dynamic double array, length 20 (20 quarters, 5 years)
 * \Brief Expected: HH_PD_REG, HH_LGD_PL_REG: double
 */
int Bank_set_pd_lgd_reg_household_loans(void)
{
    int i;

	// On last day of calendar year: sum 20 quarterly PD and LGD across 5 years
	double sum1=0,sum2=0;

	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n\n Bank_set_pd_lgd_yearly_household_loans: BEFORE summing cycle arrays\n");
	
		printf("\n HH_PD_CYCLE = [");
		for (i=0; i<HH_PD_CYCLE.size; i++)
			printf(" %.3f",HH_PD_CYCLE.array[i]);
		printf("]\n");
	
		printf("\n HH_LGD_CYCLE = [");
		for (i=0; i<HH_LGD_CYCLE.size; i++)
			printf(" %.3f",HH_LGD_CYCLE.array[i]);
		printf("]\n");
	}
	#endif

	  // sum 20 quarters of PD ratio data
	  for (i=0; i<HH_PD_CYCLE.size; i++)
	    sum1 += HH_PD_CYCLE.array[i];
	
	// PD through-the-cycle: HH_PD_REG = MA(HH_PD_YEAR)
	//Mean:
	if(HH_PD_CYCLE.size>0)
		HH_PD_PL_REG = sum1/HH_PD_CYCLE.size;
	
	/* 75th percentile of array HH_LGD_CYCLE*/
	int n = HH_LGD_CYCLE.size;
	
	double sorted_data[n];
		
	for (i=0; i<n; i++)
		sorted_data[i] = HH_LGD_CYCLE.array[i];

	gsl_sort(sorted_data, 1, n);

	//GSL function for percentiles
	//out = gsl_stats_quantile_from_sorted_data (sorted_data, 1, n, 0.75);
	
	//Simple: 75th percentile of 60 items is at element [44]
	HH_LGD_PL_REG = sorted_data[44];

	//Regulatory expected loss (Eq 19, 2nd term, and multiplied by principle outstanding)
	HH_EL_PL_REG = HH_PD_PL_REG * HH_LGD_PL_REG * PL_MORTGAGE_LOANS;

	//Regulatory LGD downturn
	HH_LGD_NPL_REG = HH_LGD_PL_REG;

	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n\n Bank_set_pd_lgd_yearly_household_loans: AFTER summing cycle arrays\n");

		printf("\n sum1 = [");
			printf(" %.3f", sum1);
		printf("]\n");
	
		printf("\n sum2 = [");
			printf(" %.3f", sum2);
		printf("]\n");

		printf("\n HH_PD_PL_REG = [");
			printf(" %.3f", HH_PD_PL_REG);
		printf("]\n");
	
		printf("\n HH_LGD_PL_REG = [");
			printf(" %.3f", HH_LGD_PL_REG);
		printf("]\n");
	}
	#endif
  
    return 0;
}

/** \fn Bank_set_risk_weight_household_loans()
 * \brief Function to compute the Basel 2 risk-weight formula for mortgage loans.
 * \Brief Frequency: 
 * \Brief After Function: 
 * \Brief Called in Function: ECB_Model/Mortgage_Market/Bank_Functions.c: Bank_set_risk_exposure_amounts()
 * \Brief Before Function: 
 * \brief Message input: None
 * \brief Message output: None
 * \brief UNITTEST: TODO
 */
int Bank_set_risk_weight_household_loans()
{
	double pd,lgd;

	//Eq(3): use last 20 quarters: MA_5(PD_YEAR), re-computed at end of each year
	pd = HH_PD_PL_REG;

	//Eq(8): use last 20 quarters: 75th quantile of array (LGD_YEAR), re-computed at end of each year
	lgd = HH_LGD_PL_REG;

	/* convert array [1369] to matrix [37][37] */
	int i,j,k;
	
	//int const n=37;
	int n=sqrt(LOOKUPTABLE_RISKWEIGHTS_MORTGAGES.size);
	

	#ifdef _DEBUG_MODE
	if (PRINT_DEBUG_CREDIT_UNITTEST)
	{
		printf("\n Bank ID %d RW mortgages =", ID);

		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
			{
				k= i*n + j;
				printf("%f ", LOOKUPTABLE_RISKWEIGHTS_MORTGAGES.array[k]); 
			}
			printf("\n");
		}
	}
	#endif

	//Look-up Basel II risk weight from table:

	i=0; //Search PD on rows, stop on exceeding cell value:
	while (pd+EPSILON > LOOKUPTABLE_RISKWEIGHTS_MORTGAGES.array[i*n] && i<(n-1) ) i++;
	
	j=0; //Search LGD on columns, stop on exceeding cell value:
	while (lgd+EPSILON > LOOKUPTABLE_RISKWEIGHTS_MORTGAGES.array[j] && j<(n-1) ) j++;

	k = i*n + j;
	
	BASEL_RISK_WEIGHT_MORTGAGE_LOANS_PL = LOOKUPTABLE_RISKWEIGHTS_MORTGAGES.array[k];

	BASEL_RISK_WEIGHT_MORTGAGE_LOANS_NPL = 0.26; 	//NOTE: NOT 0 (IRB) NOR 1 (STA), but reflecting historical weights of IRB/STA
	
	#ifdef _DEBUG_MODE	
	if (PRINT_DEBUG_CRM_RISK_WEIGHT_LOOKUP)
	{
		printf("\n [CRM_RISK_WEIGHT_LOOKUP: mortgage PL] Bank ID %d PD %f LGD %f RW %f", ID, pd, lgd, BASEL_RISK_WEIGHT_MORTGAGE_LOANS_PL);
	}
	#endif

	return 0;   
}

/** \fn Bank_update_LLR_mortgage_loans()
 * \brief Function to compute the HH_EL_PL_YEAR
 * \Brief Frequency: 
 * \Brief After Function: 
 * \Brief Called in Function: ECB_Model/Mortgage_Market/Bank_Functions.c:
 * \Brief Bank_receive_full_mortgage_payments(), Bank_process_mortgage_accepts(), Bank_reconstruct_mortgage_portfolio(), Bank_receive_full_mortgage_payments()
 * \Brief Before Function: 
 * \brief UNITTEST: TODO
 */
int Bank_update_LLR_mortgage_loans()
{
	int i;
	double delta = 0.0;

	HH_EL_PL_YEAR = 0.0;
	HH_EL_NPL_YEAR = 0.0;
	for (i=0; i<MORTGAGE_PORTFOLIO.size; i++)
	{
		//Eq. (18): Use portfolio-wide point-in-time LGD
		switch(MORTGAGE_PORTFOLIO.array[i].status)
		{
			case 0: HH_EL_PL_YEAR += HH_PD_PL_YEAR_MA * HH_LGD_PL_YEAR_MA * MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
			break;
			
			case 1: HH_EL_PL_YEAR += HH_PD_PL_YEAR_MA * HH_LGD_PL_YEAR_MA * MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
			break;
			
			case 2: HH_EL_NPL_YEAR += HH_PD_NPL_YEAR * HH_LGD_PL_YEAR_MA * MORTGAGE_PORTFOLIO.array[i].outstanding_principal;
			break;
		}
	}
	
	/*****************  SECTION: PL **************************/
	if(SWITCH_LOAN_LOSS_PROVISIONING_PL)
	{
		//Eq (19): Update the LLR with the delta:
		delta = HH_EL_PL_YEAR - LLR_PL_MORTGAGE_LOANS;

		//Transfer delta from equity to LLR:
		EQUITY  				-= delta;
		LLR_PL_MORTGAGE_LOANS 	+= delta;

		PROFITS[0] -= delta;		
	}

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\t HH_EL_PL_YEAR %f LLR_PL_MORTGAGE_LOANS %f delta %f", HH_EL_PL_YEAR, LLR_PL_MORTGAGE_LOANS, delta);
	#endif   

	//Check LLR never becomes negative:
	if (LLR_PL_MORTGAGE_LOANS < 0.0)
	{
		#ifdef _DEBUG_MODE     
		if (PRINT_DEBUG_CREDIT_RISK_MNGT) 
		{
			printf("\n\nMortgage_Market/Bank_Functions.c, Bank_update_LLR_mortgage_loans(), line 541: LLR for PL mortgage is negative, transferring to equity: %2.2f\n", LLR_PL_MORTGAGE_LOANS);
		}
		#endif

		//Transfer complete (negative) LLR to equity
		EQUITY += fabs(LLR_PL_MORTGAGE_LOANS);
		LLR_PL_MORTGAGE_LOANS = 0.0;
	}
	
	/*****************  SECTION: NPL **************************/
	//Eq (19): Update the LLR with the delta:
	delta = HH_EL_NPL_YEAR - LLR_NPL_MORTGAGE_LOANS;
	
	//Transfer delta from equity to LLR:
	EQUITY  				-= delta;
	LLR_NPL_MORTGAGE_LOANS  += delta;

	PROFITS[0] -= delta;		

	#ifdef _DEBUG_MODE   
	if (PRINT_DEBUG_CREDIT_RISK_MNGT)	printf("\n\t HH_EL_NPL_YEAR %f LLR_NPL_MORTGAGE_LOANS %f delta %f", HH_EL_NPL_YEAR, LLR_NPL_MORTGAGE_LOANS, delta);
	#endif   
	
	//Check LLR never becomes negative:
	if (LLR_NPL_MORTGAGE_LOANS < 0.0)
	{
		#ifdef _DEBUG_MODE     
		if (PRINT_DEBUG_CREDIT_RISK_MNGT) 
		{
			printf("\n\nMortgage_Market/Bank_Functions.c, Bank_update_LLR_mortgage_loans(), line 541:LLR for NPL mortgage is negative, transferring to equity: %2.2f\n", LLR_NPL_MORTGAGE_LOANS);
		}
		#endif

		//Transfer complete (negative) LLR to equity
		EQUITY += fabs(LLR_NPL_MORTGAGE_LOANS);
		LLR_NPL_MORTGAGE_LOANS = 0.0;
	}

	if(CONST_SWITCH_IRB_SHORTFALL_ADJUSTMENT)
		Bank_update_IRB_shortfall_mortgage_loans();

	LLR = LLR_FIRM_LOANS + LLR_PL_MORTGAGE_LOANS + LLR_NPL_MORTGAGE_LOANS;

	return 0;   
}
	

/** \fn Bank_update_IRB_shortfall_mortgage_loans()
 * \brief Function to adjust the IRB shortfall for mortgage loans
 * \Brief Frequency: 
 * \Brief After Function: 
 * \Brief Called in Function: 
 * \Brief Before Function: 
 * \brief UNITTEST: TODO
 */
int Bank_update_IRB_shortfall_mortgage_loans()
{
	double dE = 0.0;
	double net_transfer = 0.0;

	/* Capital adjustment : IRB shortfall
	 * dE can be >0 or <0 and reflects the amount **added** to residual equity from CAPITAL_ADJ_(N)PL_MORTGAGE_LOANS
	 * Eq (20)+(21)
	 */

	//First transfer the current IRB shortfall stock back to the residual equity, such that it starts from 0
	liability_transformation(&CAPITAL_ADJ_PL_MORTGAGE_LOANS, &EQUITY, CAPITAL_ADJ_PL_MORTGAGE_LOANS);
	liability_transformation(&CAPITAL_ADJ_NPL_MORTGAGE_LOANS, &EQUITY, CAPITAL_ADJ_NPL_MORTGAGE_LOANS);


	//int code = 1;
	int code = 2;

	/* Code A*/
	if(code==1)
	{
		//Eq (20)
		dE = min( HH_EL_PL_YEAR - HH_EL_PL_REG, 6e-3 * REA_MORTGAGE_LOANS);
		net_transfer = (-dE) - CAPITAL_ADJ_PL_MORTGAGE_LOANS;	
		liability_transformation(&EQUITY, &CAPITAL_ADJ_PL_MORTGAGE_LOANS, net_transfer);
/*
		printf("\n\nIn function:");
		printf("\n PL MORTGAGE LOANS");
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
		printf("\n net_transfer = %.2f", net_transfer);
*/
		//Eq (21)
		dE = (HH_LGD_PL_YEAR - HH_LGD_NPL_REG) * NPL_MORTGAGE_LOANS;
		net_transfer = (-dE) - CAPITAL_ADJ_NPL_MORTGAGE_LOANS;	
		liability_transformation(&EQUITY, &CAPITAL_ADJ_NPL_MORTGAGE_LOANS, net_transfer);
/*		
		printf("\n\n NPL MORTGAGE LOANS");
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
		printf("\n net_transfer = %.2f", net_transfer);
*/
	}

	/* Code B*/
	if(code==2)
	{
		//Eq (20)
		dE = min( HH_EL_PL_YEAR - HH_EL_PL_REG, 6e-3 * REA_MORTGAGE_LOANS);
		liability_transformation(&CAPITAL_ADJ_PL_MORTGAGE_LOANS, &EQUITY, dE);
/*
		printf("\n\nIn function:");
		printf("\n PL MORTGAGE LOANS");
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
*/
		//Eq (21)
		dE = (HH_LGD_PL_YEAR - HH_LGD_NPL_REG) * NPL_MORTGAGE_LOANS;
		liability_transformation(&CAPITAL_ADJ_NPL_MORTGAGE_LOANS, &EQUITY, dE);
/*
		printf("\n\n NPL MORTGAGE LOANS");
		printf("\n  dE = %.2f\n -dE = %.2f", dE, -dE);
*/
	}

	return 0;   
}
