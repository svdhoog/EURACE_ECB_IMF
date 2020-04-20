#include <CUnit/Basic.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>

//Must be repeated here:
//Global GSL RNG: global constant variable: continues to exist for whole duration of main
extern gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*
 * \fn: void unittest_standard_univariate_gaussian()
 * \brief: Unit test for: gsl_ran_gaussian().
 * Status: NOT Tested
 */
void unittest_standard_univariate_gaussian()
{

	// Draw one time a vector of length 3: (DSTI,LTV,M)	
	double result = gsl_ran_gaussian (FLAME_GSL_RNG, 1.0);

	//Print result vector:
	printf("\n\tResult from gsl_ran_gaussian:"); 
	printf("\t %f", result);	
}


/*
 * \fn: void unittest_()
 * \brief: Unit test for: 
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
