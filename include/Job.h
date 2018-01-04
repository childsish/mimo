/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <workflow/Step.h>
#include <workflow/Input.h>
#include <workflow/Output.h>
#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


namespace mimo {

    class IQueue;
    class IQueueFactory;
    class Step;

    /**
     * @brief:
     */
    class Job {
    public:

        const workflow::Step &identifier;

        Job(const workflow::Step &identifier, Step &step, const IQueueFactory &factory);

        void set_inputs(std::unordered_map<std::string, std::unique_ptr<IQueue>> &inputs);

        void run();

        bool is_complete() const;

    private:

        const IQueueFactory &factory;
        Step &step;

        std::unordered_map<std::string, std::unique_ptr<IQueue>> inputs;
        std::unordered_map<std::string, std::unique_ptr<IQueue>> outputs;

        bool completed;

    };
}

#endif //MIMO_JOB_H
