#include "sensor.h"

IR::IR(int pin, int id, char *name, char *type)
{
  this->pin = pin;
  pinMode(pin, INPUT);
  this->id = id;
  this->name = name;
  this->type = type;
  status = digitalRead(pin);
}

void IR::check()
{
  if (digitalRead(pin))
  {
    Serial.println("Detecte una obtruccion");
  }
}

door_sensor::door_sensor(int pin, int id, char *name, char *type)
{
  this->pin = pin;
  pinMode(pin, INPUT);
  this->id = id;
  this->name = name;
  this->type = type;
  status = digitalRead(pin);
}

void door_sensor::check()
{
  if(!digitalRead(pin))
  {
    Serial.println("Se abrio la puerta");
  }
}

Kpd::Kpd(int id, char *name, char *type)
{
  this->id = id;
  this->name = name;
  this->type = type;
  code = "";
  n = 0;
}

/* 
* Funcion que evalua constantemente si el usuario ha presionado alguna tecla y la almacena
* en un buffer. Cuando el usuario presiona la tecla # evalua si tiene una longitud de 6 
* digitos, si es menor o mayor la clave es invalida. Cuando el codigo tiene una longitud 
* correcta la busca en la base de datos. En todos los casos se toma una foto si hay una camara
* activa y se guarda la hora con el codigo usado en la base de datos.
*/

void Kpd::validate()
{
  access_attempt_t access_attempt;
  access_attempt.timestamp = now();
  access_attempt.code = (long)code.toInt();
  access_attempt.value = access::validate((long)code.toInt());

  String json = "{\"type\":\"access_code\",\"value\":\"" + access_attempt.value + "\",";
  json = json + "\"code\":" + access_attempt.code + ",\"device_id\":" + id + ",\"deviceType\":\"keypad\"";
  json = json + ",\"timestamp\":" + access_attempt.timestamp + ",\"commandID\":null";
  json = json + "}";

  /*
  *   Aqui me conecto al endpoint correspondiente
  *   request::audit(json);
  *   Si no existe conexion con el enpoint guardo el intento en la base se datos
  *   
  */
  access::update_users_access(access_attempt);
  code = "";
  n = 0;
}

void Kpd::check()
{
  char key = keypad.getKey();
  if (key)
  {
    if (key != '#' && n < 6)
    {
      code += key;
      n++;
    }
    else if (n > 0)
    {
      validate();
    }
  }
}