#include <algorithm>
#include <workflow/Input.h>
#include <workflow/Output.h>
#include "IQueue.h"
#include "QueueBundle.h"
#include "../errors.h"

mimo::QueueBundle::QueueBundle(
    std::shared_ptr<ConnectionMap> identifiers,
    std::shared_ptr<IQueueFactory> factory
) :
    identifiers(std::move(identifiers)),
    factory(std::move(factory))
{
    for (const auto &item : *identifiers) {
        this->queues.emplace(item.first, factory->make_unique());
    }
}

const mimo::ConnectionMap & mimo::QueueBundle::get_identifiers() const {
    return *this->identifiers;
}

std::unique_ptr<mimo::IQueue>
mimo::QueueBundle::release_queue(const workflow::Connection &id) {
    auto queue = std::move(this->queues.at(id.name));
    this->queues.emplace(id.name, this->factory->make_unique());
    return queue;
}

void mimo::QueueBundle::acquire_queue(
    const workflow::Connection &id,
    std::unique_ptr<IQueue> queue
) {
    this->queues.at(id.name) = std::move(queue);
}

mimo::IQueueBundle::PushStatus mimo::QueueBundle::get_push_status() const {
    if (this->queues.empty()) {
        return PushStatus::NO_QUEUE;
    }
    auto group_can_push = this->get_group_push_status();
    if (std::any_of(group_can_push.begin(), group_can_push.end(),
        [](const std::pair<unsigned int, bool> item){ return item.second; })
    ) {
        return PushStatus::CAN_PUSH;
    }
    return PushStatus::SYNC_QUEUE_FULL;
}

mimo::IQueueBundle::PushStatus mimo::QueueBundle::get_push_status(const std::string &name) const {
    if (this->queues.find(name) == this->queues.end()) {
        return PushStatus::NO_QUEUE;
    }
    else if (!this->queues.at(name)->can_push()) {
        return PushStatus::QUEUE_FULL;
    }
    auto group_status = this->get_group_push_status();
    if (!group_status.at(this->identifiers->at(name)->sync_group)) {
        return PushStatus::SYNC_QUEUE_FULL;
    }
    return PushStatus::CAN_PUSH;
}

void mimo::QueueBundle::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    PushStatus status = this->get_push_status(name);
    if (status == PushStatus::NO_QUEUE) {
        throw mimo::QueueError("Invalid queue name " + name + ".");
    }
    else if (status == PushStatus::QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is full.");
    }
    else if (status == PushStatus::SYNC_QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is synced with full queue.");
    }
    this->queues.at(name)->push(entity);
}

void mimo::QueueBundle::push(const std::string &name, const mimo::IQueue &queue) {
    this->queues.at(name)->push(queue);
}

mimo::IQueueBundle::PopStatus mimo::QueueBundle::get_pop_status() const {
    if (this->queues.empty()) {
        return PopStatus::NO_QUEUE;
    }
    auto groups = this->get_group_pop_status();
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
    if (this->queues.find(name) == this->queues.end()) {
        return PopStatus::NO_QUEUE;
    }
    else if (!this->queues.at(name)->can_pop()) {
        return PopStatus::QUEUE_EMPTY;
    }
    auto groups = this->get_group_pop_status();
    if (!groups.at(this->identifiers->at(name)->sync_group)) {
        return PopStatus ::SYNC_QUEUE_EMPTY;
    }
    return PopStatus::CAN_POP;
}

std::shared_ptr<mimo::Entity> mimo::QueueBundle::peek(const std::string &name) {
    PopStatus status = this->get_pop_status(name);
    if (status == PopStatus::NO_QUEUE) {
        throw mimo::QueueError("Invalid queue name " + name + ".");
    }
    else if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not peek." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->peek();
}

std::shared_ptr<mimo::Entity> mimo::QueueBundle::pop(const std::string &name) {
    PopStatus status = this->get_pop_status(name);
    if (status == PopStatus::NO_QUEUE) {
        throw mimo::QueueError("Invalid queue name " + name + ".");
    }
    else if (status == PopStatus::QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop. " + name + " is empty.");
    }
    else if (status == PopStatus::SYNC_QUEUE_EMPTY) {
        throw mimo::QueueError("Can not pop." + name + " is synced with empty queue.");
    }
    return this->queues.at(name)->pop();
}

std::unordered_map<unsigned int, bool> mimo::QueueBundle::get_group_push_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : *this->identifiers) {
        if (groups.find(item.second->sync_group) == groups.end()) {
            groups[item.second->sync_group] = true;
        }
        groups[item.second->sync_group] &= this->queues.at(item.first)->can_push();
    }
    return groups;
}

std::unordered_map<unsigned int, bool> mimo::QueueBundle::get_group_pop_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : *this->identifiers) {
        if (groups.find(item.second->sync_group) == groups.end()) {
            groups[item.second->sync_group] = true;
        }
        groups[item.second->sync_group] &= this->queues.at(item.first)->can_pop();
    }
    return groups;
}
