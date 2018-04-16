#include <workflow/Input.h>
#include "job_depots/SynchronousJobDepot.h"
#include "queues/IQueueBundle.h"
#include "IJob.h"


mimo::SynchronousJobDepot::SynchronousJobDepot(
    unsigned int capacity,
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<mimo::Step> step,
    std::shared_ptr<mimo::IJobFactory> job_factory
) :
    available(true),
    capacity(capacity),
    identifier(identifier),
    step(std::move(step)),
    job_factory(std::move(job_factory))
{
    auto job = this->job_factory->make_shared(this->identifier, this->step);
    this->current_job = job->get_job_id();
    this->jobs.emplace(this->current_job, job);
}

void mimo::SynchronousJobDepot::add_entity(const std::shared_ptr<workflow::Input> &input,
                                             std::shared_ptr<mimo::Entity> entity) {
    this->jobs.at(this->current_job)->get_inputs()->push(input->name, entity);
}

bool mimo::SynchronousJobDepot::has_runnable_job() const {
    return this->available && this->jobs.size() <= this->capacity && this->jobs.at(this->current_job)->can_run();
}

std::shared_ptr<mimo::IJob> mimo::SynchronousJobDepot::get_runnable_job() {
    if (!this->has_runnable_job()) {
        throw std::runtime_error("Job is not available.");
    }
    this->available = this->jobs.size() < this->capacity;
    auto old_job = this->jobs.at(this->current_job);
    auto new_job = this->job_factory->make_shared(this->identifier, this->step);
    this->current_job = new_job->get_job_id();
    this->jobs.emplace(this->current_job, new_job);
    return old_job;
}

void mimo::SynchronousJobDepot::return_complete_job(std::shared_ptr<mimo::IJob> job) {
    if (job->get_step_id() != this->identifier) {
        throw std::runtime_error("Returned job does not belong to manager.");
    }
    this->jobs.erase(job->get_job_id());
}
