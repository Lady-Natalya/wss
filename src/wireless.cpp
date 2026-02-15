//WiFiServer server(80);

void wifi_begin(void) {
	//WiFi.mode(WIFI_AP);
	WiFi.softAP(WIFI_DEFAULT_SSID, WIFI_DEFAULT_PASS);

	IPAddress IP = WiFi.softAPIP();

	Serial.print("Access Point IP address: ");
	Serial.println(IP);
}
