/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "job_ports/JobDepot.h"
#include "job_ports/MultiJobDepotFactory.h"

mimo::MultiJobDepotFactory::MultiJobDepotFactory(
    std::shared_ptr<mimo::ISingleJobDepotFactory> factory
) : factory(std::move(factory)) {}

mimo::MultiJobDepotFactory::~MultiJobDepotFactory() {}

void mimo::MultiJobDepotFactory::register_step(
    const std::shared_ptr<workflow::Step> &identifier,
    StepConstructor step_constructor
) {
    this->factory->register_step(identifier, step_constructor);
}

std::unique_ptr<mimo::IJobDepot>
mimo::MultiJobDepotFactory::make_manager(
    std::shared_ptr<workflow::Workflow> workflow
) const {
    return std::make_unique<JobDepot>(workflow, this->factory);
}
