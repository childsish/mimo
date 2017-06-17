#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Stream.h"
#include <iostream>


template<typename E>
class Print : public Stream {
public:

    Print() : Stream("print", {"input"}, {}) {}

    void run() {
        E *entity;
        while (can_run()) {
            entity = static_cast<E *>(inputs["input"].pop());
            std::cout << *entity << std::endl;
            collect(entity);
        }
    }
};

#endif //MIMO_CAPTURE_H
