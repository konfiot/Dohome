#include <Arduino.h>
#include "led_actuator.h"

Led_Actuator::Led_Actuator(const char *name, byte pin) : Actuator::Actuator(name), _pin(pin){
}

bool Led_Actuator::init(byte id){  
  Actuator::init(id);
  pinMode(_pin, OUTPUT);
}

bool Led_Actuator::prepare(const char *arg){
    int val;
    val = atoi(arg);
    if ((val == 0) || (val == 1)){
        _val = val;
        return true;
    } else {
        return false;
    }
}

bool Led_Actuator::exec(){
    if(_val == 1){
        digitalWrite(_pin, HIGH);
    } else if(_val == 0){
        digitalWrite(_pin, LOW);
    }
}

bool Led_Actuator::getJSONData(BufferFiller &buf){
    buf.emit_p(PSTR("{\"v\":\"$D\",\"i\":\"$D\",\"n\":\"$S\"}"), _val, _id, _name);
    return true;
}
