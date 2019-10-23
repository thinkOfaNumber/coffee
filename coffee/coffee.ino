#include <Arduino.h>

#include "task.h"
#include "timer.h"
#include "coffee.h"
#include "settings.h"
#include "inputs.h"
#include "outputs.h"
#include "ewifi.h"

// https://www.arduino.cc/en/Reference/Libraries
extern "C" {
#include "user_interface.h"
}

using namespace idb;

const int numTasks = 4;
Task *tasks[numTasks] = {
  new Inputs(),
  new Coffee(),
  new eWiFi(),
  new Outputs()
};
Timer controlLoop(100);
Timer debugLoop(1000);
unsigned int cycle = 0;

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

  for (int i = 0; i < numTasks; i++) {
    tasks[i]->Setup();
  }

  Serial.printf("Setup finished...\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  controlLoop.StartOnce();
  debugLoop.StartOnce();

  if (controlLoop.Triggered()) {
    cycle++;
    for (int i = 0; i < numTasks; i++) {
      tasks[i]->Run(cycle);
    }
  }

  if (debugLoop.Triggered()) {
    Serial.print("Debugging ");
    Serial.println(cycle);
    for (int i = 0; i < numTasks; i++) {
      tasks[i]->Debug();
    }
  }
}
