/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "mimo/Outputs.h"


mimo::Outputs::Outputs(std::shared_ptr<mimo::IQueueBundle> &outputs_) : outputs(outputs_) {}

mimo::Outputs::PushStatus mimo::Outputs::get_status() const {
    return static_cast<PushStatus>(this->outputs->get_push_status());
}

mimo::Outputs::PushStatus mimo::Outputs::get_status(const std::string &name) const {
    return static_cast<PushStatus>(this->outputs->get_push_status(name));
}

void mimo::Outputs::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    this->outputs->push(name, std::move(entity));
}
