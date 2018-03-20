/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBMANAGERFACTORY_H
#define MIMO_JOBMANAGERFACTORY_H

#include <workflow/Step.h>
#include "IJobManagerFactory.h"
#include "JobFactory.h"
#include "Step.h"


namespace mimo {

    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class JobManagerFactory : public IJobManagerFactory {
    public:

        explicit JobManagerFactory(
            unsigned int capacity,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>()
        );

        ~JobManagerFactory();

        template <typename T, typename... Args>
        void register_step(const std::shared_ptr<workflow::Step> &identifier, Args&&... args);

        std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override;

    private:

        unsigned int capacity;
        std::unordered_map<std::shared_ptr<workflow::Step>, StepConstructor> step_constructors;
        std::shared_ptr<IJobFactory> job_factory;

    };
}

template<typename T, typename... Args>
void mimo::JobManagerFactory::register_step(const std::shared_ptr<workflow::Step> &identifier, Args&&... args) {
    this->step_constructors.emplace(
        identifier,
        [&]{ return std::make_shared<T>(std::forward<Args>(args)...); }
    );
}

#endif //MIMO_JOBMANAGERFACTORY_H
