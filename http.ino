#define FR_API_FORM_BOUNDARY "--------------------------133747188241686651551404"  




void API_call_image(){
  Serial.println("[HTTP] Image API call");

  
  WiFiClient client;
  if (!client.connect(FR_API_HOSTNAME, FR_API_PORT)) {
    Serial.println("[HTTP] connection failed");
    return;
  }

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  
  if (!fb) {
    Serial.println("[Camera] Error getting frame");
    return;
  }

  Serial.println("turning blinking on");

  // The part of the request body before the image binary data
  String body_pre_image;
  body_pre_image = "--";
  body_pre_image += FR_API_FORM_BOUNDARY;
  body_pre_image += "\r\n";
  body_pre_image += "Content-Disposition: form-data; name=\"";
  body_pre_image += FR_API_FIELD_NAME;
  body_pre_image += "\"; filename=\"picture.jpg\"\r\n";
  body_pre_image += "Content-Type: image/jpeg\r\n";
  body_pre_image += "\r\n";

  // The part of the request that comes after the image binary data
  String body_post_image;
  body_post_image += "\r\n--";
  body_post_image += FR_API_FORM_BOUNDARY;
  body_post_image += "--\r\n";

  // Compute total length of body to specify it in headers
  int total_length = body_pre_image.length() + body_post_image.length() + fb->len;
  
  String header_text;
  header_text =  "POST "; // Request method
  header_text += FR_API_ROUTE; // Request route
  header_text += " HTTP/1.1\r\n";
  header_text += "cache-control: no-cache\r\n";
  header_text += "host: "; // Host header mandatory for FastAPI
  header_text += FR_API_HOSTNAME; 
  header_text += "\r\n"; 
  header_text += "authorization: Bearer ";
  header_text += JWT;
  header_text += "\r\n"; 
  header_text += "Content-Type: multipart/form-data; boundary="; // Content-type defined as multipart/form data to upload image
  header_text += FR_API_FORM_BOUNDARY;
  header_text += "\r\n";
  header_text += "content-length: ";
  header_text += String(total_length);
  header_text += "\r\n\r\n"; // Double new line for end of header

  client.print(header_text+body_pre_image);
  client.write(fb->buf, fb->len);
  client.print(body_post_image);

  // Free up memory for next round
  esp_camera_fb_return(fb);
  fb = NULL;

  
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
    Serial.println("[FR] Not recognized");
    display_face_not_recognized();
  }
  else {
    String user_name = obj["name"];
    Serial.print("[FR] Recognized user ");
    Serial.println(user_name);
    MQTT_publish_unlock();
    display_greetings(user_name);
  }
  
  


  
}
