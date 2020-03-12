#include "request.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EDB.h> //Falta configuar esta libreria en las funciones

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 66, 244);
IPAddress myDns(192, 168, 66, 1);
EthernetClient client;

char server[] = "www.arduino.cc";
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
        Serial.print("  DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
        delay(1000);
        return true;
    }
}

response1 request::ping()
{
    client.stop();
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /asciilogo.txt HTTP/1.1");
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("connection failed");
  }

  response1 rp1;
  while (client.available()) { //Cambiar el loop para leer todo el response
    char c = client.read();
    Serial.write(c);
  }
  return rp1;
}
    
response2 request::schedules()
{
    response2 rp2;
    return rp2;
}