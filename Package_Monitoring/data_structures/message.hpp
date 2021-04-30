#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** Message_t ****************/
/*******************************************/
struct Message_t{
  Message_t(){}
  Message_t(int light, int humanInteraction, float temp, int movement,int hum)
   :lightLevel(light), tampering(humanInteraction), excessiveHeat(temp), tilting(movement), humadity(hum){}

  	int  lightLevel;
  	int  tampering;
	float excessiveHeat;
	int tilting;
	int humadity;
	
};

istream& operator>> (istream& is, Message_t& msg);

ostream& operator<<(ostream& os, const Message_t& msg);


#endif // BOOST_SIMULATION_MESSAGE_HPP