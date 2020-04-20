/**
 * \file  unittest.c
 * \brief Holds main function of the unittest program.
 */
#include "../../header.h"
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
gsl_rng * FLAME_GSL_RNG;  /* global GSL random generator */

/*************************** unittest prototypes ***************************/
void unittest_standard_univariate_gaussian();
void unittest_multivariate_gaussian();

/*************************** end prototypes ***************************/

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{

    /******* GSL INIT */
    /* Use GSL Random Number Generator
    *
    * Has two environment variables:
    *   GSL_RNG_TYPE - generator type
    *   GSL_RNG_SEED - initial seed values
    * read and set through gsl_rng_env_setup()
    */

        const gsl_rng_type * T;
        unsigned long int gsl_seed;


    /* Read environment variables if set - returned as gsl_rng_default and gsl_rng_default_seed */
        gsl_rng_env_setup();

        //returns a pointer to a newly-created instance of gsl_rng
        T = gsl_rng_default;
        FLAME_GSL_RNG = gsl_rng_alloc (T);

        gsl_seed = gsl_rng_default_seed;
            
        if(gsl_seed == 0) gsl_seed = gsl_rng_default_seed;
        
        if(gsl_seed == 0) gsl_seed = (unsigned long int)time(NULL);

        gsl_rng_set(FLAME_GSL_RNG, gsl_seed);

        printf ("[GSL Random Number Generator] generator type: %s\n", gsl_rng_name (FLAME_GSL_RNG));
        printf ("[GSL Random Number Generator] seed = %lu\n", gsl_seed);
        printf ("[GSL Random Number Generator] first value = %lu\n", gsl_rng_get (FLAME_GSL_RNG));
    /******* END GSL INIT */


    CU_pSuite pSuite = NULL;
    
    /* Init FLAME */
    initialise_unit_testing();
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_Eurostat_unittests", init_suite1, clean_suite1);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* add extra tests using || */
    
    if(
        CU_add_test(pSuite, "standard_univariate_gaussian", unittest_standard_univariate_gaussian) == NULL //||
        //CU_add_test(pSuite, "multivariate_gaussian", unittest_multivariate_gaussian) == NULL //||
    )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    /* Free FLAME */
    clean_up(0);
    
    return CU_get_error();
}
