/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <memory>

#include "IJob.h"


namespace workflow {
    class Step;
}

namespace mimo {

    class IJobInputs;
    class IJobOutputs;
    class Step;

    class Job : public IJob {
    public:

        Job(
            const std::shared_ptr<workflow::Step> identifier,
            std::unique_ptr<Step> step,
            std::unique_ptr<IJobInputs> inputs,
            std::unique_ptr<IJobOutputs> outputs
        );

        const std::shared_ptr<workflow::Step> get_identifier() override;

        std::unique_ptr<IJobInputs> &get_inputs() override;

        std::unique_ptr<IJobOutputs> &get_outputs() override;

        void run() override;

    private:

        const std::shared_ptr<workflow::Step> identifier;

        bool completed;

        std::unique_ptr<mimo::Step> step;

        std::unique_ptr<IJobInputs> inputs;
        std::unique_ptr<IJobOutputs> outputs;
    };
}

#endif //MIMO_JOB_H
