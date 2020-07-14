/*
*   Liberia que maneja todos los dispositivos instalados en la radiobase y
*   valida en cada ciclo cambios de estados para emitir las notificaciones 
*   al servidor.
*
*   Ademas maneja las acciones emitidas de la nube buscando el dispositivo por 
*   el idDevice. 
*/

#include <Arduino.h>
#include <sensor.h>
#include <actuator.h>
#ifndef DEVICE_H
#define DEVICE_H

/*
* Numero de dispositivos default si se desea cambiar en numero de dispositivos
* solo tiene que agregar la linea #define NO_DEVICES <cantidad>
*/
#ifndef NO_SENSORS
#define NO_SENSORS 3
#endif

#ifndef NO_ACTUATORS
#define NO_ACTUATORS 1
#endif

namespace devices
{
void init(void);
void check(void);
void blue_alarm(void);
void yellow_alarm(void);
void red_alarm(void);
} // namespace devices

#endif