#ifndef MIMO_SINK_H
#define MIMO_SINK_H

#include "Stream.h"


class Sink : public Stream {
public:

    Sink() : Stream("sink", {"input"}, {}) {}

    void run() {
        Entity *entity;
        while (can_run()) {
            entity = inputs["input"].pop();
            collect(entity);
        }
    }
};


#endif //MIMO_SINK_H
