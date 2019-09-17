#ifndef __timer_h_
#define __timer_h_

namespace idb {
class Timer {

private:
  unsigned long expireTime;
  bool started = false;
  unsigned long time;

public:
  Timer(unsigned long milliSeconds);
  bool Triggered();
  void StartOnce();
};

} // namespace idb
#endif