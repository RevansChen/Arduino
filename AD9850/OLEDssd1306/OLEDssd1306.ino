#include "U8glib.h"

String c;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC

void setup() {
  Serial.begin(19200);
  u8g.setFont(u8g_font_unifont);
}

void loop() {
  u8g.firstPage();
  do{  
    if(Serial.available()>0){
      c = Serial.readString();
      Serial.print("that is the message:");
      Serial.println(c);
    }
    u8g.setColorIndex(1);
    u8g.drawStr(0,22,"01     789012345");
    u8g.drawStr(0,55,c.c_str());
  }while(u8g.nextPage());

  delay(50);
}

