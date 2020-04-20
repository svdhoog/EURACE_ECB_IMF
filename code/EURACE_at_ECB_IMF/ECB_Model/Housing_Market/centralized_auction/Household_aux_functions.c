#include "../../../header.h"
#include "../../../my_library_header.h"
#include "../../../Household_agent_header.h"
#include "Household_aux_header.h"

/** \fn Household_print_housing_states()
 * \brief Function to print states
 */
void Household_print_housing_states()
{
    int n=0;
    
    if (PLAN_BUY_HMR)  n=1;
    if (PLAN_SELL_HMR) n=2;
    if (PLAN_BUY_OP)   n=3;
    if (PLAN_SELL_OP)  n=4;
    
    printf("\n\nIT %d ID %d Housing Decision Branches \n", DAY, ID);
    
    printf("\n%d.%d Housing Market branch: (0) start primary states", n, 0);
    
    printf("\n\t\tIS_HOMELESS %d", IS_HOMELESS);
    printf("\n\t\tIS_TENANT   %d", IS_TENANT);
    printf("\n\t\tIS_OWNER    %d", IS_OWNER); 
    printf("\n\t\tIS_LANDLORD %d", IS_LANDLORD);
    printf("\n\t\tBTL_GENE    %d", BTL_GENE);

    printf("\n%d.%d Housing Market branch: (1) decision states", n, 1);
    
    printf("\n\t1\tPLAN_BUY_HMR  %d", PLAN_BUY_HMR);
    printf("\n\t2\tPLAN_SELL_HMR %d", PLAN_SELL_HMR);
    printf("\n\t3\tPLAN_BUY_OP   %d", PLAN_BUY_OP);
    printf("\n\t4\tPLAN_SELL_OP  %d", PLAN_SELL_OP);                
    
    printf("\n%d.%d Housing Market branch: (2) passive mortgage market states", n, 2);
    
    printf("\n\t\tHMR_MORTGAGE_GRANTED %d", HMR_MORTGAGE_GRANTED);
    printf("\n\t\tOP_MORTGAGE_GRANTED  %d", OP_MORTGAGE_GRANTED);
    printf("\n\t\tHAVE_HMR_MORTGAGE    %d", HAVE_HMR_MORTGAGE);
    printf("\n\t\tHAVE_OP_MORTGAGE     %d", HAVE_OP_MORTGAGE);
    
    printf("\n%d.%d Housing Market branch: (3) passive housing market states", n, 3);
    
    printf("\n\t\tJUST_PURCHASED_HMR %d", JUST_PURCHASED_HMR);
    printf("\n\t\tJUST_SOLD_HMR      %d", JUST_SOLD_HMR);
    printf("\n\t\tJUST_PURCHASED_OP  %d", JUST_PURCHASED_OP);
    printf("\n\t\tJUST_SOLD_OP       %d", JUST_SOLD_OP);                
    
    printf("\n%d.%d Mortgage Market branch: (4) active mortgage market states", n, 4);
    
    printf("\n\t\tHMR_MORTGAGE_ACCEPTED %d", HMR_MORTGAGE_ACCEPTED);
    printf("\n\t\tHMR_MORTGAGE_DELETED  %d", HMR_MORTGAGE_DELETED);
    printf("\n\t\tOP_MORTGAGE_ACCEPTED  %d", OP_MORTGAGE_ACCEPTED);
    printf("\n\t\tOP_MORTGAGE_DELETED   %d", OP_MORTGAGE_DELETED);

    printf("\n%d.%d Rental Market branch: (5) passive rental market states", n, 5);
    
    printf("\n\t\tPLAN_RENT_HMR      %d", PLAN_RENT_HMR);
    printf("\n\t\tPLAN_LET_OP        %d", PLAN_LET_OP);
    
    printf("\n%d.%d Rental Market branch: (5) active rental market states", n, 5);

    printf("\n\t\tDEFAULTED_ON_RENT %d", DEFAULTED_ON_RENT);
    printf("\n\t\tEVICTED_AS_TENANT %d", EVICTED_AS_TENANT);
    printf("\n\t\tEVICTED_AS_OWNER  %d", EVICTED_AS_OWNER);

    printf("\n%d.%d Housing Market branch: (6) final primary states", n, 6);
    
    printf("\n\t\tIS_HOMELESS %d", IS_HOMELESS);
    printf("\n\t\tIS_TENANT   %d", IS_TENANT);
    printf("\n\t\tIS_OWNER    %d", IS_OWNER); 
    printf("\n\t\tIS_LANDLORD %d", IS_LANDLORD);

}

