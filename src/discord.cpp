#include "wss.h"

Discord_Webhook discord;

bool discord_configured = false;
bool discord_enabled = false;
bool discord_began = false;
uint32_t lastDiscordStatusMillis = 0;

void discord_set_configuration_status(bool configured) {
	discord_configured = configured;
	if (configured) {
		Serial.println("Discord is enabled and has been configured.");
	} else Serial.println("Discord is disabled.");
}

void discord_check_if_enabled(void) {
	Serial.print("discord_check_if_enabled: ");
	discord_enabled = preferences_get_bool("discordEnabled");
	Serial.println(discord_enabled);
	if ((discord_enabled) && (discord_began == false)) discord_begin();
}

bool splitDiscordWebhook(const String& url, String& outId, String& outToken) {
  outId = "";
  outToken = "";

  int p = url.indexOf("/webhooks/");
  if (p < 0) return false;

  int start = p + String("/webhooks/").length();
  int slash = url.indexOf('/', start);
  if (slash < 0) return false;

  outId = url.substring(start, slash);
  outToken = url.substring(slash + 1);

  outId.trim();
  outToken.trim();

  return (outId.length() > 0 && outToken.length() > 0);
}

void discord_begin(void) {
	if (discord_configured && discord_enabled) {
		String ssid = preferences_get_string("wifiSSID");
		String pass = preferences_get_string("wifiPass");
		String webhook = preferences_get_string("discordWebhook");
		String channel, token;
		if (splitDiscordWebhook) {
			Serial.println("discord_begin");
			Serial.println("Channel: ");
			Serial.print(channel);
			Serial.print("  Token: ");
			Serial.print(token);
			discord.begin(channel, token);
			discord.addWiFi((char*)ssid.c_str(), (char*)pass.c_str());
			discord.connectWiFi();

			String msg = "Device: ";
			msg += preferences_get_string("deviceID");
			msg += " in building: ";
			msg += preferences_get_string("devLoc_bldgName");
			msg += "  with room name: ";
			msg += preferences_get_string("devLoc_roomName");
			msg += " is now online.\\nTemperature: ";
			msg += temperatureRead();
			msg.remove((msg.length() - 3) , 3);
			discord.send(msg);
			discord_began = true;			
		}
	}
}

void discord_send_status(void) {
/*	Serial.print("discord_configured:" );
	Serial.print(discord_configured);
	Serial.print("  discord_enabled:");
	Serial.println(discord_enabled); */
	if (discord_configured && discord_enabled) {
		if ((millis() - lastDiscordStatusMillis) > DISCORDSTATUSDELAYMS) {
			lastDiscordStatusMillis = millis();
			String msg = "Device: ";
			msg += preferences_get_string("deviceID");
			msg += "\\nBuilding: ";
			msg += preferences_get_string("devLoc_bldgName");
			msg += "\\nRoom: ";
			msg += preferences_get_string("devLoc_roomName");
			msg += "\\nTemperature: ";
			msg += temperatureRead();
			msg.remove((msg.length() - 3) , 3);
			discord.send(msg);
		}
	}
}
