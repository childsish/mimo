#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include <mimo/Step.h>
#include <iostream>


template<typename E>
class Print : public mimo::Step {
public:

    Print(
        const std::string prefix = "",
        const std::string suffix = ""
    ) :
        Step("Print", {"input"}, {}),
        prefix(prefix),
        suffix(suffix) {}

    bool run(mimo::IInputs &ins, mimo::IOutputs &outs) {
        std::shared_ptr<E> entity;
        while (ins.get_pop_status() == mimo::IInputs::PopStatus::CAN_POP) {
            entity = std::static_pointer_cast<E>(ins.pop("input"));
            std::cout << prefix << *entity << suffix << std::endl;
        }
        return ins.is_closed("input");
    }

private:

    const std::string prefix;
    const std::string suffix;

};

#endif //MIMO_CAPTURE_H
