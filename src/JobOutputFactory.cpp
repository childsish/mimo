#include <queues/IQueue.h>
#include <queues/JobOutputs.h>
#include "JobOutputFactory.h"

mimo::JobOutputFactory::JobOutputFactory(std::shared_ptr<mimo::IQueueFactory> queue_factory) :
    queue_factory(std::move(queue_factory)) {}

mimo::IJobOutputs *mimo::JobOutputFactory::make_raw(const workflow::OutputMap &outputs) const {
    return new JobOutputs(outputs, this->queue_factory);
}
