#include "wss.h"

Preferences preferences;

void preferences_do_setup(void) {
	preferences.begin("wssPrefs", true);

	Serial.print("devLoc_bldgName: ");
	Serial.println(preferences.getString("devLoc_bldgName"));
	Serial.print("devLoc_roomName: ");
	Serial.println(preferences.getString("devLoc_roomName"));
	Serial.print("deviceID: ");
	Serial.println(preferences.getString("deviceID"));
	Serial.print("tempSensorPin: ");
	Serial.println(preferences.getString("tempSensorPin"));
	Serial.print("channel: ");
	Serial.println(preferences.getUInt("discordChannelID"));
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
	} /*else if (dataType == "uint32_t") {
		preferences.putUInt(keyName, keyVal);
	}*/ else preferences.putString(keyName, keyVal);
	Serial.print(keyName);
	Serial.print(" = ");
	Serial.print(keyVal);
	Serial.println(" ");
	preferences.end();

	if (keyName = "discordEnabled") {
		discord_check_if_enabled();
	}
}

bool preferences_is_wifi_configured(void) {
	bool ret = false;
	String ssid = preferences_get_string("wifiSSID");

	if (ssid && (ssid != "")) {
		if (preferences_get_string("wifiPass")) {
			ret = true;
		}
	}
	return ret;
}

bool preferences_check_discord_configured(void) {
	bool enabled = preferences_get_string("discordEnabled");
	String webhook = preferences_get_string("discordWebhook");

	if (enabled && (webhook != "")) {
		discord_set_configuration_status(true);
		return true;
	} else discord_set_configuration_status(false);

	return false;
}
