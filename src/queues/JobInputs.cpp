/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <queues/JobInputs.h>

#include <algorithm>
#include <memory>
#include "errors.h"
#include "queues/Queue.h"


mimo::JobInputs::JobInputs(
        std::unordered_map<std::string, std::unique_ptr<mimo::Queue>> &queues_,
        const std::unordered_map<std::string, unsigned int> &sync_groups_
) : queues(queues_),
    sync_groups(sync_groups_) {}

mimo::JobInputs::PopStatus mimo::JobInputs::get_status() const {
    auto group_can_pop = this->get_group_status();
    if (std::any_of(
            group_can_pop.begin(),
            group_can_pop.end(),
            [](const std::pair<unsigned int, bool> &item){ return item.second; })
    ) {
        return PopStatus::CAN_POP;
    }
    return PopStatus::SYNC_QUEUE_EMPTY;
}

mimo::JobInputs::PopStatus mimo::JobInputs::get_status(const std::string &name) const {
    if (this->queues.at(name)->is_empty()) {
        return PopStatus::QUEUE_EMPTY;
    }
    auto groups = this->get_group_status();
    if (!groups.at(this->sync_groups.at(name))) {
        return PopStatus ::SYNC_QUEUE_EMPTY;
    }
    return PopStatus::CAN_POP;
}

std::shared_ptr<mimo::Entity> &mimo::JobInputs::peek(const std::string &name) {
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

std::unordered_map<unsigned int, bool> mimo::JobInputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first).can_pop();
    }
    return groups;
}

bool mimo::JobInputs::is_empty() const {
    return std::all_of()
}

bool mimo::JobInputs::is_closed() const {
    return false;
}
