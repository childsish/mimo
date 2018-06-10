/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ISINGLEJOBDEPOTFACTORY_H
#define MIMO_ISINGLEJOBDEPOTFACTORY_H

#include "IJobDepotFactory.h"
#include "SingleJobDepot.h"


namespace mimo {

    class Step;
    using ISingleJobDepotFactory = IJobDepotFactory<
        ISingleJobDepot,
        std::shared_ptr<workflow::Step>
    >;

    class SingleJobDepotFactory : public ISingleJobDepotFactory {
    public:
        SingleJobDepotFactory(
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>(),
            std::shared_ptr<IQueueBundleFactory> = std::make_shared<QueueBundleFactory>()
        ) :
            job_factory(job_factory),
            bundle_factory(bundle_factory) {}

        void register_step(
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step
        ) override {
            this->steps[step_id] = step;
        }

        ISingleJobDepot *make_raw(
            std::shared_ptr<workflow::Step> step_id
        ) override {
            return new SingleJobDepot(
                std::move(step_id),
                std::move(this->steps.at(step_id)),
                this->job_factory,
                this->bundle_factory
            );
        }
    private:
        std::unordered_map<std::shared_ptr<workflow::Step>, std::shared_ptr<Step>> steps;
        std::shared_ptr<IJobFactory> job_factory;
        std::shared_ptr<IQueueBundleFactory> bundle_factory;
    };
}

#endif //MIMO_ISINGLEJOBDEPOTFACTORY_H
