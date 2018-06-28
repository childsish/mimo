#include "SingleJobDepot.h"

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
    if (!this->job)
        return false;

    auto buffer_status = this->buffer->get_pop_status();
    auto job_input_status = this->job->get_inputs()->get_pop_status();
    auto job_output_status = this->job->get_outputs()->get_push_status();
    return !this->job->is_complete() && (
        buffer_status == IInputs::PopStatus::NO_QUEUE ||
        buffer_status == IInputs::PopStatus::CAN_POP ||
        job_input_status == IInputs::PopStatus::CAN_POP
    ) && (
        job_output_status == IOutputs::PushStatus::NO_QUEUE ||
        job_output_status == IOutputs::PushStatus::CAN_PUSH
    );
}

std::vector<std::unique_ptr<mimo::IJob>> mimo::SingleJobDepot::get_runnable_jobs() {
    std::vector<std::unique_ptr<IJob>> jobs;
    if (this->has_runnable_jobs()) {
        this->job->transfer_input(*this->buffer);
        jobs.emplace_back(std::move(this->job));
    }
    return jobs;
}

void mimo::SingleJobDepot::return_exhausted_job(std::unique_ptr<mimo::IJob> job) {
    this->job = std::move(job);
}
