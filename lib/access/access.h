#include <Arduino.h>
#include <request.h>
#include <EDB.h>
#include <EEPROM.h>
#include <TimeLib.h>

#ifndef ACCESS_H
#define ACCESS_H

struct access_attempt{
    String timestamp;
    long code;
    bool attempt;
};

namespace access
{
    void init(void);
    int searchID(int l, int r, unsigned long x);
    int searchInsertID(int l, int r, unsigned long x);
    void update_code_access(response1 *rp1);
    void update_users_access(time_t time, long code, bool attempt);
    bool validate(long code);
}

#endif