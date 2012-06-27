#ifndef PIECE_H
#define PIECE_H

#include <Arduino.h>
#include <EtherCard.h>
#include "sensor.h"
#include "actuator.h"
#include "vector.h"

class Piece
{
public:

                      Piece           (const char     *name    );
    bool              init            (byte           id       );
    bool              addSensor       (Sensor         &sensor  );
    bool              addActuator     (Actuator       &Actuator);
    bool              fillJSONData    (BufferFiller   &buf     );
    bool              refresh         (                        );
    virtual bool      prepare         (const char     *arg     , byte id);
    virtual bool      exec            (                        );
                      ~Piece          (                        );

private:
    Vector<Sensor*>   _sensors;
    Vector<Actuator*> _actuators;
    const char*       _name;
    byte              _id;
    byte              _prepared_id;
};

#endif // PIECE_H
