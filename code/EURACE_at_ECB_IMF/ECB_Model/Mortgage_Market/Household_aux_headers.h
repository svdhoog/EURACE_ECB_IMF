/* Headers for Household mortgage functions
 */
void recompute_mortgage_attributes(mortgage_contract_adt * mortgage);
void recompute_mortgage_attributes_including_arrears(mortgage_contract_adt * mortgage);
void adjust_mortgage_contract(mortgage_contract_adt * mortgage, double new_principal);
void adjust_mortgage_contract2(mortgage_contract_adt * mortgage, double new_principal);
void recompute_variable_rate_mortgage_contract(mortgage_contract_adt * mortgage, double new_base_rate);

void Household_default_on_all_mortgage_debt_hmr(void);
void Household_default_on_all_mortgage_debt_op(void);

void Household_service_mortgage_debt_hmr();
void Household_service_mortgage_debt_op();
