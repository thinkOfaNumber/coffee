#ifndef __IOMapper_h_
#define __IOMapper_h_

#include "lib/variable.h"
#include "coffee.h"

namespace idb {
    // handles mapping wemos I/O to Variable classes
    class IOMapper {
        private:
        // inputs
        Variable<bool> *AtPressure;
        Variable<bool> *GroupSwitch;
        Variable<bool> *TankWater;
        Variable<bool> *BoilerWater;
        // outputs
        Variable<bool> *Pump;
        Variable<bool> *Solenoid;
        Variable<bool> *Element;

        IO _io;

        public:
        IOMapper();
        void ReadInputs(IO &io);
        void SetOutputs(IO &io);
    };
}

#endif