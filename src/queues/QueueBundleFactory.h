/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLEFACTORY_H
#define MIMO_QUEUEBUNDLEFACTORY_H

#include "IQueueBundleFactory.h"

#include "../IFactory.h"
#include "IQueueBundle.h"


namespace mimo {

    using IQueueFactory = IFactory<IQueue>;

    class QueueBundleFactory : public IQueueBundleFactory {
    public:
        QueueBundleFactory();
        explicit QueueBundleFactory(std::shared_ptr<IQueueFactory> factory);

        std::shared_ptr<IQueueBundle> make_shared(workflow::InputMap &inputs) override;
        std::shared_ptr<IQueueBundle> make_shared(workflow::OutputMap &outputs) override;

        std::unique_ptr<IQueueBundle> make_unique(workflow::InputMap &inputs) override;
        std::unique_ptr<IQueueBundle> make_unique(workflow::OutputMap &outputs) override;
    private:
        std::shared_ptr<IQueueFactory> factory;

        IQueueBundle *make_raw(std::shared_ptr<ConnectionMap> connections);
        std::shared_ptr<ConnectionMap> upcast_connections(const workflow::InputMap &inputs) const;
        std::shared_ptr<ConnectionMap> upcast_connections(const workflow::OutputMap &outputs) const;
    };
}


#endif //MIMO_QUEUEBUNDLEFACTORY_H
