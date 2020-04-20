#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"


/** \fn Household_decide_to_attend_interview
 */
int Household_decide_to_attend_interview()
{
	INTERVIEWER_ID = 0;
	//WILLINGNESS_TO_ATTEND = 0;
	
	WILLINGNESS_TO_ATTEND = 1;


	//Intermediate solution: 

	//double random_nummer;
	
	//random_nummer = random_int(0,100);
	
	
	
	//RANDOM_NO_TEST = random_nummer;
	
	//random_nummer  = random_nummer/100.0;
	// If random_nummer < attending propability then the household would attend an interview
	//if(random_nummer < QUESTIONNAIRE_ATTENDING_PROPABILITY)
	//{
	//	WILLINGNESS_TO_ATTEND = 1;
		
	//}
	
	return 0;
}


/** \fn Household_respond
 */
int Household_respond()
{

	int_array id_list;
	init_int_array(&id_list);

	START_INTERVIEW_REQUEST_MESSAGE_LOOP
	
	//INTERVIEWER_ID = interview_request_message->firm_id;
	
	add_int(&id_list,interview_request_message->firm_id);

	//WILLINGNESS_TO_ATTEND = 0;
	
	FINISH_INTERVIEW_REQUEST_MESSAGE_LOOP
	//Respond if attend

	INTERVIEWER_ID = id_list.array[random_int(0,id_list.size)];

	add_interview_positive_response_message(ID, REGION_ID, INTERVIEWER_ID);
	
	
	//Free memory
	free_int_array(&id_list);

	return 0;
}



