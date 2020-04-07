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
  setTime(8, 33, 0, 25, 3, 2020); //Paso el tiempo en cada corrida
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
    Serial.println("Tomé la foto");
  }
  else
  {
    Serial.println("No hay una camara disponible");
  }
  access_attempt_t access_attempt;
  access_attempt.timestamp = now();
  access_attempt.code = (long)code.toInt();
  access_attempt.value = access::validate((long)code.toInt());
  access::update_users_access(access_attempt);

  String json = "{\"type\":\"access_code\",\"value\":\""+access_attempt.value+"\",";
  json = json+"\"device_id\":1202,\"commandID\":null,\"timestamp\":"+access_attempt.timestamp;
  json = json+"}";
  /*
  *   Aqui me conecto al endpoint correspondiente
  *   request::endpoint(json);
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
    else if(n>0)
    {
      validate();
    }
  }
}