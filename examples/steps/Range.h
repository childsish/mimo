#ifndef MIMO_RANGE_H
#define MIMO_RANGE_H

#include <mimo/Step.h>
#include "../entities/Integer.h"


class Range : public mimo::Step {
public:

    Range(int to) :
        name("Range"),
        inputs({}),
        outputs({"output"}),
        from(0),
        to(to),
        step(1)
    {
        if (to < 0) {
            throw std::runtime_error("Parameter to must be positive.");
        }
    }

    Range(int from, int to, int step = 1) :
        name("Range"),
        inputs({}),
        outputs({"output"}),
        from(from),
        to(to),
        step(step)
    {
        if (step == 0) {
            throw std::runtime_error("Parameter step must be non-zero.");
        }
        else if (from < to && step < 0 || to < from && step > 0) {
            throw std::runtime_error("Step iterates in opposite direction from 'from' to 'to'.");
        }
    }

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
        while (outs.get_push_status() == mimo::IOutputs::PushStatus::CAN_PUSH) {
            outs.push("output", std::make_shared<Integer>(this->from));
            this->from += this->step;
            if (this->step > 0 && this->from >= this->to || this->step < 0 && this->from < this->to) {
                return true;
            }
        }
        return false;
    }

private:

    std::string name;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    int from;
    int to;
    int step;

};

#endif //MIMO_RANGE_H
