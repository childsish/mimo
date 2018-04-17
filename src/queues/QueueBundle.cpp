#include <algorithm>
#include <workflow/Input.h>
#include <workflow/Output.h>
#include "IQueue.h"
#include "QueueBundle.h"
#include "../errors.h"

mimo::QueueBundle::QueueBundle(
    const workflow::InputMap &identifiers,
    std::shared_ptr<mimo::IQueueFactory> factory
) {
    for (auto &&item : identifiers) {
        this->identifiers.emplace(item.first, item.second);
        this->queues.emplace(item.first, factory->make_unique());
    }
}

mimo::QueueBundle::QueueBundle(
    const workflow::OutputMap &identifiers,
    std::shared_ptr<mimo::IQueueFactory> factory
) {
    for (auto &&item : identifiers) {
        this->identifiers.emplace(item.first, item.second);
        this->queues.emplace(item.first, factory->make_unique());
    }
}

mimo::IQueueBundle::PushStatus mimo::QueueBundle::get_push_status() const {
    auto group_can_push = this->get_group_status();
    if (std::any_of(group_can_push.begin(), group_can_push.end(),
        [](const std::pair<unsigned int, bool> item){ return item.second; })
    ) {
        return PushStatus::CAN_PUSH;
    }
    return PushStatus::SYNC_QUEUE_FULL;
}

mimo::IQueueBundle::PushStatus mimo::QueueBundle::get_push_status(const std::string &name) const {
    if (!this->queues.at(name)->can_push()) {
        return PushStatus::QUEUE_FULL;
    }
    auto group_status = this->get_group_status();
    if (!group_status.at(this->identifiers.at(name)->sync_group)) {
        return PushStatus::SYNC_QUEUE_FULL;
    }
    return PushStatus::CAN_PUSH;
}

void mimo::QueueBundle::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    PushStatus status = this->get_push_status();
    if (status == PushStatus::QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is full.");
    }
    else if (status == PushStatus::SYNC_QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is synced with full queue.");
    }
    this->queues.at(name)->push(entity);
}

mimo::IQueueBundle::PopStatus mimo::QueueBundle::get_pop_status() const {
    auto groups = this->get_group_status();
    if (std::any_of(
        groups.begin(),
        groups.end(),
        [](const auto &item){ return item.second; })
        ) {
        return PopStatus::CAN_POP;
    }
    return PopStatus::SYNC_QUEUE_EMPTY;
}

mimo::IQueueBundle::PopStatus mimo::QueueBundle::get_pop_status(const std::string &name) const {
    if (!this->queues.at(name)->can_pop()) {
        return PopStatus::QUEUE_EMPTY;
    }
    auto groups = this->get_group_status();
    if (!groups.at(this->identifiers.at(name)->sync_group)) {
        return PopStatus ::SYNC_QUEUE_EMPTY;
    }
    return PopStatus::CAN_POP;
}

std::shared_ptr<mimo::Entity> mimo::QueueBundle::peek(const std::string &name) {
    PopStatus status = this->get_pop_status();
    if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->peek();
}

std::shared_ptr<mimo::Entity> mimo::QueueBundle::pop(const std::string &name) {
    PopStatus status = this->get_pop_status();
    if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->pop();
}

void mimo::QueueBundle::close() {
    for (auto &&item : this->queues) {
        item.second->close();
    }
}

bool mimo::QueueBundle::is_closed() const {
    return std::all_of( this->queues.begin(), this->queues.end(),
               [](const auto &item){ return item.second->is_closed(); }
           );
}

std::unordered_map<unsigned int, bool> mimo::QueueBundle::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->identifiers) {
        if (groups.find(item.second->sync_group) == groups.end()) {
            groups[item.second->sync_group] = true;
        }
        groups[item.second->sync_group] &= this->queues.at(item.first)->can_push();
    }
    return groups;
}
