#include "Queue.h"

#include <iostream>
#include "../errors.h"


unsigned int mimo::Queue::CAPACITY = 100;

mimo::Queue::Queue(unsigned int capacity) :
    capacity(capacity),
    closed(false) {}

unsigned long mimo::Queue::get_size() const {
    return this->entities.size();
}

bool mimo::Queue::can_pop() const {
    return !this->entities.empty();
}

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
    this->entities.pop_front();
    return entity;
}

bool mimo::Queue::can_push() const {
    return !this->closed && this->entities.size() < this->capacity;
}

void mimo::Queue::push(std::shared_ptr<mimo::Entity> entity) {
    if (this->closed) {
        throw QueueError("Can not push to a closed queue.");
    }
    this->entities.push_back(entity);
}

void mimo::Queue::push(const mimo::IQueue &queue) {
    for (auto entity : queue) {
        this->entities.push_back(std::move(entity));
    }
}

void mimo::Queue::clear() {
    this->entities.clear();
}

void mimo::Queue::close() {
    this->closed = true;
}

bool mimo::Queue::is_closed() const {
    return this->entities.empty() && this->closed;
}

mimo::IForwardIterator<std::shared_ptr<mimo::Entity>> mimo::Queue::begin() {
    return this->entities.begin();
}

mimo::IForwardIterator<std::shared_ptr<mimo::Entity>> mimo::Queue::begin() const {
    return this->entities.begin();
}

mimo::IForwardIterator<std::shared_ptr<mimo::Entity>> mimo::Queue::end() {
    return this->entities.end();
}

mimo::IForwardIterator<std::shared_ptr<mimo::Entity>> mimo::Queue::end() const {
    return this->entities.end();
}
