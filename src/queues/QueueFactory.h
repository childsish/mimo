/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEFACTORY_H
#define MIMO_QUEUEFACTORY_H

#include <memory>
#include "Queue.h"
#include "../IFactory.h"


namespace mimo {

    class QueueFactory : public IFactory<IQueue> {
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
