#include "access.h"
#include <EDB.h>
#include <EEPROM.h>
#include <keypad.h> //Temporal mientras hago pruebas en mi casa

/*
---------- Varianbles asociadas a la base de datos en la eeprom --------------------------------
*/
#define TABLE_SIZE 4096
#define RECORDS_TO_CREATE 10

response2 event_access;

// The read and write handlers for using the EEPROM Library
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
const byte ROWS2 = 4; //four rows
const byte COLS2 = 4; //four columns
char keys2[ROWS2][COLS2] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins2[ROWS2] = {11, 14, 13, 12}; //connect to the row pinouts of the keypad
byte colPins2[COLS2] = {7, 8, 9, 10};    //connect to the column pinouts of the keypad

Keypad keypad2 = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS2, COLS2);

void access::init()
{
    //while (!request::init());
    db.create(0, TABLE_SIZE, (unsigned int)sizeof(event_access));
    event_access.id = 123;
    event_access.access_code = 457863;
    db.appendRec(EDB_REC event_access);
}

bool access::update()
{
    return false;
}

void access::validate()
{
    char customKey = keypad2.getKey();
    Serial.println(customKey);
}
