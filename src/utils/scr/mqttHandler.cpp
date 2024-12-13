#include<utils/header/mqttHandler.h>

void Mosquitto::callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void Mosquitto::connect()
{
    //connect to MQTT
    this->mqttClient.setServer(this->mqttBroker, this->mqttPort);
    this->mqttClient.setCallback(Mosquitto::callback);

    while (!this->mqttClient.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connecting to MQTT broker\n", client_id.c_str());
        if (mqttClient.connect(client_id.c_str())) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    this->mqttClient.publish(this->topic, "ESP32 is connected");
    this->mqttClient.subscribe(this->topic);
}

void Mosquitto::loop(){
    this->mqttClient.loop();
}