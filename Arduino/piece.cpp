#include <EtherCard.h>
#include "piece.h"

Piece::Piece(const char *name) : _name(name)
{
}

bool Piece::init(){
      for (int i = 0 ; i < _sensors.size() ; i++){
        _sensors[i]->init();
      }
}

bool Piece::addSensor(Sensor &sensor){
    _sensors.push_back(&sensor);
    return true;
}

bool Piece::fillJSONData(BufferFiller &buf){      
      buf.emit_p(PSTR("{\"n\":\"$S\",\"s\":["), _name);
      
      for (int i = 0 ; i < _sensors.size() ; i++){
        _sensors[i]->getJSONData(buf);
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

Piece::~Piece(){
	for (int i = 0 ; i < _sensors.size() ; i++){
		delete _sensors[i];
		_sensors[i] = 0;
	}
}
