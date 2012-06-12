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

char* BMP085_Sensor::getJSONData(){
    char buffer[100];
    String(String("[\"p\":\"") + String(_pressure) + String("\",\"t\":") + String(_temperature) + String("\",\"n\":") + String(_name) + String("\"]")).toCharArray(buffer, 100);
    return buffer;
}
