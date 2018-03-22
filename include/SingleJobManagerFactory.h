/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBMANAGERFACTORY_H
#define MIMO_JOBMANAGERFACTORY_H

#include <workflow/Step.h>
#include "IJobManagerFactory.h"
#include "JobFactory.h"


namespace mimo {

    class JobManagerFactory : public IJobManagerFactory {
    public:

        explicit JobManagerFactory(
            unsigned int capacity,
            std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>()
        );

        ~JobManagerFactory();

        std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override;

    private:

        unsigned int capacity;
        std::shared_ptr<IJobFactory> job_factory;

    };
}

#endif //MIMO_JOBMANAGERFACTORY_H
