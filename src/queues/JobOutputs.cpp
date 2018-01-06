/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "queues/JobOutputs.h"

#include <algorithm>
#include <memory>
#include "errors.h"
#include "queues/Queue.h"


mimo::JobOutputs::JobOutputs(
        const IQueueFactory &factory_,
        const std::unordered_map<std::string, unsigned int> &sync_groups_
) : run(0),
    job_ended(false),
    factory(factory_),
    sync_groups(sync_groups_) {}

mimo::JobOutputs::PushStatus mimo::JobOutputs::get_status() const {
    auto group_can_push = this->get_group_status();
    if (std::any_of(
            group_can_push.begin(),
            group_can_push.end(),
            [](const std::pair<unsigned int, bool> item){ return item.second; })
    ) {
        return PushStatus::CAN_PUSH;
    }
    return PushStatus::SYNC_QUEUE_FULL;
}

mimo::JobOutputs::PushStatus mimo::JobOutputs::get_status(const std::string &name) const {
    if (this->queues.at(name)->is_full()) {
        return PushStatus::QUEUE_FULL;
    }
    auto group_status = this->get_group_status();
    if (!group_status.at(this->sync_groups.at(name))) {
        return PushStatus::SYNC_QUEUE_FULL;
    }
    return PushStatus::CAN_PUSH;
}

void mimo::JobOutputs::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    PushStatus status = this->get_status();
    if (status == PushStatus::QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is full.");
    }
    else if (status == PushStatus::SYNC_QUEUE_FULL) {
        throw mimo::QueueError("Can not push. " + name + " is synced with full queue.");
    }
    this->queues.at(name)->push(entity);
}

std::unordered_map<unsigned int, bool> mimo::JobOutputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first)->can_push();
    }
    return groups;
}

void mimo::JobOutputs::end_run() {
    this->run += 1;
}

void mimo::JobOutputs::end_job() {
    this->job_ended = true;
}

bool mimo::JobOutputs::is_job_ended() const {
    return this->job_ended;
}
