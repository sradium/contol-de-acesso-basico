/*
*   Libreria que gestiona todas las conexiones con el servidor
*   actualiza los codigos de aceso permitidos, envia los avisos
*   de alarma y recibe las instrucciones remotas
*/


#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

#ifndef REQUEST_H
#define REQUEST_H

typedef struct next_endpoint
{
    bool schedules;
    bool actions;
} next_endpoint_t;

typedef struct access_record
{
    unsigned int id;
    time_t start;  
    time_t final;
    unsigned long access_code;
} access_record_t;

namespace request
{
    bool init(void);  
    bool ping(next_endpoint_t *ne);
    time_t convert_unix(const char* date); 
    bool schedules(access_record_t ar[]);
    bool get(const char* url, const char* msg);
    bool post(const char* url, const char* msg);
    bool audit(String json);
} 

#endif