int Household_respond_questionnaire()
{
	int i;
	double price;
	double quality;
	double random_number, logit, logit_denominator,logit_sum;
	int price_done;
	int count;

	price_done=0;
	
	START_QUESTIONNAIRE_PRICING_MESSAGE_LOOP

	
	double_array price_list;
	init_double_array(&price_list);

	logit_sum=0;

	if(price_done==0)
	{
		for(i=0;i<questionnaire_pricing_message->times_increment;i++)
		{
		
			price = questionnaire_pricing_message->start_price + i *questionnaire_pricing_message->increment; 
			add_double(&price_list, price);
			price_done=1;
		}
	}
	
	quality = questionnaire_pricing_message->quality;


	for(i=0;i<CURRENT_PRICE_QUALITY_LIST.size;i++)
	{			
		if(CURRENT_PRICE_QUALITY_LIST.array[i].id != INTERVIEWER_ID)
		{
			logit_sum += exp(GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality)-log(CURRENT_PRICE_QUALITY_LIST.array[i].price)));
		}
	}
	
	//Prevent buffer overflow error: denominator logit sum <DBL_MAX
	//Note: assert__(x) evaluates to: for ( ; !(x) ; assert(x) )
	assert__(logit_sum<DBL_MAX || !SWITCH_ON_ASSERTS_ABOUT_EXPARG)
    {
        fprintf(stderr, "\nArgument of exp: %f", GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality)-log(CURRENT_PRICE_QUALITY_LIST.array[i].price)));
        fprintf(stderr, "\nPer argument of exp: GAMMA_CONST %f GAMMA_QUALITY %f log(CURRENT_PRICE_QUALITY_LIST.array[i].quality) %f log(CURRENT_PRICE_QUALITY_LIST.array[i].price) %f", GAMMA_CONST, GAMMA_QUALITY, log(CURRENT_PRICE_QUALITY_LIST.array[i].quality), log(CURRENT_PRICE_QUALITY_LIST.array[i].price));
        fprintf(stderr, "\n");
    }
    
	//printf("average_quality %f\n",average_quality);

	logit = 0;

	for(i=0; i< price_list.size; i++)
	{		
		
		logit_denominator = exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i]))) + logit_sum;
	 
	 	//Note: assert__(x) evaluates to: for ( ; !(x) ; assert(x) )
        assert__(logit_denominator<DBL_MAX || !SWITCH_ON_ASSERTS_ABOUT_EXPARG)
        {
            fprintf(stderr, "\n logit_denominator %f exp() %f logit_sum %f", logit_denominator, exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i]))), logit_sum);
            fprintf(stderr, "\nArgument of exp: %f", GAMMA_CONST*(GAMMA_QUALITY*log(quality)-log(price_list.array[i])));
            fprintf(stderr, "\nPer argument of exp: GAMMA_CONST %f GAMMA_QUALITY %f log(quality) %f log(price_list.array[i]) %f", GAMMA_CONST, GAMMA_QUALITY, log(CURRENT_PRICE_QUALITY_LIST.array[i].quality), log(CURRENT_PRICE_QUALITY_LIST.array[i].price));
            fprintf(stderr, "\n");
        }
        
		logit = exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i])))/ logit_denominator;
	 		
		//random process for purchasing decision:

	 	random_number = random_unif();

		if(random_number< logit)
		{
			QUESTIONNAIRE[i]=1;
			
	 	}else
		{
			QUESTIONNAIRE[i]=0;
		}
	}	


	//Send filled out questionnaire to firm	
	add_filled_out_questionnaire_pricing_message( INTERVIEWER_ID,REGION_ID, QUESTIONNAIRE);	


	free_double_array(&price_list);
					
	FINISH_QUESTIONNAIRE_PRICING_MESSAGE_LOOP


	int year_under_consideration;
	double quality_growth_rate, price_growth_rate;



	START_QUESTIONNAIRE_INNOVATION_MESSAGE_LOOP



	double_array price_list;
	init_double_array(&price_list);

	logit_sum=0;

	for(i=0;i<questionnaire_innovation_message->times_increment;i++)
	{
		
		price = questionnaire_innovation_message->start_price + i *questionnaire_innovation_message->increment; 
		add_double(&price_list, price);
		price_done=1;
	}

	
	quality = questionnaire_innovation_message->quality; 
	
	year_under_consideration=questionnaire_innovation_message->year_under_consideration;
	quality_growth_rate=questionnaire_innovation_message->growth_quality_index;
	price_growth_rate=questionnaire_innovation_message->growth_price_index;

	count=0; //count occurrence of errors
	for(i=0;i<CURRENT_PRICE_QUALITY_LIST.size;i++)
	{			
		if(CURRENT_PRICE_QUALITY_LIST.array[i].id != INTERVIEWER_ID)
		{
			logit_sum += exp(GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) - log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+price_growth_rate,year_under_consideration)) ));
            
            //Prevent buffer overflow error: denominator logit sum <DBL_MAX
           	
            if(logit_sum>DBL_MAX && count==0 && SWITCH_ON_WARNING_ABOUT_EXPARG)
            {
            	count=1;
                double exp_arg = GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) - log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+price_growth_rate,year_under_consideration)));
                
                fprintf(stderr, "\nWARNING: in %s, %s, line %d:", __FILE__, __FUNCTION__, __LINE__);
                fprintf(stderr, "\n IT %d ID %d index i=%d CURRENT_PRICE_QUALITY_LIST.size=%d", DAY, ID, i, CURRENT_PRICE_QUALITY_LIST.size);
                fprintf(stderr, "\n Output of exp: %f", exp(exp_arg) );
                fprintf(stderr, "\n Argument of exp: %f", exp_arg);
                fprintf(stderr, "\n");
                fprintf(stderr, "\nHINT: This error can occur if active firms = 0.");
                fprintf(stderr, "\n");
			}			
			//Note: assert__(x) evaluates to: for ( ; !(x) ; assert(x) )
            assert__(logit_sum<DBL_MAX || !SWITCH_ON_ASSERTS_ABOUT_EXPARG)
            {
                double exp_arg = GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) - log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+price_growth_rate,year_under_consideration)));
                
                fprintf(stderr, "\nWARNING: in %s, %s, line %d:", __FILE__, __FUNCTION__, __LINE__);
                fprintf(stderr, "\n IT %d ID %d index i=%d CURRENT_PRICE_QUALITY_LIST.size=%d", DAY, ID, i, CURRENT_PRICE_QUALITY_LIST.size);
                
                fprintf(stderr, "\n Output of exp: %f", exp(exp_arg) );
                fprintf(stderr, "\n Argument of exp: %f", exp_arg);
                fprintf(stderr, "\n exp_arg = GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) - log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+price_growth_rate,year_under_consideration)) )" );
                fprintf(stderr, "\n\t\t = GAMMA_CONST*(GAMMA_QUALITY*[1] - [2])" );
                fprintf(stderr, "\n\t\t = GAMMA_CONST*(GAMMA_QUALITY*[%f] - [%f])", log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)), log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+quality_growth_rate,year_under_consideration)) );
                fprintf(stderr, "\n\t\t = %f*(%f - %f) = %f", GAMMA_CONST, GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)), log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+quality_growth_rate,year_under_consideration)), GAMMA_CONST*(GAMMA_QUALITY*log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) - log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+price_growth_rate,year_under_consideration)))  );
                
                fprintf(stderr, "\n Per argument of exp:");
                
                fprintf(stderr, "\n\t GAMMA_CONST %f GAMMA_QUALITY %f", GAMMA_CONST, GAMMA_QUALITY);
                fprintf(stderr, "\n\t [1] = log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) %f", log(CURRENT_PRICE_QUALITY_LIST.array[i].quality*pow(1+quality_growth_rate,year_under_consideration)) );
                fprintf(stderr, "\n\t [2] = log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+quality_growth_rate,year_under_consideration)) %f", log(CURRENT_PRICE_QUALITY_LIST.array[i].price*pow(1+quality_growth_rate,year_under_consideration)) );
                
                fprintf(stderr, "\n\t log(CURRENT_PRICE_QUALITY_LIST.array[i].quality) %f", log(CURRENT_PRICE_QUALITY_LIST.array[i].quality));
                fprintf(stderr, "\n\t log(CURRENT_PRICE_QUALITY_LIST.array[i].price) %f", log(CURRENT_PRICE_QUALITY_LIST.array[i].price) );

                fprintf(stderr, "\n\t pow(1+quality_growth_rate,year_under_consideration) %f", pow(1+quality_growth_rate,year_under_consideration) );
                fprintf(stderr, "\n\t quality_growth_rate %f", quality_growth_rate );
                fprintf(stderr, "\n\t year_under_consideration %d", year_under_consideration );
                
                fprintf(stderr, "\n");
                fprintf(stderr, "\nHINT: This error can occur if active firms = 0.");
                fprintf(stderr, "\n");
            }
		}
	}


	//printf("average_quality %f\n",average_quality);

	logit = 0;

	count=0; //count occurrence of errors
	for(i=0; i< price_list.size; i++)
	{		
		
		logit_denominator = exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i]))) + logit_sum;
	 	
    
        //Prevent buffer overflow error: denominator logit sum <DBL_MAX
        if(logit_denominator>DBL_MAX && count==0 && SWITCH_ON_WARNING_ABOUT_EXPARG)
        {
        	fprintf(stderr, "\nWARNING: in %s, %s, line %d:", __FILE__, __FUNCTION__, __LINE__);
            fprintf(stderr, "\n logit_denominator %f exp() %f logit_sum %f", logit_denominator, exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i]))), logit_sum);
            fprintf(stderr, "\nArgument of exp: %f", GAMMA_CONST*(GAMMA_QUALITY*log(quality)-log(price_list.array[i])));
            fprintf(stderr, "\nPer argument of exp: GAMMA_CONST %f GAMMA_QUALITY %f log(quality) %f log(price_list.array[i]) %f", GAMMA_CONST, GAMMA_QUALITY, log(CURRENT_PRICE_QUALITY_LIST.array[i].quality), log(CURRENT_PRICE_QUALITY_LIST.array[i].price));
            fprintf(stderr, "\n");            
        }
		//Note: assert__(x) evaluates to: for ( ; !(x) ; assert(x) )
      	assert__(logit_denominator<DBL_MAX || !SWITCH_ON_ASSERTS_ABOUT_EXPARG)
        {
        	fprintf(stderr, "\nWARNING: in %s, %s, line %d:", __FILE__, __FUNCTION__, __LINE__);
            fprintf(stderr, "\n logit_denominator %f exp() %f logit_sum %f", logit_denominator, exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i]))), logit_sum);
            fprintf(stderr, "\nArgument of exp: %f", GAMMA_CONST*(GAMMA_QUALITY*log(quality)-log(price_list.array[i])));
            fprintf(stderr, "\nPer argument of exp: GAMMA_CONST %f GAMMA_QUALITY %f log(quality) %f log(price_list.array[i]) %f", GAMMA_CONST, GAMMA_QUALITY, log(CURRENT_PRICE_QUALITY_LIST.array[i].quality), log(CURRENT_PRICE_QUALITY_LIST.array[i].price));
            fprintf(stderr, "\n");            
        }
        
		logit = exp(GAMMA_CONST*(GAMMA_QUALITY*log(quality) -log(price_list.array[i])))/ logit_denominator;

		
	 		
		//random process for purchasing decision:

	 	random_number = random_unif();

		

		if(random_number< logit)
		{
			QUESTIONNAIRE[i]=1;
			
	 	}else
		{
			QUESTIONNAIRE[i]=0;
		}

	

	}	

	add_filled_out_questionnaire_product_innovation_message( INTERVIEWER_ID,REGION_ID,quality, QUESTIONNAIRE, year_under_consideration );


	free_double_array(&price_list);
					
	FINISH_QUESTIONNAIRE_INNOVATION_MESSAGE_LOOP


	
	return 0;
}
