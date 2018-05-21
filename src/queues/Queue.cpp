/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between steps.
 */
#include "Queue.h"

#include <iostream>
#include "../errors.h"


unsigned int mimo::Queue::CAPACITY = 100;

mimo::Queue::Queue(unsigned int capacity) :
    capacity(capacity),
    end_of_task(false),
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
    this->entities.pop_front();
    return entity;
}

void mimo::Queue::push(std::shared_ptr<mimo::Entity> entity) {
    if (this->entities.size() >= this->capacity) {
        throw QueueError("Queue is full.");
    } else if (this->end_of_task) {
        throw QueueError("Can not push to a is_end_of_task queue.");
    }
    this->entities.push_back(entity);
}

void mimo::Queue::push(const mimo::IQueue &queue) {
    for (const auto& entity : queue) {
        this->entities.push_back(entity);
    }
}

bool mimo::Queue::can_pop() const {
    return !this->entities.empty();
}

bool mimo::Queue::can_push() const {
    return !this->end_of_task && this->entities.size() < this->capacity;
}

void mimo::Queue::end_task() {
    this->end_of_task = true;
}

bool mimo::Queue::is_end_of_task() const {
    return this->end_of_task;
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

const mimo::IQueueIterator mimo::Queue::begin() const {
    return QueueIterator(this->entities.begin());
}

const mimo::IQueueIterator mimo::Queue::end() const {
    return QueueIterator(this->entities.end());
}


mimo::QueueIterator::QueueIterator(std::deque::iterator iterator) : iterator(iterator) {}

bool mimo::QueueIterator::operator!=(const mimo::IQueueIterator &that) {
    return this->iterator != static_cast<const QueueIterator &>(that).iterator;
}

std::shared_ptr<mimo::Entity> mimo::QueueIterator::operator*() {
    return *this->iterator;
}

mimo::IQueueIterator mimo::QueueIterator::operator++() {
    ++this->iterator;
}
