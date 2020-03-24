#ifndef __TPM_HPP__
#define __TPM_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include "../data_structures/message.hpp"
#include <limits>
#include <assert.h>
#include <string>
#include<random>
using namespace cadmium;
using namespace std;
struct TransactionProcessManager_defs{
        struct VBA_IN : public in_port<int> { };
        struct Output_OUT : public out_port<Message_t> { };
    };
template<typename TIME>class TransactionProcessManager{
        public:
	TIME busytime;
	TransactionProcessManager() noexcept{
		busytime  = TIME("00:00:10");
		state.currentaccountbalance = 3000;
        state.model_active = false;
		}
struct state_type{
		int billamount,currentaccountbalance;
		bool model_active;
    
};state_type state;
    
using input_ports = tuple<typename TransactionProcessManager_defs::VBA_IN>;
using output_ports = tuple<typename TransactionProcessManager_defs:: Output_OUT>;
void internal_transition()
    {
state.model_active = false;
}
void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
{
	vector<int> vbain;
	vbain = get_messages<typename TransactionProcessManager_defs::VBA_IN>(mbs);

    if(state.model_active == false)
    {
        if(vbain[0])
        {
            state.billamount = vbain[0];
            state.currentaccountbalance = 3000 - state.billamount;
            state.model_active = true;
        }

    }
}
TIME time_advance() const {  
		TIME next_internal;
             if(state.model_active)
		{
			next_internal = busytime;
		}
	else{
		next_internal = numeric_limits<TIME>::infinity();
	}
	return next_internal;
}

typename make_message_bags<output_ports>::type output() const {
typename make_message_bags<output_ports>::type bags;
    Message_t output;
    output.valid = state.currentaccountbalance;
    output.invalid = 1;
    get_messages<typename TransactionProcessManager_defs::Output_OUT>(bags).push_back(output);
    return bags;
}
void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }
   friend std::ostringstream& operator<<(std::ostringstream& os, const typename TransactionProcessManager<TIME>::state_type& i) {
                os <<"bill amount: "<<i.billamount<< "Current Account Balance: " << i.currentaccountbalance; 
            return os;
            }			
	};
#endif
