/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between steps.
 */
#include "queues/Queue.h"

#include <iostream>
#include "errors.h"


unsigned int mimo::Queue::CAPACITY = 100;

mimo::Queue::Queue(unsigned int capacity) :
    capacity(capacity),
    end_of_run(false),
    closed(false) {}

std::shared_ptr<mimo::Entity> mimo::Queue::peek() {
    if (this->entities.empty()) {
        throw QueueError("Trying to peek in empty queue.");
    }

    return this->entities.front();
}

std::shared_ptr<mimo::Entity> mimo::Queue::pop() {
    if (this->entities.empty()) {
        throw QueueError("Trying to pop from empty queue.");
    }

    auto entity = this->entities.front();
    this->entities.pop();
    return entity;
}

void mimo::Queue::push(std::shared_ptr<mimo::Entity> entity) {
    if (this->end_of_run) {
        throw QueueError("Can not push to a is_end_of_run queue.");
    }
    this->entities.push(entity);
}

bool mimo::Queue::can_pop() const {
    return !this->entities.empty();
}

bool mimo::Queue::can_push() const {
    return !this->end_of_run && this->entities.size() < this->capacity;
}

void mimo::Queue::end_run() {
    this->end_of_run = true;
}

bool mimo::Queue::is_end_of_run() const {
    return this->end_of_run;
}

void mimo::Queue::close() {
    this->closed = true;
}

bool mimo::Queue::is_closed() const {
    return this->closed;
}

bool mimo::Queue::is_empty() const {
    return this->entities.empty();
}

bool mimo::Queue::is_full() const {
    return this->entities.size() >= this->capacity;
}
