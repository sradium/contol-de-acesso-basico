/*
*
* Libreria donde se declaran los distintos tipos de sensores, basados
* en una clase abstracta que contiene las propiedades de la matriz de
* dispositivos.
* Por los momentos se utiliza la libreria de un keypad matricial pero  
* es facimente convertible a uno que implemente el protocolo wiegand.
*
*/

#include <Keypad.h>
#include <access.h>

#ifndef SENSOR_H
#define SENSOR_H

#define MAX_INTERVAL 60000
#define ROWS 4
#define COLS 4

class Sensor
{
protected:
    int id;
    const char *name;
    const char *type;
    char *value;
    bool status;
public:
    virtual void check();
    bool getStatus()
    {
        return status;
    }
};

class IR : public Sensor
{
private:
    int pin;

public:
    IR(int pin, int id, char *name, char *type);
    void check();
};

class Door_sensor : public Sensor
{
private:
    int pin;
    long init;
    
public:
    Door_sensor(int pin, int id, char *name, char *type);
    void check();
};

class Kpd : public Sensor
{
private:
    int n;
    String code;

    char keys[ROWS][COLS] =
        {{'2', '5', '8', '0'},
         {'3', '6', '9', '#'},
         {'A', 'B', 'C', 'D'},
         {'1', '4', '7', '*'}};
    byte rowPins[ROWS] = {2, 3, 4, 5};
    byte colPins[COLS] = {6, 7, 8, 9};
    Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

    void validate();

public:
    Kpd(int id, char *name, char *type);
    void check();
};

#endif