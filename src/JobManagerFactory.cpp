#include <AsynchronousJobManager.h>
#include <SynchronousJobManager.h>
#include "JobManagerFactory.h"


mimo::JobManagerFactory::JobManagerFactory(
    unsigned int capacity,
    std::shared_ptr<IJobFactory> job_factory
) :
    capacity(capacity),
    job_factory(std::move(job_factory)) {}


mimo::JobManagerFactory::~JobManagerFactory() {}

std::unique_ptr<mimo::IJobManager> mimo::JobManagerFactory::make_manager(
    const std::shared_ptr<workflow::Step> &identifier
) const {
    std::unique_ptr<IJobManager> manager;
    if (identifier->is_synchronous()) {
        manager = std::make_unique<SynchronousJobManager>(
            this->capacity,
            identifier,
            this->step_constructors.at(identifier)(),
            this->job_factory
        );
    }
    else {
        manager = std::make_unique<AsynchronousJobManager>(
            identifier,
            this->step_constructors.at(identifier)(),
            this->job_factory
        );
    }
    return manager;
}
