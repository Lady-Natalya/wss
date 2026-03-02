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

void preferences_update_server_args(String arg0, String arg1, String arg2, String arg3, String arg4) {
	preferences.begin("wssPrefs", false);
	preferences.putString("wifiSSID", arg0);
	preferences.putString("wifiPass", arg1);
	preferences.putString("devLoc_bldgName", arg2);
	preferences.putString("devLoc_roomName", arg3);
	preferences.putString("deviceID", arg4);
	preferences.end();
}
