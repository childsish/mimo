#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Step.h"
#include <iostream>


template<typename E>
class Print : public Step {
public:

    Print(const std::string prefix = "", const std::string suffix = "") :
            Step("print", {"input"}, {}),
            _prefix(prefix),
            _suffix(suffix) {}

    void run() {
        E *entity;
        while (can_run()) {
            entity = static_cast<E *>(inputs["input"].pop());
            std::cout << _prefix << *entity << _suffix << std::endl;
            collect(entity);
        }
    }

private:

    const std::string _prefix;

    const std::string _suffix;

};

#endif //MIMO_CAPTURE_H
