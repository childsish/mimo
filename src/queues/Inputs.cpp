/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <queues/Inputs.h>

#include <algorithm>
#include <interfaces/IQueue.h>
#include <workflow/Input.h>


mimo::Inputs::Inputs(
        const std::unordered_map<std::string, std::shared_ptr<workflow::Input>> &identifiers,
        std::unordered_map<std::string, std::unique_ptr<mimo::Queue>> &queues_,
        const std::unordered_map<std::string, unsigned int> &sync_groups_
) : group_id(0),
    queues(queues_),
    sync_groups(sync_groups_)
{
    for (auto &item : identifiers) {
        this->sync_groups.emplace(item.first, this->group_id);
        this->group_id += 1;
    }
}

mimo::Inputs::PopStatus mimo::Inputs::get_pop_status() const {
    auto groups = this->get_group_status();
    if (std::any_of(
            groups.begin(),
            groups.end(),
            [](const std::pair<unsigned int, bool> &item){ return item.second; }) {
        return PopStatus::CAN_POP;
    }
    return PopStatus::SYNC_QUEUE_EMPTY;
}

mimo::Inputs::PopStatus mimo::Inputs::get_pop_status(const std::string &name) const {
    if (this->queues.at(name)->is_empty()) {
        return PopStatus::QUEUE_EMPTY;
    }
    auto group_id = this->sync_groups.at(name);
    auto groups = this->get_group_status();
    if (!groups.at(group_id)) {
        return PopStatus ::SYNC_QUEUE_EMPTY;
    }
    return PopStatus::CAN_POP;
}

std::shared_ptr<mimo::Entity> &mimo::Inputs::peek(const std::string &name) {
    return <#initializer#>;
}

std::shared_ptr<mimo::Entity> mimo::Inputs::pop(const std::string &name) {
    return std::shared_ptr<mimo::Entity>();
}

bool mimo::Inputs::is_closed() const {
    std::unordered_map<std::string, mimo::InputQueue> queues_;
    return std::all_of(queues_.begin(),
                       queues_.end(),
                       [](const std::pair<const std::string, mimo::InputQueue> &item){ return item.second.is_closed(); });
}

std::unordered_map<unsigned int, bool> mimo::Inputs::get_group_status() const {
    std::unordered_map<unsigned int, bool> groups;
    for (const auto &item : this->sync_groups) {
        if (groups.find(item.second) == groups.end()) {
            groups[item.second] = true;
        }
        groups[item.second] &= this->queues.at(item.first).can_pop();
    }
    return groups;
}
