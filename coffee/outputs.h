#ifndef __outputs_h_
#define __outputs_h_

#include "task.h"
#include "variables.h"

namespace idb {
    // handles mapping wemos I/O to Variable classes
    class Outputs : public Task {
        private:
        Variables _variables;

        public:
        Outputs();
        void Run();
    };
}

#endif