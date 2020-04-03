#include <Arduino.h>
#include <request.h>
#include <EDB.h>
#include <EEPROM.h>
#include <TimeLib.h>

#ifndef ACCESS_H
#define ACCESS_H

typedef struct access_attempt{
    time_t timestamp;
    long code;
    bool attempt;
} access_attempt_t;

namespace access
{
    void init(void);
    int searchID(int l, int r, unsigned long x);
    int searchInsertID(int l, int r, unsigned long x);
    void update_code_accesses(next_endpoint_t *rp1);
    void update_users_access(time_t time, long code, bool attempt);
    bool validate(long code);
}

#endif