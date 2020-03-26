/*
*
*  Libreria temporal mientras se trabaja desde casa
*
*/

#include <Arduino.h>
#include <Keypad.h>
#include <access.h>

#ifndef KPD_H
#define KPD_H

namespace kpd{
    bool begin();
    void check();
}
#endif