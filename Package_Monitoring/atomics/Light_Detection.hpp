#ifndef _Light_Detection_HPP__
#define _Light_Detection_HPP__


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
 struct LightDetection_defs{ 
 struct outL : public out_port<string> {};
 struct outO : public out_port<int>{};
 struct inL : public in_port<int> {};
 }; 
 
 //Atomic model class 
 template<typename TIME> class LightDetection{ 
   public: 
   int value = 150;
 //Ports definition 
 using input_ports = tuple<typename LightDetection_defs::inL>;
 using output_ports = tuple<typename LightDetection_defs::outL, typename LightDetection_defs::outO>; 
  
 //Model parameters to be overwritten during instantiation 
 struct state_type{ 
	//Declare the state variables here 
	bool hasLight;
 }; 
 state_type state; 
 
//Default constructor without parameters 
 LightDetection (){ 
  //Define the default constructor here  
  state.hasLight = false;
 }
 
 void internal_transition() {
  //Define internal transition here 
  state.hasLight = false;
 } 
 
 void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){ 
  //Define external transition here 
  vector <int> bag_port_in; //to store the input
  bag_port_in = get_messages<typename LightDetection_defs::inL>(mbs);
  if(bag_port_in[0] > value){
	  state.hasLight = true;
  }else{
	  state.hasLight = false;
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

 get_messages <typename LightDetection_defs::outO>(bags).push_back(state.hasLight);
 
 if(state.hasLight){
	 get_messages <typename LightDetection_defs::outL>(bags).push_back("increase in light levels!");
 }else{
	 get_messages <typename LightDetection_defs::outL>(bags).push_back("increase in light levels!");
 }
 
 return bags; 
  } 
  
 TIME time_advance() const { 
  TIME next_internal; 
 //Define time advance function here 
 if(state.hasLight){
	 next_internal = TIME("00:00:01:000");
 }else{
	 next_internal = numeric_limits<TIME>::infinity();
 }
 return next_internal; 
 }
 
 friend ostringstream& operator<<(ostringstream& os, const typename LightDetection<TIME>::state_type& state) { 
  //Define how to log the state here 
  os << "The box is opend. Y/N? " << state.hasLight;
	return os; 
 } 
};   
#endif //Light_Detection_HPP 
  