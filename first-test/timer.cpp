#include <Arduino.h>
#include "timer.h"

using namespace idb;

bool Timer::Expired() {
   return millis() > expireTime;
}

void Timer::StartOnce(unsigned long milliSeconds) {
  if (started)
    return;
  expireTime = millis() + milliSeconds;
  started = true;
}

void Timer::Reset() { started = false; }
