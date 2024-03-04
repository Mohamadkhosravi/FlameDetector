#ifndef MAIN_H_
#define MAIN_H_


#define ADC_IR PC3
#define ADC_PIR PD3
#define ADC_LDR PC5
#define ADC_NTC PC2
#define LDR_CONNECT PD5
#define NTC_CONNECT PD4

#define SENSOR_PIN             ADC_NTC
#define REFERENCE_RESISTANCE   10000
#define NOMINAL_RESISTANCE     10000
#define NOMINAL_TEMPERATURE    25
#define B_VALUE                3950
#define ANALOG_RESOLUTION      1024

#define CALCULATE_VLDR(ADCLDR)((5.0/1024) * ADCLDR )
#define CALCULATE_RLDR(VLDR, ResPullDown) (((5.0)-(VLDR))*ResPullDown /VLDR)



#endif