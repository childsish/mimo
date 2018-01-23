#include "JobManager.h"

#include "errors.h"


mimo::JobManager::JobManager(const workflow::Workflow &workflow__, unsigned int capacity_) :
        workflow_(workflow__),
        capacity(capacity_) {}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Input> identifier,
                                  std::shared_ptr<mimo::Entity> entity) {
    this->inputs[identifier->identifier].push(entity);
}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Output> identifier,
                                  std::shared_ptr<mimo::Entity> entity) {
    for (const auto &input : this->workflow_.get_connected_inputs(identifier)) {
        this->add_entity(input, entity);
    }
}

bool mimo::JobManager::has_job() const {
    return false;
}

std::unique_ptr<mimo::Job> mimo::JobManager::get_job() {
    if (!this->has_job()) {
        throw JobManagerError("No jobs available.");
    }
    return std::make_unique<mimo::Job>();
}
