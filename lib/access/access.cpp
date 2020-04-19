/*
*   Libreria que gestiona las bases de datos local de los codigos
*   de aceso permitidos desde la nube. Se puede implementar utilizando
*   la memoria EEPROM interna, una EEPROM externa, una tarjeta SD o
*   cualquier otro medio de escritutra por byte.
*/

#include "access.h"

#define TABLE_SIZE 1024

void writer(unsigned long address, byte data)
{
    EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
    return EEPROM.read(address);
}

EDB db_scheduled_accesses(&writer, &reader);
EDB db_access_attempts(&writer, &reader);

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
    /*
    * Si es la primera vez que se cargara el programa en el arduino se tiene
    * que comentar los open y descomentar los create. Despues de correrlo se
    * vuelve a cambiar. Esto es necesario para cargar en la memoria la db 
    * para poder acceder a ella despues de un corte de energia y recuperar los 
    * datos
    */
   
    //db_scheduled_accesses.open(0);
    access_record_t access_record;
    db_scheduled_accesses.create(0, 2 * TABLE_SIZE, (unsigned int)sizeof(access_record));
    Serial.print("Record used in table 1: ");
    Serial.print(db_scheduled_accesses.count());
    Serial.print(" ,and max of records: ");
    Serial.println(db_scheduled_accesses.limit());
    db_access_attempts.open(2049);
    //db_access_attempts.create(2049, TABLE_SIZE, (unsigned int)sizeof(access_attempt));
    Serial.print("Record used in table 2: ");
    Serial.println(db_access_attempts.count());
    Serial.print(" ,and max of records: ");
    Serial.println(db_access_attempts.limit());
}

int access::searchID(int l, int r, unsigned long x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        access_record_t access_record;
        db_scheduled_accesses.readRec(mid, EDB_REC access_record);
        if (access_record.access_code == x)
            return mid;
        if (access_record.access_code > x)
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
        access_record_t access_record;
        db_scheduled_accesses.readRec(mid, EDB_REC access_record);
        if (access_record.access_code == x)
            return mid;
        if (access_record.access_code > x)
            return searchInsertID(l, mid - 1, x);
        return searchInsertID(mid + 1, r, x);
    }
    return (r + 1);
}

void access::update_code_accesses(next_endpoint_t *ne)
{
    access_record_t accesses_record[20];
    if (request::schedules(accesses_record))
    {
        ne->schedules = false;
        for (int i = 0; accesses_record[i].id != 0 && db_scheduled_accesses.limit() != db_scheduled_accesses.count(); ++i)
        {
            int recno = searchID(1, db_scheduled_accesses.count(), accesses_record[i].access_code);
            if (recno != -1)
            {
                //db_scheduled_accesses.updateRec(recno, EDB_REC accesses_record[i]); Esta comentando para no desgastar la eeprom
            }
            else
            {
                int j = searchInsertID(1, db_scheduled_accesses.count(), accesses_record[i].access_code);
                if (j < (int)db_scheduled_accesses.count())
                {
                    db_scheduled_accesses.insertRec(j, EDB_REC accesses_record[i]);
                }
                else
                {
                    db_scheduled_accesses.appendRec(EDB_REC accesses_record[i]);
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

void access::update_users_access(access_attempt_t access_attempt)
{
    //db_access_attempts.appendRec(EDB_REC access_attempt); Esta comentando para no desgastar la eeprom
    Serial.print("Guarde el intento ");
    Serial.print(access_attempt.code);
    Serial.print(" en la memoria a la hora ");
    Serial.println(access_attempt.timestamp);
}

String access::validate(long code)
{
    int recno = searchID(1, db_scheduled_accesses.count(), code);
    if (recno != -1)
    {
        time_t current = now();
        access_record_t access_record;
        db_scheduled_accesses.readRec(recno, EDB_REC access_record);
        if (current >= access_record.start && current <= access_record.final)
        {
            Serial.println("access granted");
            return "valid";
        }
        else
        {
            Serial.println("access denied");
            return "invalid";
        }
    }
    else
    {
        Serial.println("access denied not found");
        return "invalid";
    }
}