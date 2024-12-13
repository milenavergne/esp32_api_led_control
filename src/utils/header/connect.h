#pragma once

#include <Arduino.h>
#include <WiFi.h>

struct Connection
{
    bool isConnected = false;
    const char* id; 
    const char* password;

    // Constructor with default values for id and password
    Connection(const char* name = "ESPTEST", const char* pswrd = "pleasework")
        : id(name), password(pswrd){}

    void init();
    void printMacAddrs();
};