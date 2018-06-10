#include "SingleJobDepot.h"

#include "IJob.h"

mimo::SingleJobDepot::SingleJobDepot(
    std::shared_ptr<workflow::Step> step_id,
    std::shared_ptr<Step> step,
    std::shared_ptr<IJobFactory> job_factory,
    std::shared_ptr<IQueueBundleFactory> bundle_factory
) :
    step_id(std::move(step_id)),
    step(std::move(step)),
    job(job_factory->make_unique(this->step_id, this->step)),
    buffer(bundle_factory->make_unique(this->step_id->get_inputs()))
{
}

void mimo::SingleJobDepot::push(
    const workflow::Input &input_id,
    std::shared_ptr<mimo::Entity> entity
) {
    this->buffer->push(input_id.name, std::move(entity));
}

bool mimo::SingleJobDepot::can_queue(const workflow::Input &input_id) {
    return this->buffer->get_push_status(input_id.name) == IOutputs::PushStatus::CAN_PUSH;
}

void mimo::SingleJobDepot::queue_input(
    const workflow::Input &input_id,
    const IQueue &queue
) {
    this->buffer->get_queue(input_id.name).push(queue);
}

bool mimo::SingleJobDepot::has_runnable_jobs() const {
    return bool(this->job) && this->job->can_run();
}

std::set<std::unique_ptr<mimo::IJob>, mimo::JobComparator> mimo::SingleJobDepot::get_runnable_jobs() {
    std::set<std::unique_ptr<IJob>, JobComparator> jobs;
    if (this->has_runnable_jobs()) {
        this->job->transfer_input(*this->buffer);
        jobs.emplace(std::move(this->job));
    }
    return jobs;
}

void mimo::SingleJobDepot::return_exhausted_job(std::unique_ptr<mimo::IJob> job) {
    this->job = std::move(job);
}
