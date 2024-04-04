#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* ntfyURL = "https://ntfy.sh/Esp32WokwiTest";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("CONNECTED to SSID: ");
  Serial.println(ssid);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(ntfyURL);  // Specify request destination
    http.addHeader("Content-Type", "text/plain");  // Set content type

    // Your message
    String message = "Message successful 😀";
    
    int httpResponseCode = http.POST(message);  // Send the POST request
    if (httpResponseCode > 0) {
      Serial.printf("[HTTP] POST request to ntfy.sh was successful, response code: %d\n", httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);  // Print response payload
    } else {
      Serial.printf("[HTTP] POST request to ntfy.sh failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    
    http.end();  // Free resources
  }
  
  delay(60000);  // Wait for 1 minute before sending the next message
}
