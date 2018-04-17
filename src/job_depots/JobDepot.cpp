#include <algorithm>
#include <workflow/Step.h>
#include "AsynchronousJobDepot.h"
#include "JobDepot.h"
#include "SynchronousJobDepot.h"
#include "../queues/IQueueBundle.h"
#include "../IJob.h"


mimo::JobDepot::JobDepot(
    std::shared_ptr<workflow::Workflow> workflow_,
    std::shared_ptr<ISingleJobDepotFactory> factory
) :
    workflow_(workflow_)
{
    for (const auto &step : workflow_->get_steps()) {
        this->jobs.emplace(step.second, std::move(factory->make_manager(step.second)));
    }
}

void mimo::JobDepot::add_entity(const std::shared_ptr<workflow::Input> &input,
                                  std::shared_ptr<mimo::Entity> entity) {
    auto &step = this->workflow_->get_connected_step(input);
    this->jobs[step]->add_entity(input, entity);
    if (this->jobs[step]->has_runnable_job()) {
        this->runnable_jobs.push(step);
    }
}

void mimo::JobDepot::add_entity(const std::shared_ptr<workflow::Output> &identifier,
                                  std::shared_ptr<mimo::Entity> entity) {
    for (const auto &input : this->workflow_->get_connected_inputs(identifier)) {
        this->add_entity(input, entity);
    }
}

bool mimo::JobDepot::has_runnable_job() const {
    return !this->runnable_jobs.empty();
}

std::shared_ptr<mimo::IJob> mimo::JobDepot::get_runnable_job() {
    if (!this->has_runnable_job()) {
        throw std::runtime_error("No jobs available.");
    }
    auto &step = this->runnable_jobs.front();
    this->runnable_jobs.pop();
    return this->jobs[step]->get_runnable_job();
}

void mimo::JobDepot::return_complete_job(std::shared_ptr<mimo::IJob> job) {
    this->jobs[job->get_step_id()]->return_complete_job(job);
}
