#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const Message_t& msg) {
  os << msg.lightLevel << " " << msg.tampering << " " << msg.excessiveHeat << " " << msg.tilting << " " << msg.humadity;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, Message_t& msg) {
  is >> msg.lightLevel;
  is >> msg.tampering;
  is >> msg.excessiveHeat;
  is >> msg.tilting;
  is >> msg.humadity;
  
  return is;
}
