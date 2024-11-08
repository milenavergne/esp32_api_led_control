#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include"utils/header/connect.h"

// Set your Static IP address
IPAddress local_IP(192,168,1,184);
// Set your Gateway IP address
IPAddress gateway(192,168,1,1);

IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebServer server(80);

wifi::configuration wifi;

void setup() {
  Serial.begin(115200);

  Serial.println("Setting up...");

  pinMode(LED_BUILTIN, OUTPUT);
  
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin( wifi.id, wifi.password);

  // mDMS

  if (!MDNS.begin("esp32")) {   // Set the hostname to "esp32.local"
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // API

  server.on("/on", HTTP_GET, []() {        // Turn LED on when /on is requested
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/plain", "LED is ON");
    Serial.println("Led On");
  });

  server.on("/off", HTTP_GET, []() {       // Turn LED off when /off is requested
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/plain", "LED is OFF");
    Serial.println("Led Off");
  });

  server.begin();                          // Start the web server

  Serial.println("Set up completed!");

}

void loop() {
  if (!MDNS.begin("esp32")) {   // Set the hostname to "esp32.local"
    Serial.println("Error setting up MDNS responder!");
    delay(1000);
  }
  if(WiFi.status() == WL_CONNECTED && !wifi.isConnected)
  {
    for(int i = 0; i < 5; i ++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
    wifi.isConnected = true;
  }
 else if(WiFi.status() != WL_CONNECTED)
 {
    Serial.println("Not connected to WiFi");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
 }
  server.handleClient();
}