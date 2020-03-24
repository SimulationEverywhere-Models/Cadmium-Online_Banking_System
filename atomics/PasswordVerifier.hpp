#ifndef __PV_HPP__
#define __PV_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include "../data_structures/message.hpp"
#include <limits>
#include <assert.h>
#include <string>
#include<random>
using namespace cadmium;
using namespace std;
struct PasswordVerifier_defs{
        struct Input_IN : public in_port<Message_t> { };
        struct Output_OUT : public out_port<Message_t> { };

    };
template<typename TIME>class PasswordVerifier{
        public:
	TIME busytime;
	PasswordVerifier() noexcept{
		busytime  = TIME("00:00:10");
		state.randpasswordnumber =0;
        state.model_active = false;
		}
struct state_type{
		int randpasswordnumber;
		bool model_active,valid;
            };
	state_type state;

using input_ports = tuple<typename PasswordVerifier_defs::Input_IN>;
using output_ports = tuple<typename PasswordVerifier_defs:: Output_OUT>;


void internal_transition()
	{
state.model_active = false;
}

void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
{
	vector<Message_t> input;
	input = get_messages<typename PasswordVerifier_defs::Input_IN>(mbs);
    if(state.model_active == false)
    {
        if((input[0].valid == 1 &&input[0].invalid ==1)||(input[0].valid == 1 && input[0].invalid ==0) )
        {
            state.randpasswordnumber = rand()%2;
            if(state.randpasswordnumber == 1 )
            {
                state.valid = true;
                state.model_active = true;
            }
            else if(state.randpasswordnumber == 0)
            {
                state.valid = false;
                state.model_active = true;
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
Message_t output;
    if ( state.valid == true )
        {         
            output.valid = 1;
            output.invalid = 2;
            get_messages<typename PasswordVerifier_defs::Output_OUT>(bags).push_back(output);
        }
    else
    {
        output.valid = 1;
        output.invalid = 1;
        get_messages<typename PasswordVerifier_defs::Output_OUT>(bags).push_back(output);
    }
return bags;
}

  void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }
   friend std::ostringstream& operator<<(std::ostringstream& os, const typename PasswordVerifier<TIME>::state_type& i) {
                os << "password number: " << i.randpasswordnumber; 
            return os;
            }			
	};
#endif
