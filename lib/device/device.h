/*
*   Liberia que maneja todos los dispositivos instalados en la radiobase y
*   valida en cada ciclo cambios de estados para emitir las notificaciones 
*   al servidor.
*
*   Ademas maneja las acciones emitidas de la nube buscando el dispositivo por 
*   el idDevice. 
*/

#include <Arduino.h>
#include <kpd.h>

#ifndef DEVICE_H
#define DEVICE_H

/*
* Numero de dispositivos default si se desea cambiar en numero de dispositivos
* solo tiene que agregar la linea #define NO_DEVICES <cantidad>
*/
#ifndef NO_DEVICES
#define NO_DEVICES 10
#endif

enum devices
{
    IR,
    PIR,
    SIREN,
    BUTTON,
    KEYPAD
};

typedef struct device
{
    const char *name;
    const char *type;
    char *value;
    bool status;
} device_t;

class IR
{
    private:
        const char *name;
        const char *type;
        char *value;
        bool status;

    public:
        IR(char *name, char *type, char *value, bool status);
};

namespace devices
{
void init(void);
void add(int id, const char *name, const char *type, char *value, bool status);
device_t get(int id);
device_t get(const char *name);
void check(void);
} 

#endif