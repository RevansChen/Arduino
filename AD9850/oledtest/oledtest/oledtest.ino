/*

  HelloWorld.pde
  
  "Hello World!" example code.
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/


#include "U8glib.h"


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC


void draw() {
  // graphic commands to redraw the complete screen should be placed here  
/*
for(int x=0 ; x < 128 ; ++x)
  
  for (int y = 0; y < 64; ++y) {
    //u8g.setColorIndex(i & 1);
    u8g.setColorIndex(1);
    u8g.drawPixel(x,y);
    int c=x%2;
    int d=y%2;
    if(c == 1 && d == 1){
    u8g.setColorIndex(0);
    u8g.drawPixel(x,y);
    }
    else if(c == 0 && d == 0){
    u8g.setColorIndex(0);
    u8g.drawPixel(x,y);
  }
}
*/  
int siz = 3;
for(int x=0 ; x < u8g.getWidth() ; x = x+siz)
  
  for (int y = 0; y < u8g.getHeight(); y = y+siz) {
    //u8g.setColorIndex(i & 1);
    u8g.setColorIndex(1);
    u8g.drawBox(x,y,siz,siz);
    int c=x/siz;
    int d=y/siz;
    c=c%2;
    d=d%2;
  if(c == 1 && d == 1){
    u8g.setColorIndex(0);
    u8g.drawBox(x,y,siz,siz);
    }
    else if(c == 0 && d == 0){
    u8g.setColorIndex(0);
    u8g.drawBox(x,y,siz,siz);
  }
}
/*
  const int sz = 4;
  for (int x = 0; x < u8g.getWidth(); x += sz) {
    for (int y = 0; y < u8g.getHeight(); y += sz) {
      u8g.setColorIndex((x / sz + y / sz) & 1);
      u8g.drawBox(x, y, sz, sz);
    } 
  }
*/
    
    //u8g.drawBox(0, i * 13, 128, 13);
    
    /*u8g.setColorIndex((i + 1) & 1);
    if (i & 1) {
      u8g.drawStr(0, i * 13 + 11, "012345678901234567890");
    }
    else {
      u8g.drawStr(0, i * 13 + 11, "abcdefghijklmnopqrstuvwxyz");
    }*/
  
}

void setup() {/*
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on  **
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  */
}

void loop() {
  // picture loop
  u8g.firstPage();  
  u8g.setFont(u8g_font_unifont);
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}

