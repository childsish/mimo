/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "mimo/IOutputs.h"


mimo::IOutputs::IOutputs(std::shared_ptr<mimo::IQueueBundle> &outputs_) : outputs(outputs_) {}

mimo::IOutputs::PushStatus mimo::IOutputs::get_status() const {
    return static_cast<PushStatus>(this->outputs->get_push_status());
}

mimo::IOutputs::PushStatus mimo::IOutputs::get_status(const std::string &name) const {
    return static_cast<PushStatus>(this->outputs->get_push_status(name));
}

void mimo::IOutputs::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    this->outputs->push(name, std::move(entity));
}
