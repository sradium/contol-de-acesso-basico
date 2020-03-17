#include <Arduino.h>
#include <request.h>

#ifndef ACCESS_H
#define ACCESS_H

namespace access
{
    void init(void);
    bool update(void);
    void validate(void);
}

#endif