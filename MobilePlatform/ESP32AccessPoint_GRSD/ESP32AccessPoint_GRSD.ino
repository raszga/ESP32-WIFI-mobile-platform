// ---------------------------------------------------------------------------------------
// Simple code to setup the ESP32 as access point (device used for tests: ESP32-WROOM).
// ---------------------------------------------------------------------------------------
#include <WiFi.h>                                     // needed to connect to setup an accesspoint
// SSID and password that are going to be used for the Access Point.
const char* ssid = "GRSD";
const char* password = "";
// Configure IP addresses of the local access point
IPAddress local_IP(192,168,5,22);
IPAddress gateway(192,168,5,5);
IPAddress subnet(255,255,255,0);
//-----------------------------------
int LED=23;
//------------------------------------------------------------------------------------
void setup() {
  pinMode(LED,OUTPUT);
  Serial.begin(115200);                               // init serial port for debugging
  Serial.print("Setting up Access Point ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Starting Access Point ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.print("IP address = ");
  Serial.println(WiFi.softAPIP());
   analogWrite(LED,64);
}
void loop() {
;
}
