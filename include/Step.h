#ifndef MIMO_STEP_H
#define MIMO_STEP_H

#include <string>
#include <unordered_map>
#include <queues/Inputs.h>
#include <queues/Outputs.h>


namespace mimo {

    class Step {
    public:

        /**
         * Run the step. Returns true if the step completed running.
         * @param ins input entities
         * @param outs output entities
         * @return step completed running
         */
        virtual bool run(Inputs &ins, Outputs &outs) = 0;

    };
}

#endif //MIMO_STEP_H
