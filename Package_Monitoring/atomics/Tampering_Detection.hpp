#ifndef _Tampering_Detection_HPP__
#define _Tampering_Detection_HPP__


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
 struct TamperingDetection_defs{ 
 struct outTamper : public out_port<string> {};
 struct inO : public in_port<int> {};
 struct inTamper : public in_port<int> {};
 }; 
 
 //Atomic model class 
 template<typename TIME> class TamperingDetection{ 
   public: 
 //Ports definition 
 using input_ports = tuple<typename TamperingDetection_defs::inO, typename TamperingDetection_defs::inTamper>;
 using output_ports = tuple<typename TamperingDetection_defs::outTamper>; 
  
 //Model parameters to be overwritten during instantiation 
 struct state_type{ 
	//Declare the state variables here 
	bool touched;
	bool opened;
 }; 
 state_type state; 
 
//Default constructor without parameters 
 TamperingDetection (){ 
  //Define the default constructor here  
  state.touched = false;
  state.opened = false;
 }
 
 void internal_transition() {
  //Define internal transition here 
  state.touched = false;
 } 
 
 void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){ 
  //Define external transition here 
  vector <int> bag_port_in; //to store the input
  bag_port_in = get_messages<typename TamperingDetection_defs::inO>(mbs);
  if(bag_port_in[0]){
	  state.opened = bag_port_in[0];
	  bag_port_in = get_messages<typename TamperingDetection_defs::inTamper>(mbs);
	  if(bag_port_in[0]){
		  state.touched = true;
	  }
	  
  }else{
	  state.opened = false;
	  state.touched = false;
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
 vector <string> bag_port_out;
 if(state.touched){
	 bag_port_out.push_back("The package is tampered");
 }else{
	 bag_port_out.push_back("");
 }
 
 get_messages <typename TamperingDetection_defs::outTamper>(bags) = bag_port_out;

 return bags; 
 } 
  
 TIME time_advance() const { 
  TIME next_internal; 
 //Define time advance function here 
 if(state.touched){
	 next_internal = TIME("00:00:01:000");
 }else{
	 next_internal = numeric_limits<TIME>::infinity();
 }
 return next_internal; 
 }
 
 friend ostringstream& operator<<(ostringstream& os, const typename TamperingDetection<TIME>::state_type& state) { 
  //Define how to log the state here 
  os << "The box is touched. Y/N? " << state.touched;
	return os; 
 } 
};   
#endif //Tampering_Detection_HPP 
  