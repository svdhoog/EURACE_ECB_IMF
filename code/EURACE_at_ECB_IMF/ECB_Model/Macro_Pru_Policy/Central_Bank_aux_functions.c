/*********************************
 * CentralBank_aux_functions.c 
 * CentralBank auxiliary functions.
 * *********************************/
#include "../../header.h"     
#include "../../CentralBank_agent_header.h"
#include "../../my_library_header.h"
#include "Central_Bank_aux_header.h"


/** \fn policy_response(indicator_adt * ind)
 * \brief Auxiliary function to compute the policy response, given a data structre indicator_adt ind.
 * \brief Status: OK PASS 11.10.17
 */
double policy_response(indicator_adt * ind, double y_floor, double y_ceiling)
{
	double input, output, x_floor, x_ceiling, a, b;

	input = (*ind).signal;

	x_floor = (*ind).x_floor;
	x_ceiling = (*ind).x_ceiling;

	// Transformations for linear part
	// positively sloped response functions have different coefficients than negatively sloped response functions
	if ((*ind).positively_sloped)
	{
		a = (y_ceiling*x_floor - y_floor*x_ceiling)/(x_floor - x_ceiling);

		b = (y_floor - y_ceiling)/(x_floor - x_ceiling);
	}
	else
	{
		a = (y_floor*x_floor - y_ceiling*x_ceiling)/(x_floor - x_ceiling);

		b = (y_ceiling - y_floor)/(x_floor - x_ceiling);
	}

	(*ind).intercept = a;
	(*ind).slope = b;

	// Proposed output:
	output = 0.0;

	//Catch nan values
	if (input==input)
	{
		fprintf(stderr, "\n %s/%s(), line %d", __FILE__, __FUNCTION__, __LINE__);
		fprintf(stderr, "\nIT %d input %f\n", DAY, input);
	}		
	else
		output = a + b * input;


	// Rectified linear unit output
	if ( output < y_floor )
		(*ind).response = y_floor;
	else if ( output > y_ceiling )
		(*ind).response = y_ceiling;
	else 
		(*ind).response = output;

    #ifdef _DEBUG_MODE    
    if (PRINT_MACROPRU_INFO)
    {
    	printf("\n\nIT %d ID %d %s() Policy_response of CentralBank", DAY, ID, __FUNCTION__);	 
    	printf("\n\tInputs:\n\ty_floor: %f\n\ty_ceiling: %f\n\tx_floor: %f\n\tx_ceiling: %f", y_floor, y_ceiling, (*ind).x_floor, (*ind).x_ceiling);
        printf("\n\tSignal: %f\n\tIntercept: %f\n\tSlope: %f\n\tProposed output: %f\n\tReLU response %f", input, (*ind).intercept, (*ind).slope, output, (*ind).response);
    }
    #endif
    
	return (*ind).response;
}
