/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include <workflow/Output.h>
#include "queues/Outputs.h"


mimo::Outputs::Outputs(
        const std::unordered_map<std::string, std::shared_ptr<workflow::Output>> &outputs,
        const IQueueFactory &factory_
) : group_id(0),
    factory(factory_)
{
    for (auto &item : outputs) {
        this->queues.emplace(item.first, OutputQueue(item.second));
        this->sync_groups.emplace(item.first, this->group_id);
        this->group_id += 1;
    }
}

std::unique_ptr<mimo::Queue> mimo::Outputs::release_queue(const std::string &name) {
    auto queue = std::move(this->queues.at(name).release_queue());
    this->queues.erase(name);
    return queue;
}

void mimo::Outputs::synchronise_queues(const std::vector<std::string> &group) {
    for (const auto &name : group) {
        this->sync_groups[name] = group_id;
    }
    group_id += 1;
}

bool mimo::Outputs::can_push() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first).can_push();
    }
    return std::any_of(
            groups.begin(),
            groups.end(),
            [](const std::pair<unsigned int, bool> &item){ return item.second; }
    );
}

mimo::OutputQueue &mimo::Outputs::operator[](const std::string &name) {
    return this->queues.at(name);
}

std::unordered_map<std::string, mimo::OutputQueue>::const_iterator mimo::Outputs::begin() const {
    return this->queues.begin();
}

std::unordered_map<std::string, mimo::OutputQueue>::const_iterator mimo::Outputs::end() const {
    return this->queues.end();
}

void mimo::Outputs::end_run() {
    for (auto &queue : this->queues) {
        queue.second.end_run();
    }
}

void mimo::Outputs::close() {
    for (auto &queue : this->queues) {
        queue.second.close();
    }
}

bool mimo::Outputs::is_empty() {
    return std::all_of(this->queues.begin(),
                       this->queues.end(),
                       [](const std::pair<const std::string, mimo::OutputQueue> &item){ return item.second.is_empty(); });
}
