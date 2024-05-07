#include <Arduino.h>
#include <main.hpp>
#include <avr/power.h>
#include<EEPROM.h>
#include<SoftwareSerial.h>
SoftwareSerial Serial1(A5,A2);
Watchdog watchdog;
LowPowerClass lowPowerBoard;

uint32_t readVoltageLine(uint32_t ADCValue);
uint32_t averageFilter(uint32_t numberOfPin,uint16_t samplingScale, uint16_t ADCOffset );

void setup()
{
  // watchdog.enable(Watchdog::TIMEOUT_2S);
  pinMode(LED, OUTPUT);
  pinMode(ADC_LINE_CONNECT,OUTPUT);
  pinMode(CALIBRATION_PIN, INPUT_PULLUP);

#if READ_LDR
  pinMode(LDR_CONNECT, OUTPUT);
#endif

#if READ_TEMPERATURE
  pinMode(NTC_CONNECT, OUTPUT);
#endif

#if DEBUG
  Serial.begin(9600);
  Serial1.begin(9600);


#endif
  //===============

#if FFT_PIR
  /*setup FFT Sampling*/
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
#endif
 firstTurnON=false;
 randomSeed(analogRead(A6));
  EEPROM.begin();
  calibrationState=bool(EEPROM.read(0));
  if(calibrationState)IROffset=EEPROM.read(20);


}
String LastPIR;
void loop()
{

  if(!firstTurnON)
  {
    
    while (calibrationState)
    {
        log("\nIROffset=");
        log(IROffset);
        log("\ncalibrationState");
        log(calibrationState);
        digitalWrite(LED, HIGH);
        delay(2);
        digitalWrite(LED, LOW);
        delay(2);
         log("\npin=");
         log(digitalRead(CALIBRATION_PIN));
 
      if(!digitalRead(CALIBRATION_PIN))
      {
        if(!(digitalRead(CALIBRATION_PIN)))
        {  

           EEPROM.update(ADDRESS_CALIBRATION_STATE, 0);
           if(averageFilter(ADC_IR,10,0)<15){
             IROffset=char(averageFilter(ADC_IR,10,0));
             calibrationState=false;
             EEPROM.write(ADDRESS_OFFSET_Value, IROffset);
              digitalWrite(LED, HIGH);
              delay(10);
              digitalWrite(LED, LOW);
              delay(10);
              digitalWrite(LED, HIGH);
              delay(10);
              digitalWrite(LED, LOW);
              delay(10);
             break;
           }
           else
           {
              calibrationState=true;
           }
          
          
        }
        
    }
    
   }
     uint8_t randDelay= random(33,1000);
     if(randDelay>=1000)randDelay=int(randDelay/2);
     if(randDelay>=60000)randDelay=int(randDelay/10);
    log("\n randDelay="+(String)randDelay);
    for(uint8_t repeat=0 ;repeat<= randDelay; repeat++ )
    {
      lowPowerBoard.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
    }


  }
  
  firstTurnON=true;
  digitalWrite(LED, LOW);
  lowPowerBoard.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);  
  uint32_t VoltageLine = readVoltageLine(analogRead(ADC_LINE));


#if READ_PIR
  int PIR = analogRead(ADC_PIR);

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
  PIR = PIR * 10.00;
  if (PIR < 0)
    PIR = 0;
  if (PIR > 2000)
    PIR = 1024;
  log("PIR=" + PIR);
#endif

#if READ_IR
  unsigned int IR = averageFilter(ADC_IR,5,IROffset);
  log("IR=");
  log(IR);
#endif

#if READ_LDR
  digitalWrite(LDR_CONNECT, LOW);
  double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
  double RLDR = CALCULATE_RLDR(VLDR, 10000);
  double LUX = LUX_CALC_SCALAR * pow(RLDR, LUX_CALC_EXPONENT);
  log("  LDR=" + (String)analogRead(ADC_LDR));
  log("  LUX=" + (String)LUX);
#endif

#if READ_TEMPERATURE
  digitalWrite(NTC_CONNECT, LOW);
  double celsius = temperature(analogRead(ADC_NTC), 5);
  log("  Temp=" + (String)celsius +"\n");
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
    // log("\n VoltageLine"+(String)voltage);
    return voltage;
  }

  uint32_t averageFilter(uint32_t numberOfPin,uint16_t samplingScale, uint16_t ADCOffset )
  {
    //ToDo: sampling Scale to=> vactor
    //  std::vector<uint16_t> samplingScale[];
    uint64_t buffer=0;
    for (uint16_t numberOfAverage = 0; numberOfAverage < samplingScale-1 ; numberOfAverage++)
    { 
      buffer = buffer +analogRead(numberOfPin);
      _delay_us(100);
    }
    uint32_t result =int(buffer/(samplingScale));
    if(ADCOffset==0){
      return result;
    }
    if(result>ADCOffset) return result-ADCOffset;
    else return result;
      
  }