
#include "ESP32_Mobile.h"
const char *password = "";
const char *ssid = "GRSD";
char packetBuffer[255];
//----------------------------------------------------
_intJoy xintJoy;  // variable// Joystick output array
String rS;        // joystick response string
// Driving structure motors and directions
pairPWM MotLR;  // left rear
pairPWM MotLF;
pairPWM MotRR;
pairPWM MotRF;  // Rigt Front
pairPWM BzLed;  // Rigt Front

// motor command
int TLFTF = 0;  // left front
int TLFTR = 0;  // left rear
int TRGHF = 0;  // rigth front
int TRGHR = 0;  // right rear

// define direction of movemet variables
int DX;    //  ....
int DY;    //
int J0DX;  //  ....
int J0DY;  //
int J1DX;  //  ....
int J1DY;  //
//Histeresys
int J0DX_H=50;  //  ....
int J0DY_H=50;  //
int J1DX_H=50;  //  ....
int J1DY_H=50;  //
//Neutral
int J0DX_N;  //  ....
int J0DY_N;  //
int J1DX_N;  //  ....
int J1DY_N;  //

// button
int J0_BT;
int J1_BT;

int RT;         // counter rotate//
int hist = 50;  // hys
//====================================================================
// SIGN function
//====================================================================
int sgn(int a) {
  int s = 0;
  if (a > 0) { s = 1; }
  if (a < 0) { s = -1; }
  return s;
}
//======================================================
// Motor movements Fw/Rw function
//======================================================
pairPWM MotoMove(pairPWM M, int cmd, uint8_t hst, bool Verb) {
  M.DTY = cmd;
  if (Verb) {
    Serial.print(M.NAM + "=");
    Serial.println(M.DTY);
  }
  if ((M.DTY < -hst) or (M.DTY > hst)) {
    if (M.DTY > 0) {
      ledcWrite(M.GP1, M.DTY);
      ledcWrite(M.GP0, 0);
    }  //cmd>0
    if (M.DTY < 0) {
      ledcWrite(M.GP0, -M.DTY);
      ledcWrite(M.GP1, 0);
    }  //cmd<0
  }    //if
  else {
    ledcWrite(M.GP0, 0);
    ledcWrite(M.GP1, 0);
  }  //else
  return M;
}  // END MotoMove---------------------
// Moto report ----------------------------------------------------------------
void MotoReport(pairPWM MTO) {
  Serial.println("**********************************");
  Serial.println(MTO.NAM);
  prtInt("PIN0", MTO.GP0);
  prtInt("PIN1", MTO.GP1);
  prtInt("SPEED", MTO.SPD);
  prtInt("DUTY", MTO.DTY);
  prtInt("SENSE", MTO.SNS);
}
// Pretty print int -----------------------------------------------------------
void prtInt(String T, int i) {
  Serial.print(" " + T + " ");
  Serial.println(i);
}
// transfer string
_intJoy TrS(String S) {
  _intJoy tk;
  char sz[] = "1957,2014,1979,1933,0001,0001";  // prototipe
  char buf[sizeof(sz)];
  S.toCharArray(buf, sizeof(buf));
  char *p = buf;
  char *str;
  int k = 0;
  while ((str = strtok_r(p, ",", &p)) != NULL) {
    tk._val[k] = atoi(str);
    k++;
  }
  return tk;
}