void real_hmr_buying(void)
{

    //Transition: From Tenant -> Owner-occupier: buy HMR
    if (IS_TENANT && REAL_HMR_BUYING && MORTGAGE_ELIGIBILITY)
    {
        //Eq 34
        //Behavioral heuristic based on single criterion: Comparing Monthly cash outflows 
        double expected_expense = MORTGAGE_SAMPLING_DRAW.dsti * MEAN_NET_INCOME;


        if ((ESTIMATED_HOUSE_QUALITY>EPSILON && ESTIMATED_RENTAL_PRICE < INFINITY) || random_unif() < 0.5)
        {
            if (ESTIMATED_RENTAL_PRICE < INFINITY)
            {
                if (expected_expense < HOUSING_ALPHA * ESTIMATED_RENTAL_PRICE)
                {
                    REQUEST_MORTGAGE = 1;
                    PLAN_BUY_HMR = 1;

                    HMR_DECISION_BUY = 1;

                    vlog_data("hh_hmr_decision", 5, MORTGAGE_SAMPLING_DRAW.dsti, ESTIMATED_HOUSE_QUALITY, expected_expense, ESTIMATED_RENTAL_PRICE, 1.0);
                }
                else
                {
                    vlog_data("hh_hmr_decision", 5, MORTGAGE_SAMPLING_DRAW.dsti, ESTIMATED_HOUSE_QUALITY, expected_expense, ESTIMATED_RENTAL_PRICE, 0.0);
                }
            }
            else
            {
                vlog_data("hh_hmr_decision", 5, MORTGAGE_SAMPLING_DRAW.dsti, ESTIMATED_HOUSE_QUALITY, expected_expense, ESTIMATED_RENTAL_PRICE, 1.5);
                REQUEST_MORTGAGE = 1;
                PLAN_BUY_HMR = 1;

                HMR_DECISION_BUY = 1;

                HMR_DECISION_NO_AVAILABILITY_ON_RENTAL_MARKET = 1;
            }

        }
        else
        {
            HMR_DECISION_CANNOT_AFFORD = 1;
            vlog_data("hh_hmr_decision", 5, MORTGAGE_SAMPLING_DRAW.dsti, ESTIMATED_HOUSE_QUALITY, expected_expense, ESTIMATED_RENTAL_PRICE, 0.5);
        }

    }
}

void random_hmr_buying(void)
{   
    if (IS_TENANT && RANDOM_HMR_BUYING && MORTGAGE_ELIGIBILITY && random_unif() < RANDOM_HMR_PROPENSITY)
    {
        REQUEST_MORTGAGE = 1;
        PLAN_BUY_HMR = 1;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_HOUSING)
        {
            printf("\n\nIT %d ID %d RANDOM_HMR_BUYING", DAY, ID);
        }      
        #endif
    }
}

void random_hmr_selling(void)
{
    if (IS_OWNER && RANDOM_HMR_SELLING && random_unif() < RANDOM_HMR_PROPENSITY)
    {
        if( HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active==0 || 
            (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.active && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 0) )
        {
             PLAN_SELL_HMR = 1;

            #ifdef _DEBUG_MODE    
            if (PRINT_DEBUG_HOUSING)
            {
                printf("\n\nIT %d ID %d RANDOM_HMR_SELLING", DAY, ID);
            }      
            #endif
        }
       
    }
}

void real_hmr_selling(void)
{
    if (IS_OWNER && REAL_HMR_SELLING && random_unif() < RANDOM_HMR_PROPENSITY)
    {
        if (!HAVE_HMR_MORTGAGE || HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.mortgage.status == 0)
        {
            PLAN_SELL_HMR = 1;

            #ifdef _DEBUG_MODE    
            if (PRINT_DEBUG_HOUSING)
            {
                printf("\n\nIT %d ID %d RANDOM_HMR_SELLING: OBJECT_ID %d", DAY, ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.main_residence.object_id);
            }      
            #endif
        }
    }
}


