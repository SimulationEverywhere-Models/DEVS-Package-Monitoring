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
#include "../atomics/Tilting_Detection.hpp" 
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
struct top_out: public out_port<string>{}; 

/***** (2) *****/ 
/****** Input Reader atomic model declaration *******************/ 

template<typename T> 
class InputReader_Tilting : public iestream_input<int, T> { 
	public:
		InputReader_Tilting () = default; 
		InputReader_Tilting (const char* file_path) : iestream_input<int,T> (file_path) {}
}; 

template<typename T> 
class InputReader_Sleep : public iestream_input<int, T> { 
	public:
		InputReader_Sleep () = default; 
		InputReader_Sleep (const char* file_path) : iestream_input<int,T> (file_path) {}
}; 

/***** (3) *****/
int main(){
	/****** Input Reader atomic model instantiation *******************/ 
	const char * i_input_data_Tilting = "../input_data/TiltingDetection_inputTilt_test.txt"; 
	shared_ptr<dynamic::modeling::model> input_reader_Tilting; 
	input_reader_Tilting = dynamic::translate::make_dynamic_atomic_model 
		<InputReader_Tilting, TIME, const char*>("input_reader_Tilting", move(i_input_data_Tilting)); 
		
		
	const char * i_input_data_Sleep = "../input_data/TiltingDetection_inputSleep_test.txt"; 
	shared_ptr<dynamic::modeling::model> input_reader_Sleep; 
	input_reader_Sleep = dynamic::translate::make_dynamic_atomic_model 
		<InputReader_Sleep, TIME, const char*>("input_reader_Sleep", move(i_input_data_Sleep)); 
		
	/***** (4) *****/ 
	/****** TiltingDetection atomic model instantiation *******************/ 
	shared_ptr<dynamic::modeling::model> tilt1; 
	tilt1 = dynamic::translate::make_dynamic_atomic_model<TiltingDetection, TIME>("tilt1"); 
	
	/***** (5) *****/ 
	/*******TOP MODEL********/ 
	dynamic::modeling::Ports iports_TOP; 
	iports_TOP = {}; 
	
	dynamic::modeling::Ports oports_TOP; 
	oports_TOP = {typeid(top_out)}; 
	
	dynamic::modeling::Models submodels_TOP; 
	submodels_TOP = {input_reader_Tilting, input_reader_Sleep, tilt1};
	
	dynamic::modeling::EICs eics_TOP; 
	eics_TOP = {};  // NOT NEEDED BECAUSE IT IS EMTPY IN THIS EXAMPLE 
	
	dynamic::modeling::EOCs eocs_TOP; 
	eocs_TOP = {  
		dynamic::translate::make_EOC<TiltingDetection_defs::outTilt,top_out>("tilt1")
	}; 
	
	dynamic::modeling::ICs ics_TOP; 
	ics_TOP = { 
		dynamic::translate::make_IC<iestream_input_defs<int>::out,TiltingDetection_defs::inSlp>( "input_reader_Sleep","tilt1"), 
		dynamic::translate::make_IC<iestream_input_defs<int>::out,TiltingDetection_defs::inTilt>( "input_reader_Tilting","tilt1") 
	}; 
	
	shared_ptr<dynamic::modeling::coupled<TIME>> TOP; 
	TOP = make_shared<dynamic::modeling::coupled<TIME>>(  "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP ); 
	
	/***** (6) *****/ 
	/*************** Loggers *******************/ 
	static ofstream out_messages("../simulation_results/TiltingDetection_test_output_messages.txt"); 
	struct oss_sink_messages{ 
	 static ostream& sink(){ 
		return out_messages; 
	 } 
	}; 
	
	static ofstream out_state("../simulation_results/TiltingDetection_test_output_state.txt"); 
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