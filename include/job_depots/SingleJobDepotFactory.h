/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_SINGLEJOBDEPOTFACTORY_H
#define MIMO_SINGLEJOBDEPOTFACTORY_H

#include <workflow/Step.h>
#include "ISingleJobDepotFactory.h"
#include "JobFactory.h"


namespace mimo {

    class SingleJobDepotFactory : public ISingleJobDepotFactory {
    public:

        explicit SingleJobDepotFactory(
            unsigned int capacity,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>()
        );

        ~SingleJobDepotFactory();

        void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) override;

        std::unique_ptr<IJobDepot> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override;

    private:

        unsigned int capacity;
        std::shared_ptr<IJobFactory> job_factory;
        std::unordered_map<std::shared_ptr<workflow::Step>, StepConstructor> steps;

    };
}

#endif //MIMO_SINGLEJOBDEPOTFACTORY_H
