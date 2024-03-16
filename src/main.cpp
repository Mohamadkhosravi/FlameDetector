#include <Arduino.h>
#include <main.hpp>
#include <LowPower.h>
#include "NTC.h"
#include "arduinoFFT.h"
#include "time.h"
#include "Watchdog.h"

Watchdog watchdog;
LowPowerClass lowPowerBoard;

#define CHANNEL ADC_PIR
const uint16_t samples = 8; //This value MUST ALWAYS be a power of 2
const float samplingFrequency = 1; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
float vReal[samples];
float vImag[samples];

/* Create FFT object with weighing factor storage */
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency, true);

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType);



#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
bool FIER=false;
unsigned int FIERCounter=0;
void setup()
{
   // watchdog.enable(Watchdog::TIMEOUT_2S);
    pinMode(LED, OUTPUT);
    pinMode(LDR_CONNECT, OUTPUT);
    pinMode(NTC_CONNECT, OUTPUT);
    // Serial.begin(9600); 
    //===============
    sampling_period_us = round(1000000*(1.0/samplingFrequency));

}
String LastPIR;
void loop()
{

    digitalWrite(LED, LOW);
    lowPowerBoard.powerDown(SLEEP_2S,ADC_OFF, BOD_OFF);
    // watchdog.tripped();
    //  while (1)
    //  {
    // double celsius = temperature(analogRead(ADC_NTC),5);
    // double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
    // double RLDR = CALCULATE_RLDR(VLDR,10000);
    // double LUX= LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);
    // int PIR =analogRead(ADC_PIR);
    // double PIR;
    int IR =analogRead(ADC_IR);
    // digitalWrite(LDR_CONNECT, LOW);
    // digitalWrite(NTC_CONNECT, LOW);

    /*SAMPLING*/
    //  microseconds = micros();
    //   for(int i=0; i<samples; i++)
    //   {
    //       vReal[i] = analogRead(CHANNEL);
    //       vImag[i] = 0;
    //       while(micros() - microseconds < sampling_period_us){
    //         //empty loop
    //       }
    //       microseconds += sampling_period_us;
    //   }

    //     FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
    //     FFT.compute(FFTDirection::Forward); /* Compute FFT */
    //     FFT.complexToMagnitude(); /* Compute magnitudes */
    //     PIR= FFT.majorPeak();
    //     delay(100); /* Repeat after delay */
    //   PIR=PIR*10.00;
    // if (PIR<0)PIR=0;
    // if (PIR>2000)PIR=1024;

    // String SPTR =(String)PIR;
    // if(SPTR==" NAN")SPTR=LastPIR;
    // else LastPIR=SPTR;

    // char buffer[3];
    // dtostrf(PIR, 2, 3, buffer); 

    //     String Data = 
    //     "PIR="+(String)PIR
    //     +"  IR="+(String)IR
    //     +"  LDR="+(String)analogRead(ADC_LDR)
    //     +"   LUX="+(String)LUX
    //     +"   Temp="+(String)celsius;
    //    Serial.println(Data);
    // Serial.print( "PIR="+SPTR);
    // Serial.print( "PIR="+PIR);
    // Serial.print("  IR="+(String)IR);
    // Serial.print("  LDR="+(String)analogRead(ADC_LDR));
    // Serial.print("  LUX="+(String)LUX);
    // Serial.print("  Temp="+(String)celsius);
    // Serial.print("\n");
    if (IR<10){
      FIER=true;
      FIERCounter++;
    }
    else {
      FIER = false;
      if(FIERCounter>0) FIERCounter--;
    }

    if(FIERCounter>3)
    {
      while (1)
      {
        digitalWrite(LED, HIGH);
      }
    }
     delay(100);
    // if(micros() - microseconds < 200)break;
    // lowPowerBoard.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
//  }
 
   


}
