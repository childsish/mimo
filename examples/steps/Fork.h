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
      name("Split"),
      inputs({"input"}),
      outputs({"true", "false"}),
      condition(condition) {}

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
            if (this->condition(*entity)) {
                outs.push("true", entity);
            }
            else {
                outs.push("false", entity);
            }
        }
    }

private:

    std::string name;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    bool (*condition)(const E &value);

};

#endif //MIMO_SPLIT_H
