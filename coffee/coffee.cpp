#include <Arduino.h>
#include <user_interface.h>
#include "coffee.h"

using namespace idb;

Coffee::Coffee() : _variables() {}

void Coffee::Run(unsigned int cycle) {
    MapIn();
    // 5.1
    _element = !_atPressure;

    _pump = false;
    _solenoid = false;
    if (_groupSwitch) { // group has priority over boiler fill
        // 5.4 a, b
        _pump = true;
    }
    else if (!_boilerWater) {
        // 5.3 b, c
        _pump = true;
        _solenoid = true;
    }

    // protection 5.2
    if (!_tankWater) {
        _pump = false;
    }
    // protection 5.3 a
    if (!_boilerWater) {
        _element = false;
    }

    MapOut();
}

void Coffee::Debug() const {
  uint32_t free = system_get_free_heap_size();
  Serial.printf("Group %d; Tank %d; BoilerLevel %d; Pressure %d\n", _groupSwitch, _tankWater, _boilerWater, _atPressure);
  Serial.printf("Element %d; Pump %d; Solenoid %d\n", _element, _pump, _solenoid);
  Serial.printf("Free mem %d\n", free);
}

void Coffee::MapIn() {
    _atPressure = _variables.AtPressure.Get();
    _groupSwitch = _variables.GroupSwitch.Get();
    _tankWater = _variables.TankWater.Get();
    _boilerWater = _variables.BoilerWater.Get();
}

void Coffee::MapOut() {
    _variables.Element.Set(_element);
    _variables.Pump.Set(_pump);
    _variables.Solenoid.Set(_solenoid);
}