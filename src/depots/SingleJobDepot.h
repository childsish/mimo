/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_SINGLEJOBDEPOT_H
#define MIMO_SINGLEJOBDEPOT_H

#include "ISingleJobDepot.h"
#include "../Factory.h"
#include "../JobFactory.h"
#include "../queues/QueueBundleFactory.h"


namespace mimo {

    /** @brief Storage for a single job. */
    class SingleJobDepot : public ISingleJobDepot {
    public:
        SingleJobDepot(
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>(),
            std::shared_ptr<IQueueBundleFactory> bundle_factory = std::make_shared<QueueBundleFactory>()
        );

        void push(
            const workflow::Input &input_id,
            std::shared_ptr<Entity> entity
        ) override;

        bool can_queue(const workflow::Input &input_id) override;

        void queue_input(
            const workflow::Input &input_id,
            const IQueue &queue
        ) override;

        bool has_runnable_jobs() const override;

        std::set<std::unique_ptr<IJob>> get_runnable_jobs() override;

        void return_exhausted_job(std::unique_ptr<IJob> job) override;

    private:
        std::shared_ptr<workflow::Step> step_id;
        std::shared_ptr<Step> step;
        std::unique_ptr<IJob> job;
        std::unique_ptr<IQueueBundle> buffer;
    };


    using ISingleJobDepotFactory = IFactory<ISingleJobDepot>;
    using SingleJobDepotFactory = Factory<ISingleJobDepot, SingleJobDepot, std::shared_ptr<workflow::Step>, std::shared_ptr<Step>>;
}


#endif //MIMO_SINGLEJOBDEPOT_H
