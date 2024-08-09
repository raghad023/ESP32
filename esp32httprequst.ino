#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "STC_WiFi_5G";
const char* pass = "223";

unsigned const long interval = 2000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting WiFi connection...");
  
  WiFi.begin(ssid, pass);

  int attemptCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    attemptCounter++;
    
    if (attemptCounter > 30) { // Try for 30 seconds
      Serial.println("\nFailed to connect to WiFi");
      return;
    }
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin("http://<server_ip>/get_last_direction.php"); // Replace <server_ip> with the actual server IP
      int httpResponseCode = http.GET();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println("Response payload: ");
        Serial.println(payload);
      } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}
