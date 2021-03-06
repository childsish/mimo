#include "Job.h"

#include "mimo/Step.h"
#include "queues/IQueue.h"
#include "queues/QueueBundleFactory.h"


mimo::Job::Job(
    unsigned int job_id,
    std::shared_ptr<workflow::Step> step_id,
    std::shared_ptr<Step> step
) :
    job_id(job_id),
    step_id(std::move(step_id)),
    step(std::move(step)),
    completed(false)
{
    QueueBundleFactory factory;
    this->inputs = factory.make_shared(*this->step_id->get_inputs());
    this->outputs = factory.make_shared(*this->step_id->get_outputs());
}

mimo::Job::Job(
    unsigned int job_id,
    std::shared_ptr<workflow::Step> step_id,
    std::shared_ptr<Step> step,
    std::shared_ptr<IQueueBundleFactory> factory
) :
    job_id(job_id),
    step_id(std::move(step_id)),
    step(std::move(step)),
    inputs(factory->make_shared(*this->step_id->get_inputs())),
    outputs(factory->make_shared(*this->step_id->get_outputs())),
    completed(false)
{
}

const std::shared_ptr<workflow::Step> mimo::Job::get_step_id() const {
    return this->step_id;
}

unsigned int mimo::Job::get_job_id() const {
    return this->job_id;
}

void mimo::Job::transfer_input(mimo::IQueueBundle &bundle) {
    for (const auto &item : bundle.get_identifiers()) {
        auto &to_queue = this->inputs->get_queue(item.first);
        auto &from_queue = bundle.get_queue(item.first);
        to_queue.push(from_queue);
    }
}

std::shared_ptr<mimo::IQueueBundle> mimo::Job::get_inputs() {
    return this->inputs;
}

std::shared_ptr<mimo::IQueueBundle> mimo::Job::get_outputs() {
    return this->outputs;
}

bool mimo::Job::can_run() const {
    auto pop_status = this->inputs->get_pop_status();
    auto push_status = this->outputs->get_push_status();
    return !this->completed && (
         pop_status == IInputs::PopStatus::NO_QUEUE ||
         pop_status == IInputs::PopStatus::CAN_POP
    ) && (
         push_status == IOutputs::PushStatus::NO_QUEUE ||
         push_status == IOutputs::PushStatus::CAN_PUSH
    );
}

void mimo::Job::run() {
    this->completed = this->step->run(*this->inputs, *this->outputs);
}

bool mimo::Job::is_complete() const {
    return this->completed;
}
