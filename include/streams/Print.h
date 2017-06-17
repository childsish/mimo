#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Stream.h"
#include <iostream>


class Print : public Stream {
public:

    Print() : Stream("print", {"input"}, {}) {}

    void run() {
        Entity *entity;
        while (can_run()) {
            entity = inputs["input"].pop();
            std::cout << entity << std::endl;
            collect(entity);
        }
    }
};

#endif //MIMO_CAPTURE_H
