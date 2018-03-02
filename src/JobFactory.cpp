#include "queues/JobInputs.h"
#include "queues/JobOutputs.h"
#include "Job.h"
#include "JobFactory.h"

mimo::JobFactory::JobFactory(const std::shared_ptr<IJobInputsFactory> inputs_factory,
                             const std::shared_ptr<IJobOutputsFactory> outputs_factory) :
    inputs_factory(inputs_factory),
    outputs_factory(outputs_factory) {}

mimo::IJob *
mimo::JobFactory::make_raw(const std::shared_ptr<workflow::Step> identifier,
                           std::shared_ptr<Step> step) const {
    return new Job(identifier, std::move(step), this->inputs_factory, this->outputs_factory);
}

std::shared_ptr<mimo::IJob>
mimo::JobFactory::make_shared(const std::shared_ptr<workflow::Step> identifier,
                              std::shared_ptr<Step> step) const {
    return std::make_shared<Job>(identifier, std::move(step), this->inputs_factory, this->outputs_factory);
}

std::unique_ptr<mimo::IJob>
mimo::JobFactory::make_unique(const std::shared_ptr<workflow::Step> identifier,
                              std::shared_ptr<Step> step) const {
    return std::make_unique<Job>(identifier, std::move(step), this->inputs_factory, this->outputs_factory);
}
