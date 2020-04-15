#include <Arduino.h>
#include "access.h"
#include "device.h"

const unsigned long interval = 60000;
unsigned long current;
next_endpoint_t next_connections;

void setup()
{
  access::init();
  setTime(8, 33, 0, 14, 4, 2020);
  current = millis();
}

void loop()
{
  devices::check();

  if((millis() - current) > interval)
  {
    if(request::ping(&next_connections)){
      current = millis();
    }
  }

  if(next_connections.schedules){
    access::update_code_accesses(&next_connections);
  }
}