#include <Arduino.h>
#include <main.hpp>
#include<LowPower.h>
#include "NTC.h"
LowPowerClass lowPowerBoard;

#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
bool FIER=false;
void setup()
{
    pinMode(LED, OUTPUT);
    pinMode(LDR_CONNECT, OUTPUT);
    pinMode(NTC_CONNECT, OUTPUT);
    Serial.begin(9600); 

}

void loop()
{
    double celsius = temperature(analogRead(ADC_NTC),5);
    double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
    double RLDR = CALCULATE_RLDR(VLDR,10000);
    double LUX= LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);
    int PIR =analogRead(ADC_PIR);
    int IR =analogRead(ADC_IR);
    digitalWrite(LDR_CONNECT, LOW);
    digitalWrite(NTC_CONNECT, LOW);

    String Data = 
    "PIR="+(String)PIR
    +"  IR="+(String)IR
    +"  LDR="+(String)analogRead(ADC_LDR)
    +"   LUX="+(String)LUX
    +"   Temperature="+(String)celsius;
   Serial.println(Data);

   if (IR<60)FIER=true;
   else FIER = false;
   digitalWrite(LED, FIER);
   delay(200);
    //lowPowerBoard.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 


}
