#include "device.h"
#include <HashMap.h>

device list[NO_DEVICES];
HashType<const char*, int> hashRawArray[NO_DEVICES]; 
HashMap<const char*, int> hashMap = HashMap<const char*, int>( hashRawArray , NO_DEVICES); 

void devices::add(int id, const char* name, bool status, const char* location)
{
    list[id].name = name;
    list[id].status = status;
    list[id].location = location;
    hashMap[id](name, id);
}

bool devices::getStatus(int id)
{
    return list[id].status;
}

const char* devices::getLocation(int id)
{
    return list[id].location;
}

bool devices::getStatus(const char* name)
{
    int id = hashMap.getValueOf(name);
    return list[id].status;
}

const char* devices::getLocation(const char* name)
{
    int id = hashMap.getValueOf(name);
    return list[id].location;
}