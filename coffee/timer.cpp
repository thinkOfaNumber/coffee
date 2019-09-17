#include <Arduino.h>
#include "timer.h"

using namespace idb;

Timer::Timer(unsigned long milliSeconds) {
  time = milliSeconds;
}

bool Timer::Triggered() {
   bool triggered = started && millis() > expireTime;
   if (triggered) {
    expireTime = millis() + time;
   }
   return triggered;
}

void Timer::StartOnce() {
  if (started)
    return;
  expireTime = millis() + time;
  started = true;
}
