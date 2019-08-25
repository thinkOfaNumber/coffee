#ifndef __timer_h_
#define __timer_h_

namespace idb {
class Timer {

private:
  unsigned long expireTime;
  bool started = false;

public:
  bool Expired();
  void StartOnce(unsigned long milliSeconds);
  void Reset();
  bool ExpiredRunReset();
};

} // namespace idb
#endif