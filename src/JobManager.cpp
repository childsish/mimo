#include <algorithm>
#include <workflow/Step.h>
#include "AsynchronousJobManager.h"
#include "IJob.h"
#include "SynchronousJobManager.h"
#include "queues/JobInputs.h"
#include "queues/JobOutputs.h"

#include "JobManager.h"


mimo::JobManager::JobManager(
    const workflow::Workflow &workflow_,
    std::shared_ptr<IJobManagerFactory> factory
) :
    workflow_(workflow_)
{
    for (const auto &step : workflow_.get_steps()) {
        this->jobs.emplace(step.second, std::move(factory->make_manager(step.second)));
    }
}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Input> &input,
                                  std::shared_ptr<mimo::Entity> entity) {
    auto &step = this->workflow_.get_connected_step(input);
    this->jobs[step]->add_entity(input, entity);
    if (this->jobs[step]->has_runnable_job()) {
        this->runnable_jobs.push(step);
    }
}

void mimo::JobManager::add_entity(const std::shared_ptr<workflow::Output> &identifier,
                                  std::shared_ptr<mimo::Entity> entity) {
    for (const auto &input : this->workflow_.get_connected_inputs(identifier)) {
        this->add_entity(input, entity);
    }
}

bool mimo::JobManager::has_runnable_job() const {
    return !this->runnable_jobs.empty();
}

std::shared_ptr<mimo::IJob> mimo::JobManager::get_runnable_job() {
    if (!this->has_runnable_job()) {
        throw std::runtime_error("No jobs available.");
    }
    auto &step = this->runnable_jobs.front();
    this->runnable_jobs.pop();
    return this->jobs[step]->get_runnable_job();
}

void mimo::JobManager::return_complete_job(std::shared_ptr<mimo::IJob> job) {
    this->jobs[job->get_step_id()]->return_complete_job(job);
}
