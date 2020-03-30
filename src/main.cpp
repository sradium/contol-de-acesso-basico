#include <Arduino.h>
#include "access.h"
#include "kpd.h"
#include "device.h"

const unsigned long interval = 60000;
unsigned long current;
response1 next_connections;

void setup()
{
  access::init();
  devices::add(1, "keypad", kpd::begin(), "Main door");
  current = millis();
}

void loop()
{
  kpd::check();

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