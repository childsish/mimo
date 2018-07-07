/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLEFACTORY_H
#define MIMO_IQUEUEBUNDLEFACTORY_H

#include <workflow/Step.h>


namespace mimo {

    class IQueueBundle;

    class IQueueBundleFactory {
    public:
        virtual ~IQueueBundleFactory() = default;

        virtual std::shared_ptr<IQueueBundle> make_shared(std::shared_ptr<workflow::InputMap>) = 0;
        virtual std::shared_ptr<IQueueBundle> make_shared(std::shared_ptr<workflow::OutputMap>) = 0;
    };
}

#endif //MIMO_IQUEUEBUNDLEFACTORY_H
