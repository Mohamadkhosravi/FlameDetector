#include <Arduino.h>
#include <main.hpp>
#include<NTC_Thermistor.h>
Thermistor* thermistor;
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
void setup()
{
   pinMode(LED_BUILTIN, OUTPUT);
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
    const double celsius = thermistor->readCelsius();
    double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
    double RLDR = CALCULATE_RLDR(VLDR,10000);
    double LUX= LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);


    String Data = 
    "PIR="+(String)analogRead(ADC_PIR)
    +"  IR="+(String)analogRead(ADC_IR)
    +"  LDR="+(String)analogRead(ADC_LDR)
    //+"   VLDR="+(String)VLDR
    //+"   RLDR="+(String)RLDR
    +"   LUX="+(String)LUX
    +"   Temperature="+(String)celsius;
    
    Serial.println(Data);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);


}
