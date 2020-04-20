/*********************************
 * Bank_aux_headers.h
 * Header file for Bank auxiliary functions.
 * *********************************
 * History:
 * 12.06.2016 Added PD and LGD
 * 14.10.2016 Added PD and LGD for for firm loans/household mortgage loans
 *********************************/

/* Bank auxiliary functions */

/* For firm loans */
int Bank_set_pd_lgd_daily_firm_loans();
int Bank_set_pd_lgd_quarterly_firm_loans();
int Bank_set_pd_lgd_reg_firm_loans();
int Bank_set_risk_weight_firm_loans();

/* For household mortgage loans */
int Bank_set_pd_lgd_monthly_household_loans();
int Bank_set_pd_lgd_quarterly_household_loans();
int Bank_set_pd_lgd_reg_household_loans();
int Bank_set_risk_weight_household_loans();

/* Updating LLR */
int Bank_update_LLR_mortgage_loans();
int Bank_update_LLR_firm_loans();

/* Updating IRB_shortfall */
int Bank_update_IRB_shortfall_firm_loans();
int Bank_update_IRB_shortfall_mortgage_loans();
