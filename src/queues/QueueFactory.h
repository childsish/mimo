/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEFACTORY_H
#define MIMO_QUEUEFACTORY_H

#include "Queue.h"
#include "../IFactory.h"


namespace mimo {

    using IQueueFactory = IFactory<IQueue>;

    class QueueFactory : public IQueueFactory {
    public:
        explicit QueueFactory(unsigned int capacity = 10) : capacity(capacity) {}

        IQueue *make_raw() override {
            return new Queue(this->capacity);
        }

    private:
        unsigned int capacity;
    };
}


#endif //MIMO_QUEUEFACTORY_H
