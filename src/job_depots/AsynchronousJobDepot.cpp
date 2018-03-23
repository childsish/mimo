#include <workflow/Input.h>
#include "job_ports/AsynchronousJobDepot.h"
#include "IJob.h"
#include "queues/IJobInputs.h"

#include <iostream>

mimo::AsynchronousJobDepot::AsynchronousJobDepot(
    const std::shared_ptr<workflow::Step> &identifier,
    std::shared_ptr<mimo::Step> step,
    std::shared_ptr<mimo::IJobFactory> job_factory
) :
    available(true),
    job(job_factory->make_shared(identifier, step)),
    identifier(identifier),
    step(step) {}

void mimo::AsynchronousJobDepot::add_entity(const std::shared_ptr<workflow::Input> &input,
                                              std::shared_ptr<mimo::Entity> entity) {
    this->job->get_inputs()->push(input->name, entity);
}

bool mimo::AsynchronousJobDepot::has_runnable_job() const {
    return this->available && this->job->can_run();
}

std::shared_ptr<mimo::IJob> mimo::AsynchronousJobDepot::get_runnable_job() {
    if (!this->has_runnable_job()) {
        throw std::runtime_error("Job is not available.");
    }
    this->available = false;
    return this->job;
}

void mimo::AsynchronousJobDepot::return_complete_job(std::shared_ptr<mimo::IJob> job) {
    if (job->get_step_id() != this->identifier) {
        throw std::runtime_error("Returned job does not belong to manager.");
    }
    this->available = true;
}
