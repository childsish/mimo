#include "AsynchronousJobDepot.h"
#include "SynchronousJobDepot.h"
#include "SingleJobDepotFactory.h"


mimo::SingleJobDepotFactory::SingleJobDepotFactory(
    unsigned int capacity,
    std::shared_ptr<IJobFactory> job_factory
) :
    capacity(capacity),
    job_factory(std::move(job_factory)) {}

mimo::SingleJobDepotFactory::~SingleJobDepotFactory() {}

void mimo::SingleJobDepotFactory::register_step(
    const std::shared_ptr<workflow::Step> &identifier,
    StepConstructor step_constructor
) {
    this->steps[identifier] = step_constructor;
}

std::unique_ptr<mimo::IJobDepot> mimo::SingleJobDepotFactory::make_manager(
    const std::shared_ptr<workflow::Step> &identifier
) const {
    std::unique_ptr<IJobDepot> manager;
    if (identifier->is_synchronous()) {
        manager = std::make_unique<SynchronousJobDepot>(
            this->capacity, identifier, this->steps.at(identifier)(), this->job_factory
        );
    }
    else {
        manager = std::make_unique<AsynchronousJobDepot>(
            identifier, this->steps.at(identifier)(), this->job_factory
        );
    }
    return manager;
}
