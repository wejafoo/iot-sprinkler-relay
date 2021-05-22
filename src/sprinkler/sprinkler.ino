


// AT Commands for quad relay
// Open relay 1: A0 01 01 A2
// Close relay 1: A0 01 00 A1
// Open relay 2: A0 02 01 A3
// Close relay 2: A0 02 00 A2
// Open relay 3: A0 03 01 A4
// Close relay 3: A0 03 00 A3
// Open relay 4: A0 04 01 A5
// Close relay 4: A0 04 00 A4


#include <ESP8266WiFi.h>

#define RELAY LED_BUILTIN // relay would connect to GPIO0

const char* ssid = "bilbo";
const char* password = "readyplayer1";

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
 
  pinMode(RELAY,OUTPUT);
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("LED builtin:");
  Serial.println(LED_BUILTIN);
 
  server.begin();
  Serial.println("Server started");
   Serial.print("Use this URL to connect: ");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  WiFiClient client = server.available();
  if ( ! client ) { return; }
  
  Serial.println("new client"); 
  while( ! client.available() ) { delay(1); }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if ( request.indexOf( "/RELAY=ON" ) != -1 ) {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY,LOW);
    value = LOW;
  }
  
  if (request.indexOf( "/RELAY=OFF" ) != -1 ) {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY,HIGH);
    value = HIGH;
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 RELAY Control</title></head>");
  client.print("Relay is now: ");
  if ( value == HIGH ) { client.print("OFF"); } else { client.print("ON"); }
  client.println("<br><br>");
  client.println("Turn <a href=\"/RELAY=OFF\">OFF</a> RELAY<br>");
  client.println("Turn <a href=\"/RELAY=ON\">ON</a> RELAY<br>");
  client.println("</html>");
}
