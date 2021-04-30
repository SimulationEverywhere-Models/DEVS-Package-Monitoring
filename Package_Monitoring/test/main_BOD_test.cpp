//Cadmium Simulator headers 
#include <cadmium/modeling/ports.hpp> 
#include <cadmium/modeling/dynamic_model.hpp> 
#include <cadmium/modeling/dynamic_coupled.hpp> 
#include <cadmium/modeling/dynamic_model_translator.hpp> 
#include <cadmium/engine/pdevs_dynamic_runner.hpp> 
#include <cadmium/logger/common_loggers.hpp> 

//Time class header 
#include <NDTime.hpp> 



//Atomic model headers 
#include "../atomics/Light_Detection.hpp" 
#include "../atomics/Tampering_Detection.hpp" 
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
struct top_out_1: public out_port<string>{}; 
struct top_out_2: public out_port<string>{}; 

/***** (2) *****/ 
/****** Input Reader atomic model declaration *******************/ 

template<typename T> 
class InputReader_Light : public iestream_input<int, T> { 
	public:
		InputReader_Light () = default; 
		InputReader_Light (const char* file_path) :
			iestream_input<int,T> (file_path) {}
}; 

template<typename T> 
class InputReader_Tamper : public iestream_input<int, T> { 
	public:
		InputReader_Tamper () = default; 
		InputReader_Tamper (const char* file_path) :
			iestream_input<int,T> (file_path) {}
}; 

/***** (3) *****/
int main(){
	/****** Input Reader atomic model instantiation *******************/ 
	const char * i_input_light= "../input_data/BOD_inputL_test.txt"; 
	shared_ptr<dynamic::modeling::model> input_reader_light; 
	input_reader_light = dynamic::translate::make_dynamic_atomic_model 
		<InputReader_Light, TIME, const char*>("input_reader_light", move(i_input_light)); 
		
	const char * i_input_tamper = "../input_data/BOD_inputTamper_test.txt"; 
	shared_ptr<dynamic::modeling::model> input_reader_tamper; 
	input_reader_tamper = dynamic::translate::make_dynamic_atomic_model 
		<InputReader_Tamper, TIME, const char*>("input_reader_tamper", move(i_input_tamper)); 
		
	/***** (4) *****/ 
	/****** TamperingDetection atomic model instantiation *******************/ 
	shared_ptr<dynamic::modeling::model> tamd1; 
	tamd1 = dynamic::translate::make_dynamic_atomic_model<TamperingDetection, TIME>("tamd1"); 
	
	/****** LightDetection atomic model instantiation *******************/ 
	shared_ptr<dynamic::modeling::model> ld1; 
	ld1 = dynamic::translate::make_dynamic_atomic_model<LightDetection, TIME>("ld1"); 
	
	
	
	/***** (5) *****/ 
	/*******TOP MODEL********/ 
	dynamic::modeling::Ports iports_TOP; 
	iports_TOP = {}; 
	
	dynamic::modeling::Ports oports_TOP; 
	oports_TOP = {typeid(top_out_1), typeid(top_out_2)}; 
	
	dynamic::modeling::Models submodels_TOP; 
	submodels_TOP = {input_reader_light, input_reader_tamper ,ld1 ,tamd1};
	
	dynamic::modeling::EICs eics_TOP; 
	eics_TOP = {};  // NOT NEEDED BECAUSE IT IS EMTPY IN THIS EXAMPLE 
	
	dynamic::modeling::EOCs eocs_TOP; 
	eocs_TOP = {  
		dynamic::translate::make_EOC<LightDetection_defs::outL,top_out_1>("ld1"),
		dynamic::translate::make_EOC<TamperingDetection_defs::outTamper,top_out_2>("tamd1")
	}; 
	
	dynamic::modeling::ICs ics_TOP; 
	ics_TOP = { 
		dynamic::translate::make_IC<iestream_input_defs<int>::out,LightDetection_defs::inL>( "input_reader_light","ld1"),
		dynamic::translate::make_IC<iestream_input_defs<int>::out,TamperingDetection_defs::inTamper>("input_reader_tamper","tamd1"),
		dynamic::translate::make_IC<LightDetection_defs::outO,TamperingDetection_defs::inO>("ld1","tamd1")
		
	}; 
	
	shared_ptr<dynamic::modeling::coupled<TIME>> TOP; 
	TOP = make_shared<dynamic::modeling::coupled<TIME>>(  "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP ); 
	
	/***** (6) *****/ 
	/*************** Loggers *******************/ 
	static ofstream out_messages("../simulation_results/BOD_test_output_messages.txt"); 
	struct oss_sink_messages{ 
	 static ostream& sink(){ 
		return out_messages; 
	 } 
	}; 
	
	static ofstream out_state("../simulation_results/BOD_test_output_state.txt"); 
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