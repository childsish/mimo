/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include "queues/Inputs.h"


mimo::Inputs::Inputs() : group_id(0) {}

void mimo::Inputs::add_queue(const workflow::Input &identifier) {
    this->queues.emplace(identifier.name, identifier);
    this->sync_groups[identifier.name] = group_id;
    this->group_id += 1;
}

std::unique_ptr<mimo::Queue> mimo::Inputs::release_queue(const std::string &name) {
    auto queue = std::move(this->queues.at(name).release_queue());
    this->queues.erase(name);
    return queue;
}

void mimo::Inputs::synchronise_queues(const std::vector<std::string> &group) {
    for (const auto &name : group) {
        this->sync_groups[name] = group_id;
    }
    group_id += 1;
}

bool mimo::Inputs::can_pop() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first).can_pop();
    }
    return std::any_of(
            groups.begin(),
            groups.end(),
            [](const std::pair<unsigned int, bool> &item){ return item.second; }
    );
}

mimo::InputQueue &mimo::Inputs::operator[](const std::string &name) {
    return this->queues.at(name);
}

std::unordered_map<std::string, mimo::InputQueue>::const_iterator mimo::Inputs::begin() const {
    return this->queues.begin();
}

std::unordered_map<std::string, mimo::InputQueue>::const_iterator mimo::Inputs::end() const {
    return this->queues.end();
}

bool mimo::Inputs::is_empty() const {
    return this->queues.empty();
}

bool mimo::Inputs::is_closed() const {
    return std::all_of(this->queues.begin(),
                       this->queues.end(),
                       [](const mimo::InputQueue &queue){ return queue.is_closed(); });
}
