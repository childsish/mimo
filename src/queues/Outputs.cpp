/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "queues/Outputs.h"


mimo::Outputs::Outputs(std::unique_ptr<mimo::JobOutputs> &outputs_) : outputs(outputs_) {}

mimo::Outputs::PushStatus mimo::Outputs::get_status() const {
    return static_cast<PushStatus>(this->outputs->get_status());
}

mimo::Outputs::PushStatus mimo::Outputs::get_status(const std::string &name) const {
    return static_cast<PushStatus>(this->outputs->get_status(name));
}

void mimo::Outputs::push(const std::string &name, std::shared_ptr<mimo::Entity> entity) {
    this->outputs->push(name, std::move(entity));
}
