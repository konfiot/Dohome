#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include <EtherCard.h>

class Actuator
{
public:
                    Actuator    (const char   *name);
    virtual bool    init        (byte         id   );
    virtual bool    refresh     (                  ) = 0;
    virtual bool    prepare     (const char   *arg ) = 0;
    virtual bool    exec        (                  ) = 0;
    byte            getId       (                  );
    virtual bool    getJSONData (BufferFiller &buf ) = 0;

protected:
    const char      *_name;
    byte            _id;
};

#endif // SENSOR_H
