#include <EtherCard.h>
#include "piece.h"

Piece::Piece(String name)
{
    _name = name;
}

bool Piece::addSensor(Sensor sensor){
    _sensors.push(&sensor);
    return true;
}

bool Piece::fillJSONData(BufferFiller &buf){
    buf.emit_p(PSTR("LAULLE"));
    return true;
}
