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
    this->reservations.insert(run);
    while (this->reservations.find(this->current_reserve) != this->reservations.end()) {
        this->current_reserve += 1;
    }
}

void mimo::QueueChannel::push(std::unique_ptr<mimo::Queue> queue) {
    if (this->usage() >= this->capacity) {
        throw std::runtime_error("Can not push to channel: Channel is full.");
    }
    else if (this->usage() == this->capacity - 1 && run != this->current_reserve) {
        throw std::runtime_error("Can not push to channel: Last place is reserved for next run.");
    }
    else if (this->reservations.find(queue->run) == this->reservations.end()) {
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
    if (!this->get_pop_status())  {
        throw std::runtime_error("Front queue is not the next in line.");
    }
    return this->queues.at(this->current_pop).front();
}

std::unique_ptr<mimo::Queue> mimo::QueueChannel::pop() {
    if (!this->get_pop_status()) {
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

ReserveStatus mimo::QueueChannel::get_reserve_status(unsigned int run) const {
    if (this->usage() >= this->capacity) {
        return RESERVE_FULL;
    }
    else if (this->usage() == this->capacity - 1 && run != this->current_reserve) {
        return RESERVE_NEXT;
    }
    else if (this->usage() < this->capacity - 1) {
        if (run < this->current_reserve) {
            return RESERVE_OLD;
        }
        else if (this->reservations.find(run) != this->reservations.end()) {
            return RESERVE_FOUND;
        }
    }
    return CAN_RESERVE;
}

PushStatus mimo::QueueChannel::get_push_status(unsigned int run) const {
    if (this->usage() >= this->capacity) {
        return PUSH_FULL;
    }
    else if (this->usage() == this->capacity - 1 && run != this->current_push) {
        return PUSH_NEXT;
    }
    else if (this->reservations.find(run) == this->reservations.end()) {
        return PUSH_UNEXPECTED;
    }
    return CAN_PUSH;
}

PopStatus mimo::QueueChannel::get_pop_status() const {
    if (this->queues.find(this->current_pop) != this->queues.end() && !this->queues.at(this->current_pop).empty()) {
        return CAN_POP;
    }
    return CAN_NOT_POP;
}

unsigned long mimo::QueueChannel::usage() const {
    unsigned long total = 0;
    for (auto item : this->queues) {
        total += item.second.size();
    }
    return total + this->reservations.size();
}
