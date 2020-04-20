/**
 * \file  my_library_functions.c
 * \brief Holds user created library functions.
 */
#include "header.h"
#include "my_library_header.h"
#include <stdio.h>
#include <stdarg.h>

/* EXTERNAL GLOBAL VARIABLES */
extern gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/* GLOBAL VARIABLES */
int FLAME_GSL_GEN_NO;

/** \fn int random_int(int min,int max)
 * \brief Returns an integer between and including min and max
 * \param min The minimum integer.
 * \param max The maximum integer.
 * \return The random integer.
 */
int random_int(int min,int max)
{
	FLAME_GSL_GEN_NO++;

	double random_number = gsl_rng_uniform(FLAME_GSL_RNG)*(max - min + 1) + min;

	return (int)random_number;
}

double max(double a, double b)
{
    return (a >= b) ? a : b; 
}

double min(double a, double b)
{
    return (a <= b) ? a : b; 
}


/** \fn double random_unif()
 * \brief Uniformly distributed random numbers, chosen from
 *   a uniform distribution on the closed interval (0.0,1.0).
 */
double random_unif()
{
	FLAME_GSL_GEN_NO++;

    return gsl_rng_uniform(FLAME_GSL_RNG);
}

/** \fn double random_unif_interval(double a, double b)
 * \brief Uniformly distributed random numbers, chosen from
 *   a uniform distribution on the closed interval (a,b).
 */
double random_unif_interval(double a, double b)
{
	return (a + (b-a)*random_unif());
}
/** \fn round_double_to_int(double x)
 * \brief Rounds a double to the closest integer.
 */
int round_double_to_int(double x)
{
  if(x>0) return (int)(x + 0.5);

  return (int)(x - 0.5);
}

double abs_double(double x)
{
  if(x<0) return ((-1)*x);

  return x;
}

/** \fn normal_distributed_double()
 * \brief returns a standard normal distributed random number (Polar method)
 */
double normal_distributed_double()
{
	FLAME_GSL_GEN_NO++;

	//This GSL function returns a Gaussian random variate, with mean zero and standard deviation sigma.
	//double gsl_ran_gaussian (const gsl_rng * r, double sigma)

	return gsl_ran_gaussian(FLAME_GSL_RNG, 1.0);
}

/** \fn inverse_gaussian_pdf(double x)
 * \brief PDF of the inverse Gausian distribution.
 * double x >0
 * double mu >0, the mean parameter
 * double lambda >0, the shape parameter
 */
double inverse_gaussian_pdf(double x, double mu, double lambda)
{
	#define PI 3.141592356595

	return sqrt( lambda/(2*PI*pow(x,3) )) * exp( -lambda*(x-mu)*(x-mu)/(2*mu*mu*x) );
}

/** \fn inverse_gaussian_cdf(double x)
 * \brief CDF of the inverse Gausian distribution.
 * double x >0
 * double mu >0, the mean parameter
 * double lambda >0, the shape parameter
 */
double inverse_gaussian_cdf(double x, double mu, double lambda)
{

	return 0.0;
}

//GSL library functions:
/** \fn my_gaussian_pdf(double x)
 * \brief PDF of the Gausian distribution.
 * double x >0
 * double mu >0, the mean parameter
 * double sigma >0, the shape parameter
 */
double my_gaussian_pdf(double x, double mu, double sigma)
{
	#define PI 3.141592356595

	return (1/(sigma*sqrt(2*PI))) * exp( -(x-mu)*(x-mu)/(2*sigma*sigma) );
}

/** \fn my_gaussian_cdf(double x)
 * \brief CDF of the Gausian distribution.
 * double x >0
 * double mu >0, the mean parameter
 * double sigma >0, the shape parameter
 */
double my_gaussian_cdf(double x, double mu, double sigma)
{
	return 0.0;
}

/*
extern double gsl_stats_mean(const double data[], size_t stride, size_t n);
This function returns the arithmetic mean of data, a dataset of length n with stride stride.
The arithmetic mean, or sample mean, is denoted by mu and defined as:
mu = (1/N) sum(x_i)

extern double gsl_stats_variance (const double data[], size_t stride, size_t n);
This function returns the estimated, or sample, variance of data, a dataset of length n with stride stride.
The sample variance is denoted by sigma^2 and is defined by:
sigma^2 = (1/(N-1)) \sum (x_i - mu)^2
*/

