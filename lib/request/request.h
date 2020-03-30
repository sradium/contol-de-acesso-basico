#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

#ifndef REQUEST_H
#define REQUEST_H

typedef struct response1
{
    bool schedules;
    bool actions;
} response1_t;

struct response2
{
    unsigned int id;
    time_t start;  
    time_t final;
    unsigned long access_code;
};

namespace request
{
    bool init(void);  
    bool ping(response1 *rp1);
    time_t convertUnix(const char* date); 
    bool schedules(struct response2 rp2[]);
    bool get(const char* url, const char* msg);
    bool endpoint(void);
} 

#endif