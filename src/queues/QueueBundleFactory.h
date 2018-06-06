/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLEFACTORY_H
#define MIMO_QUEUEBUNDLEFACTORY_H

#include <workflow/Connection.h>
#include <workflow/Step.h>
#include "QueueBundle.h"
#include "QueueFactory.h"


namespace mimo {

    using IQueueBundleFactory = IFactory<IQueueBundle, std::shared_ptr<workflow::ConnectionMap>>;

    class QueueBundleFactory : public IQueueBundleFactory {
    public:
        explicit QueueBundleFactory(
            std::shared_ptr<IFactory<IQueue>> factory = std::make_shared<QueueFactory>()
        ) :
            factory(std::move(factory)) {}
        
        IQueueBundle *make_raw(
            std::shared_ptr<workflow::ConnectionMap> connections
        ) const override {
            return new QueueBundle(std::move(connections), this->factory);
        }
    private:
        std::shared_ptr<IFactory<IQueue>> factory;
    };
}


#endif //MIMO_QUEUEBUNDLEFACTORY_H
