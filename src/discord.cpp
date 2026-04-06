#include "wss.h"

Discord_Webhook discord;

bool discord_configured = false;
bool discord_enabled = false;	// Primary variable for Discord
bool discord_began = false;

uint32_t lastDiscordStatusMillis = 0; // TODO: Need to change how we decide when to send a status update.
static bool lastDiscordStatusFailed = false;

bool splitDiscordWebhook(const String& url, String& outId, String& outToken) {
	outId = "";
	outToken = "";
	if (url == "") return false;

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

bool discord_setup(void) {
	discord_enabled = preferences_get_bool("discordEnabled");
	discord_configured = false;

	String webhook = preferences_get_string("discordWebhook");
	String channel, token;
	if (splitDiscordWebhook(webhook, channel, token)) discord_configured = true;

	return (discord_enabled && discord_configured);
}

void discord_begin(void) {
	discord_began = false;
	if (discord_configured && discord_enabled) {
		String ssid = preferences_get_string("wifiSSID");
		String pass = preferences_get_string("wifiPass");
		String webhook = preferences_get_string("discordWebhook");
		String channel, token;
		if (splitDiscordWebhook(webhook, channel, token)) {
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
			if (discord.send(msg)) discord_began = true;	// If initial message gets sent then we record discord_began as true.
		}
	}
}

void discord_send_status(bool force_send) {
/*	Serial.print("discord_configured:" );
	Serial.print(discord_configured);
	Serial.print("  discord_enabled:");
	Serial.println(discord_enabled); */
	if (discord_configured && discord_enabled) {
		if ((force_send) || (millis() - lastDiscordStatusMillis) > DISCORDSTATUSDELAYMS) {
			lastDiscordStatusMillis = millis();
			if (!get_wifi_connected()) {
				Serial.println("discord_send_status: Error - WiFi is Disconnected");
				lastDiscordStatusFailed = true;
			}
			// To make a new line in Discord use "\\n"
			String msg = "Device: ";
			msg += preferences_get_string("deviceID");
			msg += "   Building: ";
			msg += preferences_get_string("devLoc_bldgName");
			msg += "   Room: ";
			msg += preferences_get_string("devLoc_roomName");
			msg += "   Temperature: ";
			msg += temperatureRead();
			msg.remove((msg.length() - 3) , 3);
			discord.send(msg);
			lastDiscordStatusFailed = false;
		}
	}
}

bool discord_get_last_status_failed(void) {
	return lastDiscordStatusFailed;
}