/** \fn double my_stats_mean(double_array * data)
 * \brief This function returns the  arithmetic mean, or sample mean, of data, a dataset of length n.
 * \brief The arithmetic mean, or sample mean, is denoted by mu and defined as:
 * \brief mu = (1/N) sum(x_i)
 */
double my_stats_mean(double_array * data)
{
	int i,n=0;
	double mu,sum=0.0;

	n = (*data).size;
	
	for(i=0; i<n; i++)
		sum += (*data).array[i];

	if(n>0)
		mu = ((double)sum)/((double)n);
	else
		mu = 0.0;

	return mu;
}

/** \fn double my_stats_median(double_array * data)
 * \brief This function returns the  median of data, a dataset of length n.
 * \brief The median is defined as:
 * \brief if n is even: (data[a]+data[b])/2
 * \brief if n is odd: data[a]
 */
double my_stats_median(double_array * data)
{
	int i,a,b;
	double x;
	double median = 0.0;

    int n = (*data).size;
    
    double sorted_data[n];
        
    for (i=0; i<n; i++)
        sorted_data[i] = (*data).array[i];

    gsl_sort(sorted_data, 1, n);

    x = n/2.0;

    if (n%2==0)
    {
    	a=floor(x);
    	b=floor(x)+1;
    } else{
    	a=floor(x);
    	b=floor(x);
    }

    median = 0.5*(sorted_data[a-1]+sorted_data[b-1]);

	return median;
}

/** \fn double my_stats_variance(double_array * data)
 * \brief This function returns the estimated, or sample, variance of data, a dataset of length n.
 * \brief The sample variance is denoted by sigma^2 and is defined by:
 * \brief sigma^2 = (1/(N-1)) \sum (x_i - mu)^2
 */
double my_stats_variance(double_array * data)
{
	int i,n=0;
	double mu,sigma_sq,sum=0.0;

	n = (*data).size;

	mu = my_stats_mean(data);

	for(i=0; i<n; i++)
		sum += ((*data).array[i] - mu)*((*data).array[i] - mu);

	if(n>1)
		sigma_sq = ((double)sum)/((double)(n-1));
	else
		sigma_sq = 0.0;

	return sigma_sq;
}

/** \fn double my_stats_sd(double_array * data)
 * \brief This function returns the estimated, or sample, standard deviation of data, a dataset of length n.
 * \brief The sample sd is the square root of the sample variance.
 */
double my_stats_sd(double_array * data)
{
	return sqrt(my_stats_variance(data));
}

/** \fn double my_stats_cv(double_array * data)
 * \brief This function returns the coefficient of variance of data, a dataset of length n.
 * \brief The cv is the sample variance divided by the mean.
 */
double my_stats_cv(double_array * data)
{
	double test = my_stats_mean(data);

	if (test)
		return my_stats_sd(data)/test;
	else
		return -1;
}

void vlog_event(int id, char name[])
{
	if (VISUAL_LOG)
		printf("{VLOG:event;%d;%d;%s}\n", DAY, id, name);
}

void vlog_micro_ts(int id, char name[], double value)
{
	if (VISUAL_LOG)
		printf("{VLOG:micro_ts;%d;%d;%s;%f}\n", DAY, id, name, value);
}

void vlog_macro_ts(char name[], double value)
{
	if (VISUAL_LOG)
		printf("{VLOG:macro_ts;%d;%s;%f}\n", DAY, name, value);
}

void vlog_data(char key[], int n, ...)
{
	if (VISUAL_LOG)
	{
		printf("{VLOG:data;%d;%s", DAY, key);

		va_list valist;
		va_start(valist, n);
		int i;
		for (i = 0; i < n; i++) {
      		printf(";%f", va_arg(valist, double));
   		}
   		va_end(valist);

   		printf("}\n");
	}
}

void vlog_data_with_id(char key[], int id, int n,...)
{
	if (VISUAL_LOG)
	{
		printf("{VLOG:data;%d;%s;%d", DAY, key, id);

		va_list valist;
		va_start(valist, n);
		int i;
		for (i = 0; i < n; i++) {
      		printf(";%f", va_arg(valist, double));
   		}
   		va_end(valist);

   		printf("}\n");
	}
}

