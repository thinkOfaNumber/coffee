#include <Arduino.h>

#include "timer.h";
#include "coffee.h";
#include "settings.h";

// https://www.arduino.cc/en/Reference/Libraries

using namespace idb;

Timer *controlLoop;
Timer *debugLoop;
Coffee *controller;
struct IO io;

void setup() {
  // put your setup code here, to run once:

  pinMode(GROUP_SWITCH, INPUT);
  pinMode(TANK_WATER, INPUT);
  pinMode(BOILER_WATER, INPUT);
  pinMode(AT_PRESSURE, INPUT);

  pinMode(ELEMENT, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(SOLENOID, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();

  controlLoop = new Timer();
  debugLoop = new Timer();
  controller = new Coffee();
  
  Serial.printf("Setup finished...\n");
}

void printState(struct IO io) {
  Serial.printf("Group %d; Tank %d; BoilerLevel %d; Pressure %d\n", io.GroupSwitch, io.TankWater, io.BoilerWater, io.AtPressure);
  Serial.printf("Element %d; Pump %d; Solenoid %d\n", io.Element, io.Pump, io.Solenoid);
}

void loop() {
  // put your main code here, to run repeatedly:

  controlLoop->StartOnce(100);
  debugLoop->StartOnce(2000);
  if (controlLoop->Expired()) {
    controlLoop->Reset();
    io.GroupSwitch = digitalRead(GROUP_SWITCH);
    io.TankWater = digitalRead(TANK_WATER);
    io.BoilerWater = digitalRead(BOILER_WATER);
    io.AtPressure = digitalRead(AT_PRESSURE);
    
    controller->SetInputs(io);
    controller->Run();
    controller->GetOutputs(io);

    digitalWrite(PUMP, io.Pump);
    digitalWrite(SOLENOID, io.Solenoid);
    digitalWrite(ELEMENT, io.Element);
  }

  if (debugLoop->Expired()) {
    debugLoop->Reset();
    printState(io);
  }
}
