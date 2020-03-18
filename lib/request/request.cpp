#include "request.h"
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 66, 244);
IPAddress myDns(192, 168, 66, 1);
EthernetClient client;

char server[] = "cemco.innotica.net";

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

void request::ping(response1_t *rp1)
{
    client.stop();

    if (client.connect(server, 80))
    {
        client.println("GET /api/sirio/v1/ping HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent: sirio");
        client.println("Connection: keep-alive");
        // client.println("Content-Type: application/json");
        client.println("");
        // client.println("{\"sirio_id\": 123}");
    }
    else
    {
        Serial.println("connection failed");
        return;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("Unexpected response: ");
        Serial.println(status);
        return;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("Invalid response");
        return;
    }

    char salto[] = "\n";
    client.find(salto);
    const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, client);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    rp1 -> schedule = doc["schedules"]; // true
    rp1 -> action = doc["actions"]; // false
    return;
}

void request::schedules(struct response2 rp2[])
{
    client.stop();
    if (client.connect(server, 80))
    {
        Serial.println("connecting...");
        client.println("GET /api/sirio/v1/schedules HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent:	Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
        client.println("Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        client.println("Connection: close");
        client.println("");
    }
    else
    {
        Serial.println("connection failed");
        return;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("Unexpected response: ");
        Serial.println(status);
        return;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("Invalid response");
        return;
    }

    char salto[] = "\n";
    client.find(salto);
    const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 120;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, client);
    Serial.println(doc.size());
    for(unsigned int i=0; i<doc.size(); ++i)
    {
        rp2[i].id = doc[i]["id"]; // 1 or 2
        rp2[i].start = doc[i]["start"]; // "2020-03-25 08:30:00" or "2020-04-20 12:00:00"
        rp2[i].duration = doc[i]["duration"]; // 90 or 120
        rp2[i].access_code = doc[i]["access_code"]; // 123456 or 858585
    }
}