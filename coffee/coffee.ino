#include <Arduino.h>

#include "timer.h"
#include "coffee.h"
#include "settings.h"
#include "iomapper.h"
#include "ewifi.h"

// https://www.arduino.cc/en/Reference/Libraries
extern "C" {
#include "user_interface.h"
}


using namespace idb;

Timer *controlLoop;
Timer *debugLoop;
Coffee *controller;
IOMapper *ioMapper;
eWiFi *ewifi;
struct IO io;

void setup() {
  // put your setup code here, to run once:

  pinMode(GROUP_SWITCH, INPUT_PULLUP);
  pinMode(TANK_WATER, INPUT_PULLUP);
  pinMode(BOILER_WATER, INPUT_PULLUP);
  pinMode(AT_PRESSURE, INPUT_PULLDOWN_16); // only this GPIO16/D0 has internal pulldown

  pinMode(ELEMENT, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(SOLENOID, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  controlLoop = new Timer();
  debugLoop = new Timer();
  controller = new Coffee();
  ioMapper = new IOMapper();
  ewifi = new eWiFi();

  Serial.print("Connecting to ");
  Serial.println(ewifi->GetSsid());
  ewifi->Setup();

  Serial.printf("Setup finished...\n");
}

void printState(struct IO io) {
  uint32_t free = system_get_free_heap_size();
  Serial.printf("Group %d; Tank %d; BoilerLevel %d; Pressure %d\n", io.GroupSwitch, io.TankWater, io.BoilerWater, io.AtPressure);
  Serial.printf("Element %d; Pump %d; Solenoid %d\n", io.Element, io.Pump, io.Solenoid);
  Serial.printf("Free mem %d\n", free);
}

void loop() {
  // put your main code here, to run repeatedly:

  controlLoop->StartOnce(100);
  debugLoop->StartOnce(2000);
  if (controlLoop->ExpiredRunReset()) {
    ioMapper->ReadInputs(io);
    controller->SetInputs(io);
    controller->Run();
    ewifi->Run();
    controller->GetOutputs(io);
    ioMapper->SetOutputs(io);
  }

  if (debugLoop->ExpiredRunReset()) {
    printState(io);
    ewifi->PrintLog();
  }
}
