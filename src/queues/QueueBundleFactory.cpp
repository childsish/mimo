#include "QueueBundleFactory.h"

#include "QueueBundle.h"

mimo::QueueBundleFactory::QueueBundleFactory() :
    factory(std::make_shared<QueueFactory>()) {}

mimo::QueueBundleFactory::QueueBundleFactory(std::shared_ptr<IQueueFactory> factory) :
    factory(std::move(factory)) {}

std::shared_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_shared(
    std::shared_ptr<workflow::InputMap> inputs
) {
    return std::make_shared<QueueBundle>(std::move(inputs), this->factory);
}

std::shared_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_shared(
    std::shared_ptr<workflow::OutputMap> outputs
) {
    return std::make_shared<QueueBundle>(std::move(outputs), this->factory);
}
