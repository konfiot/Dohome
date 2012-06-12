#include "sensor.h"
#include "bmp085_sensor.h"

BMP085_Sensor::BMP085_Sensor(const char *name) : Sensor(name){
}

bool BMP085_Sensor::init(){
  _dps.init();
}
  
bool BMP085_Sensor::refresh() {
    _dps.getTemperature(&_temperature);
    _dps.getPressure(&_pressure);
    return true;
}

String BMP085_Sensor::getJSONData(){
    return String("[\"p\":\"") + String(_pressure) + String("\",\"t\":") + String(_temperature) + String("\"]");
}
