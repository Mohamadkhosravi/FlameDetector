#ifndef MAIN_H_
#define MAIN_H_
#include <LowPower.h>
#include "NTC.h"
#include "arduinoFFT.h"
#include "time.h"
#include "Watchdog.h"
#define ADC_LINE A0
#define ADC_IR A3
#define ADC_PIR A4
#define ADC_LDR PC5
#define ADC_NTC A2
#define LDR_CONNECT PD5
#define NTC_CONNECT PD4
#define SENSOR_PIN             ADC_NTC
#define REFERENCE_RESISTANCE   10000
#define NOMINAL_RESISTANCE     10000
#define NOMINAL_TEMPERATURE    25
#define B_VALUE                3950
#define ANALOG_RESOLUTION      1024
#define LED  8
#define CALCULATE_VLDR(ADCLDR)(ADCLDR/(float)1023 )*5
#define CALCULATE_RLDR(VLDR, ResPullDown) ((5/VLDR-1)*10000)

#define CHANNEL ADC_PIR

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define TRESHOLT_IR_FIER 10
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

unsigned int LineVoltage=0;


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
unsigned int FIERCounter=0;
unsigned char BlinkerTimer=0;

#endif