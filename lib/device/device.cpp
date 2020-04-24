#include "device.h"

/*
*   Dispositivos instalados en la radiobase
*/
Sensor *device[NO_DEVICES];

void devices::init()
{
    while (!request::init())
    {
        delay(300);
    }
    //access::init();
    //device[0] = new Kpd(1, (char*)"keypad", (char*)"access_code");
    device[0] = new IR(11, 1, (char *)"ir", (char *)"ir_barrier");
    device[1] = new door_sensor(12, 2, (char *)"reed_switch_nc", (char *)"door_sensor");
    setTime(8, 33, 0, 14, 4, 2020);
}

void devices::check()
{
    for (int i = 0; i < 2; i++)
    {
        device[i]->check();
    }
}
