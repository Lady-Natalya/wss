#include "wss.h"

bool isPoweredLED1 = false;
uint32_t blinkLengthMillis = 200;
static uint32_t blinkStartMillis = 0;

bool LED_isPoweredLED1(void) {
	return isPoweredLED1;
}

void LED_blink(void) {
	blinkStartMillis = millis();
	digitalWrite(PINLED1, LOW);
	isPoweredLED1 = true;
}

void LED_turnOff(void) {
	if ((millis() - blinkStartMillis) > blinkLengthMillis) {
		digitalWrite(PINLED1, HIGH);
		isPoweredLED1 = false;
	}
}
