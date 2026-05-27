#include "wss.h"

bool wifi_configured = false;
bool wifi_connected = false;
uint32_t lastWiFiStatusMillis = 0;
uint32_t lastWiFiRetryMillis = 0;

String wifiSSID = "";
String wifiPass = "";

bool wifi_setup(void) {
	wifi_configured = false;
	wifi_connected = false;
	wifiSSID = preferences_get_string("wifiSSID");
	wifiPass = preferences_get_string("wifiPass");
	if ((wifiSSID) && (wifiSSID != "") && wifiPass) {
		wifi_configured = true;
		//WiFi.mode(WIFI_MODE_NULL);	// Resetting WiFi in case it was already running.
	}
	return wifi_configured;
}

void wifi_connection_begin(void){
	if (!wifi_configured) { Serial.println("WiFi not configured.  Aborting connection attempt."); return; }
	else Serial.println("wifi_connection_begin -- searching for network \"" + wifiSSID + "\"");
	int n = WiFi.scanNetworks();
	Serial.println("WiFi.scanNetworks()");
	delay(500);
	if (n == WIFI_SCAN_FAILED) Serial.println("Scan Failed!");
	if (n == WIFI_SCAN_RUNNING) Serial.println("Scan Running...");
	if (n == 0) {
		Serial.print(F("Scan found "));
		Serial.print(n);
		Serial.println(F(" networks."));
	}
	if (n > 0) {
		for (int i = 0; i < n; ++i) {
			Serial.println("Detected network: " + WiFi.SSID(i));
			if (wifiSSID == WiFi.SSID(i)){
				Serial.print("Selected WiFi network \"" + wifiSSID + "\" is available.  Attempting connection.\n");
				WiFi.begin(wifiSSID, wifiPass);
				if (discord_get_last_status_failed()) discord_send_status(true);
				return;
			}
		}
		Serial.print("Error: Unable to find selected WiFi Network: \"" + wifiSSID + "\"\n");
	}
	wifi_connected = false;
	WiFi.mode(WIFI_AP);
	http_server_begin();
}

bool get_wifi_configured(void) { return wifi_configured; }

bool get_wifi_connected(void) { return wifi_connected; }

String get_wifi_connection_status(void){
	String ret = WIFIDISCONNECTEDSTRING;
	if (WiFi.status() == WL_CONNECTED) {
		wifi_connected = true;
		ret = "WiFi Connected.  Local IP: ";
		ret += WiFi.localIP().toString();
	} else wifi_connected = false;
	return ret;
}

void autoprint_wifi_connection_status(void){
	if ((millis() - lastWiFiStatusMillis) > WIFISTATUSDELAYMS) {
		lastWiFiStatusMillis = millis();
		if (WiFi.status() == WL_CONNECTED) {
			Serial.print("WiFi Connected.  Local IP: ");
			Serial.print(WiFi.localIP().toString());
			Serial.print("\n");
		} else	Serial.println("WiFi is Disconnected.");
	}
}

void wifi_retry_connection(void) {
	if (!wifi_configured) return;
	if (WiFi.status() == WL_CONNECTED) { wifi_connected = true; return; }

	if ((millis() - lastWiFiRetryMillis) > WIFIRETRYCONNECTIONDELAYMS) {
		lastWiFiRetryMillis = millis();
		Serial.println("Attempting to connect to WiFi...");
		wifi_connection_begin();
	}
}
