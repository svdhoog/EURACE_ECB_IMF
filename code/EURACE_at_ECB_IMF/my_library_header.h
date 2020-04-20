/**
 * \file  my_library_header.h
 * \brief Header for user created library functions.
 */
#include <limits.h> //required to test for max. double: LONG_MAX

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_cdf.h>

#define DAY iteration_loop
#define MONTH 20
#define EPSILON 1e-16 //Accuracy for doubles close to zero, eg to prevent division by zero
 
/*Macro for passing a formatted mesg to assert
* Example usage:
 assert__(remaining_liquidity > -EPSILON)
 {
    fprintf(stderr, "\n\tWARNING %s():%d: remaining_liquidity %f (step 1: after setting equal to PAYMENT_ACCOUNT)", __FUNCTION__, __LINE__, remaining_liquidity);
 }
*/
#define assert__(x) for ( ; !(x) ; assert(x) )
 
int random_int(int min,int max);
double random_unif();
double random_unif_interval(double a, double b);
double max(double a, double b);
double min(double a, double b);
int round_double_to_int(double x);
double abs_double(double x);
double normal_distributed_double();

//GSL library functions:
//extern double gsl_stats_mean(const double data[], size_t stride, size_t n);
//extern double gsl_stats_variance (const double data[], size_t stride, size_t n);
double my_gaussian_pdf(double x, double mu, double sigma);
double my_gaussian_cdf(double x, double mu, double sigma);

double inverse_gaussian_pdf(double x, double mu, double lambda);
double inverse_gaussian_cdf(double x, double mu, double lambda);

double my_stats_mean(double_array * data);
double my_stats_median(double_array * data);
double my_stats_sd(double_array * data);
double my_stats_variance(double_array * data);
double my_stats_cv(double_array * data);

void vlog_event(int id, char name[]);
void vlog_micro_ts(int id, char name[], double value);
void vlog_macro_ts(char name[], double value);

void vlog_data(char key[], int n, ...);
void vlog_data_with_id(char key[], int id, int n,...);

void log_transformation(char agent_type[], char outflow[], char inflow[], char process[], int id, double value);
void log_outflow(char agent_type[], char asset[], char liability[], char process[], int id, double value);
void log_inflow(char agent_type[], char asset[], char liability[], char process[], int id, double value);
void log_outflow_single(char agent_type[], char account[], char process[], int id, double value);
void log_inflow_single(char agent_type[], char account[], char process[], int id, double value);
void log_stock(char agent_type[], char account[], int id, double value);

//APIs for qsort() to sort dynamic arrays
/*
static int compare(const void * a, const void * b);
void qsort_int_array(int_array * A);
void qsort_double_array(double_array * A);
*/

//APIs for qsort() to sort dynamic arrays (recovered from xparser generated memory.c, but commented out)
//int quicksort_double(double *array, int elements);
//int quicksort_int(int *array, int elements);
