#include <Arduino.h>
#include <pins_arduino.h>;

#include "timer.h";
#include "coffee.h";

// https://www.arduino.cc/en/Reference/Libraries

using namespace idb;

Timer *timer;
unsigned long interval = 1000;
unsigned long duty = 4000;
int cycles = 4;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Serial.begin(115200);
  Serial.println();

  timer = new Timer();
  
  Serial.printf("Setup finished...\n");
}

void toggleLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void loop() {
  // put your main code here, to run repeatedly:

  timer->StartOnce(interval);
  if (timer->Expired()) {
    toggleLed();
    timer->Reset();
    if (--cycles == 0) {
      interval = interval / 2;
      cycles = duty / interval;

      if (interval < 60) {
        interval = 1000;
        cycles = 4;
      }
    }
  }
}
