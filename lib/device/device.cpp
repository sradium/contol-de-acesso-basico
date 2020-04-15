#include "device.h"
#include <HashMap.h>

device_t list[NO_DEVICES];
HashType<const char*, int> hashRawArray[NO_DEVICES]; 
HashMap<const char*, int> hashMap = HashMap<const char*, int>( hashRawArray , NO_DEVICES); 


/*
*   Dispositivos instalados en la radiobase
*/
Kpd keypad = Kpd(1);

void devices::add(int id, const char* name, const char* type, char* value, bool status)
{
    list[id].name = name;
    list[id].type = type;
    list[id].value = value;
    list[id].status = status;
    hashMap[id](type, id);
}

device_t devices::get(int id)
{
    return list[id];
}

device_t devices::get(const char* type)
{
    int id = hashMap.getValueOf(type);
    return list[id];
}

void devices::check()
{
    keypad.check();
}
