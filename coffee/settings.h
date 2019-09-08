#ifndef __settings_h_
#define __settings_h_

#include <pins_arduino.h>;

// setup I/O etc. to use better names

//                                = D0; // damaged?
//                   display      = D1; // display SCL - not configurable; (relay default)
//                   display      = D2; // display SDA - not configurable
// static const uint8_t PUMP      = D3; // 10k pullup; output only because LOW=bootloader
static const uint8_t ELEMENT      = D4; // 10k pullup; output; LED_BUILTIN
static const uint8_t AT_PRESSURE  = D5; // input
static const uint8_t BOILER_WATER = D6; // input
static const uint8_t GROUP_SWITCH = D7; // input
static const uint8_t TANK_WATER   = D8; // 10k pulldown; input

// eventually these will be changed to fit above:
static const uint8_t PUMP         = D1; // output; wemos relay default
static const uint8_t SOLENOID     = D2; // output

#endif
