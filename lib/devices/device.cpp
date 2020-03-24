#include "device.h"

device list[];
int n;

void devices::init(int size)
{
    n = size;
    list[n];
}

void devices::add(int id, const char* name, bool status, const char* location)
{
    list[id].id = id;
    list[id].name = name;
    list[id].status = status;
    list[id].location = location;
}

bool devices::getStatus(int id)
{
    return list[id].status;
}

const char *devices::getLocation(int id)
{
    return list[id].location;
}

bool devices::getStatus(const char* name)
{
    for (int i = 0; i < n; ++i)
    {
        if (list[i].name == name)
        {
            return list[i].status;
        }
    }
    return false;
}

const char* devices::getLocation(const char* name)
{
    for (int i = 0; i < n; ++i)
    {
        if (list[i].name == name)
        {
            return list[i].location;
        }
    }
    return "null";
}