#ifndef ESP_ENDPOINT
#define ESP_ENDPOINT

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiGratuitous.h>

class ESPEndpoint {
public:
    struct Variable {
        String name;
        String label;
        int value;
        int (*callback)(int);
        inline String json() {
            return "{ \"name\": \"" + this->name +
                   "\", \"label\": \"" + this->label +
                   "\", \"val\": " + String(this->value) + " }";
        } };

private:
    experimental::ESP8266WiFiGratuitous grat_arp;
    WiFiServer server;
    String id;
    Variable* values; int count;
    int extractValueFor(String name, String source);

public:
    ESPEndpoint(const char* ssid, const char* password, const char* server_id, Variable values[], int count);
    void iterate();
};

#endif

