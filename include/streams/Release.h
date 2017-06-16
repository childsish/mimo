#ifndef MIMO_RELEASE_H
#define MIMO_RELEASE_H

#include "Stream.h"

#include <deque>

class Release : public Stream {
public:

    std::deque<Entity *> released;

    Release(std::deque<Entity *> released_) : Stream("release", {}, {"output"}), released(released_) {}
    ~Release() {
        for (unsigned int i = 0; i < released.size(); ++i) {
            delete released[i];
        }
    }

    void run() {
        Entity *entity;
        while (can_run()) {
            outputs["output"].push(released.front());
            released.pop_front();
        }
    }

    bool can_run() const {
        return released.size() > 0 && Stream::can_run();
    }
};

#endif //MIMO_RELEASE_H
