#include "wss.h"

bool wifi_connected = false;
uint32_t lastWiFiStatusMillis = 0;

void wifi_connection_begin(void){
	String ssid = preferences_get_string("wifiSSID");
	String pass = preferences_get_string("wifiPass");

	if ((ssid) && (ssid != "null")) {
		if (pass) {
			WiFi.mode(WIFI_STA);
			WiFi.begin(ssid, pass);
			Serial.print("Connecting to WiFi network: " + ssid + "\n");
		}
	} else Serial.print("wifi_connection_begin: No WiFi network selected yet.");
}

String get_wifi_connection_status(void){
	String ret = "WiFi Disconnected";
	if (WiFi.status() == WL_CONNECTED) {
		ret = "WiFi Connected.  Local IP: ";
		ret += WiFi.localIP().toString();
	}
	return ret;
}

void autoprint_wifi_connection_status(void){
	if ((millis() - lastWiFiStatusMillis) > WIFISTATUSDELAYMS) {
		lastWiFiStatusMillis = millis();
		if (WiFi.status() == WL_CONNECTED) {
			Serial.print("WiFi Connected.  Local IP: ");
			Serial.print(WiFi.localIP().toString());
			Serial.print("\n");
		} else	Serial.println("WiFi is Disconnected");
	}
}
