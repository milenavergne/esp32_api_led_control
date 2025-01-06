#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include<IRremoteESP8266.h>
#include <IRsend.h>

#include <utils/header/connect.h>
#include <utils/header/mqttHandler.h>

const int irReceiverPin = 27;
IRsend irsend(irReceiverPin);
Connection connection("ESP", "pleasework");
WebServer server(80);
Mosquitto mqtt;

void setup()
{
  Serial.begin(115200);

  Serial.println("Setting up...");

  pinMode(LED_BUILTIN, OUTPUT);

  connection.printMacAddrs();
  // Configures static IP address and connectes to wifi
  connection.init();

  mqtt.connect();

  // API
  server.on("/on", HTTP_GET, []() { // Turn LED on when /on is requested
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/plain", "LED is ON");
    irsend.sendSAMSUNG(0xE0E040BF, 32);
    Serial.println("Led On");
  });

  server.on("/off", HTTP_GET, []() { // Turn LED off when /off is requested
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/plain", "LED is OFF");
    irsend.sendSAMSUNG(0xE0E040BF, 32);
    Serial.println("Led Off");
  });

  server.begin(); // Start the web server
  irsend.begin();
  Serial.println("Set up completed!");
}

void loop()
{
  if(Serial.available() != 0){
    int integerVariable = Serial.parseInt();
    Serial.printf("Value read from terminal: %d", integerVariable);
    if(integerVariable){
      Serial.print("### Power Buttor Comand ###");
      Serial.println();
      irsend.sendSAMSUNG(0xE0E040BF, 32);
    }
  }

  if (WiFi.status() == WL_CONNECTED && !connection.isConnected)
  {
    Serial.println("Connected");
    connection.isConnected = true;
    delay(5000);
  }
  else if (WiFi.status() != WL_CONNECTED && connection.isConnected)
  {
    Serial.println("Not connected to WiFi");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    connection.isConnected = false;

    connection.init();
    
    delay(5000);
  }
  server.handleClient();
  mqtt.loop();
}