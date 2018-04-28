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
    for (const auto &step_id : workflow_->get_steps()) {
        this->jobs.emplace(step_id.second, std::move(factory->make_depot(step_id.second)));
        if (step_id.second->get_inputs().empty()) {
            this->queue_job(step_id.second);
        }
    }
}

void mimo::JobDepot::add_entity(
    const std::shared_ptr<workflow::Input> &input,
    std::shared_ptr<mimo::Entity> entity)
{
    auto &step_id = this->workflow_->get_connected_step(input);
    this->jobs[step_id]->add_entity(input, entity);
    if (this->jobs[step_id]->has_runnable_job()) {
        this->queue_job(step_id);
    }
}

void mimo::JobDepot::add_entity(
    const std::shared_ptr<workflow::Output> &identifier,
    std::shared_ptr<mimo::Entity> entity)
{
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
    auto &step_id = this->runnable_jobs.front();
    this->runnable_jobs.pop();
    this->queued_runnable_jobs.erase(step_id);
    return this->jobs[step_id]->get_runnable_job();
}

void mimo::JobDepot::return_job(std::shared_ptr<mimo::IJob> job) {
    auto &step_id = job->get_step_id();
    this->jobs[step_id]->return_job(job);
    if (step_id->get_inputs().empty()) {
        this->queue_job(step_id);
    }
}

void mimo::JobDepot::queue_job(const std::shared_ptr<workflow::Step> step_id) {
    if (this->queued_runnable_jobs.find(step_id) == this->queued_runnable_jobs.end()) {
        this->runnable_jobs.push(step_id);
        this->queued_runnable_jobs.insert(step_id);
    }
}
