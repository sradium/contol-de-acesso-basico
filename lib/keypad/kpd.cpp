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
  setTime(8, 33, 0, 27, 3, 2020); //Paso el tiempo en cada corrida
  return true;
}

/* 
* Funcion que evalua constantemente si el usuario ha presionado alguna tecla y la almacena
* en un buffer. Cuando el usuario presiona la tecla # evalua si tiene una longitud de 6 
* digitos, si es menor o mayor la clave es invalida. Cuando el codigo tiene una longitud 
* correcta la busca en la base de datos. En todos los casos se toma una foto si hay una camara
* activa y se guarda la hora con el codigo usado en la base de datos.
*/

void kpd::validate()
{
  if (devices::getStatus("camera1"))
  {
    Serial.println("Tome la foto");
  }
  else
  {
    Serial.println("No hay una camara disponible");
  }
  bool found = access::validate((long)code.toInt());
  access::update_users_access(now(), (long)code.toInt(), found);
  /*
  *   Aqui me conecto al endpoint correspondiente
  */
  code = "";
  n = 0;
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
    else
    {
      validate();
    }
  }
}