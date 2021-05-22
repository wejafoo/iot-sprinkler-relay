

// AT Commands for quad relay
// Open     relay 1:    A0 01 01 A2
// Close    relay 1:    A0 01 00 A1
// Open     relay 2:    A0 02 01 A3
// Close    relay 2:    A0 02 00 A2
// Open     relay 3:    A0 03 01 A4
// Close    relay 3:    A0 03 00 A3
// Open     relay 4:    A0 04 01 A5
// Close    relay 4:    A0 04 00 A4

/*
    HTTP over TLS (HTTPS) example sketch
    This example demonstrates how to use WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of esp8266/Arduino project continuous integration build.
*/

// #include <Arduino.h>
// #include <WiFiClientSecure.h>
// #include <SPI.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID		"bilbo";
#define STAPSK		"readyplayer1";
#define HOST		"dev.weja.us";
#define PODID		1;
#define SLEEP		60000;
#define SLEEPDOH	600000;
#endif

const char*	ssid		= STASSID;
const char*	pwd			= STAPSK;
const char*	hst			= HOST;
const int	podId		= PODID;
const int	sleep		= SLEEP;
const int	sleepDoh	= SLEEPDOH;

void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.println(ssid);
	WiFi.mode(WIFI_STA);			// limit access to only WiFi(no router)
	WiFi.begin(ssid, pwd);

	while ( WiFi.status() != WL_CONNECTED ) {
		delay(1000);
		Serial.print(".");
	}

	Serial.print("conn:");
	Serial.println( WiFi.localIP() );

	configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");		// Set time via NTP, as required for x.509 validation
	Serial.print("Sync-ing time(NTP)");
	time_t now = time(nullptr);
	while ( now < 8 * 3600 * 2 ) {
		delay(500);
		Serial.print(".");
		now = time(nullptr);
	}
	struct tm timeinfo;
	gmtime_r( &now, &timeinfo );
	Serial.print( "Last booted: ");
	Serial.print( asctime( &timeinfo ));
}

void loop() {

	Serial.println("> RESET ZONE: 1 ..........done");
	Serial.println("> RESET ZONE: 2 ..........done");
	Serial.println("> RESET ZONE: 3 ..........done");
	Serial.println("> RESET ZONE: 4 ..........done");

	WiFiClient client;			// WiFiClientSecure client;		// Use WiFiClientSecure class to create TLS connection

	client.setTimeout( 100000 );
	if ( ! client.connect( hst, 4000 )) {
		Serial.println( "CONNDOH: TIMEOUT" );
		delay(sleepDoh);
		return;
	}

	Serial.print("> Incoming message .......");

	client.print("GET /api/pod/");
	client.print( podId );
	client.println(" HTTP/1.0");
	client.print("Host: ");
	client.println(hst);
	client.println("Connection: close");

	if ( client.println() == 0 ) {
		Serial.println("SENDDOH");
		client.stop();
		return;
	}

	char status[32] = {0};																// Check HTTP status
	client.readBytesUntil( '\r', status, sizeof( status ) );
	if ( strcmp( status, "HTTP/1.1 200 OK" ) != 0 ) {
		Serial.print("RESPDOH: ");
		Serial.println(status);
		client.stop();
		return;
	}

	char endOfHeader[] = "\r\n\r\n";													// Skip headers
	if ( ! client.find( endOfHeader )) {
		Serial.println("PARSEDOH: Found no end-of-header sequence");
		client.stop();
		return;
	}

	DynamicJsonDocument doc( 1800 );													// arduinojson.org/v6/assistant way underestimated with 256
	DeserializationError error = deserializeJson(doc, client);
	if ( error ) {
		Serial.print("DESERIALDOH: ");
		Serial.println( error.f_str() );
		client.stop();
		return;
	}

	Serial.println("RECIEVED");

	const JsonObject	podObj	= doc.as<JsonObject>();
	const int			pod		= podObj["pod"];

	Serial.print("> Confirming relevancy...");

	if ( pod == podId ) {
		Serial.println("CONFIRMED: POD1");
		Serial.print("POD1> Compiling zones...");

		JsonObject zoneObj	= podObj["payload"].as<JsonObject>();

		if ( zoneObj.containsKey("1") || zoneObj.containsKey("2") ||
			 zoneObj.containsKey("3") || zoneObj.containsKey("4")) {

			Serial.println("done");

			if (zoneObj.containsKey("1")) {
				int duration = zoneObj["1"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE1> ON: ");
					Serial.println(duration);
					delay(duration);
					Serial.println("POD1> ZONE1> OFF");
				}
			}

			if (zoneObj.containsKey("2")) {
				int duration = zoneObj["2"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE2> ON: ");
					Serial.println(duration);
					delay(duration);
					Serial.println("POD1> ZONE2> OFF");
				}
			}

			if (zoneObj.containsKey("3")) {
				int duration = zoneObj["3"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE3> ON: ");
					Serial.println(duration);
					delay(duration);
					Serial.println("POD1> ZONE3> OFF");
				}
			}

			if (zoneObj.containsKey("4")) {
				int duration = zoneObj["4"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE4> ON: ");
					Serial.println(duration);
					delay(duration);
					Serial.println("POD1> ZONE4> OFF");
				}
			}
		} else {
			Serial.println("NONE");
			Serial.println("POD1> No active zone instructions found...message ignored");
			Serial.println("POD1> EXIT");
		}
	} else {
		Serial.println("EMPTY");
		Serial.println("POD1> No instructions were found relevant to this pod...message ignored");
		Serial.println("POD1> EXIT");
	}

	client.stop();
	Serial.println("... ZzZzZzZzZzZzZzZzZz ...");
	delay(sleep);
}
