#include "pc_actuator.h"

PC_Actuator::PC_Actuator(const char *name, const byte *mac, const byte *ip) : Actuator::Actuator(name), _mac(mac), _ip(ip){
}

bool PC_Actuator::init(byte id){  
  Actuator::init(id);
}

bool PC_Actuator::refresh(){
  unsigned long timer;
  ether.clientIcmpRequest((uint8_t*)_ip);
  timer = millis();
  
  while ((millis() - timer) <= 500){
    word len = ether.packetReceive(); // go receive new packets
    word pos = ether.packetLoop(len); // respond to incoming pings
    
    // report whenever a reply to our outgoing ping comes back
    if (len > 0 && ether.packetLoopIcmpCheckReply((uint8_t*)_ip)) {
      if(_val == 2){
        _val = 1;
      }
      return true;
    }
  }
  _val = 2;
  return true;
  
}

bool PC_Actuator::prepare(const char *arg){
    int val;
    val = atoi(arg);
    if ((val == 0) || (val == 1)){
        _val = val;
        return true;
    } else {
        return false;
    }
}

bool PC_Actuator::exec(){
    if(_val == 1){
        ether.sendWol((uint8_t*)_mac);
    } /*else if(_val == 0){
        digitalWrite(_pin, LOW);
    }*/
}

bool PC_Actuator::getJSONData(BufferFiller &buf){
    buf.emit_p(PSTR("{\"v\":$D,\"i\":$D,\"n\":\"$S\"}"), _val, _id, _name);
    return true;
}
