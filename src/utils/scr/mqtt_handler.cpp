#include<utils/header/mqtt_handler.h>

void mosquitto::callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void mosquitto::connect()
{
    //connect to MQTT
    this->mqtt_client.setServer(this->mqtt_broker, this->mqtt_port);
    this->mqtt_client.setCallback(mosquitto::callback);

    while (!this->mqtt_client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connecting to MQTT broker\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str())) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.println(mqtt_client.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    this->mqtt_client.publish(this->topic, "ESP32 is connected");
    this->mqtt_client.subscribe(this->topic);
}

void mosquitto::loop(){
    this->mqtt_client.loop();
}