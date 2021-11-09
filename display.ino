void dispay_setup(){

  // backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  
  tft.init(240, 240);
  tft.setRotation(1);
  tft.setFont();
  
  
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setCursor(0, 0);
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextWrap(true);
  tft.print("Display init OK");
}

void display_disconnected(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextWrap(true);
  tft.print("Connecting...");
}

void display_ip(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeSans24pt7b);
  tft.setCursor(20, 120);
  tft.setTextColor(0x528A);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print(WiFi.localIP()[3]);
}

void display_emoji(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeSans24pt7b);
  tft.setCursor(20, 120);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print("Hi!");
}

void display_text(String text){
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont();
  tft.setCursor(20, 120);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print(text);
}
