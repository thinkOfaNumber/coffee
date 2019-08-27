#ifndef __settings_h_
#define __settings_h_

#include <pins_arduino.h>;

// setup I/O etc. to use better names

// WeMos D1 mini pins:
// D0   = 16;
// D1   = 5;
// D2   = 4;
// D3   = 0; 10k pullup (i.e. close to ground) LOW=bootloader
// D4   = 2; 10k pullup (i.e. close to ground) LED_BUILTIN
// D5   = 14;
// D6   = 12;
// D7   = 13;
// D8   = 15; 10k pulldown (i.e. close to Vcc 3.3)
// RX   = 3;
// TX   = 1;

// input
static const uint8_t GROUP_SWITCH = D7;
static const uint8_t TANK_WATER = D3;
static const uint8_t BOILER_WATER = D2;
// can't use D3 for input as low == bootloader
static const uint8_t AT_PRESSURE = D5;
// output
static const uint8_t ELEMENT = D4; // element at D4 with LED_BUILTIN
static const uint8_t PUMP = D1;
static const uint8_t SOLENOID = D6;

#endif
