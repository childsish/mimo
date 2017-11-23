/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <iostream>
#include "queues/QueueChannel.h"


unsigned int mimo::QueueChannel::CAPACITY = 10;

mimo::QueueChannel::QueueChannel(unsigned int capacity_) :
    capacity(capacity_),
    current_index(0)
{
    if (capacity_ < 1) {
        throw std::runtime_error("QueueChannel initialised with too little capacity.");
    }
}

bool mimo::QueueChannel::reserve(std::unique_ptr<mimo::Queue> &queue) {
    unsigned int usage = this->usage();
    if (usage >= this->capacity) {
        return false;
    }
    else if (this->reservations.find(queue->index) != this->reservations.end()) {
        return false;
    }
    else if (usage == this->capacity - 1
             && this->queues.find(this->current_index) == this->queues.end()
             && queue->index != this->current_index) {
        return false;
    }
    this->reservations.insert(queue->index);
}

void mimo::QueueChannel::push(std::unique_ptr<mimo::Queue> queue) {
    if (this->reservations.find(queue->index) == this->reservations.end()) {
        throw std::runtime_error("No reservation made for this queue.");
    }
    this->reservations.erase(queue->index);
    this->queues[queue->index] = std::move(queue);
}

std::unique_ptr<mimo::Queue> mimo::QueueChannel::pop() {
    if (!this->can_pop()) {
        throw std::runtime_error("Front queue is not the next in line.");
    }
    std::unique_ptr<Queue> queue(std::move(this->queues[this->current_index]));
    this->queues.erase(this->current_index);
    this->current_index += 1;
    return queue;
}

bool mimo::QueueChannel::can_pop() const {
    return this->queues.find(this->current_index) != this->queues.end();
}

unsigned int mimo::QueueChannel::usage() const {
    return this->reservations.size() + this->queues.size();
}
