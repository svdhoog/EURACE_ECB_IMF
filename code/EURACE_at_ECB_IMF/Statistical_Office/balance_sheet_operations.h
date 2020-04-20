
/* Function prototypes for balance sheet operations*/
void asset_liability_add(double * asset, double * liability, double value);
void asset_liability_subtract(double * asset, double * liability, double value);
void asset_transformation(double * from, double * to, double value);
void liability_transformation(double * from, double * to, double value);

void print_bank_balance_sheet();
void recompute_mortgage_stocks();

void mortgage_portfolio_diff(mortgage_contract_adt_array * m1, mortgage_contract_adt_array * m2);

int compute_mean_interest_rates_all_mortgages(void);
int compute_mean_interest_rates_all_firm_loans(void);

int compute_acceptance_rates_mortgage_loans(void);
