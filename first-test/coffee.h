#ifndef __coffee_h_
#define __coffee_h_

namespace idb {
struct IO {
  // inputs
  bool AtPressure;
  bool GroupSwitch;
  bool TankWater;
  bool BoilerWater;
  // outputs
  bool Pump;
  bool Solenoid;
  bool Element;
};

class Coffee {
private:
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
  void SetInputs(struct IO);
  void GetOutputs(struct IO );
};
} // namespace idb

#endif
