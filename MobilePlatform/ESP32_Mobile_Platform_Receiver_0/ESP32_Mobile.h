#ifndef ESP32_Mobile_INCLUDED
#define ESP32_Mobile_INCLUDED
#include <Arduino.h>
#include <string.h>
#define LEDC_TIMER_12_BIT 12
#define LEDC_BASE_FREQ 100

extern const char *ssid ;
extern const char *password;

extern char packetBuffer[255];
// motor pins allocated-------------------------------------------------------
const int LedArr[] = { 19, 18, 05, 17, 25, 26, 27, 14, 12, 13 };  // pins
const int NrLeds = int(sizeof(LedArr) / sizeof(LedArr[0]));
const unsigned int localPort = 80;
//-----------------------------------------------------------------------------
// joystick message structure
struct _intJoy {
  int _val[50] = { 0 };
};
// Declare PWM pair (for motor eventualy) structure---------------------------
struct pairPWM {
  String NAM = "---------";  // Name
  uint8_t GP0;               //pin 0 FW
  uint8_t GP1;               // pin 1 Rw
  int DTY = 0;               // duty cycle
  uint16_t SPD = 0;          // speedRPM if exists
  uint8_t SNS = 0;           // _CW(1) or _CCW(2) or _FRN=0 or  _FRWill=3
};
// extern variables
extern _intJoy xintJoy;  // variable command joystick
extern String rS;                     // joystick response string
// Driving structure motors and directions
extern pairPWM MotLR;  // left rear
extern pairPWM MotLF;
extern pairPWM MotRR;
extern pairPWM MotRF;  // Rigt Front
extern pairPWM BzLed;  // Rigt Front
// motor command 
extern int TLFTF;  // left front
extern int TLFTR;  // left rear
extern int TRGHF;  // rigth front
extern int TRGHR;  // right rear

// define direction of movemet variables
extern int DX;  // X vector Left /Right
extern int DY;  // Y vector Forward /back
extern int J0DX;  //  ....
extern int J0DY;  // 
extern int J1DX;  //  ....
extern int J1DY;  // 
//Hist
extern int J0DX_H;  //  ....
extern int J0DY_H;  // 
extern int J1DX_H;  //  ....
extern int J1DY_H;  // 
//neutr
extern int J0DX_N;  //  ....
extern int J0DY_N;  // 
extern int J1DX_N;  //  ....
extern int J1DY_N;  // 
extern int J0_BT;
extern int J1_BT;

extern int RT;  // counterotate//

extern int hist;  // hysteresys
extern int dtPack[20];  // integer array from joystick

// Functions-----------------------------------------------------------------
int sgn(int a);
pairPWM MotoMove(pairPWM M, int cmd, uint8_t hst, bool Verb);
void MotoReport(pairPWM MTO);
#endif
// pretty print int ---------------------------------------------------------
void prtInt(String T,int i); 
_intJoy TrS(String S);
