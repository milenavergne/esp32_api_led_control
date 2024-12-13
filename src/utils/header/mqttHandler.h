#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

class Mosquitto
{
    private:
    //MQTT variables
    const char *mqttBroker = "192.168.10.119";
    const int mqttPort = 1883;  
    const char *topic;
    WiFiClient wifiEspClient;
    PubSubClient mqttClient;

    public:
    Mosquitto(const char* tpc = "LED"): topic(tpc), mqttClient(wifiEspClient){};
    void connect();
    void loop();
    static void callback(char *topic, byte *payload, unsigned int length);
};