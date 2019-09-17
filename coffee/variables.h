#ifndef __variables_h_
#define __variables_h_

#include "variable.cpp"

namespace idb {

class Variables {
    public:
    // inputs
    static Variable<bool> AtPressure;
    static Variable<bool> GroupSwitch;
    static Variable<bool> TankWater;
    static Variable<bool> BoilerWater;
    // outputs
    static Variable<bool> Pump;
    static Variable<bool> Solenoid;
    static Variable<bool> Element;
};

}
#endif