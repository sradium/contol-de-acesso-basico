#include "access.h"

#define TABLE_SIZE 1024

struct response2 rp2[20];
response2 event_access;

void writer(unsigned long address, byte data)
{
    EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
    return EEPROM.read(address);
}

EDB db_access(&writer, &reader);
EDB db_warnings(&writer, &reader);

void access::init()
{
    while (!request::init())
    {
        delay(300);
    }
    db_access.open(0);
    Serial.print("No of records max: ");
    Serial.println(db_access.limit());
    Serial.print("No of records used: ");
    Serial.println(db_access.count());
}

int access::searchID(int l, int r, unsigned int x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        db_access.readRec(mid, EDB_REC event_access);
        if (event_access.id == x)
            return mid;
        if (event_access.id > x)
            return searchID(l, mid - 1, x);
        return searchID(mid + 1, r, x);
    }
    return -1;
}

int access::searchInsertID(int l, int r, unsigned int x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        db_access.readRec(mid, EDB_REC event_access);
        if (event_access.id == x)
            return mid;
        if (event_access.id > x)
            return searchInsertID(l, mid - 1, x);
        return searchInsertID(mid + 1, r, x);
    }
    return r;
}

void access::update(response1_t *rp1)
{
    if (request::schedules(rp2))
    {
        rp1->schedule = false;
        for (int i = 0; rp2[i].id != 0 && db_access.limit() != db_access.count(); ++i)
        {
            int recno = searchID(1, db_access.count(), rp2[i].id);
            Serial.print("Busqueda de la posicion menor de 3 es(2): ");
            Serial.println(searchInsertID(1, db_access.count(), 3));
            if (recno != -1)
            {
                db_access.updateRec(recno, EDB_REC rp2[i]);
                Serial.println("Actualice el valor");
            }
            else
            {
                //Tengo que modificar la forma en que los guardo en la db
                db_access.appendRec(EDB_REC rp2[i]);
                Serial.println("Lo inserte al final");
            }
        }
        Serial.print("No of record used: ");
        Serial.println(db_access.count());
        Serial.println();
    }
}

