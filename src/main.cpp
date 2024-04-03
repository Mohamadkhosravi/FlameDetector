#include <Arduino.h>
#include <main.hpp>

Watchdog watchdog;
LowPowerClass lowPowerBoard;

void setup()
{
   // watchdog.enable(Watchdog::TIMEOUT_2S);
    pinMode(LED, OUTPUT);
    // pinMode(LDR_CONNECT, OUTPUT);
    // pinMode(NTC_CONNECT, OUTPUT);
    // Serial.begin(9600); 
    //===============
    /*setup FFT Sampling*/
    sampling_period_us = round(1000000*(1.0/samplingFrequency));
  
}
String LastPIR;
void loop()
{
// if(!firstTurnON){

//   // while (1)
//   // {
//   //   //randomSeed(analogRead(A6));

//   //  long randDelay=randomSeed(analogRead(A6));
//   //   delay(randDelay);
//   //   break;
//   // }

// }
    firstTurnON=true;
    //  digitalWrite(LED, LOW);
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
// while (1)
// {
  BlinkerTimer++;
  if(BlinkerTimer>=4){
    digitalWrite(LED, HIGH);
    delay(1);
    digitalWrite(LED, LOW);
    BlinkerTimer=0;
  }
 
  // delay(200);
// }

    
    if (IR < TRESHOLT_IR_FIER){
      FIER=true;
      FIERCounter++;
    }
    else {
      FIER = false;
      if(FIERCounter > 0 ) FIERCounter--;
    }

    if(FIERCounter>3)
    {
      while (1)
      {
        LineVoltage=analogRead(ADC_LINE);
        digitalWrite(LED, HIGH);

      }
    }
     delay(100);
    // if(micros() - microseconds < 200)break;
    // lowPowerBoard.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
//  }
 
   


}
