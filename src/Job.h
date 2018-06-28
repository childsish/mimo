/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include "queues/QueueBundleFactory.h"
#include "IJob.h"


namespace mimo {

    class Step;

    class Job : public IJob {
    public:

        Job(
            unsigned int job_id,
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step,
            std::shared_ptr<IQueueBundleFactory> factory = std::make_shared<QueueBundleFactory>()
        );

        const std::shared_ptr<workflow::Step> get_step_id() const override;

        unsigned int get_job_id() const override;

        void transfer_input(IQueueBundle &bundle) override;

        std::shared_ptr<IInputs> get_inputs() override;

        std::shared_ptr<IOutputs> get_outputs() override;

        bool can_run() const override;

        void run() override;

        bool is_complete() const override;

    private:

        unsigned int job_id;
        std::shared_ptr<workflow::Step> step_id;
        std::shared_ptr<mimo::Step> step;

        std::shared_ptr<IInputs> inputs;
        std::shared_ptr<IOutputs> outputs;

        bool completed;
    };
}

#endif //MIMO_JOB_H
