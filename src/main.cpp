#include <Arduino.h>
#include "access.h"
#include "kpd.h"
#include "device.h"

const unsigned long interval = 60000;
unsigned long current;
response1 rp1;

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
    rp1.schedule = true;
    if(request::ping(&rp1)){
      current = millis();
    }
  }

  if(rp1.schedule == true){
    access::update_code_access(&rp1);
  }
}