#include "request.h"
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//Configuracion statica del shield ethernet
IPAddress ip(192, 168, 1, 107);
IPAddress myDns(200, 44, 32, 12);
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
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware");
            return false;
        }
        if (Ethernet.linkStatus() == LinkOFF)
        {
            Serial.println("Ethernet cable is not connected");
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

bool request::ping(response1_t *rp1)
{
    client.stop();

    if (client.connect(server, 80)) //Con esta libreria no me puedo conectar por el puerto 443
    {
        Serial.println("connecting... /api/sirio/v1/ping");
        client.println("GET /api/sirio/v1/ping HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent: sirio-ethernet");
        client.println("Connection: keep-alive");
        client.println("Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        // client.println("Content-Type: application/json");
        client.println("");
        // client.println("{\"sirio_id\": 123}");
    }
    else
    {
        Serial.println("ping connection failed");
        return false;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("ping unexpected response: ");
        Serial.println(status);
        return false;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("ping invalid response");
        return false;
    }

    char salto[] = "\n";
    client.find(salto);
    const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, client);
    if (error)
    {
        Serial.print("ping deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }

    rp1 -> schedule = doc["schedules"]; // true
    rp1 -> action = doc["actions"]; // false
    Serial.println("Ping successful\r\n");
    return true;
}

bool request::schedules(struct response2 rp2[])
{
    client.stop();
    if (client.connect(server, 80)) //Con esta libreria no me puedo conectar por el puerto 443
    {
        Serial.println("connecting... /api/sirio/v1/schedules");
        client.println("GET /api/sirio/v1/schedules HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent:	sirio-ethernet");
        client.println("Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        client.println("Connection: close");
        client.println("");
    }
    else
    {
        Serial.println("Schedules connection failed");
        return false;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        Serial.print("Schedules unexpected response: ");
        Serial.println(status);
        return false;
    }
        
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println("Schedules invalid response");
        return false;
    }

    char salto[] = "\n";
    client.find(salto);
    // En esta linea en donde declaro el tamaño del json, por los momentos tengo para 20 registros
    const size_t capacity = JSON_ARRAY_SIZE(20) + 20*JSON_OBJECT_SIZE(4) + 120;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, client);
    for(unsigned int i=0; i<doc.size(); ++i)
    {
        rp2[i].id = doc[i]["id"]; // 1 or 2
        rp2[i].start = doc[i]["start"]; // "2020-03-25 08:30:00" or "2020-04-20 12:00:00"
        rp2[i].duration = doc[i]["duration"]; // 90 or 120
        rp2[i].access_code = doc[i]["access_code"]; // 123456 or 858585
    }

    Serial.println("Schedules successful\r\n");
    return true;
}