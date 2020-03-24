//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>
//Time class header
#include <NDTime.hpp>

#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/AccountNumberVerifier.hpp"
#include "../atomics/BillPaymentManager.hpp"
#include "../atomics/PasswordVerifier.hpp"
#include "../data_structures/message.hpp"
//C++ headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;
using TIME = NDTime;

struct inp_accountnumber : public in_port<int>{};
struct outp_aelogout : public out_port<Message_t>{};
struct outp_aebillamount : public out_port<int>{};
/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_int : public iestream_input<int,T> {
public:
    InputReader_int() = default;
    InputReader_int(const char* file_path) : iestream_input<int,T>(file_path) {}
};

int main() {

/****** Input Reader atomic model instantiation *******************/
   
    const char * i_input1 ="../input_data/AE_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader1 = dynamic::translate::make_dynamic_atomic_model<InputReader_int, TIME, const char* >("input_reader1" , move(i_input1));

/****** ANV atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> ANV1 = dynamic::translate::make_dynamic_atomic_model<AccountNumberVerifier, TIME>("ANV1");

/****** BPM atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> BPM1 = dynamic::translate::make_dynamic_atomic_model<BillPaymentManager, TIME>("BPM1");

/****** PV atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> PV1 = dynamic::translate::make_dynamic_atomic_model<PasswordVerifier, TIME>("PV1");


 /*******TOP COUPLED MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_aebillamount),typeid(outp_aelogout)};
    dynamic::modeling::Models submodels_TOP = {input_reader1, ANV1, PV1, BPM1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<AccountNumberVerifier_defs::output_OUT,outp_aelogout>("ANV1"),
        dynamic::translate::make_EOC<BillPaymentManager_defs::VBA_OUT,outp_aebillamount>("BPM1")
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<int>::out, AccountNumberVerifier_defs::accountnumber_IN>("input_reader1","ANV1"),
	dynamic::translate::make_IC<AccountNumberVerifier_defs::output_OUT, PasswordVerifier_defs::Input_IN>("ANV1","PV1"),
        dynamic::translate::make_IC<PasswordVerifier_defs::Output_OUT,PasswordVerifier_defs::Input_IN>("PV1","PV1"),
	dynamic::translate::make_IC<PasswordVerifier_defs::Output_OUT, BillPaymentManager_defs::Input_IN>("PV1","BPM1")
    };
    shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

/*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/AE_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/AE_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until_passivate();
    return 0;
}


