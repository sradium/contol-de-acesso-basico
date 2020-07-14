#include "actuator.h"

Door_solenoid::Door_solenoid(int pin, int id, char *name, char *type)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->id = id;
    this->name = name;
    this->type = type;
    status = false;
}

void Door_solenoid::action()
{
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
}