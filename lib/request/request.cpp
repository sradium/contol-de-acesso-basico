#include "request.h"

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//Configuracion statica del shield ethernet
IPAddress ip(192, 168, 1, 106);
IPAddress myDns(200, 44, 32, 12);
EthernetClient client;

char server[] = "cemco.innotica.net";

bool request::init()
{
    Serial.begin(9600);
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

/*
*   Metodo get generico para la conexion a los endpoint
*/

bool request::get(const char* url, const char* msg)
{
    client.stop();
    if (client.connect(server, 80))
    {
        client.print("GET ");
        client.print(url);
        client.println(" HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent:	sirio-ethernet");
        client.println("Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        client.println("Connection: keep-alive");
        client.print(msg);
    }
    else
    {
        return false;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        return false;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        return false;
    }

    char salto[] = "\n";
    client.find(salto);
    return true;
}


/*
*   Metodo post generico para la conexion a los endpoint
*/

bool request::post(const char* url, const char* msg)
{
    client.stop();
    if (client.connect(server, 80))
    {
        client.print("POST ");
        client.print(url);
        client.println(" HTTP/1.1");
        client.println("Host: cemco.innotica.net");
        client.println("User-Agent:	sirio-ethernet");
        client.println("Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
        client.println("Connection: keep-alive");
        client.print(msg);
    }
    else
    {
        return false;
    }

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status + 9, "200 OK") != 0)
    {
        return false;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        return false;
    }

    char salto[] = "\n";
    client.find(salto);
    return true;
}

bool request::ping(next_endpoint_t *ne)
{
    if (get("/api/sirio/v1/ping", "\r\n"))
    {
        const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
        DynamicJsonDocument doc(capacity);

        DeserializationError error = deserializeJson(doc, client);
        if (error)
        {
            Serial.print("ping deserializeJson() failed: ");
            Serial.println(error.c_str());
            return false;
        }

        ne->schedules = doc["schedules"]; // true
        ne->actions = doc["actions"];     // false
        Serial.println("Ping successful\r\n");
        return true;
    }
    else
    {
        Serial.println("connection ping failed\r\n");
        return false;
    }
}

time_t request::convertUnix(const char *date)
{
    int j = 0;
    unsigned int elements[6] = {0};
    for (int i = 0; i < 19; ++i)
    {
        if (date[i] == ' ' || date[i] == '-' || date[i] == ':')
        {
            ++j;
        }
        else
        {
            elements[j] = (elements[j] * 10) + ((int)date[i] - 48);
        }
    }
    tmElements_t unix_date;
    unix_date.Year = (elements[0] - 1970);
    unix_date.Month = elements[1];
    unix_date.Day = elements[2];
    unix_date.Hour = elements[3];
    unix_date.Minute = elements[4];
    unix_date.Second = elements[5];
    return makeTime(unix_date);
}

bool request::schedules(access_record_t ar[])
{
    if (get("/api/sirio/v1/schedules", "\r\n"))
    {
        const size_t capacity = JSON_ARRAY_SIZE(20) + 20 * JSON_OBJECT_SIZE(4) + 120;
        DynamicJsonDocument doc(capacity);

        deserializeJson(doc, client);
        for (unsigned int i = 0; i < doc.size(); ++i)
        {
            JsonObject root = doc[i];
            ar[i].id = root["id"];
            time_t start = convertUnix(root["start"]);
            ar[i].start = start;
            time_t final = root["duration"];
            ar[i].final = (60 * final) + start;
            ar[i].access_code = root["access_code"];
        }
        Serial.println("Connection schedules successful\r\n");
        return true;
    }
    else
    {
        Serial.println("Connection schedules failed\r\n");
        return false;
    }
}

bool request::audit(String json)
{
    String msg = "Content-Length: 15\r\n\r\n";
    msg += json;
    char* buffer = (char*) msg.c_str();
    if (get("/api/sirio/v1/audit", buffer))
    {
        while ((client.available()))
        {
            char c = client.read();
            Serial.write(c);
        }
        return true;
    }
    else
    {
        Serial.println("Connection endpoint failed\r\n");
        return false;
    }
}