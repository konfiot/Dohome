#include "home.h"

Home::Home(const char *name) : _name(name){
}

bool Home::init(){  
    for (byte i = 0 ; i < _pieces.size() ; i++){
        _pieces[i]->init(i);
    }
}

bool Home::addPiece(Piece &piece){
    _pieces.push_back(&piece);
    return true;
}

bool Home::refresh(){
    for (byte i = 0 ; i < _pieces.size() ; i++){
      _pieces[i]->refresh();
    }
    return true;  
}

bool Home::prepare(const char *arg, byte id_p, byte id_a){
    _prepared_id = id_p;
    return _pieces[id_p]->prepare(arg, id_a);  
}

bool Home::exec(){
    return _pieces[_prepared_id]->exec();  
}

bool Home::fillJSONData(BufferFiller &buf){      
      buf.emit_p(PSTR("\"h\":{\"n\":\"$S\",\"p\":["), _name);
      
      for (byte i = 0 ; i < _pieces.size() ; i++){
        if (i > 0) buf.emit_p(PSTR(","));
        _pieces[i]->fillJSONData(buf);
      }
      
      buf.emit_p(PSTR("]}"));
    return true;
}

Home::~Home(){
	for (int i = 0 ; i < _pieces.size() ; i++){
		delete _pieces[i];
		_pieces[i] = NULL;
    	}
}
