/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLEFACTORY_H
#define MIMO_QUEUEBUNDLEFACTORY_H

#include "IQueueBundleFactory.h"


namespace mimo {

    class IQueueFactory;

    class QueueBundleFactory : public IQueueBundleFactory {
    public:
        QueueBundleFactory();
        explicit QueueBundleFactory(std::shared_ptr<IQueueFactory> factory);

        std::shared_ptr<IQueueBundle> make_shared(std::shared_ptr<workflow::InputMap>) override;
        std::shared_ptr<IQueueBundle> make_shared(std::shared_ptr<workflow::OutputMap>) override;
    private:
        std::shared_ptr<IQueueFactory> factory;
    };
}


#endif //MIMO_QUEUEBUNDLEFACTORY_H
