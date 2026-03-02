// Wireless Sensor System
#include "src/wss.h"

uint32_t frameDelay = 5000;
uint32_t lastMillis = 0;

void setup() {
	Serial.begin(115200);

	preferences_do_setup();
	gpio_begin();
	http_server_begin();
}

void doFrame() {
	Serial.print(String(temperatureRead()));
	Serial.print("\n");

	lastMillis = millis();
	LED_blink();
}

void loop() {
	if (LED_isPoweredLED1()) LED_turnOff();
	if ((millis() - lastMillis) > frameDelay) doFrame();

	http_server_handler();
}
