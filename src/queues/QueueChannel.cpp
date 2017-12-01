/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <iostream>
#include "queues/QueueChannel.h"


mimo::QueueChannel::QueueChannel(unsigned int capacity_) :
    capacity(capacity_),
    current_reserve(0),
    current_push(0),
    current_pop(0)
{
    if (capacity_ < 1) {
        throw std::runtime_error("QueueChannel initialised with too little capacity.");
    }
}

void mimo::QueueChannel::reserve(unsigned int run) {
    if (!this->can_reserve(run)) {
        throw std::runtime_error("Can not reserve run.");
    }
    this->reservations.insert(run);
    while (this->reservations.find(this->current_reserve) != this->reservations.end()) {
        this->current_reserve += 1;
    }
}

void mimo::QueueChannel::push(std::unique_ptr<mimo::Queue> queue) {
    if (this->reservations.find(queue->run) == this->reservations.end()) {
        throw std::runtime_error("No reservation made for this queue.");
    }
    this->reservations.erase(queue->run);
    if (queue->closed()) {
        this->closed_queues.insert(queue->run);
    }
    this->queues[queue->run].push(std::move(queue));
    while (this->queues.find(this->current_push) != this->queues.end()) {
        this->current_push += 1;
    }
}

const std::unique_ptr<mimo::Queue> &mimo::QueueChannel::peek() const {
    if (!this->can_pop())  {
        throw std::runtime_error("Front queue is not the next in line.");
    }
    return this->queues.at(this->current_pop).front();
}

std::unique_ptr<mimo::Queue> mimo::QueueChannel::pop() {
    if (!this->can_pop()) {
        throw std::runtime_error("Front queue is not the next in line.");
    }
    std::unique_ptr<Queue> queue(std::move(this->queues.at(this->current_pop).front()));
    this->queues.at(this->current_pop).pop();
    if (this->closed_queues.find(this->current_pop) != this->closed_queues.end()) {
        this->queues.erase(this->current_pop);
        this->closed_queues.erase(this->current_pop);
        this->current_pop += 1;
    }
    return queue;
}

bool mimo::QueueChannel::can_reserve(unsigned int run) const {
    return this->usage() < this->capacity - 1
              && run >= this->current_reserve
              && this->reservations.find(run) == this->reservations.end()
            || this->usage() == this->capacity - 1
              && this->current_reserve == run;
}

bool mimo::QueueChannel::can_push(unsigned int run) const {
    return this->usage() < this->capacity - 1
              && this->reservations.find(run) != this->reservations.end()
           || this->usage() == this->capacity - 1
              && this->current_push == run;
}

bool mimo::QueueChannel::can_pop() const {
    return this->queues.find(this->current_pop) != this->queues.end()
           && !this->queues.at(this->current_pop).empty();
}

bool mimo::QueueChannel::has_reservation(unsigned int run) const {
    return this->reservations.find(run) != this->reservations.end();
}

unsigned int mimo::QueueChannel::usage() const {
    unsigned int total = 0;
    for (auto item : this->queues) {
        total += item.second.size();
    }
    return total + this->reservations.size();
}
