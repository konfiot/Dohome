#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{
public:

                    Sensor      (const char *name   );
    virtual bool    refresh     (                   ) = 0;
    virtual String  getJSONData (                   ) = 0;
private:

    String          _name;
};

#endif // SENSOR_H
