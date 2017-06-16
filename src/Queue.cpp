#include "Queue.h"

#include <iostream>


Queue::Queue(unsigned int threshold) : _threshold(threshold) {}

Queue::Queue(const Queue &&other) : _threshold(other._threshold), _entities(other._entities) {}

Queue::~Queue() {
    if (_entities.size() > 0) {
        std::cerr << "Warning: queue not emptied before deletion." << std::endl;
    }
}

bool Queue::push(Entity *entity) {
    _entities.push_back(entity);
    return can_push();
}

Entity *Queue::peek() {
    if (_entities.size() == 0) {
        throw std::runtime_error("Trying to peek in empty queue.");
    }

    return _entities.front();
}

Entity *Queue::pop() {
    if (_entities.size() == 0) {
        throw std::runtime_error("Trying to pop from empty queue.");
    }

    Entity *entity = _entities.front();
    _entities.pop_front();
    return entity;
}

bool Queue::can_push() const {
    return _entities.size() < _threshold;
}

bool Queue::can_pop() const {
    return _entities.size() > 0;
}
