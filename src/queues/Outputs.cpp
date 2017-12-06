/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include "queues/Outputs.h"

mimo::Outputs::Outputs(bool synchronous_) : synchronous(synchronous_) {}

void mimo::Outputs::add_queue(const std::string &name, std::unique_ptr<mimo::Queue> queue) {
    this->queues.emplace({name, queue});
}

std::unique_ptr<mimo::Queue> mimo::Outputs::release_queue(const std::string &name) {
    std::unique_ptr<mimo::Queue> queue = this->queues[name].release_queue();
    this->queues.erase(name);
    return queue;
}

bool mimo::Outputs::can_push() const {
    if (this->synchronous) {
        return std::all_of(
                this->queues.begin(),
                this->queues.end(),
                [](std::pair<std::string, std::unique_ptr<mimo::Queue>> item){ return item.second->can_push(); }
        );
    }
    return std::any_of(
            this->queues.begin(),
            this->queues.end(),
            [](std::pair<std::string, std::unique_ptr<mimo::Queue>> item){ return item.second->can_push(); }
    );
}

mimo::OutputQueue &mimo::Outputs::operator[](const std::string name) {
    return this->queues[name];
}

std::unordered_map<std::string, mimo::OutputQueue>::iterator mimo::Outputs::begin() const {
    return this->queues.begin();
}

std::unordered_map<std::string, mimo::OutputQueue>::iterator mimo::Outputs::end() const {
    return this->queues.end();
}
