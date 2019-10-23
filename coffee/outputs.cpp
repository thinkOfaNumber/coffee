#include <Arduino.h>

#include "outputs.h"
#include "settings.h"

using namespace idb;

Outputs::Outputs() : _variables() { }

void Outputs::Run(unsigned int cycle) {
    // pass the output through the Variable class via a getter in case they're forced
    digitalWrite(PUMP, _variables.Pump.Get());
    digitalWrite(SOLENOID, _variables.Solenoid.Get());
    digitalWrite(ELEMENT, _variables.Element.Get());
}