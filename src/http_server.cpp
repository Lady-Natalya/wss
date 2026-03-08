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

const uint8_t config_wifi_select_html0[] = R"=====(<label for="wifissid">Select WiFi Network: </label>
<select name="wifissid" id="wifissid"  form="selections" >
	)=====";
const uint8_t config_wifi_select_html1[] = R"=====(</select><br>)=====";

const uint8_t config_body_html1[] = R"=====(
	<form action="/post"  id="selections">)=====";

const uint8_t config_close_param[] = R"=====("><br>)=====";
const uint8_t config_wifipass[] = R"=====(
	<label for="wifipass">Wifi Password:</label>
	<input type="text" id="wifipass" name="wifipass" placeholder=")=====";
const uint8_t config_bldg[] = R"=====(<label for="devLoc_bldgName">Building Name:</label>
	<input type="text" id="devLoc_bldgName" name="devLoc_bldgName" placeholder=")=====";
const uint8_t config_room[] = R"=====(<label for="devLoc_roomName">Room Name:</label>
	<input type="text" id="devLoc_roomName" name="devLoc_roomName" placeholder=")=====";
const uint8_t config_devid[] = R"=====(<label for="deviceID">Device ID:</label>
	<input type="text" id="deviceID" name="deviceID" placeholder=")=====";
const uint8_t config_probe_pin[] = R"=====(<br><label for="tempSensorPin">Temp Sensor Pin:</label>
	<input type="text" id="tempSensorPin" name="tempSensorPin" placeholder=")=====";

const uint8_t config_body_html2[] = R"=====(<br><input type="submit" value="Submit">
	</form>
	<br>
</body>
</html>
)=====";

const uint8_t return_home_html[] = R"=====(<a href="/">Return to Home</a>)=====";

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
	message += "&deg;C</span><br><br><br>";

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
	message += (char*)config_wifipass;
	message += preferences_get_string((char*)"wifipass");
	message += (char*)config_close_param;
	message += (char*)config_bldg;
	message += preferences_get_string((char*)"devLoc_bldgName");
	message += (char*)config_close_param;
	message += (char*)config_room;
	message += preferences_get_string((char*)"devLoc_roomName");
	message += (char*)config_close_param;
	message += (char*)config_devid;
	message += preferences_get_string((char*)"deviceID");
	message += (char*)config_close_param;
	message += (char*)config_probe_pin;
	message += preferences_get_string((char*)"tempSensorPin");
	message += (char*)config_close_param;

	message += (char*)config_body_html2;
	server.send(200, "text/html",  message);

	Serial.println(message);
	Serial.println("Root done");

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
		preferences_update_server_pref((char*)server.argName(i).c_str(), server.arg(i));
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
