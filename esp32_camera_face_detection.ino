/*
 * BOARD SETTINGS
 * 
 * Board type: ESP32 Wrover Module
 * 
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> // MQTT
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_camera.h" // Camera library
#include "fd_forward.h" // Face detection library
#include <AsyncTCP.h> // Needed for web server
#include <ESPAsyncWebServer.h> // Web server
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Update.h>


// Getting WIFI_SSID and WIFI_PASSWORD from external credentials file
#include "credentials.h";
#include "iot_config_home.h"; // MQTT broker URL + connection config

//#include "pin_mapping_ai_thinker.h";
//#include "pin_mapping_t-journal.h";
//#include "pin_mapping_t-camera.h";
#include "pin_mapping_t-camera_plus.h";

//Device info
#define DEVICE_TYPE "face-detection-camera"
#define DEVICE_FIRMWARE_VERSION "0.1.1"

// Face recognition API settings
#define FR_API_HOSTNAME "192.168.1.3"
#define FR_API_PORT 30020
#define FR_API_ROUTE "/recognize"
#define FR_API_FIELD_NAME "image"

// MQTT parameters
#define MQTT_LOCK_COMMAND_TOPIC "lock/command"
#define MQTT_LOCK_STATUS_TOPIC "lock/status"
#define MQTT_RETAIN true
#define MQTT_RECONNECT_PERIOD 1000

// Display pin mapping
#define TFT_CS         12
#define TFT_DC         15
#define TFT_BACKLIGHT  2
#define TFT_MOSI       19
#define TFT_MISO       22
#define TFT_SCLK       21
#define TFT_RST        -1 // RST unknown

// required for OTA update
#define U_PART U_SPIFFS


AsyncWebServer server(80);
HTTPClient http; // Probably unused
WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

static mtmn_config_t mtmn_config = {0};


boolean locked = false;

boolean cooling_down = false;
long cooldown_start_time = 0;

long display_start_time = 0;


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("ESP32 camera face detection");

  dispay_setup();
  camera_init();
  wifi_setup();
  server_init();
  face_detection_init();
  MQTT_setup();
}

void loop() {
  MQTT_connection_manager();
  MQTT_client.loop();

  manage_display_extinction();

  

  
  
 
  // on detection change
  long current_time = millis();
  if(current_time - cooldown_start_time > 5000){
    
    if(cooling_down){
      // Acknowledge end of cooldown time
      cooling_down = false;
      Serial.println("[EventLoop] Cooldown finished");
      display_nothing();
    }

    boolean detected = face_detection();
    if(detected){
      display_face_detected();
      API_call_image();
      cooldown_start_time = millis();
    }
  }
  else {
    if(!cooling_down){
      // Acknowledge start of cooldown time
      cooling_down = true;
      Serial.println("[EventLoop] Cooldown started");
      
    }
  }

  

  


  
}
