#include <Arduino.h>

#ifndef REQUEST_H
#define REQUEST_H

typedef struct response1
{
    bool schedule;
    bool action;
} response1_t;

struct response2
{
    unsigned int id;
    const char* start;  
    unsigned int duration;
    unsigned int access_code;
};

namespace request
{
    bool init(void);
    void ping(response1 *rp1);
    bool schedules(struct response2 rp2[]);
} 

#endif