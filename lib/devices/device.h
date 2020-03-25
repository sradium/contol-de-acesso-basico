#include <Arduino.h>
#include <StandardCplusplus.h>
#include <map>

#ifndef DEVICE_H
#define DEVICE_H

#ifndef NO_DEVICES
    #define NO_DEVICES 10 //Numero de dispositivos 
#endif

device list[NO_DEVICES];
std::map <const char*, int > dictionary;

struct device{
    int id;
    const char* name;
    bool status;
    const char* location;
};

namespace devices{
    void add(int id, const char* name, bool status, const char* location);
    bool getStatus(int id);
    bool getStatus(const char* name);
    const char* getLocation(int id);
    const char* getLocation(const char* name);
}
#endif