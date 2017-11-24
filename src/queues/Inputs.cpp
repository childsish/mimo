/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <algorithm>
#include "queues/Inputs.h"

mimo::Inputs::Inputs() {}

mimo::Inputs::Inputs(std::vector<std::string> names, unsigned int threshold_) {
    for (auto name : names) {
        this->queues.emplace(name, InputQueue{});
    }
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
