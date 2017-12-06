#ifndef MIMO_CAPTURE_H
#define MIMO_CAPTURE_H

#include "Step.h"


class Capture : public Step {
public:

    std::vector<Entity *> captured;

    Capture() : Step("capture", {"input"}, {}) {}
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
