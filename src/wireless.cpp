//WiFiServer server(80);
#include <WiFi.h>
#include "WiFi.h"

void wifi_begin(void) {
	#include <WiFi.h>
	#include "WiFi.h"
	//WiFi.mode(WIFI_AP);
	WiFi.softAP(WIFI_DEFAULT_SSID, WIFI_DEFAULT_PASS);

	IPAddress IP = WiFi.softAPIP();

	Serial.print("Access Point IP address: ");
	Serial.println(IP);
}
