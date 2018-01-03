/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include <workflow/Input.h>
#include "queues/Inputs.h"
#include "queues/InputQueue.h"


mimo::Inputs::Inputs(const std::unordered_map<std::string, std::shared_ptr<workflow::Input>> &inputs) : group_id(0) {
    for (auto &item : inputs) {
        this->queues.emplace(item.first, InputQueue(item.second));
        this->sync_groups.emplace(item.first, this->group_id);
        this->group_id += 1;
    }
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
    std::unordered_map<std::string, mimo::InputQueue> queues_;
    return std::all_of(queues_.begin(),
                       queues_.end(),
                       [](const std::pair<const std::string, mimo::InputQueue> &item){ return item.second.is_closed(); });
}
