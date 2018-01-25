#include <Rotary.h>

Rotary r = Rotary(2, 3);
//Rotary rr = Rotary(2, 4);
int x = 0;
void setup() {
  Serial.begin(9600);
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
}

void loop() {

}


ISR(PCINT2_vect) {
  unsigned char result = r.process();
	//unsigned char result2 = rr.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {
    x++;
    Serial.print("ClockWise:");
    Serial.println(x);
  }
  else if (result == DIR_CCW) {
    x--;
    Serial.print("CounterClockWise:");
    Serial.println(x);
  }
	/*if (result2 == DIR_NONE) {
    // do nothing
  }
  else if (result2 == DIR_CW) {
    x=x+10;
    Serial.print("ClockWise:");
    Serial.println(x);
  }
  else if (result2 == DIR_CCW) {
    x=x-10;
    Serial.print("CounterClockWise:");
    Serial.println(x);
  }*/
}
