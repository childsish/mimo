#include "mimo/Step.h"

mimo::Step::Step(
    const std::string &name,
    const std::vector<std::string> &inputs,
    const std::vector<std::string> &outputs
) :
    name(name),
    inputs(inputs),
    outputs(outputs) {}

const std::string &mimo::Step::get_name() const {
    return this->name;
}

const std::vector<std::string> &mimo::Step::get_inputs() const {
    return this->inputs;
}

const std::vector<std::string> &mimo::Step::get_outputs() const {
    return this->outputs;
}
