#include "coffee.h"

using namespace idb;

Coffee::Coffee() {
    _pump = false;
    _solenoid = false;
    _element = false;
}

void Coffee::Run() {
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
}

void Coffee::SetInputs(struct IO io) {
    _atPressure = io.AtPressure;
    _groupSwitch = io.GroupSwitch;
    _tankWater = io.TankWater;
    _boilerWater = io.BoilerWater;
}

void Coffee::GetOutputs(struct IO io) {
    io.Element = _element;
    io.Pump = _pump;
    io.Solenoid = _solenoid;
}