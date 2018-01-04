/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <Job.h>

#include <interfaces/IQueueFactory.h>


mimo::Job::Job(const workflow::Step &identifier_, mimo::Step &step_, const IQueueFactory &factory_) :
    identifier(identifier_),
    step(step_),
    factory(factory_)
{
    for (auto &input : identifier_.ins) {
        this->inputs.emplace(input.first, std::move(factory.make()));
    }
    for (auto &output : identifier_.outs) {
        this->outputs.emplace(output.first, std::move(factory.make()));
    }
}

void mimo::Job::set_inputs(std::unordered_map<std::string, std::unique_ptr<mimo::IQueue>> &inputs_) {
    this->inputs.empty();
    for (auto &item : inputs_) {
        this->inputs.emplace(item.first, std::move(item.second));
    }
}

void mimo::Job::run() {
    mimo::Inputs inputs_(this->identifier.ins, this->inputs);
    mimo::Outputs outputs_(this->identifier.outs, this->outputs);
    this->completed = this->step.run(inputs_, outputs_);
    if (this->completed) {
        outputs_.end_run();
        if (inputs_.is_empty() || inputs_.is_closed()) {
            outputs_.close();
        }
    }
}

bool mimo::Job::is_complete() const {
    return this->completed;
}
