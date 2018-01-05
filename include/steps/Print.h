#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Step.h"
#include <iostream>


template<typename E>
class Print : public mimo::Step {
public:

    Print(const std::string prefix = "", const std::string suffix = "") :
            _prefix(prefix),
            _suffix(suffix) {}

    bool run(mimo::Inputs &ins, mimo::Outputs &outs) {
        E *entity;
        while (ins.can_pop()) {
            entity = static_cast<E *>(ins["input"].pop());
            std::cout << _prefix << *entity << _suffix << std::endl;
        }
        return true;
    }

private:

    const std::string _prefix;

    const std::string _suffix;

};

#endif //MIMO_CAPTURE_H
