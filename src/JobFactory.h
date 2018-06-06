/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBFACTORY_H
#define MIMO_JOBFACTORY_H

#include "IFactory.h"
#include "queues/QueueBundleFactory.h"


namespace mimo {

    using IJobFactory = IFactory<IJob, std::shared_ptr<workflow::Step>, std::shared_ptr<Step>>;

    class JobFactory : public IJobFactory {
    public:
        explicit JobFactory(std::shared_ptr<IFactory<QueueBundle>> inputs_factory = std::make_shared<QueueBundleFactory>());

        IJob *make_raw(const std::shared_ptr<workflow::Step> &step_id, std::shared_ptr<Step> step) const override;

    private:
        const std::shared_ptr<IQueueBundleFactory> factory;
    };
}

#endif //MIMO_JOBFACTORY_H
