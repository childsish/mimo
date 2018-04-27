#include <workflow/Step.h>
#include "mimo/Step.h"
#include "Job.h"
#include "queues/IQueue.h"
#include "queues/IQueueBundle.h"
#include "mimo/Inputs.h"
#include "mimo/Outputs.h"


unsigned int NEXT_JOB_ID = 0;

mimo::Job::Job(
        const std::shared_ptr<workflow::Step> &step_id,
        std::shared_ptr<Step> step,
        std::shared_ptr<IQueueBundleFactory> factory
) : step_id(step_id),
    job_id(NEXT_JOB_ID),
    step(std::move(step)),
    inputs(factory->make_shared(step_id->get_inputs())),
    outputs(factory->make_shared(step_id->get_outputs()))
{
    NEXT_JOB_ID += 1;
}

const std::shared_ptr<workflow::Step> mimo::Job::get_step_id() const {
    return this->step_id;
}

unsigned int mimo::Job::get_job_id() const {
    return this->job_id;
}

std::shared_ptr<mimo::IQueueBundle> &mimo::Job::get_inputs() {
    return this->inputs;
}

std::shared_ptr<mimo::IQueueBundle> &mimo::Job::get_outputs() {
    return this->outputs;
}

bool mimo::Job::can_run() const {
    auto pop_status = this->inputs->get_pop_status();
    auto push_status = this->outputs->get_push_status();
    return !this->completed && (
         pop_status == IQueueBundle::PopStatus::NO_QUEUE ||
         pop_status == IQueueBundle::PopStatus::CAN_POP
    ) && (
         push_status == IQueueBundle::PushStatus::NO_QUEUE ||
         push_status == IQueueBundle::PushStatus::CAN_PUSH
    );
}

void mimo::Job::run() {
    Inputs inputs_(this->inputs);
    Outputs outputs_(this->outputs);
    this->completed = this->step->run(inputs_, outputs_);
}
