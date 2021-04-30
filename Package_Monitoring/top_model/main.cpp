//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/Light_Detection.hpp"
#include "../atomics/Tampering_Detection.hpp"
#include "../atomics/Excessive_Heat_Detection.hpp"
#include "../atomics/Tilting_Detection.hpp"
#include "../atomics/Humidity_Detection.hpp"

//C++ headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>


using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct inp_1 : public in_port<int>{};
struct inp_2 : public in_port<int>{};
struct inp_3 : public in_port<float>{};
struct inp_4 : public in_port<int>{};
struct inp_5 : public in_port<int>{};

/***** Define output ports for coupled model *****/
struct outp_1 : public out_port<string>{};
struct outp_2 : public out_port<string>{};
struct outp_3 : public out_port<string>{};
struct outp_4 : public out_port<string>{};
struct outp_5 : public out_port<string>{};


/****** Input Reader atomic model declaration *******************/
/*template<typename T>
class InputReader_Message_t : public iestream_input<Message_t,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : iestream_input<Message_t,T>(file_path) {}
};
*/
template<typename T>
class InputReader : public iestream_input<int,T> {
    public:
        InputReader () = default;
        InputReader (const char* file_path) : iestream_input<int,T>(file_path) {}
};

int main(int argc, char ** argv) {

    if (argc < 2) {
        cout << "Program used with wrong parameters. The program must be invoked as follow:";
        cout << argv[0] << " path to the input file " << endl;
        return 1; 
    }
    /****** Input Reader atomic model instantiation *******************/
    string input = argv[1];
    const char * i_input = input.c_str();
    shared_ptr<dynamic::modeling::model> input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader, TIME, const char* >("input_reader" , move(i_input));

    /****** LightDetection atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> ld1 = dynamic::translate::make_dynamic_atomic_model<LightDetection, TIME>("ld1");

    /****** TamperingDetection atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> tampd1 = dynamic::translate::make_dynamic_atomic_model<TamperingDetection, TIME>("tampd1");

    /****** ExcessiveHeatDetection atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> heat1 = dynamic::translate::make_dynamic_atomic_model<ExcessiveHeatDetection, TIME>("heat1");
	
	/****** TiltingDetection atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> tilt1 = dynamic::translate::make_dynamic_atomic_model<TiltingDetection, TIME>("tilt1");
	
	/****** HumidityDetection atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> hum1 = dynamic::translate::make_dynamic_atomic_model<HumidityDetection, TIME>("hum1");
	

    /*******NETWORKS COUPLED MODEL********/
    dynamic::modeling::Ports iports_BOD = {typeid(inp_1),typeid(inp_2)};
    dynamic::modeling::Ports oports_BOD = {typeid(outp_1),typeid(outp_2)};
    dynamic::modeling::Models submodels_BOD = {ld1, tampd1};
	
    dynamic::modeling::EICs eics_BOD = {
        dynamic::translate::make_EIC<inp_1, LightDetection_defs::inL>("ld1"),
         dynamic::translate::make_EIC<inp_2, TamperingDetection_defs::inTamper>("tampd1")
    };
    dynamic::modeling::EOCs eocs_BOD = {
        dynamic::translate::make_EOC<LightDetection_defs::outL,outp_1>("ld1"),
        dynamic::translate::make_EOC<TamperingDetection_defs::outTamper,outp_2>("tampd1")
    };
    dynamic::modeling::ICs ics_BOD = {
		dynamic::translate::make_IC<LightDetection_defs::outO,TamperingDetection_defs::inO>("ld1","tampd1")
	};
	
    shared_ptr<dynamic::modeling::coupled<TIME>> BOD;
    BOD = make_shared<dynamic::modeling::coupled<TIME>>(
        "BOD", submodels_BOD, iports_BOD, oports_BOD, eics_BOD, eocs_BOD, ics_BOD 
    );

    /*******Package Monitoring (TOP) SIMULATOR COUPLED MODEL********/
    dynamic::modeling::Ports iports_PM = {typeid(inp_1),typeid(inp_2),typeid(inp_3),typeid(inp_4),typeid(inp_5)};
    dynamic::modeling::Ports oports_PM = {typeid(outp_1),typeid(outp_2), typeid(outp_3), typeid(outp_4),typeid(outp_5)};
    dynamic::modeling::Models submodels_PM = {BOD, heat1, tilt1, hum1};
	
    dynamic::modeling::EICs eics_PM = {
         dynamic::translate::make_EIC<inp_3, ExcessiveHeatDetection_defs::inT>("heat1"),
         dynamic::translate::make_EIC<inp_4, TiltingDetection_defs::inTilt>("tilt1"),
		 dynamic::translate::make_EIC<inp_5, HumidityDetection_defs::inHum>("hum1")
    };
	
    dynamic::modeling::EOCs eocs_PM = {
        dynamic::translate::make_EOC<ExcessiveHeatDetection_defs::outT ,outp_3>("heat1"),
        dynamic::translate::make_EOC<TiltingDetection_defs::outTilt ,outp_4>("tilt1"),
		dynamic::translate::make_EOC<HumidityDetection_defs::outHum ,outp_5>("hum1")
    };
	
    dynamic::modeling::ICs ics_PM = {
        dynamic::translate::make_IC<ExcessiveHeatDetection_defs::outSlp, TiltingDetection_defs::inSlp>("heat1","tilt1")
    };
	
    shared_ptr<dynamic::modeling::coupled<TIME>> PM;
    PM = make_shared<dynamic::modeling::coupled<TIME>>(
        "PM", submodels_PM, iports_PM, oports_PM, eics_PM, eocs_PM, ics_PM
    );



    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/PM_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/PM_output_state.txt");
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
    dynamic::engine::runner<NDTime, logger_top> r(PM, {0});
    r.run_until_passivate();
    return 0;
}