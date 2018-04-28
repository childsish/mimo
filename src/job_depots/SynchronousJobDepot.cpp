#include <workflow/Input.h>
#include "SynchronousJobDepot.h"
#include "../queues/IQueueBundle.h"
#include "../IJob.h"


mimo::SynchronousJobDepot::SynchronousJobDepot(
    unsigned int capacity,
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<mimo::Step> step,
    std::shared_ptr<mimo::IJobFactory> job_factory
) :
    active_jobs(0),
    capacity(capacity),
    identifier(identifier),
    step(std::move(step)),
    job_factory(std::move(job_factory))
{
    this->make_job();
}

void mimo::SynchronousJobDepot::add_entity(
    const std::shared_ptr<workflow::Input> &input,
    std::shared_ptr<mimo::Entity> entity)
{
    this->jobs.back()->get_inputs()->push(input->name, entity);
}

bool mimo::SynchronousJobDepot::has_runnable_job() const {
    return this->jobs.front()->can_run() &&
        this->active_jobs <= this->capacity;
}

std::shared_ptr<mimo::IJob> mimo::SynchronousJobDepot::get_runnable_job() {
    if (!this->has_runnable_job()) {
        std::stringstream message;
        message << this->identifier->name << " is not available.";
        throw std::runtime_error(message.str());
    }
    auto job = this->jobs.front();
    this->jobs.pop_front();
    if (this->jobs.empty()) {
        this->make_job();
    }
    return job;
}

void mimo::SynchronousJobDepot::return_job(std::shared_ptr<mimo::IJob> job) {
    if (job->get_step_id() != this->identifier) {
        std::stringstream message;
        message << "Can not return a " << job->get_step_id()->name << " to a "
                << this->identifier->name << " depot.";
        throw std::runtime_error(message.str());
    }
    if (job->is_complete()) {
        this->active_jobs -= 1;
    }
    else {
        this->jobs.push_front(job);
    }
}

void mimo::SynchronousJobDepot::make_job() {
    this->jobs.push_front(this->job_factory->make_shared(this->identifier, this->step));
    this->active_jobs += 1;
}
