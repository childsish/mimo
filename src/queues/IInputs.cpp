/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "mimo/IInputs.h"


mimo::IInputs::IInputs(std::shared_ptr<mimo::IQueueBundle> &inputs_) : inputs(inputs_) {}

mimo::IInputs::PopStatus mimo::IInputs::get_status() const {
    return static_cast<PopStatus>(this->inputs->get_pop_status());
}

mimo::IInputs::PopStatus mimo::IInputs::get_status(const std::string &name) const {
    return static_cast<PopStatus>(this->inputs->get_pop_status(name));
}

std::shared_ptr<mimo::Entity> mimo::IInputs::peek(const std::string &name) {
    return this->inputs->peek(name);
}

std::shared_ptr<mimo::Entity> mimo::IInputs::pop(const std::string &name) {
    return this->inputs->pop(name);
}
