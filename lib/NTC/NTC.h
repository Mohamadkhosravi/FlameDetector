
#ifndef NTC_H
#define NTC_H


// if you dont want used "math.h" you can comment next line >> " //#define _USE_MATH_H "
#define _USE_MATH_H

#ifdef _USE_MATH_H

	#include <math.h>
    #define LOG_FUNCTION log

#else
	double custom_log(double x);
	#define LOG_FUNCTION custom_log
	
#endif


#define RES_PULLDOWN_NTC 10 //resistor Pull Down with NTC ; if resistor Pull Down is(10000ohm/10k )value is ==> 10 
#define RES_PULLUP_NTC 0    //resistor pull up with NTC;if resistor Pull Up is(10000ohm/10k )value is ==> 10 
//#define VDD 3.3 

#define  ADC_12bit 4095
#define  ADC_10bit 1023
#define  ADC_8bit 255

//select  ADC resloation
#define ADCNumerOfbits  ADC_10bit

// if your NTC is pull up difinid _NTC_IS_PULLUP_ else if your NTC is pull down  comment next line//#define _NTC_IS_PULLUP_
#define _NTC_IS_PULLUP_

#define CALCULATE_VNTC(ADC_NTC , ADCNumerOfbits , VCC)( (VCC/ADCNumerOfbits) * ADC_NTC )

#ifdef _NTC_IS_PULLUP_

	#define _NTC_IS_PULLUP_
	#define CALCULATE_RNTC(VNTC, VCC, ResPullDown) (((VCC)-(VNTC))*ResPullDown /VNTC)

#else	
	#define CALCULATE_VNTC(VNTC, VCC, RES_PULLUP_NTC)( (RES_PULLUP_NTC * VCC / VNTC) - RES_PULLUP_NTC)	*/
#endif



/*  3 point from your ntc in different temperatures , you can use from
https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
*/
	/*
	#define  A  0.001129148
	#define  B  0.000234125
	#define  C  0.0000000876741*/
	
	#define  A  0.001277368 //    T1=-30^C   R1=154882kR
	#define  B  0.000208223 //    T2=25^C    R2=10000KR
	#define  C  0.0000002032989// T3=80^C R3=1228KR

	float temperature(int ADCValue, float VDD);
   

#endif