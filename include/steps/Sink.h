#ifndef MIMO_SINK_H
#define MIMO_SINK_H

#include "Step.h"


class Sink : public Step {
public:

    Sink() : Step("sink", {"input"}, {}) {}

    void run() {
        Entity *entity;
        while (can_run()) {
            entity = inputs["input"].pop();
            collect(entity);
        }
    }
};


#endif //MIMO_SINK_H
