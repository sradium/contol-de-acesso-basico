#include "access.h"
#include <EDB.h>
#include <EEPROM.h>
#include <keypad.h> 

/*
---------- Varianbles asociadas a la base de datos en la EEPROM --------------------------------
*/
#define TABLE_SIZE 4096

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

EDB db(&writer, &reader);

/*
---------------------- Variables asociadas al keypad -------------------------------------------
*/
const byte ROWS2 = 4; 
const byte COLS2 = 4; 
char keys2[ROWS2][COLS2] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins2[ROWS2] = {11, 14, 13, 12}; 
byte colPins2[COLS2] = {7, 8, 9, 10};    

Keypad keypad2 = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS2, COLS2);

void access::init()
{
    while(!request::init()){
        delay(300);
    }
    db.open(0);
    Serial.print("No of records max: ");
    Serial.println(db.limit());
    Serial.print("No of records used: ");
    Serial.println(db.count());
}

void access::update(response1_t *rp1)
{
    if (request::schedules(rp2))
    {
        rp1->schedule = false;
        db.clear(); //Borro todos los registros existentes. Esto cambiara solo lo tengo para probar
        for(int i = 0; rp2[i].id !=0 || db.limit() == db.count(); ++i)
        {
            db.appendRec(EDB_REC rp2[i]);
        }
        Serial.print("No of record used: ");
        Serial.println(db.count());
        Serial.println();
    }
}

void access::validate()
{
    char customKey = keypad2.getKey();
    Serial.println(customKey);
}
