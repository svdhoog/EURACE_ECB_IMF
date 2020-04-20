#include "../header.h"
#include "../Mall_agent_header.h"
#include "../my_library_header.h"


int Mall_send_id_to_firms()
{
    
    add_msg_mall_id_to_firms_message(ID,REGION_ID);
    return 0;
}


int Mall_initialize_firm_arrays()
{
    
if(XML_CLONED==1)
{

	// Reset the revenue array
	reset_sales_in_mall_array(&FIRM_REVENUES);

	// Reset up the current stock array
	reset_mall_stock_array(&CURRENT_STOCK);


	//read the messages sent by firms

	START_MSG_FIRM_ID_TO_MALLS_MESSAGE_LOOP
	
	add_mall_stock(&CURRENT_STOCK,msg_firm_id_to_malls_message->firm_id,  msg_firm_id_to_malls_message->region_id , 0.0,1.0,0.0,0.0) ;


	//Add on FIRM_REVENUES array
	add_sales_in_mall(&FIRM_REVENUES,msg_firm_id_to_malls_message->firm_id,0.0);

	FINISH_MSG_FIRM_ID_TO_MALLS_MESSAGE_LOOP



}	

    return 0;
}
