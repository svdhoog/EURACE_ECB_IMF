/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "my_library_header.h"

#include <CUnit/Basic.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>

//Global GSL RNG: global constant variable: continues to exist for whole duration of main
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{
    
    const gsl_rng_type * T;
    unsigned long int gsl_seed;

    /******* GSL INIT */
    /* Use GSL Random Number Generator
    *
    * Has two environment variables:
    *   GSL_RNG_TYPE - generator type
    *   GSL_RNG_SEED - initial seed values
    * read and set through gsl_rng_env_setup()
    */

    /* Read environment variables if set - returned as gsl_rng_default and gsl_rng_default_seed */
    gsl_rng_env_setup();

    //returns a pointer to a newly-created instance of gsl_rng
    T = gsl_rng_default;
    FLAME_GSL_RNG = gsl_rng_alloc (T);

    gsl_seed = gsl_rng_default_seed;
        
    gsl_seed = 0;
    //gsl_seed = 1;
    
    if(gsl_seed == 0) gsl_seed = gsl_rng_default_seed;
    
    if(gsl_seed == 0) gsl_seed = (unsigned long int)time(NULL);

    gsl_rng_set(FLAME_GSL_RNG, gsl_seed);

    printf ("[GSL Random Number Generator] generator type: %s\n", gsl_rng_name (FLAME_GSL_RNG));
    printf ("[GSL Random Number Generator] seed = %lu\n", gsl_seed);
    printf ("[GSL Random Number Generator] first value = %lu\n", gsl_rng_get (FLAME_GSL_RNG));
/******* END GSL INIT */

    int i,j,k;
    int n=3;

    //Step 1. Sampling from multivariate distribution
    //Means: mortgage_sampling_means[3]
    //VarCovar matrix: mortgage_sampling_covar[9]

    double MORTGAGE_SAMPLING_MEANS[3]= {-1.7149,-0.3952,3.0422};
    double MORTGAGE_SAMPLING_COVAR[9]= {0.5412804112,0.0809859624,0.0329746046,
                                        0.0809859624,0.2892559524,0.0823459858,
                                        0.0329746046,0.0823459858,0.2043532423};
    double MORTGAGE_SAMPLING_FLOOR_CAPS[6]={0.05, 0.2, 10, 0.6, 1.2, 40};

    double sampled_loan_to_value=0.0;
    double exp_loan_to_value=0.0;
    double loan_to_value=0.0;

    double sampled_dsti=0.0;
    double exp_dsti=0.0;
    double dsti=0.0;
    
    double sampled_maturity=0.0;
    double exp_maturity=0.0;
    double maturity=0.0;

    double floor, ceil;

    //Initialize vectors
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

    //Sampling
    //n = 10;
    n = 60000;
    
    printf("\nDSTI,LTV,M,exp(DSTI),exp(LTV),exp(M),cap(DSTI),cap(LTV),cap(M)");

    for (i = 0; i < n; i++)
    {
        gsl_ran_multivariate_gaussian (FLAME_GSL_RNG, v, m, result);


        //Sample DSTI: use result from gsl_ran_multivariate_gaussian()
        sampled_dsti = gsl_vector_get(result,0);
        exp_dsti = exp(sampled_dsti);

        //transform using floor/ceiling:
        floor = MORTGAGE_SAMPLING_FLOOR_CAPS[0];
        ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[3];
        dsti = min(max(floor, exp_dsti), ceil);

        //Sample LTV: use result from gsl_ran_multivariate_gaussian()
        sampled_loan_to_value =  gsl_vector_get(result,1);
        exp_loan_to_value = exp(sampled_loan_to_value);

        //transform using floor/ceiling:
        floor = MORTGAGE_SAMPLING_FLOOR_CAPS[1];
        ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[4];
        loan_to_value = min(max(floor, exp_loan_to_value), ceil);
        
        //Sample Maturity in years: use result from gsl_ran_multivariate_gaussian()
        sampled_maturity =  gsl_vector_get(result,2);
        exp_maturity = exp(sampled_maturity);

        //transform using floor/ceiling:
        floor = MORTGAGE_SAMPLING_FLOOR_CAPS[2];
        ceil = MORTGAGE_SAMPLING_FLOOR_CAPS[5];
        maturity = min(max(floor, exp_maturity), ceil);


        printf("\n%f,%f,%f,%f,%f,%f,%f,%f,%f", gsl_vector_get(result,0), gsl_vector_get(result,1), gsl_vector_get(result,2), exp_dsti, exp_loan_to_value, exp_maturity, dsti, loan_to_value, maturity);
    }   

}
