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
	<h1">WSS Configuration Page</h1>
	<label for="wifissid">Select WiFi Network: </label>)=====";

const uint8_t config_wifi_select_html0[] = R"=====(<select name="wifissid" id="wifissid"  form="selections" >
	)=====";
const uint8_t config_wifi_select_html1[] = R"=====(</select><br>)=====";

const uint8_t config_body_html1[] = R"=====(

	<br><br>
	<form action="/post"  id="selections">
		<label for="wifipass">Wifi Password:</label>
		<input type="text" id="wifipass" name="wifipass"><br><br>
		<label for="device_building">Building Name:</label>
		<input type="text" id="device_building" name="device_building"><br><br>
		<label for="device_room">Room Name:</label>
		<input type="text" id="device_room" name="device_room"><br><br>
		<label for="device_id">Device ID:</label>
		<input type="text" id="device_id" name="device_id"><br><br>
		<input type="submit" value="Submit">
	</form>
	<br>
</body>
</html>
)=====";

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

	if (n > 0) {
		message += (char*)config_wifi_select_html0;
		for (int i = 0; i < n; ++i) {
			message += "<option value=\"";
			message += WiFi.SSID(i);
			message += "\">";
			message += WiFi.SSID(i);
			message += "</option>";
		}
		message += (char*)config_wifi_select_html1;
	} else {
		message += "<p>Error: No WiFi Networks Available</p>";
	}

	message += "<p></p>";
	message += (char*)config_body_html1;
	server.send(200, "text/html",  message);

	Serial.println(message);
	Serial.println("Root done");

}

// submit button pressed
void handlePost(void) {

//  ------------------- debug to client
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
  }
  message += "\n\n";
  server.send(200, "text/plain", message);
// ------------------------------------------------------------------

	preferences_update_server_args(server.arg(0), server.arg(1), server.arg(2), server.arg(3), server.arg(4));
	Serial.println("Submit done");

/*
	server.arg(0).toCharArray( MySettings.localSSID, 45);
	server.arg(1).toCharArray(MySettings.localpassword, 45) ;
	server.arg(2).toCharArray(MySettings.BoxUser, 45) ;
	server.sendHeader("Connection", "close");
	webserverrequired = false;
	Serial.println("Submit done");
	SaveSettings(true);*/
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
