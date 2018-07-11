#ifndef MIMO_RANGE_H
#define MIMO_RANGE_H

#include <mimo/Step.h>
#include "../entities/Integer.h"


class Range : public mimo::Step {
public:

    Range(int to) :
        Step("Range", {}, {"output"}),
        from(0),
        to(to),
        step(1)
    {
        if (to < 0) {
            throw std::runtime_error("Parameter to must be positive.");
        }
    }

    Range(int from, int to, int step = 1) :
        Step("Range", {}, {"output"}),
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

    bool run(mimo::IInputs &ins, mimo::IOutputs &outs) {
        while (outs.get_push_status() == mimo::IOutputs::PushStatus::CAN_PUSH) {
            outs.push("output", std::make_shared<Integer>(this->from));
            this->from += this->step;
            if (this->step > 0 && this->from >= this->to || this->step < 0 && this->from < this->to) {
                outs.close();
                return true;
            }
        }
        return false;
    }

private:

    int from;
    int to;
    int step;

};

#endif //MIMO_RANGE_H
