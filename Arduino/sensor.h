#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <EtherCard.h>

class Sensor
{
public:
                    Sensor      (const char   *name);
    virtual bool    init        (                  ) = 0;
    virtual bool    refresh     (                  ) = 0;
    virtual bool    getJSONData (BufferFiller &buf ) = 0;

protected:
    const char*     _name;
};

#endif // SENSOR_H
