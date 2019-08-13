#include <Arduino.h>
#include "timer.h"

using namespace idb;

bool Timer::Expired() {
   return millis() > expireTime;
}

void Timer::StartOnce(unsigned long length) {
  if (started)
    return;
  expireTime = millis() + length;
  started = true;
}

void Timer::Reset() { started = false; }
