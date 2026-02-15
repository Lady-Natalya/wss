// Wireless Sensor System
#include "src/wss.h"
#include <WiFi.h>
#include "WiFi.h"

#if defined(ARDUINO_SUPER_MINI_ESP32C3)
  static uint8_t pinLED1 = BUILTIN_LED;   // LED connected to digital pin
#else
  static uint8_t pinLED1 = 8;             // LED connected to digital pin
#endif

uint8_t frameDelay = 60000;
uint32_t lastMillis = 0;
bool isPoweredLED1 = false;
static uint8_t ledOn = LOW;

void setup() {
	pinMode(pinLED1, OUTPUT);
	Serial.begin(115200);


}

void doFrame() {
	Serial.print("\n");
	Serial.print(String(temperatureRead()));
	if (isPoweredLED1) {
		digitalWrite(pinLED1, LOW);
		isPoweredLED1 = false;
	} else {
		digitalWrite(pinLED1, HIGH);
		isPoweredLED1 = true;
	}
	lastMillis = millis();
}

void loop() {
	if (millis() - lastMillis > frameDelay) doFrame();
}
