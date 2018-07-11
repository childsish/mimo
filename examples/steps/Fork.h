/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_SPLIT_H
#define MIMO_SPLIT_H

#include <mimo/Step.h>


template<typename E>
class Fork : public mimo::Step {
public:

    explicit Fork(
        bool (*condition)(const E &value)
    ) :
        Step("Fork", {"input"}, {"true", "false"}),
        condition(condition) {}

    bool run(mimo::IInputs &ins, mimo::IOutputs &outs) {
        std::shared_ptr<E> entity;
        while (ins.get_pop_status() == mimo::IInputs::PopStatus::CAN_POP) {
            entity = std::static_pointer_cast<E>(ins.pop("input"));
            if (this->condition(*entity)) {
                outs.push("true", entity);
            }
            else {
                outs.push("false", entity);
            }
        }
        return ins.is_closed("input");
    }

private:

    bool (*condition)(const E &value);

};

#endif //MIMO_SPLIT_H
