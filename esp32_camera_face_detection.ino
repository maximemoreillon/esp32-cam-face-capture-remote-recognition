#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <Update.h>
#include <HTTPClient.h>
#include "fd_forward.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSans24pt7b.h>
#include <ArduinoJson.h>

/*
 * BOARD SETTINGS
 * 
 * Board type: ESP32 Wrover Module
 * 
 */

// Getting WIFI_SSID and WIFI_PASSWORD from external credentials file
#include "credentials.h";


//#include "pin_mapping_ai_thinker.h";
//#include "pin_mapping_t-journal.h";
//#include "pin_mapping_t-camera.h";
#include "pin_mapping_t-camera_plus.h";

// Display pin mapping
#define TFT_CS         12
#define TFT_DC         15
#define TFT_BACKLIGHT  2
#define TFT_MOSI       19
#define TFT_MISO       22
#define TFT_SCLK       21
#define TFT_RST        -1 // RST unknown

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

WebServer web_server(80);
HTTPClient http;

static mtmn_config_t mtmn_config = {0};

boolean last_detected = false;


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_init();
  wifi_setup();
  web_server_setup();
  dispay_setup();
  face_Detection_init();
}

void loop() {
  wifi_connection_manager();
  web_server.handleClient();
  boolean detected = face_detection();

  // on detection change
  
  if(detected != last_detected){
    last_detected = detected;
    if(detected){
      display_text("face detected");
      API_call_image();
    }
    else {
      if(WiFi.status() == WL_CONNECTED){
        display_ip();
      }
      else {
        display_disconnected();
      }
    }
  }

  
}
