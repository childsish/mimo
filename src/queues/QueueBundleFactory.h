/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLEFACTORY_H
#define MIMO_QUEUEBUNDLEFACTORY_H

#include <workflow/Connection.h>
#include <workflow/Step.h>
#include "IQueueBundleFactory.h"
#include "QueueFactory.h"


namespace mimo {

    class QueueBundleFactory : public IQueueBundleFactory {
    public:
        explicit QueueBundleFactory(std::shared_ptr<IQueueFactory> factory = std::make_shared<QueueFactory>());
        
        IQueueBundle *make_raw(const workflow::InputMap &inputs) const override;
        std::shared_ptr<IQueueBundle> make_shared(const workflow::InputMap &inputs) const override;
        std::unique_ptr<IQueueBundle> make_unique(const workflow::InputMap &inputs) const override;

        IQueueBundle *make_raw(const workflow::OutputMap &outputs) const override;
        std::shared_ptr<IQueueBundle> make_shared(const workflow::OutputMap &outputs) const override;
        std::unique_ptr<IQueueBundle> make_unique(const workflow::OutputMap &outputs) const override;

    private:
        std::shared_ptr<IQueueFactory> factory;
    };
}


#endif //MIMO_QUEUEBUNDLEFACTORY_H
