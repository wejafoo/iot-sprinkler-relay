
// NOTE: guh -- https://forum.arduino.cc/t/pyserial-and-esptools-directory-error/671804/5

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define relay_on LOW
#define relay_off HIGH
#define relay_pin_1 D1                                      // Relay #1
#define relay_pin_2 D2                                      // Relay #2
#define relay_pin_3 D5                                      // Relay #3
#define relay_pin_4 D6                                      // Relay #4
#define led_on LOW                                          // LED
#define led_off HIGH
#define led_pin 16

#define ssid "bilbo"                                        // WiFi
#define password "readyplayer1"

ESP8266WebServer server(80);


void setup() {
  
  pinMode(relay_pin_1, OUTPUT);                              // Relay #1
  pinMode(relay_pin_2, OUTPUT);                              // Relay #2
  pinMode(relay_pin_3, OUTPUT);                              // Relay #3
  pinMode(relay_pin_4, OUTPUT);                              // Relay #4
    
  SetPin(relay_pin_1, relay_off);
  SetPin(relay_pin_2, relay_off);
  SetPin(relay_pin_3, relay_off);
  SetPin(relay_pin_4, relay_off);
  
  Serial.begin(115200);                                       // start console
  
  pinMode(led_pin, OUTPUT);                                   // LED
  
  Serial.println();                                           // Start WiFi
  Serial.println("Configuring access point...");
  
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println( "WiFi connected!" );
  Serial.print(   "IP address: \n"  );
  Serial.println(  WiFi.localIP()   );

  server.on("/"                      , []() { RerenderButtons();                   });
  server.on("/led/1"                 , []() { SetPin(led_pin,       led_on      ); });
  server.on("/led/0"                 , []() { SetPin(led_pin,       led_off     ); });
  server.on("/pod/1/zone/1/action/1" , []() { SetPin(relay_pin_1,   relay_on    ); });
  server.on("/pod/1/zone/1/action/0" , []() { SetPin(relay_pin_1,   relay_off   ); });
  server.on("/pod/1/zone/2/action/1" , []() { SetPin(relay_pin_2,   relay_on    ); });
  server.on("/pod/1/zone/2/action/0" , []() { SetPin(relay_pin_2,   relay_off   ); });
  server.on("/pod/1/zone/3/action/1" , []() { SetPin(relay_pin_3,   relay_on    ); });
  server.on("/pod/1/zone/3/action/0" , []() { SetPin(relay_pin_3,   relay_off   ); });
  server.on("/pod/1/zone/4/action/1" , []() { SetPin(relay_pin_4,   relay_on    ); });
  server.on("/pod/1/zone/4/action/0" , []() { SetPin(relay_pin_4,   relay_off   ); });
  Serial.print("JSON API server starting..." );
  server.begin();
  Serial.print("done" );
}


void loop() { server.handleClient(); }


void RerenderButtons() {
  
  bool led_is_on = digitalRead(led_pin)         == led_on;
  Serial.print( "LED is on: " );
  Serial.print( led_is_on );
  bool zone1_is_on = digitalRead(relay_pin_1)   == relay_on;
  Serial.print( " -- Zone #1 is " );
  Serial.print( zone1_is_on );
  bool zone2_is_on = digitalRead(relay_pin_2)   == relay_on;
  Serial.print( " -- Zone #2 is " );
  Serial.print( zone2_is_on );
  bool zone3_is_on = digitalRead(relay_pin_3)   == relay_on;
  Serial.print( " -- Zone #3 is " );
  Serial.print( zone3_is_on );
  bool zone4_is_on = digitalRead(relay_pin_4)   == relay_on;
  Serial.print( " -- Zone #4 is " );
  Serial.print( zone4_is_on );

  String HTML = "{\"zone1\":" + String(zone1_is_on) + "\"zone2\":" + String(zone2_is_on) + "\"zone3\":" + String(zone3_is_on) + "\"zone4\":" + String(zone4_is_on) + "}";


  // HTML += R"( <h2>  LED Light  </h2>)";
  // HTML += R"( <style> input[type="button"] { display: inline-block; width: 64px; padding: 2px; margin-bottom: 4px; }</style>)";
  // HTML += R"( <input type="button" onClick="location.href='/led/on';"                                   )";
  // HTML +=           led_is_on?"Disabled": "";
  // HTML += R"(       value="ON">                                                                         )";
  // HTML += R"( <input type="button" onClick="location.href='/led/off';"                                  )";
  // HTML +=           ! led_is_on?"Disabled": "";
  // HTML += R"(       value="OFF">                                                                        )";
  // HTML += R"( <br>                                                                                      )";
  // HTML += R"( <h2>  Zone #1  </h2>                                                                      )";
  // HTML += R"( <style> input[type="button"] { width: 64px; padding: 2px; margin-bottom: 4px; }</style>   )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/1/mode/on'"                      )";
  // HTML +=         zone1_is_on?"Disabled": "";
  // HTML += R"(     value="ON">                                                                           )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/1/mode/off'"                     )";
  // HTML +=         ! zone1_is_on?"Disabled": "";
  // HTML += R"(     value="OFF">                                                                          )";
  // HTML += R"( <br>                                                                                      )";
  // HTML += R"( <h2>  Zone #2  </h2>                                                                      )";
  // HTML += R"( <style>input[type="button"] { width: 64px; padding: 2px; margin-bottom: 4px; }</style>    )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/2/mode/on'"                      )";
  // HTML +=         zone2_is_on?"Disabled": "";
  // HTML += R"(     value="ON">                                                                           )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/2/mode/off'"                     )";
  // HTML +=         ! zone2_is_on?"Disabled": "";
  // HTML += R"(     value="OFF">                                                                          )";
  // HTML += R"( <br>                                                                                      )";
  // HTML += R"( <h2>  Zone #3  </h2>                                                                      )";
  // HTML += R"( <style>input[type="button"] { width: 64px; padding: 2px; margin-bottom: 4px; }</style>    )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/3/mode/on'"                      )";
  // HTML +=         zone3_is_on ? "Disabled" : "";
  // HTML += R"(     value="ON">                                                                           )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/3/mode/off'"                     )";
  // HTML +=         ! zone3_is_on?"Disabled": "";
  // HTML += R"(     value="OFF">                                                                          )";
  // HTML += R"( <br>                                                                                      )";
  // HTML += R"( <h2>  Zone #4  </h2>                                                                      )";
  // HTML += R"( <style>input[type="button"] { width: 64px; padding: 2px; margin-bottom: 4px; }</style>    )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/4/mode/on'"                      )";
  // HTML +=         zone4_is_on?"Disabled": "";
  // HTML += R"(     value="ON">                                                                           )";
  // HTML += R"( <input type="button" onClick="location.href='/pod/1/zone/4/mode/off'"                     )";
  // HTML +=         ! zone4_is_on?"Disabled": "";
  // HTML += R"(     value="OFF">                                                                          )";



  server.send(200, "text/json", HTML);


}


void SetPin(byte pin_number, byte new_pin_state) {
  
  digitalWrite(pin_number, new_pin_state);
  
  Serial.print("\nSetting pin#");
  Serial.print(pin_number);
  Serial.print(" to: ");
  Serial.println(new_pin_state);

  RerenderButtons();
}
