#include "queues/JobInputs.h"

#include <algorithm>
#include "errors.h"
#include "queues/IQueue.h"


mimo::JobInputs::JobInputs() : group_id(0) {}

void mimo::JobInputs::add_queue(const std::string &name, std::unique_ptr<mimo::IQueue> queue) {
    this->queues.emplace(name, std::move(queue));
    this->sync_groups.emplace(name, this->group_id);
    this->group_id += 1;
}

void mimo::JobInputs::synchronise_queues(const std::vector<std::string> &queues) {
    for (const auto &name : queues) {
        this->sync_groups[name] = this->group_id;
    }
    this->group_id += 1;
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
    if (!groups.at(this->sync_groups.at(name))) {
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

bool mimo::JobInputs::is_empty() const {
    return this->queues.empty();
}

bool mimo::JobInputs::is_closed() const {
    return std::all_of(
        this->queues.begin(),
        this->queues.end(),
        [](const auto &item){ return item.second->is_closed(); }
    );
}

// private:

std::unordered_map<unsigned int, bool> mimo::JobInputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first)->can_pop();
    }
    return groups;
}
