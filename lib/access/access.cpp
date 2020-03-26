#include "access.h"
#include <device.h>

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
EDB db_users_access(&writer, &reader);

void access::init()
{
    while (!request::init())
    {
        delay(300);
    }
    devices::add(0, "Camera1", false, "Main door");
    db_access.open(0);
    //db_access.create(0, 2*TABLE_SIZE, (unsigned int)sizeof(event_access));
    db_users_access.open(2049);
    //db_users_access.create(2049, TABLE_SIZE, (unsigned int)sizeof(try_access));
    Serial.print("No of records max tabla 1: ");
    Serial.println(db_access.limit());
    Serial.print("No of records used tabla 1: ");
    Serial.println(db_access.count());
    Serial.print("No of records max tabla 2: ");
    Serial.println(db_users_access.limit());
    Serial.print("No of records used tabla 2: ");
    Serial.println(db_users_access.count());
}

int access::searchID(int l, int r, unsigned long x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        db_access.readRec(mid, EDB_REC event_access);
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
        db_access.readRec(mid, EDB_REC event_access);
        if (event_access.access_code == x)
            return mid;
        if (event_access.access_code > x)
            return searchInsertID(l, mid - 1, x);
        return searchInsertID(mid + 1, r, x);
    }
    return (r + 1);
}

void access::update_code_access(response1_t *rp1)
{
    if (request::schedules(rp2))
    {
        rp1->schedule = false;
        for (int i = 0; rp2[i].id != 0 && db_access.limit() != db_access.count(); ++i)
        {
            /*
            * Se guardan en memoria todos los registros recibidos de schedules
            */
            int recno = searchID(1, db_access.count(), rp2[i].access_code);
            if (recno != -1)
            {
                //db_access.updateRec(recno, EDB_REC rp2[i]);
                Serial.println("Actualice el valor");
            }
            else
            {
                int j = searchInsertID(1, db_access.count(), rp2[i].access_code);
                if (j < (int)db_access.count())
                {
                    db_access.insertRec(j, EDB_REC rp2[i]);
                    Serial.println("Inserte el registro");
                }
                else
                {
                    db_access.appendRec(EDB_REC rp2[i]);
                    Serial.println("Inserte al final");
                }
            }
        }
        Serial.print("No of record used: ");
        Serial.println(db_access.count());
        Serial.println();
    }
}

void access::update_code_access(String time, long code, bool attempt)
{
    access_attempt try_access;
    try_access.timestamp = time;
    try_access.code = code;
    try_access.attempt = attempt;
    //db_users_access.appendRec(EDB_REC try_access);
    Serial.println("Guarde el intento en la memoria");
}

bool access::validate(long code)
{
    int recno = searchID(1, db_access.count(), code);
    if (recno != -1)
    {
        Serial.println("access granted");
        return true;
    }
    else
    {
        Serial.println("access denied");
        return false;
    }
}