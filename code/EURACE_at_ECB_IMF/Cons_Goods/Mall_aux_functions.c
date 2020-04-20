/*********************************
 * Mall_aux_functions.c 
 * Mall auxiliary functions.
 * *********************************
 * History:
 * 29/10/08 Sander 
 *********************************/
#include "../header.h"
#include "../Mall_agent_header.h"
#include "Mall_aux_header.h"


/* \fn: int sales_statistics_list_rank_sales_function(const void *x, const void *y)
 * \brief: 
 */
int sales_statistics_list_rank_sales_function(const void *x, const void *y)
{
    if( ((temporary_sales_statistics *)x)->sales < ((temporary_sales_statistics *)y)->sales) return -1;
    else if( ((temporary_sales_statistics *)x)->sales > ((temporary_sales_statistics *)y)->sales) return 1;
    else return 0;
}


