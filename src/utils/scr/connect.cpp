#include<utils/header/connect.h>
#include<ESP32Ping.h>
#include<ESPmDNS.h>

void Connection::init()
{
    // Connect to wifi
    WiFi.begin(this->id, this->password);

    Serial.printf("WIFI config: user: %s, password: %s \n", this->id, this->password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(this->id, this->password);
        Serial.printf("WIFI config: user: %s, password: %s \n", this->id, this->password);

        delay(5000);
    }
    // Start mDNS service
    if (MDNS.begin("esp32")) {  // "esp32" is the hostname for mDNS
        Serial.println("mDNS responder started");
    } else {
        Serial.println("Error starting mDNS");
    }

    if(Ping.ping("8.8.8.8"))
    {
        Serial.println("Connected to WiFi");
        this->isConnected = true;
    }
    else
    {
        Serial.println("Unable to ping google. Reconnecting ...");
        this->init();
    }    
}

void Connection::printMacAddrs()
{
    // Variable to store the MAC address
    uint8_t baseMac[6];
    
    // Get MAC address of the WiFi station interface
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    Serial.print("Station MAC: ");
    for (int i = 0; i < 5; i++) {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);
    
    // Get the MAC address of the Wi-Fi AP interface
    esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
    Serial.print("SoftAP MAC: ");
    for (int i = 0; i < 5; i++) {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);
    
    // Get the MAC address of the Bluetooth interface
    esp_read_mac(baseMac, ESP_MAC_BT);
    Serial.print("Bluetooth MAC: ");
    for (int i = 0; i < 5; i++) {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);

    // Get the MAC address of the Ethernet interface
    esp_read_mac(baseMac, ESP_MAC_ETH);
    Serial.print("Ethernet MAC: ");
    for (int i = 0; i < 5; i++) {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);
}