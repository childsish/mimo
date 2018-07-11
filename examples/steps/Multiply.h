/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MULTIPLY_H
#define MIMO_MULTIPLY_H

#include <mimo/Step.h>
#include "entities/Integer.h"

class Multiply : public mimo::Step {
public:
    Multiply() : Step("multiply", {"multiplicand", "multiplier"}, {"result"}) {}

    bool run(mimo::IInputs &ins, mimo::IOutputs &outs) {
        std::shared_ptr<Integer> multiplicand;
        std::shared_ptr<Integer> multiplier;
        while (ins.get_pop_status() == mimo::IInputs::PopStatus::CAN_POP) {
            multiplicand = std::static_pointer_cast<Integer>(ins.pop("multiplicand"));
            multiplier = std::static_pointer_cast<Integer>(ins.pop("multiplier"));
            outs.push("result", std::make_shared<Integer>(multiplicand->value * multiplier->value));
        }
        return ins.is_closed("multiplicand") || ins.is_closed("multiplier");
    }
};

#endif //MIMO_MULTIPLY_H
