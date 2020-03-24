#ifndef __AAM_HPP__
#define __AAM_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include<random>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

struct AccountAccessManager_defs{
        struct input : public in_port<Message_t>{};
	struct van : public out_port<int> { };
        struct logout : public out_port<int> { };
    };

template<typename TIME>class AccountAccessManager{
        public:
	//Parameters to be overwriten when instantiating the atomic model
        // default constructor
        AccountAccessManager() noexcept{
		state.login_status =0;
		state.ian_status=0;
		state.account_number_enter = 0;
		state.model_active = false;
		
		}

	struct state_type{
		int account_number_enter, login_status, ian_status;
		bool model_active;
		double randAccountNumber;
		            };
	state_type state;

	using input_ports = tuple<typename AccountAccessManager_defs::input>;
	using output_ports = tuple<typename AccountAccessManager_defs:: van, typename AccountAccessManager_defs:: logout>;

	void internal_transition()
	{
		state.model_active =false;
	}
 // time_advance function
    TIME time_advance() const {
 	TIME next_internal;
        if(state.model_active == true)
        {
            next_internal = TIME("00:00:10:000");
        }
        else
        {
            next_internal = numeric_limits<TIME>::infinity();
        }
             return next_internal;
        }
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
	vector<Message_t> inputlogin;
	inputlogin = get_messages<typename AccountAccessManager_defs::input>(mbs);

		if(state.model_active == false)
		{
		if(inputlogin[0].valid == 1 && inputlogin[0].invalid == 0)
		{
			state.login_status = 1;
			state.ian_status = 0;
			state.randAccountNumber =  ((double)rand()/(double)RAND_MAX ) * 1000000;
			state.model_active = true;
		}
		
		
	
	
		if(inputlogin[0].valid == 1 && inputlogin[0].invalid == 1)
		{
		state.ian_status = 1;
		state.login_status = 1;
		state.randAccountNumber =  ((double)rand()/(double)RAND_MAX ) * 1000000;
		state.model_active = true;
		}
	
	}
	
		
	
}
 typename make_message_bags<output_ports>::type output() const {
typename make_message_bags<output_ports>::type bags;
       if ((state.ian_status == 1) && (state.login_status == 1) )
        {
get_messages<typename AccountAccessManager_defs::logout>(bags).push_back(1);
         
        }
	 if ( (state.login_status == 1) && (state.ian_status == 0) )
        {
	int account_number;
         account_number = (int) (state.randAccountNumber);
	get_messages<typename AccountAccessManager_defs::van>(bags).push_back(account_number);
        
        }

return bags;
	}
  void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename AccountAccessManager<TIME>::state_type& i) {
                os << "login_status: " << i.login_status<< " & ian_status " << i.ian_status << " Account number: " << i.randAccountNumber; 
            return os;
            }			
	};	
#endif
