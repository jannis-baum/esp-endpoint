#ifndef ESP_ENDPOINT
#define ESP_ENDPOINT

#include <Arduino.h>
#include <ESP8266WiFi.h>

class ESPEndpoint {
public:
    // struct Value {
    //     String name;
    //     int value;
    //     int (*callback)(int);
    // };

private:
    WiFiServer server;
    String name;
    // Value* values; int count;
    int extractValueFor(String name, String source);

public:
    ESPEndpoint(const char* ssid, const char* password, const char* name);//, Value* values, int count);
    void iterate();
};

#endif

