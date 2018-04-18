#include "JobDepot.h"
#include "MultiJobDepotFactory.h"

mimo::MultiJobDepotFactory::MultiJobDepotFactory(
    std::shared_ptr<mimo::ISingleJobDepotFactory> factory
) : factory(std::move(factory)) {}

mimo::MultiJobDepotFactory::~MultiJobDepotFactory() {}

void mimo::MultiJobDepotFactory::register_step(
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<Step> step
) {
    this->factory->register_step(identifier, step);
}

std::unique_ptr<mimo::IJobDepot>
mimo::MultiJobDepotFactory::make_depot(
    std::shared_ptr<workflow::Workflow> workflow
) const {
    return std::make_unique<JobDepot>(workflow, this->factory);
}
