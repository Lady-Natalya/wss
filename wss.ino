// Wireless Sensor Systems
#include "src/wss.h"

uint32_t frameDelay = 5000;
uint32_t lastMillis = 0;

void setup() {
	Serial.begin(115200);
	Serial.println(F("#### Wireless Sensor Systems ####"));
	Serial.print(F("Version: "));
	Serial.print(FW_VERSION);
	Serial.print(F("  Date: "));
	Serial.println(BUILD_DATE);
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
	if (get_wifi_connected() && !get_discord_began()) discord_retry_begin();
	if (get_wifi_connected() && get_discord_began()) { discord_send_status(false); }
}

void loop() {
	if (LED_isPoweredLED1()) LED_turnOff();
	if ((millis() - lastMillis) > frameDelay) doFrame();

	http_server_handler();
}
