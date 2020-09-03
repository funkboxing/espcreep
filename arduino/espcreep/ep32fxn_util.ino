//========================================
//==============UTILITY FXNS==============
//========================================
const int LEDRED = 33;      //---STATUS LED PIN
const int LEDWHITE = 4;     //---FLASH LED PIN

//===SERVO CONTROL VARIABLES
#define TIMER_WIDTH 16

unsigned int servo0Pin = 13;
unsigned int servo1Pin = 14;
unsigned int servo0Ch = 6;
unsigned int servo1Ch = 7;
unsigned int flashLEDPin = 4;
unsigned int flashLEDCh = 5;

void setServos(int s0, int s1){
    ledcWrite(servo0Ch, s0);
    ledcWrite(servo1Ch, s1);
}

void setFlash(int i){ //---SET FLASH, 0-255 I THINK
    ledcWrite(flashLEDCh, i);
}

void setupGPIO(){
  pinMode(33, OUTPUT); //--RED STATUS LED (HIGH-OFF|LOW-ON) 
  digitalWrite(33, HIGH);
  ledcSetup(servo0Ch, 50, TIMER_WIDTH);
  ledcAttachPin(servo0Pin, servo0Ch);
  ledcSetup(servo1Ch, 50, TIMER_WIDTH);
  ledcAttachPin(servo1Pin, servo1Ch);
  ledcSetup(flashLEDCh, 50, TIMER_WIDTH);
  ledcAttachPin(flashLEDPin, flashLEDCh);
}

void blinkInteger(int num, int tdel){
  for (int i = 0; i < num; i++) {
    digitalWrite(LEDRED, HIGH); delay(tdel); digitalWrite(LEDRED, LOW); delay(tdel); 
  }
}

void dbo(String x){   //---DEBUG OUTPUT FUNCTION
  if (debugoutput){
    Serial.print(x);
  }
}
