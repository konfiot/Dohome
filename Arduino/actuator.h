#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>

class Actuator
{
public:
                    Actuator    (const char *name   );
    virtual bool    init        (                   ) = 0;
    virtual bool    setValue    (int        value   ) = 0;
    virtual char*   getJSONData (                   ) = 0;

protected:
    String          _name;
};

#endif // SENSOR_H
