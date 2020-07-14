#include "device.h"

/*
*   Dispositivos instalados en la radiobase
*/
Sensor *sensors[NO_SENSORS];
Actuator *actuators[NO_ACTUATORS];

void devices::init()
{
    while (!request::init())
    {
        delay(300);
    }
    access::init();
    
    actuators[0] = new Door_solenoid(10, 3, (char *)"Door", (char *)"Door");

    sensors[0] = new Kpd(0, (char *)"keypad", (char *)"access_code");
    sensors[1] = new IR(11, 1, (char *)"ir", (char *)"ir_barrier");
    sensors[2] = new Door_sensor(12, 2, (char *)"reed_switch_nc", (char *)"door_sensor");

    setTime(9, 0, 0, 24, 4, 2020);
}

void devices::check()
{
    for (int i = 0; i < NO_SENSORS; i++)
    {
        sensors[i]->check();
    }
    int count_sensors = 0;
    for (int i = 0; i < NO_SENSORS; i++)
    {
        if (sensors[i]->getStatus())
        {
            count_sensors++;
            Serial.println("Se ha activado un sensor");
        }
    }
}