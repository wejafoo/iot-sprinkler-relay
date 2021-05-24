

// NOTE: guh -- https://forum.arduino.cc/t/pyserial-and-esptools-directory-error/671804/5
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define relay_on LOW
#define relay_off HIGH
#define relay_pin_1 D1                   // Relay #1
#define relay_pin_2 D2                   // Relay #2
#define relay_pin_3 D5                   // Relay #3
#define relay_pin_4 D6                   // Relay #4
#define led_on LOW                       // LED
#define led_off HIGH
#define led_pin 16
#define ssid "bilbo"                     // WiFi
#define password "readyplayer1"

ESP8266WebServer server(80);

void ReturnStatus() {
    bool led_is_on    = digitalRead(led_pin)     == led_on;
    Serial.printf( "LED is on: %s", led_is_on       ?"TRUE": "FALSE" );
    bool zone1_is_on  = digitalRead(relay_pin_1) == relay_on;
    Serial.printf( " -- Zone #1 is %s", zone1_is_on ?"TRUE": "FALSE" );
    bool zone2_is_on  = digitalRead(relay_pin_2) == relay_on;
    Serial.printf( " -- Zone #2 is %s", zone2_is_on ?"TRUE": "FALSE" );
    bool zone3_is_on  = digitalRead(relay_pin_3) == relay_on;
    Serial.printf( " -- Zone #3 is %s", zone3_is_on ?"TRUE": "FALSE" );
    bool zone4_is_on  = digitalRead(relay_pin_4) == relay_on;
    Serial.printf( " -- Zone #4 is %s", zone4_is_on ?"TRUE": "FALSE" );

    // If you want one, you can write it yourself. Here's what you need to think about:
    // - Who is going to allocate the storage for the returned string?
    // - Who is going to free the storage for the returned string?
    // - Is it going to be thread-safe or not?
    // - Is there going to be a limit on the maximum length of the returned string or not?

    char statusReturn[800];
    int truncCount = snprintf(statusReturn, 800, "{led: [led1:%s], pod1: [zone1:%s, zone2:%s, zone3:%s, zone4:%s], pod2: [zone1:%s, zone2:%s, zone3:%s, zone4:%s]}",
        led_is_on?"1": "0", zone1_is_on?"1": "0", zone2_is_on?"1": "0", zone3_is_on?"1": "0", zone4_is_on?"1": "0", zone1_is_on?"1": "0", zone2_is_on?"1": "0", zone3_is_on?"1": "0", zone4_is_on?"1": "0"
    );

    Serial.println("Truncation count: " + String(truncCount));

    server.send(200,"text/plain", statusReturn);
}

void SetPin(byte pin_number, byte new_pin_state) {
    digitalWrite(pin_number, new_pin_state);
    Serial.print("\nSetting pin#");
    Serial.print(pin_number);
    Serial.print(" to: ");
    Serial.println(new_pin_state);
    ReturnStatus();
}

void setup() {
    pinMode(relay_pin_1, OUTPUT);
    pinMode(relay_pin_2, OUTPUT);
    pinMode(relay_pin_3, OUTPUT);
    pinMode(relay_pin_4, OUTPUT);
    SetPin(relay_pin_1, relay_off);
    SetPin(relay_pin_2, relay_off);
    SetPin(relay_pin_3, relay_off);
    SetPin(relay_pin_4, relay_off);

    Serial.begin(115200);
    pinMode(led_pin, OUTPUT);

    Serial.println();
    Serial.println("Configuring access point...");
    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED ) {
        delay(500);
        Serial.print(".");
    }
    Serial.println( "WiFi connected!");
    Serial.print("IP address: \n");
    Serial.println(WiFi.localIP());

    server.on("/stat"                   , []() { ReturnStatus();                  });
    server.on("/led/1"                  , []() { SetPin(led_pin,     led_on    ); });
    server.on("/led/0"                  , []() { SetPin(led_pin,     led_off   ); });
    server.on("/pod/1/zone/1/action/1"  , []() { SetPin(relay_pin_1, relay_on  ); });
    server.on("/pod/1/zone/1/action/0"  , []() { SetPin(relay_pin_1, relay_off ); });
    server.on("/pod/1/zone/2/action/1"  , []() { SetPin(relay_pin_2, relay_on  ); });
    server.on("/pod/1/zone/2/action/0"  , []() { SetPin(relay_pin_2, relay_off ); });
    server.on("/pod/1/zone/3/action/1"  , []() { SetPin(relay_pin_3, relay_on  ); });
    server.on("/pod/1/zone/3/action/0"  , []() { SetPin(relay_pin_3, relay_off ); });
    server.on("/pod/1/zone/4/action/1"  , []() { SetPin(relay_pin_4, relay_on  ); });
    server.on("/pod/1/zone/4/action/0"  , []() { SetPin(relay_pin_4, relay_off ); });
    server.on("/pod/2/zone/1/action/1"  , []() { SetPin(relay_pin_1, relay_on  ); });
    server.on("/pod/2/zone/1/action/0"  , []() { SetPin(relay_pin_1, relay_off ); });
    server.on("/pod/2/zone/2/action/1"  , []() { SetPin(relay_pin_2, relay_on  ); });
    server.on("/pod/2/zone/2/action/0"  , []() { SetPin(relay_pin_2, relay_off ); });
    server.on("/pod/2/zone/3/action/1"  , []() { SetPin(relay_pin_1, relay_on  ); });
    server.on("/pod/2/zone/3/action/0"  , []() { SetPin(relay_pin_1, relay_off ); });
    server.on("/pod/2/zone/4/action/1"  , []() { SetPin(relay_pin_2, relay_on  ); });
    server.on("/pod/2/zone/4/action/0"  , []() { SetPin(relay_pin_2, relay_off ); });
    Serial.print("HTTP server starting..." );
    server.begin();
    Serial.print("done" );
}

void loop() { server.handleClient(); }
