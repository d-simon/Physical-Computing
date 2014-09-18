#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
#define PIN_DATA 0 // 0 = Serial Data Input 
#define PIN_OE 1// 1 = Output Enable
#define PIN_LATCH 2 // 2 = STCP storage register clock input LATCH
#define PIN_CLOCK 3 // 3 = SHCP shift register clock input CLOCK

void setup() {
	pinMode(PIN_DATA, OUTPUT);
	// pinMode(PIN_OE, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
}

void loop() {
	for (int i = 0; i < 256; i++) {
		digitalWrite(PIN_LATCH, LOW); // LATCH_PIN auf LOW = Beginn der Daten

		shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, i); // Daten shiften

		digitalWrite(PIN_LATCH, HIGH); // LATCH_PIN auf HIGH = LEDs leuchten

		delay(100);
	}
}
