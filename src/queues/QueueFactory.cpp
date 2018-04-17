/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "Queue.h"
#include "QueueFactory.h"

mimo::QueueFactory::QueueFactory(unsigned int capacity) : capacity(capacity) {}

mimo::IQueue *mimo::QueueFactory::make_raw() const {
    return new Queue(this->capacity);
}
