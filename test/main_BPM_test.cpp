//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include "../data_structures/message.hpp"
//Time class header
#include <NDTime.hpp>
//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/BillPaymentManager.hpp"
#include <iostream>
#include <string>
using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;
using TIME = NDTime;

struct outp_vba : public cadmium::out_port<int>{};


template<typename T>
class InputReader_Message_t : public iestream_input<Message_t,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : iestream_input<Message_t,T>(file_path) {}
};


int main(){

  const char * i_input_data = "../input_data/BPM_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_Message_t, TIME, const char* >("input_reader" , move(i_input_data));


    shared_ptr<dynamic::modeling::model> BPM1 = dynamic::translate::make_dynamic_atomic_model<BillPaymentManager, TIME>("BPM1");

dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_vba)};
    dynamic::modeling::Models submodels_TOP = {input_reader, BPM1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<BillPaymentManager_defs::VBA_OUT,outp_vba>("BPM1")
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<Message_t>::out,BillPaymentManager_defs::Input_IN>("input_reader","BPM1"),

    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );
static ofstream out_messages("../simulation_results/BPM_test_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/BPM_test_output_state.txt");
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
    r.run_until(NDTime("04:00:00:000"));
    return 0;
}
