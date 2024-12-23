# FlameDetector
Flame Detector
# Sensor System Documentation

This document explains the implementation and functionality of the provided sensor system code. Each section of the code is described in detail to facilitate understanding, maintenance, and potential enhancements.

---

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Dependencies](#dependencies)
4. [Pin Configuration](#pin-configuration)
5. [Macro Definitions](#macro-definitions)
6. [Setup Function](#setup-function)
7. [Loop Function](#loop-function)
8. [Utility Functions](#utility-functions)
9. [Future Improvements](#future-improvements)

---

## Overview
The sensor system is a low-power embedded application designed for environmental monitoring. It incorporates calibration mechanisms, power-saving features, and sensor data acquisition to detect parameters like light intensity, temperature, motion, and fire risk. The application ensures reliable operation through EEPROM-stored configuration and random delay techniques.

---

## Features
- **Sensor Calibration**: Manual calibration with EEPROM storage.
- **Randomized Startup Delay**: Reduces simultaneous current draw when multiple devices are powered on.
- **Low-Power Modes**: Utilizes sleep modes to conserve energy.
- **Sensor Support**: Compatible with light (LDR), temperature (NTC), motion (PIR), and IR sensors.
- **Fire Detection**: Implements thresholds and counters to monitor fire conditions.

---

## Dependencies

```cpp
#include <Arduino.h>
#include <main.hpp>
#include <avr/power.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
```

- **Arduino.h**: Provides core Arduino functions and macros.
- **main.hpp**: Includes application-specific constants and definitions.
- **avr/power.h**: Power management library for AVR-based boards.
- **EEPROM.h**: Manages non-volatile memory storage.
- **SoftwareSerial.h**: Enables serial communication on arbitrary digital pins.

---

## Pin Configuration

### Serial Communication
```cpp
SoftwareSerial Serial1(A5, A2);
```
Defines `Serial1` for debugging via pins A5 (RX) and A2 (TX).

### Sensor and System Pins
- **LED**: Visual indicator for system status.
- **ADC_LINE_CONNECT**: Enables ADC for line voltage measurement.
- **CALIBRATION_PIN**: Trigger for manual calibration.

### Additional Sensor Pins
- **ADC_IR**: Pin for the IR sensor used in fire detection.
- **ADC_LDR**: Pin for the LDR sensor measuring light intensity.
- **ADC_NTC**: Pin for the NTC temperature sensor.
- **ADC_PIR**: Pin for the PIR motion sensor.

### Conditional Pins (Based on Compile-Time Flags)
- `LDR_CONNECT`: Controls the light sensor.
- `NTC_CONNECT`: Activates the temperature sensor.

---

## Macro Definitions

The `main.hpp` file defines several macros that configure the behavior of the system. Users can modify these macros to adjust system functionality without altering the main code. Below is a detailed explanation of key macros:

### Sensor Macros
```cpp
#define LDR_CONNECT    PORTA7
#define NTC_CONNECT    PORTA6
```
- **LDR_CONNECT**: Pin connected to the light-dependent resistor (LDR).
- **NTC_CONNECT**: Pin connected to the negative temperature coefficient (NTC) sensor.
- **Customization**: Update these macros to reflect the actual pins used in your hardware configuration.

### Fire Detection Threshold
```cpp
#define THRESHOLD_IR_FIRE   300
#define MAXIMUM_REPEAT_FIRE 5
```
- **THRESHOLD_IR_FIRE**: ADC threshold value below which fire is detected.
- **MAXIMUM_REPEAT_FIRE**: Number of consecutive readings required to confirm a fire.
- **Customization**: Adjust these values to match the sensitivity and environment of your application.

### Random Delay Parameters
```cpp
#define MINIMUM_RANDOM_DELAY   100
#define MAXIMUM_RANDOM_DELAY   1000
```
- **MINIMUM_RANDOM_DELAY**: Minimum delay time (in milliseconds).
- **MAXIMUM_RANDOM_DELAY**: Maximum delay time (in milliseconds).
- **Customization**: Modify these values to control the range of startup delays.

### ADC Configuration
```cpp
#define ADC_10bit   1024
#define VCC         3.3
```
- **ADC_10bit**: Resolution of the ADC (10-bit by default).
- **VCC**: Voltage reference for ADC readings.
- **Customization**: Change these values to match your microcontroller's ADC resolution and supply voltage.

### Debugging
```cpp
#define DEBUG   1
```
- **DEBUG**: Enables or disables debugging output.
- **Customization**: Set to `0` to disable debugging for production.

---

## Setup Function

The `setup()` function initializes the hardware components, EEPROM, and software serial communication. It also prepares the system for sensor calibration and power-saving operations.

### Key Steps

#### Pin Initialization
```cpp
pinMode(LED, OUTPUT);
pinMode(ADC_LINE_CONNECT, OUTPUT);
pinMode(CALIBRATION_PIN, INPUT_PULLUP);
```
Configures LED as an output, ADC_LINE_CONNECT as an output, and CALIBRATION_PIN with an internal pull-up resistor.

#### Debugging Setup
```cpp
#if DEBUG
    Serial.begin(9600);
    Serial1.begin(9600);
#endif
```
Initializes serial communication for debugging if the `DEBUG` flag is enabled.

#### Random Seed Setup
```cpp
randomSeed(analogRead(A6));
```
Improves randomness for the startup delay by using an analog pin value.

#### EEPROM Initialization
```cpp
EEPROM.begin();
calibrationState = bool(EEPROM.read(0));
if (calibrationState) IROffset = EEPROM.read(20);
```
Retrieves the calibration state and offset from EEPROM for consistent operation across power cycles.

---

## Loop Function

The `loop()` function handles sensor data collection, calibration, and system power management. It ensures efficient operation by incorporating conditional logic and power-saving techniques.

### Key Sections

#### Calibration Routine

The calibration routine enables the user to reset sensor parameters when necessary.

```cpp
while (calibrationState) {
    if (!digitalRead(CALIBRATION_PIN)) {
        // Debounced calibration button press
        delay(50);
        if (!digitalRead(CALIBRATION_PIN)) {
            // Perform calibration and save state
            IROffset = char(averageFilter(ADC_IR, 10, 0));
            EEPROM.update(ADDRESS_CALIBRATION_STATE, 0);
            EEPROM.write(ADDRESS_OFFSET_Value, IROffset);
            calibrationState = false;
        }
    }
}
```
- Detects button presses for calibration.
- Updates EEPROM with the new calibration data.

#### Randomized Startup Delay
```cpp
uint16_t randDelay = random(MINIMUM_RANDOM_DELAY, MAXIMUM_RANDOM_DELAY);
for (uint16_t repeat = 0; repeat <= randDelay; repeat++) {
    lowPowerBoard.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
}
```
- Introduces a delay to distribute current draw evenly across devices.
- Uses sleep mode to minimize power consumption during the delay.

#### Sensor Data Processing

1. **Voltage Line Reading**
   ```cpp
   uint32_t VoltageLine = readVoltageLine(analogRead(ADC_LINE));
   ```
   Converts ADC readings to voltage.

2. **PIR Sensor**
   ```cpp
   int PIR = analogRead(ADC_PIR);
   ```
   Measures motion intensity.

3. **LDR Sensor**
   ```cpp
   double VLDR = CALCULATE_VLDR(analogRead(ADC_LDR));
   ```
   Calculates light intensity using ADC readings.

4. **Temperature Sensor**
   ```cpp
   double celsius = temperature(analogRead(ADC_NTC), 5);
   ```
   Converts ADC values to temperature in Celsius.

#### Fire Detection

The system monitors IR sensor values to detect fire conditions and triggers alarms if necessary.

```cpp
if (IR < THRESHOLD_IR_FIRE) {
    FIRE = true;
    FIRECounter++;
} else {
    FIRE = false;
    if (FIRECounter > 0) FIRECounter--;
}

if (FIRECounter > MAXIMUM_REPEAT_FIRE) {
    while (1) {
        VoltageLine = readVoltageLine(analogRead(ADC_LINE));
        if (VoltageLine < MINIMUM_VOLTAGE_LINE_VALID) {
            FIRECounter = 0;
            FIRE = false;
            break;
        }
    }
}
```

---

## Utility Functions

### `readVoltageLine`

Converts ADC readings into voltage values based on a voltage divider circuit.

```cpp
uint32_t readVoltageLine(uint32_t ADCValue) {
    uint32_t R1 = 220;
    uint32_t R2 = 22;
    uint16_t VCC = 3;
    digitalWrite(ADC_LINE_CONNECT, LOW);
    uint32_t voltage = (R1 + R2) / R2 * ADCValue * VCC / ADC_10bit;
    return voltage;
}
```

### `averageFilter`

Reduces noise in ADC readings by averaging multiple samples.

```cpp
uint32_t averageFilter(uint32_t numberOfPin, uint16_t samplingScale, uint16_t ADCOffset) {
    uint64_t buffer = 0;
    for (uint16_t numberOfAverage = 0; numberOfAverage < samplingScale - 1; numberOfAverage++) {
        buffer += analogRead(numberOfPin);
        _delay_us(100);
    }
    uint32_t result = int(buffer / samplingScale);
    return (ADCOffset == 0) ? result : max(result - ADCOffset, 0);
}
```

#### Noise Reduction
The averaging process reduces random fluctuations in ADC readings, improving the accuracy of measurements.

#### Delay Justification
A small delay between samples ensures stability and minimizes interference, particularly for sensors sensitive to transient signals.

---

## Future Improvements
- Optimize power management by dynamically adjusting sleep durations.
- Implement adaptive thresholds for fire detection based on environmental conditions.
- Enhance calibration by providing feedback to users during the process.
- Extend support for additional sensor types.

---
