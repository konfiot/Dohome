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

bool BMP085_Sensor::getJSONData(BufferFiller &buf){
    buf.emit_p(PSTR("{\"t\":\"p\",\"v\":$L},{\"v\":$D,\"t\":\"t\"}"), _pressure, int(_temperature));
    return true;
}