void real_op_selling(void)
{
    int i;
    double income_expected __attribute__((unused)) = 0.0;
    double expenses_expected __attribute__((unused)) = 0.0;
    int object_id = 0;
    double value __attribute__((unused)) = 0.0;
    double expected_value __attribute__((unused)) = 0.0;
    double growth_factor __attribute__((unused)) = 0.0;


    if (IS_LANDLORD && REAL_OP_SELLING)
    {
        //Behavioral heuristic based on single criterion: Comparing Total cash outflows and inflows only
        //PLAN_SELL_OP==0: Only allow to sell one OP unit at a time
        //PLAN_SELL_HMR==0:  Only allow to sell one OP unit if not also selling HMR
        for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
        {
            if (PLAN_SELL_OP==0 && PLAN_SELL_HMR==0)
            {
                object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;

                if (object_id==0) fprintf(stderr, "\n\nIT %d ID: REAL_OP_SELLING %d WARNING object_id=0 in rental_property_list", DAY, ID);


    			//Only allow real OP selling if a mortgage exists (needed for setting expenses) and has PL status (0)
                if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.active 
                    && HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status==0)
                {
                    // Step 1: Set expected income, expenses, value for OP

                    //Total rental income EXPECTED across REMAINING mortgage maturity period
                    income_expected = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].monthly_rent * PRICE_INDEX / RENTAL_UNIT.cpi_base;

                    //Monthly mortgage interest EXPECTED compared to monthly rent income expected
                    expenses_expected = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.monthly_total_repayment;

                    //Property value
                    value = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].purchase_price;
                    
                    //Property value EXPECTED across FORWARD-looking horizon, using data on BACKWARD-looking horizon
                    // Eq (39-40):
                    growth_factor = pow( HOUSING_MARKET_PRICE_GROWTH_FACTOR, ((double) CONST_HOUSING_MARKET_FORWARD_HORIZON) / CONST_HOUSING_MARKET_BACKWARD_HORIZON );
                    
                    // Step 2: Check conditions Eq (35, 39)

                    //Only CAPITAL_GAIN_OBJECTIVE
                    if(growth_factor < 1.0 )
                    {
                        PLAN_SELL_OP = 1;
                            //SELL_OP_OBJECT_INDEX = i;
                        SELL_OP_OBJECT_ID = object_id;

                        vlog_data("hh_op_decide_sell", 4, growth_factor);

                            #ifdef _DEBUG_MODE    
                        if (PRINT_DEBUG_HOUSING)
                        {
                            printf("\n\n IT %d ID %d REAL_OP_SELLING, CAPITAL_GAIN_OBJECTIVE: SELL_OP_OBJECT_ID %d", DAY, ID, SELL_OP_OBJECT_ID);
                        }      
                            #endif
                    }
                } //if mortgage conditions
            } //if (PLAN_SELL_OP==0 && PLAN_SELL_HMR==0)
        } //for-loop
    } //IS_LANDLORD && REAL_OP_SELLING
}

void random_op_selling(void)
{
    int i;
    int object_id = 0;

    if (IS_LANDLORD && RANDOM_OP_SELLING && random_unif() < RANDOM_OP_PROPENSITY)
    {

        //PLAN_SELL_OP==0: Only allow to sell one OP unit at a time
        //PLAN_SELL_HMR==0:  Only allow to sell one OP unit if not also selling HMR
        for (i=0; i<HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.size; i++)
        {
            if (PLAN_SELL_OP==0 && PLAN_SELL_HMR==0)
            {
                object_id = HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id;

                if (object_id==0) fprintf(stderr, "\n\nIT %d ID: RANDOM_OP_SELLING %d WARNING object_id=0 in rental_property_list", DAY, ID);

                //Only allow random OP selling if: no mortgage, or mortgage is in PL status (0)
                if (HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].mortgage.status==0)
                {
                    PLAN_SELL_OP = 1;
                    //SELL_OP_OBJECT_INDEX = i;
                    SELL_OP_OBJECT_ID = object_id;



                    #ifdef _DEBUG_MODE    
                    if (PRINT_DEBUG_HOUSING)
                    {
                        printf("\n\n IT %d ID %d RANDOM_OP_SELLING: object_id %d SELL_OP_OBJECT_ID %d", DAY, ID, HOUSEHOLD_BALANCE_SHEET_CALENDAR.stocks.rental_property_list.array[i].object_id, SELL_OP_OBJECT_ID);
                    }      
                    #endif
                }
            } //if (PLAN_SELL_OP==0 && PLAN_SELL_HMR==0)
        } //for
    } //RANDOM_OP_SELLING
}

