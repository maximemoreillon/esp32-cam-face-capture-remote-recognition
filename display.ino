void dispay_setup(){

  // backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  tft.init(240, 240);
  tft.setRotation(1);
  tft.setFont();
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  Serial.println("[Display] init");
}

void display_wifi_disconnected(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print("Wifi disconnected");
}

void display_wifi_connected(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print("Wifi connected");
  display_start_time = millis();
}

void display_face_detected(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.print("\n");
  tft.print("Face detected");
  tft.print("\n\n");
  tft.print("Analyzing...");
}

void display_nothing(){
  tft.fillScreen(ST77XX_BLACK);
}

void display_face_not_recognized(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  tft.setTextWrap(true);
  tft.print("\n");
  tft.print("Face not recognized");
  tft.print("\n\n");
  tft.setTextSize(3);
  tft.print("Access denied");
}

void display_greetings(String user_name){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(true);
  
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("\n");
  tft.print("Face recognized");
  tft.print("\n\n");
  tft.print("Welcome");
  tft.print("\n\n");
  tft.setTextSize(3);
  tft.print(user_name);
}

void manage_display_extinction(){
  static boolean displaying_something = false;

  if(millis() - display_start_time > 30000){
    if(displaying_something) {
      Serial.println("[Display] Stopping display");
      display_nothing();
      displaying_something = false;
    }
  }
  else {
    if(!displaying_something) {
      Serial.println("[Display] displaying something");
      displaying_something = true;
    }
  }
}
