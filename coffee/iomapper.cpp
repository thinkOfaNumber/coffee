#include <Arduino.h>

#include "iomapper.h"
#include "settings.h"

using namespace idb;

IOMapper::IOMapper() {
  // inputs
  _atPressure = new Variable<bool>();
  _groupSwitch = new Variable<bool>();
  _tankWater = new Variable<bool>();
  _boilerWater = new Variable<bool>();
  // outputs
  _pump = new Variable<bool>();
  _solenoid = new Variable<bool>();
  _element = new Variable<bool>();
}

void IOMapper::ReadInputs(IO &io) {
    // pass the actual inputs through the Variable class
    _atPressure->Set(digitalRead(AT_PRESSURE));
    _groupSwitch->Set(!digitalRead(GROUP_SWITCH));
    _tankWater->Set(!digitalRead(TANK_WATER));
    _boilerWater->Set(!digitalRead(BOILER_WATER));
    // retun the value from the Variable class in case they are in forced mode
    io.AtPressure = _atPressure->Get();
    io.GroupSwitch = _groupSwitch->Get();
    io.TankWater = _tankWater->Get();
    io.BoilerWater = _boilerWater->Get();
}

void IOMapper::SetOutputs(IO &io) {
    // set the Variable classes
    _pump->Set(io.Pump);
    _solenoid->Set(io.Solenoid);
    _element->Set(io.Element);
    // write the Variable class value in case they are in forced mode
    digitalWrite(PUMP, _pump->Get());
    digitalWrite(SOLENOID, _solenoid->Get());
    digitalWrite(ELEMENT, _element->Get());
}