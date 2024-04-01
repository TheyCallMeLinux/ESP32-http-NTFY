//
//
////////////////////////////////////////////////////////////////////
//   ___  __   __          ___ ___  __          ___  ___          //
//  |__  /__` |__) __ |__|  |   |  |__) __ |\ |  |  |__  \ /      //
//  |___ .__/ |       |  |  |   |  |       | \|  |  |     |       //
////////////////////////////////////////////////////////////////////
//
//
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* ntfyURL = "https://ntfy.sh/Esp32WokwiTest";

void setup() {
  Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);           
  display.setTextColor(SSD1306_WHITE);      
  display.setCursor(0,0); 
  display.println("Connecting to WiFi...");
  display.display();
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("CONNECTED to SSID: ");
  Serial.println(ssid);
  display.clearDisplay();
  display.print("Connected to ");
  display.println(ssid);
  display.display();
  delay(5000);
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
      display.clearDisplay();
      display.setTextSize(1);
      printCenter("Notification sent", 0, 32);
      display.display();
    } else {
      Serial.printf("[HTTP] POST request to ntfy.sh failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      display.print("Notification delivery FAILED ");
      display.display();
    }
    
    http.end();  // Free resources
  }
  
  delay(60000);  // Wait for 1 minute before sending the next message
}
void printCenter(const String buf, int x, int y)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
  display.setCursor((x - w / 2) + (128 / 2), y);
  display.print(buf);
}
