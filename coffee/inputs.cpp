#include <Arduino.h>

#include "inputs.h"
#include "settings.h"

using namespace idb;

Inputs::Inputs() : _variables() { }

void Inputs::Run(unsigned int cycle) {
    // pass the actual inputs through the Variable class
    _variables.AtPressure._value = digitalRead(AT_PRESSURE);
    _variables.GroupSwitch._value = !digitalRead(GROUP_SWITCH);
    _variables.TankWater._value = !digitalRead(TANK_WATER);
    _variables.BoilerWater._value = !digitalRead(BOILER_WATER);
}
