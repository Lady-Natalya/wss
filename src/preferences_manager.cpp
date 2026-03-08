#include "wss.h"

Preferences preferences;

void preferences_do_setup(void) {
	preferences.begin("wssPrefs", true);

	Serial.print("wifiSSID: ");
	Serial.println(preferences.getString("wifiSSID"));
	Serial.print("wifiPass: ");
	Serial.println(preferences.getString("wifiPass"));
	Serial.print("devLoc_bldgName: ");
	Serial.println(preferences.getString("devLoc_bldgName"));
	Serial.print("devLoc_roomName: ");
	Serial.println(preferences.getString("devLoc_roomName"));
	Serial.print("deviceID: ");
	Serial.println(preferences.getString("deviceID"));

	preferences.end();
}

String preferences_get_string(char *keyName) {
	String ret = "default";
	preferences.begin("wssPrefs", true);
	ret = preferences.getString(keyName);
	preferences.end();
	return ret;
}

void preferences_update_server_pref(char *keyName, String keyVal) {
	preferences.begin("wssPrefs", false);
	preferences.putString(keyName, keyVal);
	Serial.print(keyName);
	Serial.print(" = ");
	Serial.print(keyVal);
	Serial.println(" ");
	preferences.end();
}
