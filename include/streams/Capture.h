#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Stream.h"


class Capture : public Stream {
public:

    std::vector<Entity *> captured;

    Capture() : Stream("capture", {"input"}, {}) {}
    ~Capture() {
        for (auto entity : captured) {
            collect(entity);
        }
    }

    void run() {
        while (can_run()) {
            captured.push_back(inputs["input"].pop());
        }
    }
};

#endif //MIMO_CAPTURE_H
