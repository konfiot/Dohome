#include "sensor.h"

Sensor::Sensor(const char *name){
    _name = name;
}

double Sensor::get_data(){
    return _data;
}
