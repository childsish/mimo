#include "queues/Queue.h"

#include <iostream>

unsigned int mimo::Queue::CAPACITY = 100;

mimo::Queue::Queue(unsigned int run_, unsigned int capacity) :
    run(run_),
    capacity(capacity),
    end_of_run(false) {}

/*mimo::Queue::~Queue() {
    if (!this->entities.empty()) {
        std::cerr << "Warning: queue not emptied before deletion." << std::endl;
    }
}*/

bool mimo::Queue::push(std::shared_ptr<mimo::Entity> entity) {
    if (this->end_of_run) {
        throw std::runtime_error("Can not push to a is_end_of_run queue.");
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

void mimo::Queue::end_run() {
    this->end_of_run = true;
}

bool mimo::Queue::is_end_of_run() const {
    return this->end_of_run;
}

bool mimo::Queue::can_push() const {
    return !this->end_of_run && this->entities.size() < this->capacity;
}

bool mimo::Queue::can_pop() const {
    return !this->entities.empty();
}
