#ifndef MIMO_IQUEUEFACTORY_H
#define MIMO_IQUEUEFACTORY_H

#include <memory>


namespace mimo {

    class IQueue;

    class IQueueFactory {
    public:

        virtual std::unique_ptr<IQueue> make() = 0;

    };
}


#endif //MIMO_IQUEUEFACTORY_H
