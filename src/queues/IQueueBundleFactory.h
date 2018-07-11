/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLEFACTORY_H
#define MIMO_IQUEUEBUNDLEFACTORY_H

#include <workflow/Step.h>


namespace mimo {

    class IQueueBundle;

    class IQueueBundleFactory {
    public:
        virtual ~IQueueBundleFactory() = default;

        virtual std::shared_ptr<IQueueBundle> make_shared(workflow::InputMap &inputs) = 0;
        virtual std::shared_ptr<IQueueBundle> make_shared(workflow::OutputMap &outputs) = 0;

        virtual std::unique_ptr<IQueueBundle> make_unique(workflow::InputMap &inputs) = 0;
        virtual std::unique_ptr<IQueueBundle> make_unique(workflow::OutputMap &outputs) = 0;
    };
}

#endif //MIMO_IQUEUEBUNDLEFACTORY_H