void real_op_buying(void)
{
    double value __attribute__((unused)) = 0.0;
    double value_expected __attribute__((unused)) = 0.0;
    double growth_factor = 0.0;

    double monthly_debt_service_expected = 0.0;

    int buy_rental_yield = 0;
    int buy_capital_gain = 0;

    if (BTL_GENE && REAL_OP_BUYING && MORTGAGE_ELIGIBILITY && isfinite(ESTIMATED_RENTAL_PRICE))
    {
        if(SWITCH_RENTAL_YIELD_OBJECTIVE)
        {
            //Total mortgage interest expected to pay across mortgage maturity period
            //expenses_expected = MORTGAGE_TOTAL_INTEREST_MEAN;
            //Set debt_service_expected equal to monthly mortgage payment (annuity)
            monthly_debt_service_expected = (1/12.0) * MORTGAGE_SAMPLING_DRAW.dsti * TOTAL_GROSS_ANNUAL_INCOME;

            //if (ESTIMATED_HOUSE_PRICE > EPSILON)
            //    monthly_debt_service_expected = monthly_debt_service_expected * min(1.0,(ESTIMATED_HOUSE_PRICE/MAX_EXPENDITURE));

            if( ESTIMATED_RENTAL_PRICE > HOUSING_GAMMA_RENTAL_YIELD * monthly_debt_service_expected )
            {
                buy_rental_yield = 1;



                #ifdef _DEBUG_MODE    
                if (PRINT_DEBUG_HOUSING)
                {
                    printf("\n\nIT %d ID %d REAL_OP_BUYING, RENTAL_YIELD_OBJECTIVE", DAY, ID);
                }      
                #endif
            }
        }
        else
        {
            buy_rental_yield = 1;
        }

        if(SWITCH_CAPITAL_GAIN_OBJECTIVE)
        {
            
            /* Property value GROWTH RATE across the BACKWARD looking horizon s in [t-1, t-S-1]
             * Required: a price vector of historical housing market prices for the past S+1 periods
             * price[1] = p_{t-1}
             * price[S+1] = p_{t-(S+1)} = p_{t-S-1}
             * PRICE_GROWTH_FACTOR = (p_{t-1}/p_{t-S-1})
             *
			 * Catch Exceptions:
			 * if p_{t-S-1}==0:  PRICE_GROWTH_FACTOR =  (p_{t-1}/p_{t-S})
			 * if p_{t-S}  ==0:  PRICE_GROWTH_FACTOR =  (p_{t-1}/p_{t-S+1}) etc.
			 * Look for first non-zero starting from price[S+1] to price[1].
			 */

            // Received from Eurostat
            // HOUSING_MARKET_PRICE_GROWTH_FACTOR = housing_market_price_history.array[1] / housing_market_price_history.array[s+1]

            //Property value EXPECTED across FORWARD-looking horizon, using data on BACKWARD-looking horizon
            // Eq (35-36):
            growth_factor = pow( HOUSING_MARKET_PRICE_GROWTH_FACTOR, ((double) CONST_HOUSING_MARKET_FORWARD_HORIZON) / CONST_HOUSING_MARKET_BACKWARD_HORIZON );

            //Not needed here: values are irrelevant because for cap. gain obj. we only care about the growth factor
            //value = HOUSING_MARKET_PRICE_MEAN;
            //value_expected = value * growth_factor;

            if(growth_factor > HOUSING_GAMMA_CAP_GAIN)
            {
                buy_capital_gain = 1;
                #ifdef _DEBUG_MODE    
                if (PRINT_DEBUG_HOUSING)
                {
                    printf("\n\nIT %d ID %d REAL_OP_BUYING, CAPITAL_GAIN_OBJECTIVE", DAY, ID);
                }      
                #endif
            }

        }
        else
        {
            buy_capital_gain = 1;
        }
        
        PLAN_BUY_OP = buy_rental_yield && buy_capital_gain;

        OP_DECISION_BUY = 1;

        if (PLAN_BUY_OP)
            vlog_data("hh_op_decide_buy", 4, MORTGAGE_SAMPLING_DRAW.dsti, ESTIMATED_RENTAL_PRICE, monthly_debt_service_expected, growth_factor);

        REQUEST_MORTGAGE = PLAN_BUY_OP;
    }
}

void random_op_buying(void)
{
    if (BTL_GENE && RANDOM_OP_BUYING && MORTGAGE_ELIGIBILITY && random_unif() < RANDOM_OP_PROPENSITY)
    {
        REQUEST_MORTGAGE = 1;
        PLAN_BUY_OP = 1;

        #ifdef _DEBUG_MODE    
        if (PRINT_DEBUG_HOUSING)
        {
            printf("\n\nIT %d ID %d RANDOM_OP_BUYING", DAY, ID);
        }      
        #endif
    }
}
