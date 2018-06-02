#include "SingleJobDepot.h"

#include "IJob.h"

mimo::SingleJobDepot::SingleJobDepot(
    std::shared_ptr<workflow::Step> identifier,
    std::shared_ptr<mimo::Step> step,
    std::shared_ptr<mimo::IJobFactory> job_factory
) :
    identifier(std::move(identifier)),
    step(step),
    job(job_factory->make_unique(identifier, step)),
    buffer(identifier->get_inputs())
{
}

void mimo::SingleJobDepot::push(
    const std::shared_ptr<workflow::Input> &input_id,
    std::shared_ptr<mimo::Entity> entity
) {
    this->job->push(input_id, std::move(entity));
}

bool mimo::SingleJobDepot::can_queue(const std::shared_ptr<workflow::Input> &input_id) {
    return this->buffer.get_push_status(input_id->name) == IQueueBundle::PushStatus::CAN_PUSH;
}

void mimo::SingleJobDepot::queue_input(
    const std::shared_ptr<workflow::Input> &input_id,
    const IQueue &queue
) {
    this->buffer.push(input_id->name, queue);
}

bool mimo::SingleJobDepot::has_runnable_jobs() const {
    return bool(this->job);
}

std::set<std::unique_ptr<mimo::IJob>> mimo::SingleJobDepot::get_runnable_jobs() {
    std::set<std::unique_ptr<IJob>> jobs;
    if (this->job) {
        this->job->transfer_input(this->buffer);
        jobs.emplace(std::move(this->job));
    }
    return jobs;
}

void mimo::SingleJobDepot::return_exhausted_job(std::unique_ptr<mimo::IJob> job) {
    this->job = std::move(job);
}
