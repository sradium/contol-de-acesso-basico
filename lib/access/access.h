#include <Arduino.h>
#include <request.h>
#include <EDB.h>
#include <EEPROM.h>

#ifndef ACCESS_H
#define ACCESS_H

typedef struct access_attempt{
    time_t timestamp;
    long code;
    String value;
} access_attempt_t;

namespace access
{
    void init(void);
    int searchID(int l, int r, unsigned long x);
    int searchInsertID(int l, int r, unsigned long x);
    void update_code_accesses(next_endpoint_t *rp1);
    void update_users_access(access_attempt_t access_attempt);
    String validate(long code);
}

#endif