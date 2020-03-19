#include <Arduino.h>
#include <request.h>

#ifndef ACCESS_H
#define ACCESS_H

namespace access
{
    void init(void);
    void update(response1 *rp1);
    void validate(void);
}

#endif