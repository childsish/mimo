/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLEFACTORY_H
#define MIMO_IQUEUEBUNDLEFACTORY_H

#include <workflow/Step.h>


namespace mimo {

    class IQueueBundle;

    class IQueueBundleFactory {
    public:
        virtual IQueueBundle *make_raw(const workflow::InputMap &inputs) const = 0;
        virtual std::shared_ptr<IQueueBundle> make_shared(const workflow::InputMap &inputs) const = 0;
        virtual std::unique_ptr<IQueueBundle> make_unique(const workflow::InputMap &inputs) const = 0;

        virtual IQueueBundle *make_raw(const workflow::OutputMap &outputs) const = 0;
        virtual std::shared_ptr<IQueueBundle> make_shared(const workflow::OutputMap &outputs) const = 0;
        virtual std::unique_ptr<IQueueBundle> make_unique(const workflow::OutputMap &outputs) const = 0;
    };
}


#endif //MIMO_IQUEUEBUNDLEFACTORY_H
