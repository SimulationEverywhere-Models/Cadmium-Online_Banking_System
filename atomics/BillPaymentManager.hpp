#ifndef __BPM_HPP__
#define __BPM_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include "../data_structures/message.hpp"
#include <limits>
#include <assert.h>
#include <string>
#include<random>
using namespace cadmium;
using namespace std;
struct BillPaymentManager_defs{
        struct Input_IN : public in_port<Message_t> { };
        struct VBA_OUT : public out_port<int> { };

    };
template<typename TIME>class BillPaymentManager{
        public:
	TIME busytime;
	BillPaymentManager() noexcept{
		busytime  = TIME("00:00:10");
		state.randbillamount =0;
		state.currentaccountbalance = 3000;
		state.model_active = false;
		state.invalidbill = false;
		}
struct state_type{
		int randbillamount,currentaccountbalance;
		bool model_active, invalidbill;
            };
	state_type state;
using input_ports = tuple<typename BillPaymentManager_defs::Input_IN>;
using output_ports = tuple<typename BillPaymentManager_defs:: VBA_OUT>;
void internal_transition()
	{
state.invalidbill = false;
state.model_active = false;
}
void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
{
	vector<Message_t> input;
	input = get_messages<typename BillPaymentManager_defs::Input_IN>(mbs);


    if(state.model_active == false)
    {
        if(input[0].valid==1 && input[0].invalid != 1)
        {
            state.randbillamount = rand() %4000 +1;
            state.model_active = true;
            while(state.randbillamount>=state.currentaccountbalance)
            {
                state.randbillamount = rand() %4000 +1;
            }
        }
    }
}
TIME time_advance() const {  
		TIME next_internal;
    if(state.model_active)
		{
			next_internal = busytime;
		}
	else
        {
		next_internal = numeric_limits<TIME>::infinity();
	}
	return next_internal;
}

typename make_message_bags<output_ports>::type output() const {
typename make_message_bags<output_ports>::type bags;

get_messages<typename BillPaymentManager_defs::VBA_OUT>(bags).push_back(state.randbillamount);


return bags;
}
void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }
   friend std::ostringstream& operator<<(std::ostringstream& os, const typename BillPaymentManager<TIME>::state_type& i) {
                os << "bill amount: " << i.randbillamount << "Invalid bill? " <<i.invalidbill; 
            return os;
            }			
	};
#endif
