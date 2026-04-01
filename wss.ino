// Wireless Sensor Systems
#include "src/wss.h"

uint32_t frameDelay = 5000;
uint32_t lastMillis = 0;

void setup() {
	Serial.begin(115200);

	preferences_do_setup();
	gpio_begin();
	http_server_begin();
	if (preferences_is_wifi_configured()) wifi_connection_begin();
	if (preferences_check_discord_configured()) discord_check_if_enabled();
}

void doFrame() {
	Serial.print(String(temperatureRead()));
	Serial.print("\n");

	lastMillis = millis();
	LED_blink();
	autoprint_wifi_connection_status();
	discord_send_status();
}

void loop() {
	if (LED_isPoweredLED1()) LED_turnOff();
	if ((millis() - lastMillis) > frameDelay) doFrame();

	http_server_handler();
}
