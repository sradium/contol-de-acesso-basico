#include <Arduino.h>
#include "access.h"

const unsigned long interval = 60000;
unsigned long current;
response1 rp1;

void setup()
{
  access::init();
  current = millis();
}

void loop()
{
  if((millis() - current) > interval)
  {
    if(request::ping(&rp1)){
      current = millis();
    }
  }

  if(rp1.schedule == true){
    access::update(&rp1);
  }
}