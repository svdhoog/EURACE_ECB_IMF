/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include <CUnit/Basic.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_statistics.h>

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{


//Global GSL RNG: global constant variable: continues to exist for whole duration of main
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */
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

