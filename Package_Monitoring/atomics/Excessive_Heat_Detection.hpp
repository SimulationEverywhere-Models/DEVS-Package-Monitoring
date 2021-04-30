#ifndef _Excessive_Heat_Detection_HPP__
#define _Excessive_Heat_Detection_HPP__


//Include simulator headers 
#include <cadmium/modeling/ports.hpp> 
#include <cadmium/modeling/message_bag.hpp>
 
//Include other headers needed for the C++ implementation of the model #include <limits> 
#include <math.h>  
#include <assert.h> 
#include <limits>
#include <string>

//Include the relative path to the message types 
//#include "../data_structures/message.hpp" 

using namespace cadmium; 
using namespace std; 

//Port definition
 struct ExcessiveHeatDetection_defs{ 
 struct outT : public out_port<string> {};
 struct outSlp : public out_port<int> {};
 struct inT : public in_port<float> {};
 }; 
 
 //Atomic model class 
 template<typename TIME> class ExcessiveHeatDetection{ 
   public: 
   float value = 50.0;
 //Ports definition 
 using input_ports = tuple<typename ExcessiveHeatDetection_defs::inT>;
 using output_ports = tuple<typename ExcessiveHeatDetection_defs::outT, typename ExcessiveHeatDetection_defs::outSlp>; 
  
 //Model parameters to be overwritten during instantiation 
 struct state_type{ 
	//Declare the state variables here 
	bool hot;
 }; 
 state_type state; 
 
//Default constructor without parameters 
 ExcessiveHeatDetection (){ 
  //Define the default constructor here  
  state.hot = false;
 }
 
 void internal_transition() {
  //Define internal transition here 
  state.hot = false;
 } 
 
 void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){ 
  //Define external transition here 
  vector <float> bag_port_in; //to store the input
  bag_port_in = get_messages<typename ExcessiveHeatDetection_defs::inT>(mbs);
  if(bag_port_in[0] > value){
	 // state.tempValue = bag_port_in;
	  state.hot = true;
	}
 }
 
 void confluence_transition(TIME e,typename make_message_bags<input_ports>::type mbs){ 
  //Define confluence transition here 
 //Default definition 
 internal_transition(); 
 external_transition(TIME(), std::move(mbs)); 
 } 
  
 typename make_message_bags<output_ports>::type output() const { 
 
 typename make_message_bags<output_ports>::type bags; 
 
 get_messages <typename ExcessiveHeatDetection_defs::outSlp>(bags).push_back(state.hot);
 
 if(state.hot){
	  get_messages <typename ExcessiveHeatDetection_defs::outT>(bags).push_back("It's getting hot!");
 }else{
	  get_messages <typename ExcessiveHeatDetection_defs::outT>(bags).push_back("");
 }
 
 
 return bags; 
 } 
  
 TIME time_advance() const { 
  TIME next_internal; 
 //Define time advance function here 
 if(state.hot){
	 next_internal = TIME("00:00:01:000");
 }else{
	 next_internal = numeric_limits<TIME>::infinity();
 }
 return next_internal; 
 }
 
 friend ostringstream& operator<<(ostringstream& os, const typename ExcessiveHeatDetection<TIME>::state_type& state) { 
  //Define how to log the state here 
  os << "The box is hot. Y/N? " << state.hot;
	return os; 
 } 
};   
#endif //Heat_Detection_HPP 
  