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
#define PIN_PRIME 4

#define PIN_THERM_EXHAUST 0
#define PIN_THERM_WATER 1

#define TEMP_SETPOINT 55
#define TEMP_BAND 5

// Uncomment to use dual water temperature sensors.
#define USE_DUAL_WATER_TEMP 0

#define MIN_TIME_IDLE 30 * 1000
#define MIN_TIME_CIRCULATE 20 * 1000
#define TIME_PRE_IGNITION 5 * 1000
#define TIME_IGNITION 20 * 1000
#define MIN_TIME_POST_IGNITION 20 * 1000
#define MAX_TIME_POST_IGNITION 60 * 1000
#define TIME_BURN 30 * 1000
#define TIME_COOL_DOWN 30 * 1000
#define TIME_FAIL 15 * 60 * 1000

#define MAX_FAILS 3

// Advanced configuration
// Chinese sensors
#define THERM_SH_COEFF_A 0.00017540525
#define THERM_SH_COEFF_B 0.00031471521
#define THERM_SH_COEFF_C -0.0000001788484

// webasto NTC
// #define THERM_SH_COEFF_A 0.001080501848
// #define THERM_SH_COEFF_B 0.0002827971224
// #define THERM_SH_COEFF_C 0.0000000748981639

#define THERM_SH_EXHAUST_A 0.0007798429504
#define THERM_SH_EXHAUST_B 0.0002064686006
#define THERM_SH_EXHAUST_C 0.0000001291708513

#define BIAS_RESISTOR_OHMS 20000

#define MODE_CHANGE_DELAY 3000

#define EXHAUST_LIT_DELTA 3

#define PID_Kp 0.8
#define PID_DT 5000
#define FUEL_MAX_HZ 4.0
#define FUEL_MIN_HZ 2.0

#endif //INTELLIBASTO_CONFIG_H
