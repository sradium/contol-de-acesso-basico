#include <Arduino.h>

#ifndef DEVICE_H
#define DEVICE_H

struct device{
    int id;
    const char* name;
    bool status;
    const char* location;
};

namespace devices{
    void init(int No_devices);
    void add(int id, const char* name, bool status, const char* location);
    bool getStatus(int id);
    bool getStatus(const char* name);
    const char* getLocation(int id);
    const char* getLocation(const char* name);
}
#endif