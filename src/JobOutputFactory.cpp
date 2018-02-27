#include <queues/IQueue.h>
#include <queues/JobOutputs.h>
#include "JobOutputFactory.h"

mimo::JobOutputFactory::JobOutputFactory(std::shared_ptr<mimo::IQueueFactory> queue_factory) :
    queue_factory(std::move(queue_factory)) {}

mimo::IJobOutputs *mimo::JobOutputFactory::make_raw(const workflow::OutputMap &outputs) const {
    return new JobOutputs(outputs, this->queue_factory);
}

std::shared_ptr<mimo::IJobOutputs>
mimo::JobOutputFactory::make_shared(const workflow::OutputMap &outputs) const {
    return std::make_shared<JobOutputs>(outputs, this->queue_factory);
}

std::unique_ptr<mimo::IJobOutputs>
mimo::JobOutputFactory::make_unique(const workflow::OutputMap &outputs) const {
    return std::make_unique<JobOutputs>(outputs, this->queue_factory);
}
