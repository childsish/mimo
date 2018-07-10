#include "QueueBundleFactory.h"

#include "QueueBundle.h"
#include <workflow/Input.h>
#include <workflow/Output.h>

mimo::QueueBundleFactory::QueueBundleFactory() :
    factory(std::make_shared<QueueFactory>()) {}

mimo::QueueBundleFactory::QueueBundleFactory(std::shared_ptr<IQueueFactory> factory) :
    factory(std::move(factory)) {}

std::shared_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_shared(
    workflow::InputMap &inputs
) {
    return std::make_shared<QueueBundle>(this->upcast_connections(inputs), this->factory);
}

std::shared_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_shared(
    workflow::OutputMap &outputs
) {
    return std::make_shared<QueueBundle>(this->upcast_connections(outputs), this->factory);
}

std::unique_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_unique(
    workflow::InputMap &inputs
) {
    return std::make_unique<QueueBundle>(this->upcast_connections(inputs), this->factory);
}

std::unique_ptr<mimo::IQueueBundle> mimo::QueueBundleFactory::make_unique(
    workflow::OutputMap &outputs
) {
    return std::make_unique<QueueBundle>(this->upcast_connections(outputs), this->factory);
}

mimo::IQueueBundle *mimo::QueueBundleFactory::make_raw(
    std::shared_ptr<ConnectionMap> connections
) {
    return new QueueBundle(connections, this->factory);
}

std::shared_ptr<mimo::ConnectionMap>
mimo::QueueBundleFactory::upcast_connections(const workflow::InputMap &inputs) const {
    auto connections = std::make_shared<ConnectionMap>();
    for (const auto &input : inputs) {
        connections->emplace(input.first, input.second);
    }
    return connections;
}

std::shared_ptr<mimo::ConnectionMap>
mimo::QueueBundleFactory::upcast_connections(const workflow::OutputMap &outputs) const {
    auto connections = std::make_shared<ConnectionMap>();
    for (const auto &output : outputs) {
        connections->emplace(output.first, output.second);
    }
    return connections;
}
