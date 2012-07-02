#ifndef PC_ACTUATOR_H
#define PC_ACTUATOR_H

#include <Arduino.h>
#include <EtherCard.h>
#include "Actuator.h"
#include "defines.h"

class PC_Actuator : public Actuator
{
public:
                    PC_Actuator  (const char *name  , const byte *mac, const byte *ip);
    virtual bool    init         (byte         id   );
    virtual bool    refresh      (                  );
    virtual bool    prepare      (const char   *arg );
    virtual bool    exec         (                  );
    virtual bool    getJSONData  (BufferFiller &buf );
    
private:
    byte            _val;
    byte            _id;
    const byte      *_mac;
    const byte      *_ip;
};

#endif // PC_ACTUATOR_H
