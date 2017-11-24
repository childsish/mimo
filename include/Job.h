/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"

namespace mimo {

    class Job {
    public:

        Step &step;
        Inputs &ins;
        Outputs &outs;

        bool completed;

        Job(Step &step, Inputs &ins, Outputs &outs);

        void run();

    private:

    };
}

#endif //MIMO_JOB_H
