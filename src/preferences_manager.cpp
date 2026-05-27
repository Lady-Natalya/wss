#include "wss.h"

Preferences preferences;

void preferences_reload(void) {
	Serial.println("Loading Preferences...");
	if (wifi_setup()) {
		Serial.println("WiFi is Configured.  Attempting connection...");
		wifi_connection_begin();
		if (get_wifi_connection_status() == WIFIDISCONNECTEDSTRING) {
			Serial.println("WiFi is currently disconnected.");
		}
	}
	if (discord_setup()) {
		if (get_wifi_connected()) {
			Serial.println(F("Discord is set up and WiFi is connected."));
			discord_begin();
		} else {
			Serial.println(F("Discord is set up, but Wifi is disconnected.  Waiting for WiFi connection before starting Discord."));
		}

	}
	else Serial.println("Discord is Disabled.");
}

void preferences_do_setup(void) {
	preferences.begin("wssPrefs", true);

	preferences_reload();

	Serial.print("devLoc_bldgName: ");
	Serial.println(preferences.getString("devLoc_bldgName"));
	Serial.print("devLoc_roomName: ");
	Serial.println(preferences.getString("devLoc_roomName"));
	Serial.print("deviceID: ");
	Serial.println(preferences.getString("deviceID"));
	Serial.print("tempSensorPin: ");
	Serial.println(preferences.getString("tempSensorPin"));
	Serial.print("token: ");
	Serial.println(preferences.getString("discordToken"));

	preferences.end();
}

bool preferences_get_bool(char *keyName) {
	bool ret = false;
	preferences.begin("wssPrefs", true);
	ret = preferences.getBool(keyName);
	preferences.end();
	return ret;
}

String preferences_get_string(char *keyName) {
	String ret = "default";
	preferences.begin("wssPrefs", true);
	ret = preferences.getString(keyName);
	preferences.end();
	return ret;
}

uint32_t preferences_get_uint(char *keyName) {
	uint32_t ret = 0;
	preferences.begin("wssPrefs", true);
	ret = preferences.getUInt(keyName);
	preferences.end();
	return ret;
}

void preferences_update_server_pref(char *keyName, String keyVal, String dataType) {
	preferences.begin("wssPrefs", false);
	if (dataType == "bool") {
		if (keyVal == "true") preferences.putBool(keyName, true);
		else if (keyVal == "false") preferences.putBool(keyName, false);
	} else preferences.putString(keyName, keyVal);
	Serial.print(keyName);
	Serial.print(" = ");
	Serial.print(keyVal);
	Serial.println(" ");
	preferences.end();

	preferences_reload();
}
