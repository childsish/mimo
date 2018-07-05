#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include <mimo/Step.h>
#include <iostream>
#include "../entities/Integer.h"


template<typename E>
class Print : public mimo::Step {
public:

    Print(
        const std::string prefix = "",
        const std::string suffix = ""
    ) :
        name("Print"),
        inputs({"input"}),
        outputs({}),
        prefix(prefix),
        suffix(suffix) {}

    const std::string &get_name() const override {
        return this->name;
    }

    const std::vector<std::string> &get_inputs() const override {
        return this->inputs;
    }

    const std::vector<std::string> &get_outputs() const override {
        return this->outputs;
    }

    bool run(mimo::IInputs &ins, mimo::IOutputs &outs) {
        std::shared_ptr<E> entity;
        while (ins.get_pop_status() == mimo::IInputs::PopStatus::CAN_POP) {
            entity = std::static_pointer_cast<E>(ins.pop("input"));
            std::cout << prefix << *entity << suffix << std::endl;
        }
        return ins.is_closed("input");
    }

private:

    std::string name;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    const std::string prefix;
    const std::string suffix;

};

#endif //MIMO_CAPTURE_H
