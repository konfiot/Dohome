#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{
public:
                    Sensor      (const char *name   );
    virtual bool    init        (                   ) = 0;
    virtual bool    refresh     (                   ) = 0;
    virtual char*   getJSONData (                   ) = 0;

protected:
    String          _name;
};

#endif // SENSOR_H
