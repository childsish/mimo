#include <workflow/Step.h>
#include "Step.h"
#include "queues/IQueue.h"
#include "queues/IJobInputs.h"
#include "queues/IJobOutputs.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "JobInputFactory.h"
#include "JobOutputFactory.h"
#include "Job.h"


mimo::Job::Job(
        const std::shared_ptr<workflow::Step> identifier,
        std::shared_ptr<Step> step,
        std::shared_ptr<IJobInputsFactory> inputs_factory,
        std::shared_ptr<IJobOutputsFactory> outputs_factory
) : identifier(identifier),
    step(std::move(step)),
    inputs(inputs_factory->make_unique(identifier->get_inputs())),
    outputs(outputs_factory->make_unique(identifier->get_outputs())),
    outputs_factory(outputs_factory) {}

const std::shared_ptr<workflow::Step> mimo::Job::get_identifier() {
    return this->identifier;
}

std::unique_ptr<mimo::IJobInputs> &mimo::Job::get_inputs() {
    return this->inputs;
}

std::unique_ptr<mimo::IJobOutputs> &mimo::Job::get_outputs() {
    return this->outputs;
}

bool mimo::Job::can_run() const {
    return this->inputs->get_status() == IJobInputs::PopStatus::CAN_POP
           && this->outputs->get_status() == IJobOutputs::PushStatus::CAN_PUSH;
}

void mimo::Job::run() {
    Inputs inputs_(this->inputs);
    Outputs outputs_(this->outputs);
    this->completed = this->step->run(inputs_, outputs_);
    this->outputs->end_run();
    if (this->completed && this->inputs->is_closed()) {
        this->outputs->close();
    }
}
