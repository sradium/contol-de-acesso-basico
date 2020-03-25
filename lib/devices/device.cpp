#include "device.h"

void devices::add(int id, const char* name, bool status, const char* location)
{
    list[id].id = id;
    list[id].name = name;
    list[id].status = status;
    list[id].location = location;
    dictionary.insert(std::pair<const char*, int>(name, id));
}

bool devices::getStatus(int id)
{
    return list[id].status;
}

bool devices::getStatus(const char* name)
{
    return list[id].status;
} 

const char *devices::getLocation(int id)
{
    return list[id].location;
}