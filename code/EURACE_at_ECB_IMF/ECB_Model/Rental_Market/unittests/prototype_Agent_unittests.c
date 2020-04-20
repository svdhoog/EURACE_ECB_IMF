/*
 * 1) initialize agent
 * 2) add memory pre-conditions
 * 3) initialise MBs
 * 4) add message pre-conditions
 * 5) evaluate function to be tested
 * 6) assertions on memory post-conditions
 * 7) create MB iterators
 * 8) assertions on message post-conditions
 * 9) free agent
 * 10) free MBs
 */

/*
 * \fn: void unittest_<Agentname>_function()
 * \brief: Unit test for: <Agentname>_function.
 * Status: NOT Tested
 */
//void unittest_<Agentname>_function()
//{
// //	int rc;
	
//     /************* At start of unit test, add one agent **************/
//  	unittest_init_<Agentname>_agent();
	
//     /***** Variables: Memory pre-conditions **************************/

// 	/***** Messages: initialize message boards **********************************/
// 	//MB_Create(&b_<message>, sizeof(m_<message>));

// 	/***** Messages: pre-conditions **********************************/
//     //add_<message>_message();
    	    
//     /***** Function evaluation ***************************************/
//  	<Agentname>_function();
    
//     /***** Variables: Memory post-conditions *****/
// //	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);


//     /***** Message: Adding message iterators ***************************************/
// 	//MB_Iterator_Create(b_<message>, &i_<message>);

//     /***** Messages: Message post-conditions *****/
// 	//start a reading loop
// /*
//     START_<message>_MESSAGE_LOOP
// 	     CU_ASSERT_EQUAL(<message>_message->var, value);
// 	     CU_ASSERT_DOUBLE_EQUAL(<message>_message->var, value, 1e-3);
// 	FINISH_<message>_MESSAGE_LOOP
// */	
//     /************* At end of unit test, free the agent **************/
//	unittest_free_<Agentname>_agent();
//     /************* At end of unit tests, free all Messages **********/
//     free_messages();
//}
