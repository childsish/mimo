#include <algorithm>
#include <workflow/Input.h>
#include "queues/JobInputs.h"
#include "errors.h"


mimo::JobInputs::JobInputs(
    const workflow::InputMap inputs,
    std::shared_ptr<IQueueFactory> factory
) :
    inputs(inputs)
{
    for (const auto &item : inputs) {
        this->queues.emplace(item.first, factory->make_unique());
    }
}

mimo::IJobInputs::PopStatus mimo::JobInputs::get_status() const {
    auto group_can_pop = this->get_group_status();
    if (std::any_of(
        group_can_pop.begin(),
        group_can_pop.end(),
        [](const auto &item){ return item.second; })
    ) {
        return PopStatus::CAN_POP;
    }
    return PopStatus::SYNC_QUEUE_EMPTY;
}

mimo::IJobInputs::PopStatus mimo::JobInputs::get_status(const std::string &name) const {
    if (this->queues.at(name)->is_empty()) {
        return PopStatus::QUEUE_EMPTY;
    }
    auto groups = this->get_group_status();
    if (!groups.at(this->inputs.at(name)->sync_group)) {
        return PopStatus ::SYNC_QUEUE_EMPTY;
    }
    return PopStatus::CAN_POP;
}

std::shared_ptr<mimo::Entity> mimo::JobInputs::peek(const std::string &name) {
    PopStatus status = this->get_status();
    if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->peek();
}

std::shared_ptr<mimo::Entity> mimo::JobInputs::pop(const std::string &name) {
    PopStatus status = this->get_status();
    if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->pop();
}

bool mimo::JobInputs::is_closed() const {
    return this->queues.empty() ||
        std::all_of(
        this->queues.begin(),
        this->queues.end(),
        [](const auto &item){ return item.second->is_closed(); }
    );
}

// private:

std::unordered_map<unsigned int, bool> mimo::JobInputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->inputs) {
        if (groups.find(item.second->sync_group) == groups.end()) {
            groups[item.second->sync_group] = true;
        }
        groups[item.second->sync_group] &= this->queues.at(item.first)->can_pop();
    }
    return groups;
}
