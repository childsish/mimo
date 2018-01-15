/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "queues/JobOutputs.h"

#include <algorithm>
#include "errors.h"
#include "IQueueFactory.h"
#include "queues/IQueue.h"


mimo::JobOutputs::JobOutputs(IQueueFactory &factory_, const std::vector<std::string> &outputs) :
    group_id(0),
    run(0),
    job_ended(false),
    factory(factory_)
{
    for (const auto &output : outputs) {
        this->queues.emplace(output, this->factory.make());
        this->sync_groups.emplace(output, this->group_id);
        this->group_id += 1;
    }
}

std::unique_ptr<mimo::IQueue> mimo::JobOutputs::get_queue(const std::string name) {
    auto queue = std::move(this->queues.at(name));
    this->queues[name] = this->factory.make();
    return queue;
}

void mimo::JobOutputs::synchronise_queues(const std::vector<std::string> &queues) {
    for (const auto &name : queues) {
        this->sync_groups[name] = this->group_id;
    }
    this->group_id += 1;
}

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

void mimo::JobOutputs::end_run() {
    this->run += 1;
}

void mimo::JobOutputs::end_job() {
    this->job_ended = true;
}

bool mimo::JobOutputs::is_job_ended() const {
    return this->job_ended;
}

// private:

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
