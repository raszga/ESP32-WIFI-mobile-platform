#include <WiFi.h>
#include <stdio.h>
#include <string.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>
#include "ESP32_Mobile.h"
WiFiUDP udp;
WiFiServer server(localPort);  // Set web server port number to 80
//---------------------------------------------------------------------------------
void setup() {
  // evrythig to 0**********************************************************
  // directions
  Serial.begin(115200);
  // ***************************WIFI*********************************************
  int i = 0;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // connect with the phone to GRSD to fix the address
  IPAddress ip(192, 168, 5, 30);       // desired IP address
  IPAddress gateway(192, 168, 5, 5);   // gateway of my network
  IPAddress subnet(255, 255, 255, 0);  // subnet mask of my network
  IPAddress dns(192, 168, 5, 22);      // dns
  WiFi.config(ip, dns, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    i++;
    delay(50);
    Serial.print(F("."));
    Serial.println(i);
  }
  udp.begin(localPort);
  Serial.printf("UDP server : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);
  delay(50);
  // Alocate resources*******************************************************
  Serial.println("Connecting PWM");
  for (int i = 0; i < NrLeds; i++) {
    bool LedResult = ledcAttach(LedArr[i], LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    Serial.print(i);
    Serial.print(LedResult);
    if (!LedResult) {
      Serial.println(" Error");
    } else {
      Serial.println(" Succes");
    }
  }
  // Motor Alocation
  // Left Rear ----------------
  MotLR.NAM = "Left Rear ";
  MotLR.GP0 = LedArr[0];
  MotLR.GP1 = LedArr[1];
  MotoReport(MotLR);
  // Left Front ---------------
  MotLF.NAM = "Left Front ";
  MotLF.GP0 = LedArr[2];
  MotLF.GP1 = LedArr[3];
  MotoReport(MotLF);
  // Right Rear ---------------
  MotRR.NAM = "Right Rear ";
  MotRR.GP0 = LedArr[4];
  MotRR.GP1 = LedArr[5];
  MotoReport(MotRR);
  // Right Front --------------
  MotRF.NAM = "Right Front ";
  MotRF.GP0 = LedArr[6];
  MotRF.GP1 = LedArr[7];
  MotoReport(MotRF);
  // Buz Led-------------------
  BzLed.NAM = "Buz Led ";
  BzLed.GP0 = LedArr[8];
  BzLed.GP1 = LedArr[9];
  MotoReport(BzLed);
//****************Detect Neutrals**********************
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);  // was 255
    if (len > 0) { packetBuffer[len - 1] = 0; }
    rS = String(packetBuffer);
    xintJoy=TrS(rS);
    J0DX_N = xintJoy._val[0];
    J0DY_N = xintJoy._val[1];
    J1DX_N = xintJoy._val[2];
    J1DY_N = xintJoy._val[3];
    J0_BT  = xintJoy._val[4];   
    J1_BT  = xintJoy._val[5];   
  }
}
//----------------------------------------------------------------------------------
void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    udp.read(packetBuffer,128);  // was 255
    rS = String(packetBuffer);
    xintJoy=TrS(rS);
    J0DX=xintJoy._val[0]-J0DX_N;
    J0DY=xintJoy._val[1]-J0DY_N;
    J1DX=xintJoy._val[2]-J1DX_N;
    J1DY=xintJoy._val[3]-J1DY_N;
    J0_BT  = xintJoy._val[4];   
    J1_BT  = xintJoy._val[5];   

    udp.clear();
    
    Serial.print(J0DX);Serial.print("  ");
    Serial.print(J0DY);Serial.print("  ");
    Serial.print(J1DX);Serial.print("  ");
    Serial.print(J1DY);Serial.print("  ");
    Serial.print(J0_BT);Serial.print("  ");
    Serial.print(J1_BT);Serial.print("  ");

    MotoMove(MotLF,J0DY,50,0);
    MotoMove(MotLR,J0DY,50,0);
    MotoMove(MotRF,J1DY,50,0);
    MotoMove(MotRR,-J1DY,50,0);

    Serial.println();
    delay(10);
  }
}

