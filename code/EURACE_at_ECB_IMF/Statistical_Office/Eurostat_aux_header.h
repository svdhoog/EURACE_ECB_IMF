/*********************************
 * Eurostat_aux_header.h
 * Header file for Eurostat auxiliary functions.
 * *********************************
 * History:
 * 02/02/09 Sander, added new auxiliary functions
 * 15/09/08 Sander 
 *********************************/

/* Eurostat auxiliary functions */
void Eurostat_reset_data(void);
void Eurostat_compute_mean_price(void);

void Eurostat_read_firm_data(void);
void Eurostat_compute_region_firm_data(void);
void Eurostat_compute_global_firm_data(void);

void Eurostat_read_household_data(void);
void Eurostat_compute_subsistence_level(void);

void Eurostat_compute_region_household_data(void);
void Eurostat_compute_global_household_data(void);

void Eurostat_calc_macro_data(void);

/* Functions for time series */

void sum_array(double_array * x, int start, int end, double * sum_out);
void sum_array_backwards(double_array * x, int start, int end, double * sum_out);

double ratio_qoq(double_array * x);
double ratio_yoy(double_array * x);

double pct_change_qoq(double_array * x);
double pct_change_yoy(double_array * x);

void compute_statistics_economy(data_adt * x);
void update_timeseries_economy(data_adt * x, double value);

void compute_statistics_regions(data_adt * x, int i);
void update_timeseries_regions(data_adt * x, int i, double value);

int set_with_inf_nan_checking(double * a, double b, double inf_replacement, double nan_replacement);

void Eurostat_compute_timeseries_statistics(void);
void Eurostat_compute_macro_indicators(void);
