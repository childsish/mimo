/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <memory>
#include <JobInputFactory.h>
#include <JobOutputFactory.h>
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
            std::shared_ptr<Step> step,
            std::shared_ptr<IJobInputsFactory> inputs_factory,
            std::shared_ptr<IJobOutputsFactory> outputs_factory
        );

        const std::shared_ptr<workflow::Step> get_identifier() override;

        std::unique_ptr<IJobInputs> &get_inputs() override;

        std::unique_ptr<IJobOutputs> &get_outputs() override;

        bool can_run() const override;

        void run() override;

    private:

        const std::shared_ptr<workflow::Step> identifier;

        bool completed;

        std::shared_ptr<mimo::Step> step;
        std::shared_ptr<IJobOutputsFactory> outputs_factory;

        std::unique_ptr<IJobInputs> inputs;
        std::unique_ptr<IJobOutputs> outputs;
    };
}

#endif //MIMO_JOB_H
