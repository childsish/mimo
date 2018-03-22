#include <job_managers/AsynchronousJobManager.h>
#include <job_managers/SynchronousJobManager.h>
#include "job_managers/SingleJobManagerFactory.h"


mimo::SingleJobManagerFactory::SingleJobManagerFactory(
    unsigned int capacity,
    std::shared_ptr<IJobFactory> job_factory
) :
    capacity(capacity),
    job_factory(std::move(job_factory)) {}

mimo::SingleJobManagerFactory::~SingleJobManagerFactory() {}

void mimo::SingleJobManagerFactory::register_step(
    const std::shared_ptr<workflow::Step> &identifier,
    StepConstructor step_constructor
) {
    this->steps[identifier] = step_constructor;
}

std::unique_ptr<mimo::IJobManager> mimo::SingleJobManagerFactory::make_manager(
    const std::shared_ptr<workflow::Step> &identifier
) const {
    std::unique_ptr<IJobManager> manager;
    if (identifier->is_synchronous()) {
        manager = std::make_unique<SynchronousJobManager>(
            this->capacity, identifier, this->steps.at(identifier)(), this->job_factory
        );
    }
    else {
        manager = std::make_unique<AsynchronousJobManager>(
            identifier, this->steps.at(identifier)(), this->job_factory
        );
    }
    return manager;
}
