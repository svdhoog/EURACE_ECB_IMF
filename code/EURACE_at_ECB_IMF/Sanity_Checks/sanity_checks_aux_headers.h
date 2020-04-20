
/*********************************
 * sanity_checks_aux_headers.h
 * Header file for sanity_checks auxiliary functions.
 * *********************************
 * History:
 * 06.06.2017 Added header file
 *********************************/

/* Household auxiliary functions */
int print_agent_info(int agent_id);
int print_agent_info_day(int agent_id, int day);
int print_agent_info_monthly(int agent_id, int day_in_month);
int print_agent_object_info(void);
void enforce_hh_valid_payment_account(const char * string);

/* Bank auxiliary functions */
int Bank_sanity_checks(void);
void enforce_bank_balanced_balance_sheet(char string[]);

/* Firm auxiliary functions */
int Firm_sanity_checks(void);

/* Eurostat auxiliary functions */
int Eurostat_sanity_checks(void);

/* CentralBank auxiliary functions */
int CentralBank_sanity_checks(void);
