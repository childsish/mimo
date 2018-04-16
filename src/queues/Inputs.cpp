/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "queues/Inputs.h"


mimo::Inputs::Inputs(std::shared_ptr<mimo::IQueueBundle> &inputs_) : inputs(inputs_) {}

mimo::Inputs::PopStatus mimo::Inputs::get_status() const {
    return static_cast<PopStatus>(this->inputs->get_pop_status());
}

mimo::Inputs::PopStatus mimo::Inputs::get_status(const std::string &name) const {
    return static_cast<PopStatus>(this->inputs->get_pop_status(name));
}

std::shared_ptr<mimo::Entity> mimo::Inputs::peek(const std::string &name) {
    return this->inputs->peek(name);
}

std::shared_ptr<mimo::Entity> mimo::Inputs::pop(const std::string &name) {
    return this->inputs->pop(name);
}