void log_transformation(char agent_type[], char outflow[], char inflow[], char process[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		if (fabs(value)>1e-6)
		{
			printf("{$->;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, outflow, process, value);
			printf("{$<-;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, inflow, process, value);
		}
}

void log_outflow(char agent_type[], char asset[], char liability[], char process[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		if (fabs(value)>1e-6)
		{
			printf("{$->;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, asset, process, value);
			printf("{$->;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, liability, process, value);
		}
}

void log_inflow(char agent_type[], char asset[], char liability[], char process[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		if (fabs(value)>1e-6)
		{
			printf("{$<-;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, asset, process, value);
			printf("{$<-;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, liability, process, value);
		}
}

void log_outflow_single(char agent_type[], char account[], char process[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		if (fabs(value)>1e-6)
			printf("{$->;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, account, process, value);

}

void log_inflow_single(char agent_type[], char account[], char process[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		if (fabs(value)>1e-6)
			printf("{$<-;%d;%d;%s;%s;%s;%f}\n", DAY, id, agent_type, account, process, value);

}

void log_stock(char agent_type[], char account[], int id, double value)
{
	if (STOCK_FLOW_LOG)
		printf("{$;%d;%d;%s;%s;stock;%f}\n", DAY, id, agent_type, account, value);
}

//FUNCTIONS TO USE QSORT FROM C'S STDLIB
/** \fn static int compare(const void * a, const void * b);
 * \brief Comparison function for qsort_int_array and qsort_double_array.
 */
/*static int compare(const void * a, const void * b)
{
  return (*(double*)a > *(double*)b) ? 1 : (*(double*)a < *(double*)b) ? -1:0 ;
}*/

/** \fn void qsort_int_array(int_array * A);
 * \brief API to qsort() from C's stdlib library to sort dynamic array of ints.
 */
/*void qsort_int_array(int_array * A)
{
	printf("\n qsort_double_array: A.size = %d\n", &A.size);
	qsort(&A.array, A.size, sizeof(int), compare);
}*/

/** \fn void qsort_double_array(int_array * A);
 * \brief API to qsort() from C's stdlib library to sort dynamic array of double.
 */
/*void qsort_double_array(double_array * A)
{
	printf("\n qsort_double_array: A.size = %d\n", &A.size);
	qsort(&A.array, A.size, sizeof(double), compare);
}*/

//Functions from xparser generated memory.c file

/** \fn int quicksort_int(int *array, int elements)
 *  \brief Sorts the elements of the integer array in ascending order.
 *  \param Pointer to integer array
 *  \param Number of elements that must be sorted
 *  \return integer value indicating success(0) or failure(1)
 */
/*
int quicksort_int(int * array, int elements)
{
	#define  LEVEL  1000
	int  pivot, begin[LEVEL], end[LEVEL], i=0, left, right ;
	begin[0]=0; end[0]=elements;
	while (i>=0)
	{
		left=begin[i]; right=end[i]-1;
		if (left<right)
		{
			pivot=array[left]; if (i==LEVEL-1) return 1;
			while (left<right)
			{
				while (array[right]>=pivot && left<right) right--;
				if (left<right) array[left++]=array[right];
				while (array[left]<=pivot && left<right) left++;
				if (left<right) array[right--]=array[left];
			}
			array[left]=pivot;
			begin[i+1]=left+1;
			end[i+1]=end[i];
			end[i++]=left;
		}
	    else
	    {
	      i--;
	    }
	}
	return 0;
}*/

/** \fn int quicksort_double(double *array, int elements)
 *  \brief Sorts the elements of the double array in ascending order.
 *  \param Pointer to double array
 *  \param Number of elements that must be sorted
 *  \return integer value indicating success(0) or failure(1)
 */
/*
int quicksort_double(double * array, int elements)
{
	#define  LEVEL  1000
	double pivot, begin[LEVEL], end[LEVEL];
	int  i=0, left, right ;
	begin[0]=0; end[0]=elements;
	while (i>=0)
	{
		left=begin[i]; right=end[i]-1;
		if (left<right)
		{
			pivot=array[left]; if (i==LEVEL-1) return 1;
			while (left<right)
			{
				while (array[right]>=pivot && left<right) right--;
				if (left<right) array[left++]=array[right];
				while (array[left]<=pivot && left<right) left++;
				if (left<right) array[right--]=array[left];
			}
			array[left]=pivot;
			begin[i+1]=left+1;
			end[i+1]=end[i];
			end[i++]=left;
		}
	    else
	    {
	      i--;
	    }
	}

	return 0;
}*/
