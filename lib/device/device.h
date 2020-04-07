#include <Arduino.h>

#ifndef DEVICE_H
#define DEVICE_H

/*
* Numero de dispositivos default si se desea cambiar en numero de dispositivos
* solo tiene de agregar la linea #define NO_DEVICES <cantidad>
*/

#ifndef NO_DEVICES
    #define NO_DEVICES 10 
#endif

typedef struct device{
    int id;
    const char* name;
    bool status;
    const char* location;
} device_t;

namespace devices{
    void add(int id, const char* name, bool status, const char* location);
    bool getStatus(int id);
    const char* getLocation(int id);
    bool getStatus(const char* name);
    const char* getLocation(const char* name);
    bool setStatus(int id, bool status);
}
#endif