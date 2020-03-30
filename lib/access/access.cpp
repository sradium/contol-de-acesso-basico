#include "access.h"
#include <device.h>

#define TABLE_SIZE 1024

struct response2 rp2[20];
response2 event_access;
access_attempt try_access;

void writer(unsigned long address, byte data)
{
    EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
    return EEPROM.read(address);
}

EDB scheduled_accesses(&writer, &reader);
EDB access_attempts(&writer, &reader);

/*
*
*   Las funciones que estan a continuacion se encargan de administrar la base de datos
*   de los accesos programados a la radiobase, por los momentos se almacenan en memoria 
*   los registros nuevos ordenados los codigos de accceso de menor a mayor, para poder 
*   implementar una busqueda binaria al buscar un codigo en la db
*
*/

void access::init()
{
    while (!request::init())
    {
        delay(300);
    }
    devices::add(0, "Camera1", false, "Main door");

    /*
    * Si es la primera vez que se cargara el programa en el arduino se tiene
    * que comentar los open y descomentar los create. Despues de correrlo se
    * vuelve a cambiar. Esto es necesario para cargar en la memoria la db 
    * para poder acceder a ella despues de un corte de energia y recuperar los 
    * datos
    */

    scheduled_accesses.open(0);
    //scheduled_accesses.create(0, 2 * TABLE_SIZE, (unsigned int)sizeof(event_access));
    Serial.print("Record used in table 1: ");
    Serial.println(scheduled_accesses.count());
    access_attempts.open(2049);
    //access_attempts.create(2049, TABLE_SIZE, (unsigned int)sizeof(try_access));
    Serial.print("Record used in table 2: ");
    Serial.println(access_attempts.count());
}

int access::searchID(int l, int r, unsigned long x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        scheduled_accesses.readRec(mid, EDB_REC event_access);
        if (event_access.access_code == x)
            return mid;
        if (event_access.access_code > x)
            return searchID(l, mid - 1, x);
        return searchID(mid + 1, r, x);
    }
    return -1;
}

int access::searchInsertID(int l, int r, unsigned long x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        scheduled_accesses.readRec(mid, EDB_REC event_access);
        if (event_access.access_code == x)
            return mid;
        if (event_access.access_code > x)
            return searchInsertID(l, mid - 1, x);
        return searchInsertID(mid + 1, r, x);
    }
    return (r + 1);
}

void access::update_code_accesses(response1_t *rp1)
{
    if (request::schedules(rp2))
    {
        rp1->schedules = false;
        for (int i = 0; rp2[i].id != 0 && scheduled_accesses.limit() != scheduled_accesses.count(); ++i)
        {
            int recno = searchID(1, scheduled_accesses.count(), rp2[i].access_code);
            if (recno != -1)
            {
                //scheduled_accesses.updateRec(recno, EDB_REC rp2[i]); Esta comentando para no desgastar la eeprom
            }
            else
            {
                int j = searchInsertID(1, scheduled_accesses.count(), rp2[i].access_code);
                if (j < (int)scheduled_accesses.count())
                {
                    scheduled_accesses.insertRec(j, EDB_REC rp2[i]);
                }
                else
                {
                    scheduled_accesses.appendRec(EDB_REC rp2[i]);
                }
            }
        }
    }
}

/*
*
* Las funciones a continuacion son las que se encargan de administrar la
* base de datos de los intentos de acceso a la radiobase. La primera almacena
* Los datos en la db, la segunda elimina todos los registros y la tercera es la
* que se encarga de buscar en la db de los accesos programados para permitir el
* acceso
*
*/

void access::update_users_access(time_t time, long code, bool attempt)
{
    try_access.timestamp = time;
    try_access.code = code;
    try_access.attempt = attempt;
    //access_attempts.appendRec(EDB_REC try_access); Esta comentando para no desgastar la eeprom
    Serial.print("Guarde el intento ");
    Serial.print(code);
    Serial.print(" en la memoria a la hora ");
    Serial.println(time);
}

bool access::validate(long code)
{
    int recno = searchID(1, scheduled_accesses.count(), code);
    if (recno != -1)
    {
        time_t current = now();
        scheduled_accesses.readRec(recno, EDB_REC event_access);
        if (current >= event_access.start && current <= event_access.final)
        {
            Serial.println("access granted");
        }
        else
        {
            Serial.println("access denied");
        }

        return true;
    }
    else
    {
        Serial.println("access denied not found");
        return false;
    }
}