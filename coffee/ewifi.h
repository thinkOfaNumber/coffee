#ifndef __ewifi_h_
#define __ewifi_h_

namespace idb {

class eWiFi {
private:

public:
    void Setup();
    const char *GetSsid() const;
    void PrintLog() const;
};

} // namespace idb
#endif