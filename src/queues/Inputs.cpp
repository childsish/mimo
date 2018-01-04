/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <queues/Inputs.h>

#include <algorithm>
#include <interfaces/IQueue.h>
#include <workflow/Input.h>


mimo::Inputs::Inputs(
        const std::unordered_map<std::string, std::shared_ptr<workflow::Input>> &inputs,
        std::unordered_map<std::string, std::unique_ptr<mimo::IQueue>> &queues_
) : group_id(0),
    queues(queues_)
{
    for (auto &item : inputs) {
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

std::unique_ptr<mimo::IQueue> &mimo::Inputs::operator[](const std::string &name) {
    return this->queues.at(name);
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
