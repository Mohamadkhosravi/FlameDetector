#ifndef MAIN_H_
#define MAIN_H_
#include <LowPower.h>
#include "NTC.h"
#include "arduinoFFT.h"
#include "time.h"
#include "Watchdog.h"



#define Enable 1
#define Disable 0
#define READ_IR             Enable
#define READ_TEMPERATURE    Disable
#define READ_LDR            Disable
#define READ_PIR            Disable
#define FFT_PIR             Disable
#define POWER_DISABLE       Disable
#define DEBUG               Enable   


# if DEBUG
  #define log  Serial1.print
#else
  #define  log  
#endif



#define MINIMUM_RANDOM_DELAY 33 // 33*15ms=~ 0.5s
#define MAXIMUM_RANDOM_DELAY 1000 //1000*15ms=1.5s
#define MINIMUM_OFFSET_VALID 15 //MINIMUM_OFFSET_VALID when IR pins is shortcut
// ADC Chanels
#define ADC_LINE A0
#define ADC_IR A3
#define ADC_PIR A4
#define ADC_LDR PC5
#define ADC_NTC A2
#define CALIBRATION_PIN  DD2
// 
#define LDR_CONNECT PD5
#define NTC_CONNECT PD4
#define ADC_LINE_CONNECT A1




#define LED  8
#define currentPin

#define CALCULATE_VLDR(ADCLDR)(ADCLDR/(float)ADC_10bit )*5
#define CALCULATE_RLDR(VLDR, ResPullDown) ((5/VLDR-1)*10000)
#define CHANNEL ADC_PIR


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define TRESHOLT_IR_FIER 7
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define MINIMUM_VOLTAGE_LINE_VALID 9



/*=====================================================================*/
/* FFT Parameters and Setting*/
/*=====================================================================*/
const uint16_t samples = 8; //This value MUST ALWAYS be a power of 2
const float samplingFrequency = 1; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us;
unsigned long microseconds;
bool firstTurnON=false;
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

/*=====================================================================*/
/* LUX Parameters */
/*=====================================================================*/
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
/*=====================================================================*/
/*  Fier Detection Parameters*/
/*=====================================================================*/
bool FIER=false;
uint32_t FIERCounter=0;
uint8_t BlinkerTimer=0;
uint32_t debounce=0;
uint8_t IROffset=0;
bool calibrationState=false;
    #define ADDRESS_CALIBRATION_STATE 0
    #define ADDRESS_OFFSET_Value 20
#endif