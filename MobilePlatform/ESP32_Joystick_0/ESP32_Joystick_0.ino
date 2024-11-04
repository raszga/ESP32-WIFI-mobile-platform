// client
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;
char packetBuffer[255];
unsigned int localPort = 80;
const char *ssid = "GRSD";
const char *password = "";
// readings==============================================================
#define CONVERSIONS_PER_PIN 10
uint8_t adc_pins[] = {
  35,
  34,
  36,
  39,
};  //some of ADC1 pins for ESP32 wroom in board layout
uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);
uint8_t digi_pins[] = {
  26,
  25,
};  //some of DIGI pins for ESP32 wroom in board layout
uint8_t digi_pins_count = sizeof(digi_pins) / sizeof(uint8_t);
volatile bool adc_coversion_done = false;
adc_continuous_data_t *result = NULL;
// program variables***********************************************************
String outString;  // data string
//==============================================================================
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // connect with the phone to GRSD to fix the address
  IPAddress ip(192, 168, 5, 20);       // desired IP address
  IPAddress gateway(192, 168, 5, 5);   // gateway of my network
  IPAddress subnet(255, 255, 255, 0);  // subnet mask of my network
  IPAddress dns(192, 168, 5, 22);      // dns
  WiFi.config(ip, dns, gateway, subnet);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    i++;
    delay(500);
    Serial.print(F("."));
    Serial.println(i);
  }
  udp.begin(localPort);
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);
  analogContinuousSetWidth(12);
  analogContinuousSetAtten(ADC_11db);
  analogContinuous(adc_pins, adc_pins_count, CONVERSIONS_PER_PIN, 20000, &adcComplete);
  analogContinuousStart();
  for (int i = 0; i < digi_pins_count; i++) { pinMode(digi_pins[i], INPUT_PULLUP); }
}

void loop() {
  udp.beginPacket("192.168.5.30", localPort);
  String S = readAnalogs();
  Serial.println(S);
  int Slength = S.length() + 1;
  char buf[Slength];
  S.toCharArray(buf, Slength);
  udp.printf(buf);
  udp.endPacket();
  delay(10);
}
//*****************************************************************************
String readAnalogs() {
  if (adc_coversion_done == true) {
    adc_coversion_done = false;
    if (analogContinuousRead(&result, 0)) {
      outString = "";
      for (int i = 0; i < adc_pins_count; i++) {
        char s[4];
        int R = result[i].avg_read_raw;
        sprintf(s, "%4d", R);
        if (i == 0) {
          outString += String(s);
        } else {
          outString += "," + String(s);
        }
      }
      delay(3);
    } else {
      Serial.println("Error occurred during reading data. Set Core Debug Level to error or lower for more information.");
      return "Error";
    }

    for (int i = 0; i < digi_pins_count; i++) {
      char s[4];
      int R = digitalRead(digi_pins[i]);
      sprintf(s, "%4d", R);
      outString += "," + String(s);
    }
    //outString +=',';
  }
  //Serial.print(outString);
  return outString;
}
//*****************************************************************************
void ARDUINO_ISR_ATTR adcComplete() {
  adc_coversion_done = true;
}
//*****************************************************************************