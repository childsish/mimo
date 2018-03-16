/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBMANAGERFACTORY_H
#define MIMO_JOBMANAGERFACTORY_H

#include <workflow/Step.h>
#include "IJobManagerFactory.h"
#include "JobFactory.h"


namespace mimo {

    class Step;

    class JobManagerFactory : public IJobManagerFactory {
    public:

        explicit JobManagerFactory(
            unsigned int capacity,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>()
        );

        std::unique_ptr<AsynchronousJobManager> make_asynchronous(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) const override;

        std::unique_ptr<SynchronousJobManager> make_synchronous(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) const override;

    private:

        unsigned int capacity;
        std::shared_ptr<IJobFactory> job_factory;

    };
}


#endif //MIMO_JOBMANAGERFACTORY_H
