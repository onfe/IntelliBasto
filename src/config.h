#ifndef INTELLIBASTO_CONFIG_H
#define INTELLIBASTO_CONFIG_H

// Pins
#define PIN_MATRIX 10
#define PIN_FUEL 9
#define PIN_WATER 6
#define PIN_FAN 5
#define PIN_GLOW 3

#define PIN_RFH_1 7
#define PIN_RFH_2 8

#define PIN_THERM_EXHAUST 0
#define PIN_THERM_WATER_A 1
#define PIN_THERM_WATER_B 2

// Uncomment to use dual water temperature sensors.
#define USE_DUAL_WATER_TEMP 1

// Advanced configuration
#define ANALOG_READ_RESOLUTION 16

#define THERM_SH_COEFF_A 0.00017540525
#define THERM_SH_COEFF_B 0.00031471521
#define THERM_SH_COEFF_C -0.0000001788484

#define BIAS_RESISTOR_OHMS 20000

#define MODE_CHANGE_DELAY 3000

#endif //INTELLIBASTO_CONFIG_H
