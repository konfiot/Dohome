#ifndef BMP085_SENSOR_H
#define BMP085_SENSOR_H

#include <EtherCard.h>

#include "BMP085.h"
#include "sensor.h"

class BMP085_Sensor : public Sensor
{
public:
                    BMP085_Sensor   (const char   *name);
    virtual bool    init            (                  );
    virtual bool    refresh         (                  );
    virtual bool    getJSONData     (BufferFiller &buf );

private:
    BMP085          _dps;
    long            _temperature;
    long            _pressure;

};

#endif // BMP085_SENSOR_H
