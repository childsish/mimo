#include <queues/JobInputs.h>
#include "queues/JobInputFactory.h"

mimo::JobInputFactory::JobInputFactory(std::shared_ptr<mimo::IQueueFactory> queue_factory) :
    queue_factory(std::move(queue_factory)) {}

mimo::IJobInputs *mimo::JobInputFactory::make_raw(const workflow::InputMap &inputs) const {
    return new JobInputs(inputs, this->queue_factory);
}
