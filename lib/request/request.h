#include <Arduino.h>

#ifndef REQUEST_H
#define REQUEST_H

struct response1
{
    bool schedule;
    bool action;
};

struct response2
{
    int id;
    const char* start;  
    long duration; 
    int type;
    int access_code;
};

namespace request
{
    bool init(void);
    response1 ping(void);
    response2 schedules(void);
} 

#endif