#include "device.h"

device list[NO_DEVICES];

void devices::add(int id, String name, bool status, String location)
{
    list[id].name = name;
    list[id].status = status;
    list[id].location = location;
}

bool devices::getStatus(int id)
{
    return list[id].status;
}

String devices::getLocation(int id)
{
    return list[id].location;
}
