

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

// #include <WiFiClientSecure.h>
// #include <SPI.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>            // Include NTPClient library
#include <TimeLib.h>              // Include Arduino time library
#include <WiFiUdp.h>

#ifndef STASSID
#define 	STASSID	"bilbo"
#define 	STAPSK	"readyplayer1"
#endif

#define HOST		"dev.weja.us"
#define PORT		5423
#define PODID		1
#define SLEEP		60000
#define SLEEPDOH	60000
#define DEBUG		false

WiFiUDP		ntpUDP;
NTPClient	timeClient(ntpUDP, "time.nist.gov", 3600, 60000);

String		deviceMac;
bool		debug		= DEBUG;
const char*	ssid		= STASSID;
const char*	pwd			= STAPSK;
const char*	hst			= HOST;
const int	prt			= PORT;
const int	podId		= PODID;
const int	sleep		= SLEEP;
const int	sleepDoh	= SLEEPDOH;
char		Time[]		= "00:00:00";
char		Date[]		= "00/00/2000";
int			year_;
byte		last_second,
			second_,
			minute_,
			hour_,
			day_,
			month_;

void getTimeDate() {
	timeClient.update();
	unsigned long unix_epoch	= timeClient.getEpochTime();
	second_						= second(unix_epoch);
	if ( last_second != second_ ) {
		minute_	= minute(unix_epoch);
		hour_	= hour(unix_epoch)-7;
		day_	= day(unix_epoch);
		month_	= month(unix_epoch);
		year_	= year(unix_epoch);
		Time[7]	= second_		% 10 + 48;
		Time[6]	= second_		/ 10 + 48;
		Time[4]	= minute_		% 10 + 48;
		Time[3]	= minute_		/ 10 + 48;
		Time[1]	= hour_			% 10 + 48;
		Time[0]	= hour_			/ 10 + 48;
		Date[0]	= month_		/ 10 + 48;
		Date[1]	= month_		% 10 + 48;
		Date[3]	= day_			/ 10 + 48;
		Date[4]	= day_			% 10 + 48;
		Date[8]	= (year_ / 10)	% 10 + 48;
		Date[9]	= year_	% 10	% 10 + 48;
	// Time[12]		= second_	% 10 + 48;
	// Time[11]		= second_	/ 10 + 48;
	// Time[9]		= minute_	% 10 + 48;
	// Time[8]		= minute_	/ 10 + 48;
	// Time[6]		= hour_		% 10 + 48;
	// Time[5]		= hour_		/ 10 + 48;
	// Date[5]		= month_	/ 10 + 48;
	// Date[6]		= month_	% 10 + 48;
	// Date[8]		= day_		/ 10 + 48;
	// Date[9]		= day_		% 10 + 48;
	// Date[13]	= (year_ / 10)	% 10 + 48;
	// Date[14]	= year_	% 10  	% 10 + 48;
	}
}


void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.println(ssid);
	WiFi.mode(WIFI_STA);			// limit access to only WiFi(no router)
	deviceMac = WiFi.macAddress();
	WiFi.begin(ssid, pwd);
	while ( WiFi.status() != WL_CONNECTED ) {
		delay(1000);
		Serial.print(".");
	}

	timeClient.begin();

	Serial.print("conn: ");
	Serial.println(WiFi.localIP());
	Serial.print("device: ");
	Serial.println(WiFi.macAddress());
}


void loop() {
	getTimeDate();

	Serial.print(Date);
	Serial.print(" ");
	Serial.print(Time);

	WiFiClient client;		// WiFiClientSecure client;	// Creates a TLS connection

	client.setTimeout( 10000 );
	if ( ! client.connect( hst, prt )) {
		Serial.println( "CONNDOH: TIMEOUT" );
		delay(sleepDoh);
		return;
	}
	if (debug) Serial.print("> Incoming message ...... ");

	client.print("GET /api/pod/");
	client.print( podId );
	client.print( "?device=" + deviceMac );
	client.println(" HTTP/1.1");
	client.print("Host: ");
	client.println(hst);
	client.print("Referer: ");
	client.println(deviceMac);
	client.println("Connection: close");

	if ( client.println() == 0 ) {
		Serial.println("SENDDOH");
		client.stop();
		delay(sleepDoh);
		return;
	}
	char status[32] = {0};										// Check HTTP status
	client.readBytesUntil( '\r', status, sizeof( status ) );
	if ( strcmp( status, "HTTP/1.1 200 OK" ) != 0 ) {
		Serial.print("RESPDOH: ");
		Serial.println(status);
		client.stop();
		delay(sleepDoh);
		return;
	}
	char endOfHeader[] = "\r\n\r\n";							// Skip headers
	if ( ! client.find( endOfHeader )) {
		Serial.println("PARSEDOH: Found no end-of-header sequence");
		client.stop();
		delay(sleepDoh);
		return;
	}
	DynamicJsonDocument doc( 1800 );	// BTW, arduinojson.org/v6/assistant way underestimated with 256
	DeserializationError error = deserializeJson(doc, client);
	if ( error ) {
		Serial.print("DESERIALDOH: ");
		Serial.println( error.f_str() );
		client.stop();
		delay(sleepDoh);
		return;
	}

	if (debug) Serial.println("RECIEVED");
	const JsonObject	podObj	= doc.as<JsonObject>();
	const int			pod		= podObj["pod"];
	if (debug) Serial.print("> Confirming relevancy...");

	if ( pod == podId ) {
		Serial.println(" FOUND: pod1");
		if (debug) Serial.print("POD1> Compiling zones... ");

		JsonObject zoneObj	= podObj["payload"].as<JsonObject>();

		if (
			zoneObj.containsKey("1") ||
			zoneObj.containsKey("2") ||
			zoneObj.containsKey("3") ||
			zoneObj.containsKey("4")
		) {

			Serial.println("done");
			if (zoneObj.containsKey("1")) {
				int duration = zoneObj["1"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE1> ON: ");
					Serial.println(duration);
					delay(duration);
					if (debug) Serial.println("POD1> ZONE1> OFF");
				}
			}
			if (zoneObj.containsKey("2")) {
				int duration = zoneObj["2"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE2> ON: ");
					Serial.println(duration);
					delay(duration);
					if (debug) Serial.println("POD1> ZONE2> OFF");
				}
			}
			if (zoneObj.containsKey("3")) {
				int duration = zoneObj["3"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE3> ON: ");
					Serial.println(duration);
					delay(duration);
					if (debug) Serial.println("POD1> ZONE3> OFF");
				}
			}
			if (zoneObj.containsKey("4")) {
				int duration = zoneObj["4"].as<int>();
				if (duration > 0) {
					Serial.print("POD1> ZONE4> ON: ");
					Serial.println(duration);
					delay(duration);
					if (debug) Serial.println("POD1> ZONE4> OFF");
				}
			}
		} else {
			Serial.println("POD1> No relevant zones... ignored");
			if (debug) Serial.println(" Reply contains a relevant pod, but no relevant zones... ignored");
			if (debug) Serial.println("POD1> No active zone instructions found...message ignored");
			if (debug) Serial.println("POD1> EXIT");
		}
	} else {
		Serial.println(" No pod1 instructions... ignored");
		if (debug) Serial.println(" Reply contains valid JSON, but no relevant pod... ignored");
		if (debug) Serial.println("POD1> Nothing relevant found... message ignored");
		if (debug) Serial.println("POD1> EXIT");
	}
	client.stop();
	Serial.println("--- ZzZzZzZzZzZzZzZzZz ---");
	delay(sleep);
}

