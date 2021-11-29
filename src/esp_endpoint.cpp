#include "esp_endpoint.hpp"

ESPEndpoint::ESPEndpoint(const char* ssid, const char* password, const char* name, ESPEndpoint::Variable values[], int count)
: server(80), name(name), count(count) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(3);
    this->server.begin();

    this->values = (ESPEndpoint::Variable*)malloc(this->count * sizeof(*(this->values)));
    memcpy(this->values, values, this->count * sizeof(*this->values));

    Serial.println(WiFi.localIP());
}

void ESPEndpoint::iterate() {
    WiFiClient client = this->server.available();
    if (!client) return;

    while(!client.available()) delay(3);

    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/json\n");
    client.println("{");

    int extr = this->extractValueFor("info", request);
    if (extr + 2) {
        client.print("    \"name\": \""); client.print(this->name); client.println("\",");
    }

    for (int i = 0; i < this->count; i++) {
        ESPEndpoint::Variable *var = this->values + i;
        extr = this->extractValueFor(var->name, request);
        if (extr > -2) {
            client.print("    \""); client.print(var->name); client.print("\": ");
            if (extr > -1) {
                var->value = (var->callback)(extr);
            }
            client.print(var->value); client.println(",");
        }
    }

    client.println("}");
}

// -2 when `name` not found in `source`
// -1 when no digits follow `name`
// else: returns number following `name`
int ESPEndpoint::extractValueFor(String name, String source) {
    int val_start = source.indexOf(name) + name.length();
    if (val_start < (signed int)name.length()) return -2;
    int val_end = val_start;
    while (isdigit(source[val_end++]));
    if (val_end == val_start + 1) return -1;
    return source.substring(val_start, val_end).toInt();
}

