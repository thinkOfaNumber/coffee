#ifndef __settings_h_
#define __settings_h_

#include <pins_arduino.h>;

// setup I/O etc. to use better names

// WeMos D1 mini pins:

// #define LED_BUILTIN 2
// static const uint8_t D0   = 16;
// static const uint8_t D1   = 5;
// static const uint8_t D2   = 4;
// static const uint8_t D3   = 0;
// static const uint8_t D4   = 2;
// static const uint8_t D5   = 14;
// static const uint8_t D6   = 12;
// static const uint8_t D7   = 13;
// static const uint8_t D8   = 15;
// static const uint8_t RX   = 3;
// static const uint8_t TX   = 1;

// input
static const uint8_t GROUP_SWITCH = D0;
static const uint8_t TANK_WATER = D1;
static const uint8_t BOILER_WATER = D2;
static const uint8_t AT_PRESSURE = D3;
// output
static const uint8_t ELEMENT = D4;
static const uint8_t PUMP = D5;
static const uint8_t SOLENOID = D6;

#endif
