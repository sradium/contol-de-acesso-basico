#include <Arduino.h>

#ifndef ACTUATOR_H
#define ACTUATOR_H

class Actuator
{
protected:
    int id;
    const char *name;
    const char *type;
    char *value;
    bool status;

public:
    virtual void action();
};

class Door_solenoid : public Actuator
{
private:
    int pin;

public:
    Door_solenoid(int pin, int id, char *name, char *type);
    void action();
};

#endif