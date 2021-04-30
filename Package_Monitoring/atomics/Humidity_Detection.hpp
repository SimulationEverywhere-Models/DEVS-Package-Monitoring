#ifndef _Humidity_Detection_HPP__
#define _Humidity_Detection_HPP__


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
 struct HumidityDetection_defs{ 
 struct outHum : public out_port<string> {};
 struct inHum : public in_port<int> {}; // Assuming input humidity is int for simplicity
 }; 
 
 //Atomic model class 
 template<typename TIME> class HumidityDetection{ 
   public: 
   int value = 50;
 //Ports definition 
 using input_ports = tuple<typename HumidityDetection_defs::inHum>;
 using output_ports = tuple<typename HumidityDetection_defs::outHum>; 
  
 //Model parameters to be overwritten during instantiation 
 struct state_type{ 
	//Declare the state variables here 
	bool isHumid;
 }; 
 state_type state; 
 
//Default constructor without parameters 
 HumidityDetection (){ 
  //Define the default constructor here  

  state.isHumid = false;
 }
 
 void internal_transition() {
  //Define internal transition here 
  state.isHumid = false;
 } 
 
 void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){ 
  //Define external transition here 
  vector <int> bag_port_in; //to store the input
  bag_port_in = get_messages<typename HumidityDetection_defs::inHum>(mbs);
  if(bag_port_in[0] > value){
	  //state.current = bag_port_in;
	  state.isHumid = true;
  }else{
	  state.isHumid = false;
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
  
 if(state.isHumid){
	  get_messages <typename HumidityDetection_defs::outHum>(bags).push_back("It is getting humid!");
 }else{
	  get_messages <typename HumidityDetection_defs::outHum>(bags).push_back("");
 }


 return bags; 
  } 
  
 TIME time_advance() const { 
  TIME next_internal; 
 //Define time advance function here 
 if(state.isHumid){
	 next_internal = TIME("00:00:01:000");
 }else{
	 next_internal = numeric_limits<TIME>::infinity();
 }
 return next_internal; 
 }
 
 friend ostringstream& operator<<(ostringstream& os, const typename HumidityDetection<TIME>::state_type& state) { 
  //Define how to log the state here 
  os << "Humidity is high Y/N? " << state.isHumid;
	return os; 
 } 
};   
#endif 
  