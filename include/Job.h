/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <memory>


namespace workflow {
    class Step;
}

namespace mimo {

    class IJobInputs;
    class IJobOutputs;
    class Step;

    /**
     * @brief:
     */
    class Job {
    public:

        const std::shared_ptr<workflow::Step> identifier;

        Job(
            const std::shared_ptr<workflow::Step> identifier,
            std::unique_ptr<Step> step,
            std::unique_ptr<IJobInputs> inputs,
            std::unique_ptr<IJobOutputs> outputs
        );

        std::unique_ptr<IJobInputs> &ins();

        std::unique_ptr<IJobOutputs> &outs();

        void run();

    private:

        bool completed;

        std::unique_ptr<mimo::Step> step;

        std::unique_ptr<IJobInputs> inputs;
        std::unique_ptr<IJobOutputs> outputs;
    };
}

#endif //MIMO_JOB_H
