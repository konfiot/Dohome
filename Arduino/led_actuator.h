#ifndef LED_ACTUATOR_H
#define LED_ACTUATOR_H

#include <Arduino.h>
#include "defines.h"
#include "actuator.h"

class Led_Actuator : public Actuator
{
public:
                    Led_Actuator (const char   *name, byte pin);
    virtual bool    init         (byte         id   );
    virtual bool    refresh      (                  );
    virtual bool    prepare      (const char   *arg );
    virtual bool    exec         (                  );
    virtual bool    getJSONData  (BufferFiller &buf );
    
private:
    byte            _pin;
    int             _val;
};

#endif // LED_ACTUATOR_H
