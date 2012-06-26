#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include <EtherCard.h>

class Actuator
{
public:
                    Actuator    (const char   *name);
    virtual bool    init        (                  ) = 0;
    virtual bool    setValue    (int          value) = 0;
    virtual bool    getJSONData (BufferFiller &buf ) = 0;

protected:
    const char      *_name;
};

#endif // SENSOR_H
