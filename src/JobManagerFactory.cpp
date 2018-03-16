#include <AsynchronousJobManager.h>
#include <SynchronousJobManager.h>
#include "JobManagerFactory.h"


mimo::JobManagerFactory::JobManagerFactory(
    unsigned int capacity,
    std::shared_ptr<IJobFactory> job_factory
) :
    capacity(capacity),
    job_factory(std::move(job_factory)) {}

std::unique_ptr<mimo::AsynchronousJobManager> mimo::JobManagerFactory::make_asynchronous(
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<Step> step
) const {
    return std::make_unique<AsynchronousJobManager>(identifier, step, this->job_factory);
}

std::unique_ptr<mimo::SynchronousJobManager> mimo::JobManagerFactory::make_synchronous(
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<Step> step
) const {
    return std::make_unique<SynchronousJobManager>(this->capacity, identifier, step, this->job_factory);
}
