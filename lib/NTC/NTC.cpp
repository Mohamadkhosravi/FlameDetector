#include "NTC.h"
#include <Arduino.h>

#ifdef _USE_MATH_H
// If _USE_MATH_H is defined, no additional code is needed since math.h is used.
#else
// If math.h is not used, a custom log function is provided.

// Custom natural logarithm function to calculate the natural logarithm of a number.
// This is used if math.h is not available.
double custom_log(double x) {
    int n = 0;

    if (x <= 0) {
        return -1.0; // Logarithm is not defined for non-positive numbers
    }

    float epsilon = 1e-10; // A small value for accuracy
    float result = 0.0;
    float term = (x - 1) / (x + 1);
    float term_squared = term * term;
    float term_squared_power = term_squared;
    float current_term = term;

    for (n = 1; current_term > epsilon; n++) {
        result += current_term;
        current_term = term_squared_power * term;
        term_squared_power *= term_squared;
        current_term /= 2 * n + 1;
    }

    return 2 * result;
}
#endif

// Function to calculate the temperature in Celsius from an ADC value and VDD voltage.
float temperature(int ADCValue, float VDD) {
    // Calculate the voltage across the NTC using the provided macros/functions.
    float VNTC = CALCULATE_VNTC(ADCValue, ADCNumerOfbits, VDD);
    // Calculate the resistance of the NTC using the provided macros/functions.
    float RNTC = CALCULATE_RNTC(VNTC, VDD, RES_PULLDOWN_NTC);

    // Calculate the natural logarithm of the resistance.
    float logResistance = LOG_FUNCTION(RNTC * VDD);

    // Calculate the temperature in Celsius using a complex formula.
    // The formula is of the form 1 / (A + B * ln(RNTC) + C * ln(RNTC)^3) - 273.15.
    float temperatureCelsius = 1.0 / (A + B * logResistance + C * logResistance * logResistance * logResistance) - 273.15;
    
    // The temperature is returned in units of 0.1 degrees Celsius.
    return temperatureCelsius / 10;
}