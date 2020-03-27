/*
*
*  Libreria temporal mientras se trabaja desde casa
*
*/

#include <Arduino.h>
#include <Keypad.h>
#include <access.h>
#include <device.h>
#include <TimeLib.h>

#ifndef KPD_H
#define KPD_H


#define TIME_HEADER  "T"  
#define TIME_REQUEST  7     

namespace kpd{
    bool begin();
    void check();
    void validate();
}
#endif