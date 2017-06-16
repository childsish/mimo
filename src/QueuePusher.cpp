#include "QueuePusher.h"

bool QueuePusher::push(Entity *entity) {
    if (_queues.size() == 0) {
        throw std::runtime_error("No queues to push to.");
    }

    entity->reference_count += _queues.size() - 1;
    bool can_push = true;
    for (unsigned int i = 0; i < _queues.size(); ++i) {
        can_push = can_push && _queues[i]->push(entity);
    }
    return can_push;
}

bool QueuePusher::can_push() const {
    if (_queues.size() == 0) {
        return false;
    }

    bool can_push = true;
    for (unsigned int i = 0; i < _queues.size(); ++i) {
        can_push = can_push && _queues[i]->can_push();
    }
    return can_push;
}

void QueuePusher::add_queue(Queue *queue) {
    _queues.push_back(queue);
}
