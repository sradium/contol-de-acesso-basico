#include <Arduino.h>
#include <request.h>
#include <EDB.h>
#include <EEPROM.h>

#ifndef ACCESS_H
#define ACCESS_H

namespace access
{
    void init(void);
    int searchID(int l, int r, unsigned int x);
    int searchInsertID(int l, int r, unsigned int x);
    void update(response1 *rp1);
    void validate(void);
}

#endif