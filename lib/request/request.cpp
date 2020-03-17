#include "request.h"
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 66, 244);
IPAddress myDns(192, 168, 66, 1);
EthernetClient client;

char server[] = "arduinojson.org";
// IPAddress server(192,168,66,138);

bool request::init()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    Serial.println("Initialize Ethernet with DHCP:");
    Ethernet.init(53);
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        if (Ethernet.hardwareStatus() == EthernetNoHardware)
        {
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            return false;
        }
        if (Ethernet.linkStatus() == LinkOFF)
        {
            Serial.println("Ethernet cable is not connected.");
            delay(1000);
            return false;
        }
        Ethernet.begin(mac, ip, myDns);
        Serial.print("My IP address: ");
        Serial.println(Ethernet.localIP());
        delay(1000);
        return true;
    }
    else
    {
        Serial.print("DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
        delay(1000);
        return true;
    }
}

response1 request::ping()
{
    client.stop();
    response1 rp1;

    if (client.connect(server, 80))
    {
        Serial.println("connecting...");
        client.println("POST /api/v1/ping HTTP/1.1");
        client.println("Host: arduinojson.org"); 
        client.println("User-Agent: sirio-ethernet");
        client.println("Content-Type: application/json");
        client.println();
        client.println("{\"sirio_id\": 123}");
    }
    else
    {
        Serial.println("connection failed");
        return rp1;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("Unexpected response: ");
        Serial.println(status);
        return rp1;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("Invalid response");
        return rp1;
    }

    const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, client);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return rp1;
    }

    rp1.schedule = doc["schedules"]; // true
    rp1.action = doc["actions"]; // false
    return rp1;
}

response2 request::schedules()
{
    client.stop();
    response2 rp2;

    if (client.connect(server, 80))
    {
        Serial.println("connecting...");
        client.println("GET /api/v1/schedules HTTP/1.1");
        client.println("Host: arduinojson.org"); 
        client.println("Connection: close");
    }
    else
    {
        Serial.println("connection failed");
        return rp2;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("Unexpected response: ");
        Serial.println(status);
        return rp2;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("Invalid response");
        return rp2;
    }

    const size_t capacity = JSON_OBJECT_SIZE(5) + 60;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, client);

    rp2.id = doc["id"]; // 123
    rp2.start = doc["start"]; // "TIMESTAMP in UTC"
    rp2.duration = doc["duration"]; // 90
    rp2.type = doc["type"]; // 15222
    rp2.access_code = doc["access_code"]; // 451278

    return rp2;
}