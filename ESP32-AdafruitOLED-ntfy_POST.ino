// Built mostly by hand and using Bitcoin ESP32 Bitcoin Price Tracker on Wokwi https://wokwi.com/
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
//#include <ArduinoJson.h> can be useful for later
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ntfyServer = "ntfy.sh";
const int ntfyPort = 80;
const char* ntfyToken = "Esp32WokwiTest";

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Send push notification
  sendNotification("Hello from ESP32");
}

void loop() {
  // Nothing to do here
}

void sendNotification(String message) {
  WiFiClient client;
  
  if (!client.connect(ntfyServer, ntfyPort)) {
    Serial.println("Failed to connect to ntfy.sh");
    return;
  }

  String payload = "{\"action\": \"push\", \"title\": \"ESP32\", \"message\": \"" + message + "\"}";

  client.print(String("POST /Esp32WokwiTest HTTP/1.1\r\n") +
               "Host: " + ntfyServer + "\r\n" +
               "Actions: http, Push notification, http://" + ntfyServer + ntfyToken + ", method=POST, body=" + payload + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(500); // Wait for server response

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Notification sent");
  client.stop();
}
