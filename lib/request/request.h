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
    unsigned long access_code;
};

namespace request
{
    bool init(void);  
    bool ping(response1 *rp1); 
    bool schedules(struct response2 rp2[]);
    bool get(const char* url);
    bool endpoint(void);
} 

#endif