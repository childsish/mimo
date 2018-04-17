/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <iostream>
#include "QueueChannel.h"
#include <queues/OutputQueue.h>


unsigned int mimo::QueueChannel::CAPACITY = 10;

mimo::QueueChannel::QueueChannel(unsigned int capacity_) :
    capacity(capacity_),
    current_push(0),
    current_pop(0)
{
    if (capacity_ < 1) {
        throw std::runtime_error("QueueChannel initialised with too little capacity.");
    }
}

void mimo::QueueChannel::push(mimo::OutputQueue &queue) {
    PushStatus status = this->get_push_status(queue.get_task());
    if (status == PUSH_FULL) {
        throw std::runtime_error("Can not push: no space left in channel for push.");
    }
    else if (status == PUSH_NEXT) {
        throw std::runtime_error("Can not push: last place must be pushed by next task.");
    }
    else if (status == PUSH_ENDED) {
        throw std::runtime_error("Can not push: queue is from task that has ended");
    }

    if (queue.is_end_of_task()) {
        this->ended_queues.insert(queue.get_task());
    }
    this->queues[queue.get_task()].push(std::move(queue.release()));
}

const std::unique_ptr<mimo::Queue> &mimo::QueueChannel::peek() const {
    if (this->get_pop_status() == CAN_NOT_POP) {
        throw std::runtime_error("Can not peek: Front queue is not the next in line.");
    }
    return this->queues.at(this->current_pop).front();
}

std::unique_ptr<mimo::Queue> mimo::QueueChannel::pop() {
    if (this->get_pop_status() == CAN_NOT_POP) {
        throw std::runtime_error("Can not pop: Front queue is not the next in line.");
    }
    std::unique_ptr<Queue> queue(std::move(this->queues.at(this->current_pop).front()));
    this->queues.at(this->current_pop).pop();
    if (this->ended_queues.find(this->current_pop) != this->ended_queues.end()) {
        this->queues.erase(this->current_pop);
        this->ended_queues.erase(this->current_pop);
        this->current_pop += 1;
    }
    return queue;
}

mimo::QueueChannel::PushStatus mimo::QueueChannel::get_push_status(unsigned int task) const {
    if (this->ended_queues.find(task) != this->ended_queues.end() || task < this->current_push) {
        return PUSH_ENDED;
    }
    else if (this->usage() >= this->capacity) {
        return PUSH_FULL;
    }
    else if (this->usage() == this->capacity - 1 && task != this->current_push) {
        return PUSH_NEXT;
    }
    return CAN_PUSH;
}

mimo::QueueChannel::PopStatus mimo::QueueChannel::get_pop_status() const {
    if (this->queues.find(this->current_pop) != this->queues.end() && !this->queues.at(this->current_pop).empty()) {
        return CAN_POP;
    }
    return CAN_NOT_POP;
}

unsigned long mimo::QueueChannel::usage() const {
    unsigned long total = 0;
    for (const auto &item : this->queues) {
        total += item.second.size();
    }
    return total;;
}

void mimo::QueueChannel::lock() {
    this->mutex.lock();
}

void mimo::QueueChannel::unlock() {
    this->mutex.unlock();
}
