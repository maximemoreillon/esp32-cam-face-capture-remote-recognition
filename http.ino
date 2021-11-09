#define BOUNDARY "--------------------------133747188241686651551404"  

#define API_HOSTNAME "192.168.1.2"
#define API_PORT 7070
#define ROUTE "/find_match"
#define FIELD_NAME "image"


void API_call_image(){
  Serial.println("[HTTP] Image API call");

  
  WiFiClient client;
  if (!client.connect(API_HOSTNAME, API_PORT)) {
    Serial.println("[HTTP] connection failed");
    return;
  }

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  
  if (!fb) {
    Serial.println("[Camera] Error getting frame");
    return;
  }
  
  String body_pre_image;
  body_pre_image = "--";
  body_pre_image += BOUNDARY;
  body_pre_image += F("\r\n");
  body_pre_image += F("Content-Disposition: form-data; name=\"");
  body_pre_image += FIELD_NAME;
  body_pre_image += F("\"; filename=\"picture.jpg\"\r\n");
  body_pre_image += F("Content-Type: image/jpeg\r\n");
  body_pre_image += F("\r\n");

  String body_post_image = String("\r\n--")+BOUNDARY+String("--\r\n");

  int total_length = body_pre_image.length()+body_post_image.length()+fb->len;
  
  String header_text;
  header_text =  F("POST ");
  header_text += ROUTE;
  header_text += F(" HTTP/1.1\r\n");
  header_text += F("cache-control: no-cache\r\n");
  // Host header mandatory for FastAPI
  header_text += F("host: 192.168.1.2\r\n");
  // Content-type defined as multipart/form data to upload image
  header_text += F("Content-Type: multipart/form-data; boundary=");
  header_text += BOUNDARY;
  header_text += "\r\n";
  header_text += F("content-length: ");
  header_text += String(total_length);
  header_text += "\r\n";
  header_text += "\r\n";

  client.print(header_text+body_pre_image);
  client.write(fb->buf, fb->len);
  client.print(body_post_image);

  // Free up memory for next round
  esp_camera_fb_return(fb);
  fb = NULL;


  // might not be necessary
  //delay(20);
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("[HTTP] request timeout");
      client.stop();
      return;
    }
  }

  String response_string;
  while(client.available()) {
    response_string = client.readStringUntil('\r');
  }

  // conversion to JSON
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response_string);
  JsonObject obj = doc.as<JsonObject>();

  

  if(obj["name"].isNull()){
    Serial.println("Not recognized");
    display_text("Not recognized");
  }
  else {
    String user_name = obj["name"];
    Serial.println(user_name);
    display_text(user_name);
  }
  
  


  
}
