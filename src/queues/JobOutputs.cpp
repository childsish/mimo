#include <algorithm>
#include <workflow/Output.h>
#include "queues/JobOutputs.h"
#include "errors.h"



mimo::JobOutputs::JobOutputs(const workflow::OutputMap &outputs,
                             std::shared_ptr<IQueueFactory> factory) :
    run(0),
    job_ended(false),
    outputs(outputs),
    factory(std::move(factory))
{
    for (const auto &output : outputs) {
        this->queues.emplace(output.first, this->factory->make_unique());
    }
}

mimo::IJobOutputs::PushStatus mimo::JobOutputs::get_status() const {
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

mimo::IJobOutputs::PushStatus mimo::JobOutputs::get_status(const std::string &name) const {
    if (this->queues.at(name)->is_full()) {
        return PushStatus::QUEUE_FULL;
    }
    auto group_status = this->get_group_status();
    if (!group_status.at(this->outputs.at(name)->sync_group)) {
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

void mimo::JobOutputs::close() {
    this->job_ended = true;
}

bool mimo::JobOutputs::is_closed() const {
    return this->job_ended;
}

// private:

std::unordered_map<unsigned int, bool> mimo::JobOutputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->outputs) {
        if (groups.find(item.second->sync_group) == groups.end()) {
            groups[item.second->sync_group] = true;
        }
        groups[item.second->sync_group] &= this->queues.at(item.first)->can_push();
    }
    return groups;
}
