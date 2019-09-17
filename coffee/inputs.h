#ifndef __iomapper_h_
#define __iomapper_h_

#include "task.h"
#include "variables.h"

namespace idb {
    // handles mapping wemos I/O to Variable classes
    class Inputs : public Task {
        private:
        Variables _variables;

        public:
        Inputs();
        void Run();
    };
}

#endif