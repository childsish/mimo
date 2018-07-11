/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBFACTORY_H
#define MIMO_JOBFACTORY_H

#include "IFactory.h"
#include "Job.h"
#include "queues/QueueBundleFactory.h"


namespace mimo {

    using IJobFactory = IFactory<IJob, std::shared_ptr<workflow::Step>, std::shared_ptr<Step>>;

    class JobFactory : public IJobFactory {
    public:
        explicit JobFactory(
            std::shared_ptr<IQueueBundleFactory> factory = std::make_shared<QueueBundleFactory>()
        ) :
            job_id(0),
            factory(std::move(factory)) {}

        IJob *make_raw(
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step
        ) override {
            return new Job(this->job_id++, std::move(step_id), std::move(step), this->factory);
        }

    private:
        unsigned int job_id;
        std::shared_ptr<IQueueBundleFactory> factory;
    };
}

#endif //MIMO_JOBFACTORY_H
