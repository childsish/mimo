#ifndef MIMO_IQUEUEFACTORY_H
#define MIMO_IQUEUEFACTORY_H

#include <memory>


namespace mimo {

    class Entity;

    class IQueueFactory {
    public:

        virtual std::unique_ptr<Entity> make() const = 0;

    };
}


#endif //MIMO_IQUEUEFACTORY_H
