#include "JobManager.h"

#include <algorithm>
#include "errors.h"
#include "Job.h"


mimo::JobManager::JobManager(const workflow::Workflow &workflow__, unsigned int capacity_) :
        workflow_(workflow__),
        capacity(capacity_) {}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Input> input,
                                  std::shared_ptr<mimo::Entity> entity) {
    this->inputs[input->identifier].push(entity);
    for (const auto &step : this->workflow_.get_connected_steps(input)) {
        if (this->step_is_ready(step)) {
            this->ready_steps.push_back(step);
        }
    }
}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Output> identifier,
                                  std::shared_ptr<mimo::Entity> entity) {
    for (const auto &input : this->workflow_.get_connected_inputs(identifier)) {
        this->add_entity(input, entity);
    }
}

bool mimo::JobManager::has_job() const {
    return std::any_of(
        this->ready_steps.begin(),
        this->ready_steps.end(),
        [=](const std::shared_ptr<workflow::Step> &step) {
            return this->counts.at(step->identifier).use_count() < this->capacity;
        }
    );
}

std::unique_ptr<mimo::Job> mimo::JobManager::get_job() {
    if (!this->has_job()) {
        throw JobManagerError("No jobs available.");
    }
    auto identifier = this->ready_steps.front();
    this->ready_steps.pop_front();
    auto step = std::make_shared<Step>();
    return std::make_unique<mimo::Job>(step, this->constructors[step]);
}

bool mimo::JobManager::step_is_ready(const std::shared_ptr<workflow::Step> step) const {
    auto inputs = this->workflow_.get_connected_inputs(step);
    return std::all_of(
        inputs.begin(),
        inputs.end(),
        [=](const std::shared_ptr<workflow::Input> &input) {
            return !this->inputs.at(input->identifier).empty();
        }
    );
}
