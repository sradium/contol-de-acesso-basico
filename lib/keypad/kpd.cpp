#include "kpd.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] =
    {
        {'2', '5', '8', '0'},
        {'3', '6', '9', '#'},
        {'A', 'B', 'C', 'D'},
        {'1', '4', '7', '*'}};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad kpd_device = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int n = 0;
String code;

bool kpd::begin()
{
  return true;
}

void kpd::check()
{
  char key = kpd_device.getKey();
  if (key)
  {
    if (key != '#' && n < 6)
    {
      code += key;
      n++;
    }
    else if (n == 6 && key == '#')
    {
      if(devices::getStatus(0)){
        Serial.println("Tome la foto");
      }
      else
      {
        Serial.println("No hay una camara disponible");
      }
      bool attempt = access::validate((long)code.toInt());
      access::update_users_access("Time", (long)code.toInt(), attempt);
      code = "";
      n = 0;
    }
    else
    {
      if(devices::getStatus(0)){
        Serial.println("Tome la foto");
      }
      else
      {
        Serial.println("No hay una camara disponible");
      }
      access::update_users_access("Time", (long)code.toInt(), false);
      code = "";
      n = 0;
    }
  }
}