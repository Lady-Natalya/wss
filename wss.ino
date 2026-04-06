// Wireless Sensor Systems
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
	Serial.println(String(temperatureRead()));

	lastMillis = millis();
	LED_blink();
	autoprint_wifi_connection_status();
	if (!get_wifi_connected() && get_wifi_configured()) wifi_retry_connection();
	discord_send_status(false);
}

void loop() {
	if (LED_isPoweredLED1()) LED_turnOff();
	if ((millis() - lastMillis) > frameDelay) doFrame();

	http_server_handler();
}
