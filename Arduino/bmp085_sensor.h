#ifndef BMP085_SENSOR_H
#define BMP085_SENSOR_H

#include "BMP085.h"

class BMP085_Sensor : public Sensor
{
public:
                    BMP085_Sensor   (                   );
    virtual bool    refresh         (                   );
    virtual String  getJSONData     (                   );

private:
    BMP085          _dps;
    long            _temperature;
    long            _pressure;

};

#endif // BMP085_SENSOR_H
