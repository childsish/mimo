/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "job_managers/JobManager.h"
#include "job_managers/MultiJobManagerFactory.h"

mimo::MultiJobManagerFactory::MultiJobManagerFactory(
    std::shared_ptr<mimo::ISingleJobManagerFactory> factory
) : factory(std::move(factory)) {}

mimo::MultiJobManagerFactory::~MultiJobManagerFactory() {}

void mimo::MultiJobManagerFactory::register_step(
    const std::shared_ptr<workflow::Step> &identifier,
    StepConstructor step_constructor
) {
    this->factory->register_step(identifier, step_constructor);
}

std::unique_ptr<mimo::IJobManager>
mimo::MultiJobManagerFactory::make_manager(
    std::shared_ptr<workflow::Workflow> workflow
) const {
    return std::make_unique<JobManager>(workflow, this->factory);
}
