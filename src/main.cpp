#include <Arduino.h>
#include <main.hpp>
#include<NTC_Thermistor.h>
#include<LowPower.h>
LowPowerClass lowPowerBoard;
Thermistor* thermistor;
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
void setup()
{
    pinMode(8, OUTPUT);
    pinMode(LDR_CONNECT, OUTPUT);
    pinMode(NTC_CONNECT, OUTPUT);
    Serial.begin(9600); 
    thermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE,
    ANALOG_RESOLUTION 
  );
}

void loop()
{
  //   const double celsius = thermistor->readCelsius();
  //   double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
  //   double RLDR = CALCULATE_RLDR(VLDR,10000);
  //   double LUX= LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);
  //   int PIR =analogRead(ADC_PIR);
  //   int IR =analogRead(ADC_IR);
  //   digitalWrite(LDR_CONNECT, LOW);
  //   digitalWrite(NTC_CONNECT, LOW);
  //   String Data = 
   
  //   "PIR="+(String)PIR
    
  //   +"  IR="+(String)IR

  //   +"  LDR="+(String)analogRead(ADC_LDR)
  //   //+"   VLDR="+(String)VLDR
  //   //+"   RLDR="+(String)RLDR
  //   +"   LUX="+(String)LUX
  //   +"   Temperature="+(String)celsius;
    
  //   Serial.println(Data);
  //   digitalWrite(8, HIGH);
  //  delay(10);
    // digitalWrite(8, LOW);
    // delay(100);
  
    lowPowerBoard.longPowerDown(SLEEP_1S);
  
   


}
