/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <iostream>
#include "queues/QueueChannel.h"


unsigned int mimo::QueueChannel::CAPACITY = 100;

mimo::QueueChannel::QueueChannel(unsigned int capacity_) :
    capacity(capacity_),
    current_identifier(0)
{
    if (capacity_ < 1) {
        throw std::runtime_error("QueueChannel initialised with too little capacity.");
    }
}

unsigned int mimo::QueueChannel::reserve() {
    if (!this->can_push()) {
        throw std::runtime_error("Channel has no extra capacity.");
    }
    this->reservations.insert(this->current_identifier);
    return this->current_identifier++;
}

void mimo::QueueChannel::push(std::unique_ptr<mimo::Queue> queue, unsigned int identifier) {
    if (this->reservations.find(identifier) == this->reservations.end()) {
        throw std::runtime_error("No reservation made for this queue.");
    }
    this->reservations.erase(identifier);
    this->queues.push(std::move(queue));
}

std::unique_ptr<mimo::Queue> mimo::QueueChannel::pop() {
    std::unique_ptr<Queue> queue(std::move(queues.front()));
    this->queues.pop();
    return queue;
}

bool mimo::QueueChannel::can_push() const {
    return this->reservations.size() + this->queues.size() < this->capacity;
}

bool mimo::QueueChannel::can_pop() const {
    return !this->queues.empty();
}
