#include <WiFi.h>
#include <PubSubClient.h>

class mosquitto
{
    private:
    //MQTT variables
    const char *mqtt_broker = "192.168.10.119";
    const int mqtt_port = 1883;  
    const char *topic;
    WiFiClient wifi_esp_client;
    PubSubClient mqtt_client;

    public:
    mosquitto(const char* tpc = "LED"): topic(tpc), mqtt_client(wifi_esp_client){};
    void connect();
    void loop();
    static void callback(char *topic, byte *payload, unsigned int length);
};