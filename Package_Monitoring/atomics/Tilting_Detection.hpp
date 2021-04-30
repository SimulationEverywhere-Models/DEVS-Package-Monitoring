#ifndef _Tilting_Detection_HPP__
#define _Tilting_Detection_HPP___HPP__


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
 struct TiltingDetection_defs{ 
 struct outTilt : public out_port<string> {};
 struct inTilt : public in_port<int> {}; // Assuming input tilting is bool as a means to bypass computationally intensive workloads
 struct inSlp : public in_port<int> {};
 }; 
 
 //Atomic model class 
 template<typename TIME> class TiltingDetection{ 
   public: 
 //float value = 50;
 //Ports definition 
 using input_ports = tuple<typename TiltingDetection_defs::inTilt, typename TiltingDetection_defs::inSlp>;
 using output_ports = tuple<typename TiltingDetection_defs::outTilt>; 
  
 //Model parameters to be overwritten during instantiation 
 struct state_type{ 
	//Declare the state variables here 
	bool Tilted;
 }; 
 state_type state; 
 
//Default constructor without parameters 
 TiltingDetection (){ 
  //Define the default constructor here  
  state.Tilted = false;
 }
 
 void internal_transition() {
  //Define internal transition here 
  state.Tilted = false;
 } 
 
 void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){ 
  //Define external transition here 
  vector <int> bag_port_in; //to store the input
  bag_port_in = get_messages<typename TiltingDetection_defs::inSlp>(mbs);
  if(!bag_port_in[0]){
	  bag_port_in = get_messages<typename TiltingDetection_defs::inTilt>(mbs);
	  if(bag_port_in[0]){
	  state.Tilted = true;
	  }
  }else{
	  state.Tilted = false;
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

 if(state.Tilted){
	 get_messages <typename TiltingDetection_defs::outTilt>(bags).push_back("Tilting Detected!");
 }else{
	 get_messages <typename TiltingDetection_defs::outTilt>(bags).push_back("");
 } 



 return bags; 
  } 
  
 TIME time_advance() const { 
  TIME next_internal; 
 //Define time advance function here 
 if(state.Tilted){
	 next_internal = TIME("00:00:01:000");
 }else{
	 next_internal = numeric_limits<TIME>::infinity();
 }
 return next_internal; 
 }
 
 friend ostringstream& operator<<(ostringstream& os, const typename TiltingDetection<TIME>::state_type& state) { 
  //Define how to log the state here 
  os << "Tilting Y/N?" << state.Tilted;
	return os; 
 } 
};   
#endif 
  