#include "wss.h"

const char *ssid = "wss_esp32-c3-sm";
WebServer server(80);

const uint8_t config_head_html0[] = R"=====(<!DOCTYPE html>
<html>
<head>
)=====";
const uint8_t config_title_html0[] = R"=====(	<title>Wireless Sensor Systems&trade; V )=====";
const uint8_t config_title_html1[] = R"=====(</title>
)=====";
const uint8_t config_style_html[] = R"=====(	<style>
		body {
			/* background: #000000; */
			background-image: linear-gradient(45deg, #030 5.88%, #000 5.88%, #000 50%, #030 55.88%, #000 55.88%, #000 100%);
			background-size: 17px 17px;
			font-family: "Georgia", serif;
			Color: #33FF33 ;
			font-size: 1.25rem;
		}
		input {
			background: #DDD;
			border: 2px solid #33FF33;
			border-radius: 4px;
		}
	</style>
)=====";
const uint8_t config_head_html1[] = R"=====(</head>)=====";

const uint8_t config_body_html0[] = R"=====(<body>
	<h1">WSS Configuration Page</h1><br><br>)=====";

const uint8_t status_title_html[] = R"=====(
	<div>Device Status:</div>
)=====";

const uint8_t config_wifi_select_html0[] = R"=====(<label for="wifiSSID">Select WiFi Network: </label>
<select name="wifiSSID" id="wifiSSID"  form="selections" >
	)=====";
const uint8_t config_wifi_select_html1[] = R"=====(</select><br>)=====";

const uint8_t config_body_html1[] = R"=====(
	<form action="/post"  id="selections">)=====";

const uint8_t config_close_param[] = R"=====("><br>)=====";

const uint8_t config_email_enabled[] = R"=====(<br><label for="email_enabled">Enable SMTP Email Alerts: </label>
	<select name="email_enabled" id="email_enabled"  form="selections" >
		<option value="unset">unset</true>
		<option value="true">enable</true>
		<option value="false">disable</true>
	</select><br>)=====";

const uint8_t config_body_html2[] = R"=====(<br><input type="submit" value="Submit">
	</form>
	<br>
</body>
</html>
)=====";

const uint8_t return_home_html[] = R"=====(<a href="/">Return to Home</a>)=====";


String add_config_page_param(String paramName, String paramLabelText, bool usePlaceholder) {
	String ret = "";
	ret += "<label for=\"";
	ret += paramName;
	ret += "\">";
	ret += paramLabelText;
	ret += ":</label>\n	<input type=\"text\" id=\"";
	ret += paramName;
	ret += "\" name=\"";
	ret += paramName;
	ret += "\"";
	if (usePlaceholder) {
		ret += " placeholder=\"";
		ret += preferences_get_string((char*)paramName.c_str());
		ret += "\"";
	}
	ret += (char*)config_close_param;
	return ret;
}


// root of the intial web page
void handleRoot(void) {
	int n = WiFi.scanNetworks();

	String message="";
	message += (char*)config_head_html0;
	message += (char*)config_title_html0;
	message += FW_VERSION;
	message += (char*)config_title_html1;
	message += (char*)config_style_html;
	message += (char*)config_head_html1;
	message += (char*)config_body_html0;

	message += (char*)status_title_html;
	message += "<span>Temperature: ";
	message += temperatureRead();
	message += "&deg;C</span><br>";
	message += "<span>WiFi Status: ";
	message += get_wifi_connection_status();
	message += "</span><br><br><br>";

	if (n > 0) {
		message += (char*)config_wifi_select_html0;
		message += "<option value=\"unset\">&nbsp;</option>";
		for (int i = 0; i < n; ++i) {
			message += "<option value=\"";
			message += WiFi.SSID(i);
			message += "\">";
			message += WiFi.SSID(i);
			message += "</option>";
		}
		message += "<option value=\"null\">FORGET WIFI NETWORK -- WILL NEED TO REBOOT</option>";
		message += (char*)config_wifi_select_html1;
	} else {
		message += "<p>Error: No WiFi Networks Available</p>";
	}

	message += "<p></p>";
	message += (char*)config_body_html1;
	message += add_config_page_param("wifiPass", "WiFi Password", true);
	message += add_config_page_param("devLoc_bldgName", "Building Name", true);
	message += add_config_page_param("devLoc_roomName", "Room Name", true);
	message += add_config_page_param("deviceID", "Device ID", true);
	message += add_config_page_param("tempSensorPin", "Temp Sensor Pin", true);
	message += (char*)config_email_enabled;
	message += add_config_page_param("smtpHost", "SMTP Host", true);
	message += add_config_page_param("smtpPort", "SMTP Port", true);
	message += add_config_page_param("smtpSender", "Sender Address", true);
	message += add_config_page_param("smtpSenderPass", "Sender Pass", false);
	message += add_config_page_param("smtpRecipient", "Recipient Address", true);

	message += (char*)config_body_html2;
	server.send(200, "text/html",  message);

	Serial.println(message);
	Serial.println("handleRoot complete");

}

// submit button pressed
void handlePost(void) {

	String message = "SUBMITTED\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\n\n There are ";
	message += server.args();
	message += " variables passed ";
	message += "\n\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
		if (server.arg(i) != "") {
			if (server.arg(i) == "unset") {
				continue;	// Ignore non-selections for <select> drop-downs.
			}
			preferences_update_server_pref((char*)server.argName(i).c_str(), server.arg(i));
		}
	}
	message += "\n\n";
	//message += (char*)return_home_html;
	server.send(200, "text/plain", message);
	Serial.println("Submit done");

}

//****************HANDLE NOT FOUND*********************//
void handleNotFound(void)
{
	String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "<H2><a href=\"/\">go home</a></H2><br>";
  server.send(404, "text/plain", message);
}

void http_server_begin(void) {
	// Setup the web server
	WiFi.softAP(ssid, "");
	server.on("/", handleRoot);
	server.on("/post", handlePost);
	server.onNotFound(handleNotFound);
	server.begin();
	Serial.println("HTTP server started");
}

void http_server_handler(void) {
	server.handleClient();
	delay(2);
}
