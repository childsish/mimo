#include "JobManager.h"

mimo::JobManager::JobManager(const workflow::Workflow &workflow__, unsigned int capacity_) :
        workflow_(workflow__),
        capacity(capacity_) {}

bool mimo::JobManager::can_make_job(const std::shared_ptr<workflow::Step> &step) const {
    return this->counts.at(step->identifier) < this->capacity;
}

std::unique_ptr<mimo::Job> mimo::JobManager::make_job(const std::shared_ptr<workflow::Step> &step) {
    auto job = std::make_unique<mimo::Job>(step);
    this->counts.at(step->identifier) += 1;
    return job;
}

void mimo::JobManager::destroy_job(const std::shared_ptr<workflow::Step> &step) {
    this->counts.at(step->identifier) -= 1;
}
