#include <Arduino.h>
#include <main.hpp>
#include <avr/power.h>

Watchdog watchdog;
LowPowerClass lowPowerBoard;

void setup()
{
  // watchdog.enable(Watchdog::TIMEOUT_2S);
  pinMode(LED, OUTPUT);
  pinMode(ADC_LINE_CONNECT,OUTPUT);
#if READ_LDR
  pinMode(LDR_CONNECT, OUTPUT);
#endif

#if READ_TEMPERATURE
  pinMode(NTC_CONNECT, OUTPUT);
#endif
Serial.begin(9600);
#if DEBUG
  Serial.begin(9600);
#endif
  //===============

#if FFT_PIR
  /*setup FFT Sampling*/
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
#endif
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
  // firstTurnON=true;
  // digitalWrite(LED, LOW);

  lowPowerBoard.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  
  digitalWrite(ADC_LINE_CONNECT, LOW);

  uint32_t R1 = 220;
  uint32_t R2 = 22;
  // todo : pin low
  uint32_t VoltageLine = (R1 + R2)/R2  * analogRead(ADC_LINE)*3 / ADC_10bit;
  // watchdog.tripped();
 Serial.print("\n VoltageLine=");
   Serial.print(VoltageLine);


#if READ_TEMPERATURE 
  digitalWrite(NTC_CONNECT, LOW);
  double celsius = temperature(analogRead(ADC_NTC), 5);
#endif
#if READ_LDR
  digitalWrite(LDR_CONNECT, LOW);
  double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
  double RLDR = CALCULATE_RLDR(VLDR, 10000);
  double LUX = LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);
#endif
  // double PIR;
  unsigned int IR = analogRead(ADC_IR);
 Serial.print("\n IR=");
   Serial.print(IR);
#if READ_PIR
  int PIR = analogRead(ADC_PIR);
#endif

#if FFT_PIR
  /*SAMPLING*/
  microseconds = micros();
  for (int i = 0; i < samples; i++)
  {
    vReal[i] = analogRead(CHANNEL);
    vImag[i] = 0;
    while (micros() - microseconds < sampling_period_us)
    {
      // empty loop
    }
    microseconds += sampling_period_us;
  }
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Weigh data */
  FFT.compute(FFTDirection::Forward);                       /* Compute FFT */
  FFT.complexToMagnitude();                                 /* Compute magnitudes */
  PIR = FFT.majorPeak();
  delay(100); /* Repeat after delay */
              // char buffer[3];
              // dtostrf(PIR, 2, 3, buffer);
#endif

#if DEBUG
  //     String Data =
  //     "PIR="+(String)PIR
  //     +"  IR="+(String)IR
  //     +"  LDR="+(String)analogRead(ADC_LDR)
  //     +"   LUX="+(String)LUX
  //     +"   Temp="+(String)celsius;
  //    Serial.println(Data);
  PIR = PIR * 10.00;
  if (PIR < 0)
    PIR = 0;
  if (PIR > 2000)
    PIR = 1024;
  Serial.print("PIR=" + PIR);
  Serial.print("  IR=" + (String)IR);
  Serial.print("  LDR=" + (String)analogRead(ADC_LDR));
  Serial.print("  LUX=" + (String)LUX);
  Serial.print("  Temp=" + (String)celsius);
  Serial.print("\n");
#endif


#if POWER_DISABLE
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_usart0_disable();
#endif

  if (VoltageLine < MINIMUM_VOLTAGE_LINE_VALID)
  {
    digitalWrite(LED, LOW);
  }
  else
  {

    BlinkerTimer++;
    if (BlinkerTimer >= 3)
    {
      digitalWrite(LED, HIGH);
      _delay_us(100);
      digitalWrite(LED, LOW);
      BlinkerTimer = 0;
    }
  }
  if (IR < TRESHOLT_IR_FIER)
  {
    FIER = true;
    FIERCounter++;
  }
  else
  {
    FIER = false;
    if (FIERCounter > 0)
      FIERCounter--;
  }

  if (FIERCounter > 3)
  {
    while (1)
    {
       VoltageLine = (R1 + R2)/R2  * analogRead(ADC_LINE)*3 / ADC_10bit;

      if (VoltageLine < MINIMUM_VOLTAGE_LINE_VALID)
      {
        digitalWrite(LED, LOW);
        FIERCounter = 0;
        FIER=false;
        break;
      }
      else
      {
        digitalWrite(LED, HIGH);
      }
    }
  }
  delay(100);
}
