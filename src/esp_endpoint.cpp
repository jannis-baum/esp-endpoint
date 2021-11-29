#include "esp_endpoint.hpp"

ESPEndpoint::ESPEndpoint(const char* ssid, const char* password, const char* name)
: server(80), name(name) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(3);
    this->server.begin();

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
    client.print("    \"name\": \""); client.print(this->name); client.println("\"");
    client.println("}");
}

