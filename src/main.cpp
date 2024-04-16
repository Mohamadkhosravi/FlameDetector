#include <Arduino.h>
#include <main.hpp>
#include <avr/power.h>

Watchdog watchdog;
LowPowerClass lowPowerBoard;
uint32_t readVoltageLine(uint32_t ADCValue);
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

#if DEBUG
  Serial.begin(9600);
#endif
  //===============

#if FFT_PIR
  /*setup FFT Sampling*/
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
#endif
 firstTurnON=false;
 randomSeed(analogRead(A6));
}
String LastPIR;
void loop()
{
   
  if(!firstTurnON){
   
    uint32_t randDelay= random(3000);
     if(randDelay>=6000)randDelay=int(randDelay/2);
     if(randDelay>=60000)randDelay=int(randDelay/10);
     log("randDelay="+(String)randDelay);
   
    while (1)
    {
      // watchdog.tripped();
      if(randDelay>0)
      {
        --randDelay;
      }
      else
      {
        break;
      }
    
      delay(1);
    }
  }
  firstTurnON=true;
  digitalWrite(LED, LOW);
  lowPowerBoard.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);  
 uint32_t VoltageLine = readVoltageLine(analogRead(ADC_LINE));

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

unsigned int IR = analogRead(ADC_IR);

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

#if READ_PIR
  PIR = PIR * 10.00;
  if (PIR < 0)
    PIR = 0;
  if (PIR > 2000)
    PIR = 1024;
  log("PIR=" + PIR);
#endif
#if READ_IR
  log("  IR=" + (String)IR);
#endif
#if READ_LDR
  log("  LDR=" + (String)analogRead(ADC_LDR));
  log("  LUX=" + (String)LUX);
#endif
#if READ_TEMPERATURE
  log("  Temp=" + (String)celsius +"\n");
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
      _delay_us(200);
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
      // watchdog.tripped();
       VoltageLine = readVoltageLine(analogRead(ADC_LINE));
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
  // delay(100);
}

 uint32_t readVoltageLine(uint32_t ADCValue)
 {
  uint32_t R1 = 220;
  uint32_t R2 = 22;
  uint16_t VCC = 3;
  digitalWrite(ADC_LINE_CONNECT, LOW);
  uint32_t voltage=(R1 + R2)/R2  * ADCValue * VCC / ADC_10bit;
  log("\n VoltageLine"+(String)voltage);
  return voltage;
 }