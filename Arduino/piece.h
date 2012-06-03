#ifndef PIECE_H
#define PIECE_H

#include <Arduino.h>
#include <EtherCard.h>
#include <iostream>
#include "sensor.h"

class Piece
{
public:

                    Piece           (String         name    );
    bool            addSensor       (Sensor         sensor  );
    bool            fillJSONData    (BufferFiller   &buf    );

private:

    vector<Sensor*> _sensors;
    String          _name;
};

#endif // PIECE_H
