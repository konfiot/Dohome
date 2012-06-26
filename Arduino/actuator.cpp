#include "actuator.h"

Actuator::Actuator(const char *name) : _name(name){
}

bool Actuator::init(byte id){
  _id = id;
}

byte Actuator::getId(){
  return _id;
}
