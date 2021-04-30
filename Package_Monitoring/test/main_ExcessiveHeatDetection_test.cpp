//Cadmium Simulator headers 
#include <cadmium/modeling/ports.hpp> 
#include <cadmium/modeling/dynamic_model.hpp> 
#include <cadmium/modeling/dynamic_coupled.hpp> 
#include <cadmium/modeling/dynamic_model_translator.hpp> 
#include <cadmium/engine/pdevs_dynamic_runner.hpp> 
#include <cadmium/logger/common_loggers.hpp> 

//Time class header 
#include <NDTime.hpp> 

//Messages structures 
//#include "../data_structures/message.hpp" 

//Atomic model headers 
#include "../atomics/Excessive_Heat_Detection.hpp" 
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//C++ libraries 
#include <iostream> 
#include <string> 

using namespace std; 
using namespace cadmium; 
using namespace cadmium::basic_models::pdevs; 

using TIME = NDTime; 
/***** (1) *****/ 
/***** Define input port for coupled models *****/ 

/***** Define output ports for coupled model *****/ 
struct top_out_T: public out_port<string>{}; 
struct top_out_Slp: public out_port<int>{}; 


/***** (2) *****/ 
/****** Input Reader atomic model declaration *******************/ 

template<typename T> 
class InputReader : public iestream_input<float, T> { 
	public:
		InputReader () = default; 
		InputReader (const char* file_path) :
			iestream_input<float,T> (file_path) {}
}; 

/***** (3) *****/
int main(){
	/****** Input Reader atomic model instantiation *******************/ 
	const char * i_input_data = "../input_data/ExcessiveHeatDetection_input_test.txt"; 
	
	shared_ptr<dynamic::modeling::model> input_reader; 
	
	input_reader = dynamic::translate::make_dynamic_atomic_model 
		<InputReader, TIME, const char*>("input_reader", move(i_input_data)); 
		
	/***** (4) *****/ 
	/****** ExcessiveHeatDetection atomic model instantiation *******************/ 
	shared_ptr<dynamic::modeling::model> heat1; 
	heat1 = dynamic::translate::make_dynamic_atomic_model<ExcessiveHeatDetection, TIME>("heat1"); 
	
	/***** (5) *****/ 
	/*******TOP MODEL********/ 
	dynamic::modeling::Ports iports_TOP; 
	iports_TOP = {}; 
	
	dynamic::modeling::Ports oports_TOP; 
	oports_TOP = {typeid(top_out_T), typeid(top_out_Slp)}; 
	
	dynamic::modeling::Models submodels_TOP; 
	submodels_TOP = {input_reader, heat1};
	
	dynamic::modeling::EICs eics_TOP; 
	eics_TOP = {};  // NOT NEEDED BECAUSE IT IS EMTPY IN THIS EXAMPLE 
	
	dynamic::modeling::EOCs eocs_TOP; 
	eocs_TOP = {  
		dynamic::translate::make_EOC<ExcessiveHeatDetection_defs::outT,top_out_T>("heat1"),
		dynamic::translate::make_EOC<ExcessiveHeatDetection_defs::outSlp,top_out_Slp>("heat1")
	}; 
	
	dynamic::modeling::ICs ics_TOP; 
	ics_TOP = { 
		dynamic::translate::make_IC<iestream_input_defs<float>::out,ExcessiveHeatDetection_defs::inT>( "input_reader","heat1") 
	}; 
	
	shared_ptr<dynamic::modeling::coupled<TIME>> TOP; 
	TOP = make_shared<dynamic::modeling::coupled<TIME>>(  "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP ); 
	
	/***** (6) *****/ 
	/*************** Loggers *******************/ 
	static ofstream out_messages("../simulation_results/ExcessiveHeatDetection_test_output_messages.txt"); 
	struct oss_sink_messages{ 
	 static ostream& sink(){ 
		return out_messages; 
	 } 
	}; 
	
	static ofstream out_state("../simulation_results/ExcessiveHeatDetection_test_output_state.txt"); 
	struct oss_sink_state{ 
	 static ostream& sink(){ 
		return out_state; 
	 }
	};
	  
	  
	using state = logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>; 
	using log_messages = logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>; 
	using global_time_mes = logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
	using global_time_sta = logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;
	using logger_top  = logger::multilogger<state, log_messages, global_time_mes, global_time_sta>; 
	
	/***** (7) *****/ 
	/************** Runner call ************************/
	dynamic::engine::runner<NDTime, logger_top> r(TOP, {0}); 
	r.run_until(NDTime("00:01:00:000"));
	return 0;
}