#ifndef __settings_h_
#define __settings_h_

#include <pins_arduino.h>;

// setup I/O etc. to use better names than D0, D1, ...

// notes: GPIO0 (D3) 
static const uint8_t AT_PRESSURE  = D0; // IC pull-down; input
//                   display      = D1; // display SCL - not configurable; (relay default)
//                   display      = D2; // display SDA - not configurable
static const uint8_t PUMP         = D3; // 10k pullup; output only because LOW=bootloader
static const uint8_t ELEMENT      = D4; // 10k pullup; output; LED_BUILTIN
static const uint8_t TANK_WATER   = D5; // input; needs pullup due to Giotto wiring
static const uint8_t BOILER_WATER = D6; // input
static const uint8_t GROUP_SWITCH = D7; // input
static const uint8_t SOLENOID     = D8; // 10k pulldown; output only (bootloader)

#endif
