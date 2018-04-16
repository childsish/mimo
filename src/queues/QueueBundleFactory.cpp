#include "queues/IQueue.h"
#include "queues/QueueBundle.h"
#include "queues/QueueBundleFactory.h"

mimo::QueueBundleFactory::QueueBundleFactory(std::shared_ptr<mimo::IQueueFactory> factory) :
    factory(std::move(factory)) {}

mimo::IQueueBundle *mimo::QueueBundleFactory::make_raw(const workflow::InputMap &inputs) const {
    return new QueueBundle(inputs, this->factory);
}

std::shared_ptr<mimo::IQueueBundle>
mimo::QueueBundleFactory::make_shared(const workflow::InputMap &inputs) const {
    return std::make_shared<QueueBundle>(inputs, this->factory);
}

std::unique_ptr<mimo::IQueueBundle>
mimo::QueueBundleFactory::make_unique(const workflow::InputMap &inputs) const {
    return std::make_unique<QueueBundle>(inputs, this->factory);
}

mimo::IQueueBundle *mimo::QueueBundleFactory::make_raw(const workflow::OutputMap &outputs) const {
    return new QueueBundle(outputs, this->factory);
}

std::shared_ptr<mimo::IQueueBundle>
mimo::QueueBundleFactory::make_shared(const workflow::OutputMap &outputs) const {
    return std::make_shared<QueueBundle>(outputs, this->factory);
}

std::unique_ptr<mimo::IQueueBundle>
mimo::QueueBundleFactory::make_unique(const workflow::OutputMap &outputs) const {
    return std::make_unique<QueueBundle>(outputs, this->factory);
}
