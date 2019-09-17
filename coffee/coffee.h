#ifndef __coffee_h_
#define __coffee_h_

#include "variables.h"
#include "task.h"

namespace idb {

class Coffee : public Task {
private:
  Variables _variables;
  // inputs
  bool _atPressure;
  bool _groupSwitch;
  bool _tankWater;
  bool _boilerWater;
  // outputs
  bool _pump;
  bool _solenoid;
  bool _element;

public:
  Coffee();
  void Run();
  void Debug() const;
  void MapIn();
  void MapOut();
};
} // namespace idb

#endif
