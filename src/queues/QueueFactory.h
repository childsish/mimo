/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEFACTORY_H
#define MIMO_QUEUEFACTORY_H

#include <memory>
#include "../Factory.h"


namespace mimo {

    class IQueue;
    using IQueueFactory = IFactory<IQueue>;

    class QueueFactory : public IQueueFactory {
    public:
        explicit QueueFactory(unsigned int capacity = 10);

        IQueue *make_raw() const override;

    private:
        unsigned int capacity;
    };
}


#endif //MIMO_QUEUEFACTORY_H
