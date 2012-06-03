#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{
public:

                    Sensor      (const char *name   );
    virtual bool    refresh     (                   ) = 0;
    virtual double  get_data    (                   );
private:

    String          _name;
    double          _data;
};

#endif // SENSOR_H
