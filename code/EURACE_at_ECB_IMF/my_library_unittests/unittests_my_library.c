#include <CUnit/Basic.h>
#include "../header.h"
#include "../my_library_header.h"

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>

#include <limits.h>

//Must be repeated here:
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

void unittest_random_int()
{   
	int i,a;
	for(i=0;i<100;i++)
	{
		a= random_int(0,100);
		printf("%d ",a);
	}

}

void unittest_random_unif()
{   
	int i;
	for(i=0;i<100;i++)
	{
		printf("%f ",random_unif());
	}

}


void unittest_my_stats_mean()
{
	double_array data;
	int i;
	double mu;

	for(i=0;i<4;i++)
		add_double(&data, (double)i);

	mu = my_stats_mean(&data);

	printf("\n mu=%.2f", mu);

	CU_ASSERT_DOUBLE_EQUAL(mu, 2.0, 1e-3);
	//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);
}

void unittest_my_stats_sd()
{
	double_array data;
	int i;
	double sd;

	for(i=1;i<4;i++)
		add_double(&data, (double)i);

	sd = my_stats_sd(&data);

	printf("\n sd=%.2f", sd);

	CU_ASSERT_DOUBLE_EQUAL(sd, 1.0, 1e-3);
	//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);
    //	CU_ASSERT_EQUAL(var, result);
}

/*
void unittest_inverse_gaussian()
{
	int i;
	double x,y;
	double mu = 0.0;
	double lambda = 1.0;

	for (i=0; i<10; i++)
	{
		x = (double) i;

		y = inverse_gaussian(x,mu,lambda);
		
		printf("\n inverse_gaussian: %.4f\n", y);
	}
}
*/

/*
 * \fn: void unittest_Household_mortgage_loan_application()
 * \brief: Unit test for: Household_mortgage_loan_application.
 * Status: NOT Tested
 */
void unittest_multivariate_gaussian()
{
	int i,j,k;
	const int n=3;

	//Step 1. Sampling from multivariate distribution
	//Means: mortgage_sampling_means[3]
	//VarCovar matrix: mortgage_sampling_covar[9]

	double MORTGAGE_SAMPLING_MEANS[3]= {1.0,1.0,1.0};
	double MORTGAGE_SAMPLING_COVAR[9]= {1.0,1.0,1.0,
										1.0,1.0,1.0,
										1.0,1.0,1.0};


	//Initialize vector
	
	gsl_vector * v = gsl_vector_alloc (n);
	gsl_vector * result = gsl_vector_alloc (n);

 	for (i = 0; i < n; i++)
    	{
    		gsl_vector_set (v, i, MORTGAGE_SAMPLING_MEANS[i]);
    	}

	//Initialize matrix
	gsl_matrix * m = gsl_matrix_alloc (n, n);
  
	for (i = 0; i < n; i++)
    	for (j = 0; j < n; j++)
    	{
    		k=i*n+j;
    		gsl_matrix_set (m, i, j, MORTGAGE_SAMPLING_COVAR[k]);
		}

	// Draw one time a vector of length 3: (DSTI,LTV,M)	
	gsl_ran_multivariate_gaussian (FLAME_GSL_RNG, v, m, result);

	//Print result vector:
	printf("\n\tResult from gsl_ran_multivariate_gaussian:"); 
	printf("\t Sampled LTV: %f", gsl_vector_get(result,0) );
	printf("\t Sampled DSTI: %f", gsl_vector_get(result,1));
	printf("\t Sampled Maturity: %f", gsl_vector_get(result,2));
}

void unittest_max_exp()
{
	printf("\n");

	printf("\n DBL_DIG: %d", DBL_DIG);
	printf("\n DBL_MAX: %g", DBL_MAX);
	

	int x;
	for (x=708; x<=710; x++)
		printf("\n%d %g", x, exp(x), 2*exp(x), exp(x)/(2*exp(x)));
	printf("\n");

	double y;
	double z;
	for (x=708; x<=710; x++)
	{
		z=(double) x;
		printf("\n%.1f %g %g %g %d %d", z, exp(z), 2*exp(z), exp(z)/(2*exp(z)), z<710, exp(z)<DBL_MAX);

		y = x+0.5;
		printf("\n%.1f %g %g %g %d %d", y, exp(y), 2*exp(y), exp(y)/(2*exp(y)), z<710, exp(z)<DBL_MAX);
	}
	printf("\n");

}
