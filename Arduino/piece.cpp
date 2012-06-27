#include <EtherCard.h>
#include "piece.h"

Piece::Piece(const char *name) : _name(name)
{
}

bool Piece::init(byte id){
    _id = id;  
  
    for (byte i = 0 ; i < _sensors.size() ; i++){
        _sensors[i]->init();
    }
    for (byte i = 0 ; i < _actuators.size() ; i++){
        _actuators[i]->init(i);
    }
}

bool Piece::addSensor(Sensor &sensor){
    _sensors.push_back(&sensor);
    return true;
}

bool Piece::addActuator(Actuator &actuator){
    _actuators.push_back(&actuator);
    return true;
}

bool Piece::fillJSONData(BufferFiller &buf){      
      buf.emit_p(PSTR("{\"n\":\"$S\",\"i\":$D,\"s\":["), _name, _id);
      
      for (int i = 0 ; i < _sensors.size() ; i++){
        _sensors[i]->getJSONData(buf);
      }
      
      buf.emit_p(PSTR("],\"a\":["));
      for (int i = 0 ; i < _sensors.size() ; i++){
        _actuators[i]->getJSONData(buf);
      }
      buf.emit_p(PSTR("]}"));
    return true;
}

bool Piece::refresh(){
    for (int i = 0 ; i < _sensors.size() ; i++){
      _sensors[i]->refresh();
    }
    return true;  
}

bool Piece::prepare(const char *arg, byte id){
    _prepared_id = id;
    return _actuators[id]->prepare(arg);  
}

bool Piece::exec(){
    return _actuators[_prepared_id]->exec();  
}

Piece::~Piece(){
	for (int i = 0 ; i < _sensors.size() ; i++){
		delete _sensors[i];
		_sensors[i] = 0;
	}
}
