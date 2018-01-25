#define W_CLK 8                        // AD9850 Module pins.    
#define FQ_UD 7       
#define DATA  6       
#define RESET 5   

#define AD9850_CLOCK 125000000         // Module crystal frequency. Tweak here for accuracy.

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

#include "U8glib.h"
#include <Rotary.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC

String strfreq,kfreq;
unsigned long freq;         // Set initial frequency.
//long unsigned int freqOld = freq;

Rotary r = Rotary(2, 3);

void setup() {
  pinMode(FQ_UD, OUTPUT);              // Configure pins for output to AD9850 module.
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);

  Serial.begin(9600);
  freq = 10000;
  sendFrequency(freq);
	
  u8g.setFont(u8g_font_unifont); //oled
  
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
	
}

void loop() {
	/*if(Serial.available()>0){
		strfreq = Serial.readString();
		oled();
		freq = strfreq.toInt();
		Serial.print("freq : ");
		Serial.println(freq);
		sendFrequency(freq);
    }*/

	u8g.firstPage();
  do{  
    if(Serial.available()>0){
      strfreq = Serial.readString();
      //Serial.print("that is the message:");
      //Serial.println();
			freq = strfreq.toInt();
			Serial.println(freq);
			sendFrequency(freq);
			
    }
    u8g.setColorIndex(1);
    u8g.drawStr(0,22,"Signal generator");
    u8g.drawStr(0,55,"Hz : ");
		// if(freq > 1000){
			// Serial.println("999");
			// freq = freq/1000;
			// kfreq = String(freq) + "KHz";
			// u8g.drawStr(42,55,kfreq.c_str());*/
		// }else if(){
			
		// }else{
			 u8g.drawStr(42,55,strfreq.c_str());
		// }
		
  }while(u8g.nextPage());

  delay(50);
	
	//*****************************************************************//
	u8g.firstPage();
  do{  
    if(){
			sendFrequency(freq);
			strfreq = String(freq);
    }
    u8g.setColorIndex(1);
    u8g.drawStr(0,22,"Signal generator");
    u8g.drawStr(0,55,"Hz : ");
		u8g.drawStr(42,55,strfreq.c_str());
	}while(u8g.nextPage());

  delay(50);
	
	
}

void carry() {
	
}



void tfr_byte(byte data) {
  for (int i = 0; i < 8; i++, data >>= 1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}

void sendFrequency(double frequency) {
  int32_t freq1 = frequency * 4294967295/AD9850_CLOCK;  // note 125 MHz clock on 9850
  for (int b = 0; b < 4; b++, freq1 >>= 8) {
    tfr_byte(freq1 & 0xFF);
  }
  tfr_byte(0x000);                     // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);                    // Done!  Should see output
}

ISR(PCINT2_vect) {
  unsigned char result = r.process();
	//unsigned char result2 = rr.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW && freq < 62500000) {
    freq = freq + 1000;
    Serial.print("ClockWise:");
    Serial.println(freq);
  }
  else if (result == DIR_CCW && freq > 1) {
    freq = freq - 1000;
    Serial.print("CounterClockWise:");
    Serial.println(freq);
  }
}