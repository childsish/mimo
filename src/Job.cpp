/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "Job.h"

#include <workflow/Step.h>
#include "Step.h"
#include "queues/JobInputs.h"
#include "queues/JobOutputs.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


mimo::Job::Job(
        const std::shared_ptr<workflow::Step> identifier_,
        std::unique_ptr<Step> step_,
        std::unique_ptr<JobInputs> inputs_,
        std::unique_ptr<JobOutputs> outputs_
) : identifier(identifier_),
    step(std::move(step_)),
    inputs(std::move(inputs_)),
    outputs(std::move(outputs_)) {}

std::unique_ptr<mimo::JobInputs> &mimo::Job::ins() {
    return this->inputs;
}

std::unique_ptr<mimo::JobOutputs> &mimo::Job::outs() {
    return this->outputs;
}

void mimo::Job::run() {
    mimo::Inputs inputs_(this->inputs);
    mimo::Outputs outputs_(this->outputs);
    this->completed = this->step->run(inputs_, outputs_);
    if (this->completed) {
        this->outputs->end_run();
        if (this->inputs->is_empty() || this->inputs->is_closed()) {
            this->outputs->close();
        }
    }
}

bool mimo::Job::is_complete() const {
    return this->completed;
}
