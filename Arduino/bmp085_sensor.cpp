#include "bmp085_sensor.h"

BMP085_Sensor::BMP085_Sensor()
{
    Sensor::Sensor();
    _dps.init();
}

bool BMP085_Sensor::refresh(){
    _dps.getTemperature(&_temperature);
    _dps.getPressure(&_pressure);
    return true;
}

String BMP085_Sensor::getJSONData(){
    return String("[\"p\":\"") + _pressure + String("\",\"t\":") + (float)(_temperature/10) + String("\"]");
}
