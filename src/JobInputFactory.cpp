#include "JobInputFactory.h"

mimo::JobInputFactory::JobInputFactory(std::shared_ptr<mimo::IQueueFactory> queue_factory) :
    queue_factory(std::move(queue_factory)) {}

mimo::IJobInputs *mimo::JobInputFactory::make_raw(const workflow::InputMap &inputs) const {
    return new JobInputs(inputs, this->queue_factory);
}

std::shared_ptr<mimo::IJobInputs>
mimo::JobInputFactory::make_shared(const workflow::InputMap &inputs) const {
    return std::make_shared<JobInputs>(inputs, this->queue_factory);
}

std::unique_ptr<mimo::IJobInputs>
mimo::JobInputFactory::make_unique(const workflow::InputMap &inputs) const {
    return std::make_unique<JobInputs>(inputs, this->queue_factory);
}
