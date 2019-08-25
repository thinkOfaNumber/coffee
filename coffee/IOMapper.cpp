#include <Arduino.h>

#include "IOMapper.h"
#include "settings.h"

using namespace idb;

IOMapper::IOMapper() {
  // inputs
  AtPressure = new Variable<bool>();
  GroupSwitch = new Variable<bool>();
  TankWater = new Variable<bool>();
  BoilerWater = new Variable<bool>();
  // outputs
  Pump = new Variable<bool>();
  Solenoid = new Variable<bool>();
  Element = new Variable<bool>();
}

void IOMapper::ReadInputs(IO &io) {
    // pass the actual inputs through the Variable class
    AtPressure->Set(digitalRead(AT_PRESSURE));
    GroupSwitch->Set(digitalRead(GROUP_SWITCH));
    TankWater->Set(digitalRead(TANK_WATER));
    BoilerWater->Set(digitalRead(BOILER_WATER));
    // retun the value from the Variable class in case they are in forced mode
    io.AtPressure = AtPressure->Get();
    io.GroupSwitch = GroupSwitch->Get();
    io.TankWater = TankWater->Get();
    io.BoilerWater = BoilerWater->Get();
}

void IOMapper::SetOutputs(IO &io) {
    // set the Variable classes
    Pump->Set(io.Pump);
    Solenoid->Set(io.Solenoid);
    Element->Set(io.Element);
    // write the Variable class value in case they are in foced mode
    digitalWrite(PUMP, Pump->Get());
    digitalWrite(SOLENOID, Solenoid->Get());
    digitalWrite(ELEMENT, Element->Get());
}