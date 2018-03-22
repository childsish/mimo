/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_SINGLEJOBMANAGERFACTORY_H
#define MIMO_SINGLEJOBMANAGERFACTORY_H

#include <workflow/Step.h>
#include "ISingleJobManagerFactory.h"
#include "JobFactory.h"


namespace mimo {

    class SingleJobManagerFactory : public ISingleJobManagerFactory {
    public:

        explicit SingleJobManagerFactory(
            unsigned int capacity,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>()
        );

        ~SingleJobManagerFactory();

        void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) override;

        std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override;

    private:

        unsigned int capacity;
        std::shared_ptr<IJobFactory> job_factory;
        std::unordered_map<std::shared_ptr<workflow::Step>, StepConstructor> steps;

    };
}

#endif //MIMO_SINGLEJOBMANAGERFACTORY_H
