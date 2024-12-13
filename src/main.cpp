#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <utils/header/connect.h>
#include <utils/header/mqttHandler.h>

Connection connection();
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
    Serial.println("Led On");
  });

  server.on("/off", HTTP_GET, []() { // Turn LED off when /off is requested
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/plain", "LED is OFF");
    Serial.println("Led Off");
  });

  server.begin(); // Start the web server

  Serial.println("Set up completed!");
}

void loop()
{
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