#include <Arduino.h>
#include <kpd.h>

#ifndef DEVICE_H
#define DEVICE_H

/*
* Numero de dispositivos default si se desea cambiar en numero de dispositivos
* solo tiene que agregar la linea #define NO_DEVICES <cantidad>
*/

#ifndef NO_DEVICES
    #define NO_DEVICES 10 
#endif

typedef struct device{
    const char* name;
    const char* type;
    char* value;
    bool status;
} device_t;

namespace devices{
    void init(void);
    void add(int id, const char* name, const char* type, char* value, bool status);
    device_t get(int id);
    device_t get(const char* name);
    void check(void); 
}

#endif