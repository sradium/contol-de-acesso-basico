#include <Arduino.h>
#include <Keypad.h>
#include <access.h>

#ifndef KPD_H
#define KPD_H

#define ROWS 4
#define COLS 4

class Kpd
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
        int id;
        void check();
        Kpd(int deviceID); 
};
#endif