#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Mega Who";
const char* password = "23571234";
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "ESP8266 Web Server");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "Body not received");
    return;
  }
  
  String requestBody = server.arg("plain");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, requestBody);

  if (error) {
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  int x = doc["x"];
  int y = doc["y"];

  // Print the values to serial monitor
  Serial.print("(");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(")");

  // Send response
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"status\":\"received\"}");

  // Forward data to Arduino Nano
  //Serial.print(x);
  //Serial.print(",");
  //Serial.println(y);
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/post", HTTP_POST, handlePost);
  server.on("/post", HTTP_OPTIONS, handleOptions);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
