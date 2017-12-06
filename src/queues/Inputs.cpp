/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include "queues/Inputs.h"

mimo::Inputs::Inputs(bool synchronous_) : synchronous(synchronous_) {}

void mimo::Inputs::add_queue(const std::string &name, std::unique_ptr<mimo::Queue> queue) {
    this->queues.emplace({name, queue});
}

std::unique_ptr<mimo::Queue> mimo::Inputs::release_queue(const std::string &name) {
    std::unique_ptr<mimo::Queue> queue = this->queues[name].release_queue();
    this->queues.erase(name);
    return queue;
}

bool mimo::Inputs::can_pop() const {
    return std::any_of(
            this->queues.begin(),
            this->queues.end(),
            [](std::pair<std::string, mimo::Queue> item){ return item.second.can_pop(); }
    );
}

mimo::InputQueue &mimo::Inputs::operator[](const std::string &name) {
    return this->queues.at(name);
}

mimo::InputQueue &mimo::Inputs::operator[](const std::string name) {
    return this->queues[name];
}

std::unordered_map<std::string, mimo::InputQueue>::iterator mimo::Inputs::begin() const {
    return this->queues.begin();
}

std::unordered_map<std::string, mimo::InputQueue>::iterator mimo::Inputs::end() const {
    return this->queues.end();
}
