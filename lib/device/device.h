#include <Arduino.h>

#ifndef DEVICE_H
#define DEVICE_H

/*
*
* Numero de dispositivos default si se desea cambiar en numero de dispositivos
* solo tiene de agregar la linea #define NO_DEVICES <cantidad>.
*
*/

#ifndef NO_DEVICES
    #define NO_DEVICES 10 
#endif

struct device{
    int id;
    String name;
    bool status;
    String location;
};

namespace devices{
    void add(int id, String name, bool status, String location);
    bool getStatus(int id);
    String getLocation(int id);
}
#endif