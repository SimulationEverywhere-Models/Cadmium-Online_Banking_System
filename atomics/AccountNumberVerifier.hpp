#ifndef __ANV_HPP__
#define __ANV_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include "../data_structures/message.hpp"
#include <limits>
#include <assert.h>
#include <string>
#include<random>
using namespace cadmium;
using namespace std;

struct AccountNumberVerifier_defs{
        struct accountnumber_IN : public in_port<int> { };
	struct output_OUT : public out_port<Message_t> { };
    };


template<typename TIME>class AccountNumberVerifier{
        public:
	TIME busytime;
	AccountNumberVerifier() noexcept{
		busytime  = TIME("00:00:10");
		state.randaccountnumbervalid =0;
        state.model_active = false;
		}


struct state_type{
		int randaccountnumbervalid;
		bool model_active;
            };
	state_type state;


using input_ports = tuple<typename AccountNumberVerifier_defs::accountnumber_IN>;
	using output_ports = tuple<typename AccountNumberVerifier_defs:: output_OUT>;



void internal_transition()
	{
state.model_active = false;
}

void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
{
	vector<int> inputaccountnumber;
	inputaccountnumber = get_messages<typename AccountNumberVerifier_defs::accountnumber_IN>(mbs);
	if(inputaccountnumber[0])
	{
		state.randaccountnumbervalid = rand()%2;
		state.model_active = true;
	}

}


typename make_message_bags<output_ports>::type output() const {
typename make_message_bags<output_ports>::type bags;
Message_t output;
 if ( state.randaccountnumbervalid == 1 )
        {         
            output.valid = 1;
            output.invalid =0;
            get_messages<typename AccountNumberVerifier_defs::output_OUT>(bags).push_back(output);
        }
else
        {    
            output.valid = 0;
            output.invalid =1;
            get_messages<typename AccountNumberVerifier_defs::output_OUT>(bags).push_back(output);
         }
return bags;
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

  void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }


   friend std::ostringstream& operator<<(std::ostringstream& os, const typename AccountNumberVerifier<TIME>::state_type& i) {
                os << "account number valid: " << i.randaccountnumbervalid; 
            return os;
            }			
	};
#endif	
