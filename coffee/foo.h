#ifndef __iomapper_h_
#define __iomapper_h_

#include "variable.cpp"
#include "coffee.h"

namespace idb {
    // handles mapping wemos I/O to Variable classes
    class IOMapper {
        private:
        // inputs
        Variable<bool> *_atPressure;
        Variable<bool> *_groupSwitch;
        Variable<bool> *_tankWater;
        Variable<bool> *_boilerWater;
        // outputs
        Variable<bool> *_pump;
        Variable<bool> *_solenoid;
        Variable<bool> *_element;

        public:
        IOMapper();
        void ReadInputs(IO &io);
        void SetOutputs(IO &io);
    };
}

#endif