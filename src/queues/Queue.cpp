#include "queues/Queue.h"

#include <iostream>

unsigned int mimo::Queue::CAPACITY = 100;

mimo::Queue::Queue(unsigned int run_, unsigned int capacity) :
    run(run_),
    capacity(capacity),
    _closed(false) {}

/*mimo::Queue::~Queue() {
    if (!this->entities.empty()) {
        std::cerr << "Warning: queue not emptied before deletion." << std::endl;
    }
}*/

bool mimo::Queue::push(std::shared_ptr<mimo::Entity> entity) {
    if (this->_closed) {
        throw std::runtime_error("Can not push to a closed queue.");
    }
    this->entities.push(entity);
    return this->can_push();
}

std::shared_ptr<mimo::Entity> mimo::Queue::peek() {
    if (this->entities.empty()) {
        throw std::runtime_error("Trying to peek in empty queue.");
    }

    return this->entities.front();
}

std::shared_ptr<mimo::Entity> mimo::Queue::pop() {
    if (this->entities.empty()) {
        throw std::runtime_error("Trying to pop from empty queue.");
    }

    auto entity = this->entities.front();
    this->entities.pop();
    return entity;
}

void mimo::Queue::close() {
    this->_closed = true;
}

bool mimo::Queue::closed() const {
    return this->_closed;
}

bool mimo::Queue::can_push() const {
    return !this->_closed && this->entities.size() < this->capacity;
}

bool mimo::Queue::can_pop() const {
    return !this->entities.empty();
}
