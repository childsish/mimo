/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include <queues/Queue.h>
#include "QueueFactory.h"

mimo::QueueFactory::QueueFactory(unsigned int capacity) : capacity(capacity) {}

mimo::IQueue *mimo::QueueFactory::make_raw() const {
    return new Queue(this->capacity);
}

std::shared_ptr<mimo::IQueue> mimo::QueueFactory::make_shared() const {
    return std::make_shared<Queue>(this->capacity);
}

std::unique_ptr<mimo::IQueue> mimo::QueueFactory::make_unique() const {
    return std::make_unique<Queue>(this->capacity);
}
