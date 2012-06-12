#ifndef PIECE_H
#define PIECE_H

#include <Arduino.h>
#include <EtherCard.h>
#include "sensor.h"
#include "vector.h"

class Piece
{
public:

                    Piece           (String         name    );
    bool            addSensor       (Sensor         &sensor );
    bool            fillJSONData    (BufferFiller   &buf    );
    bool            refresh         (                       );
                    ~Piece          (                       );

private:
    Vector<Sensor*> _sensors;
    String          _name;
};

#endif // PIECE_H
