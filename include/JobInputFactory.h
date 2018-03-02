/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBINPUTFACTORY_H
#define MIMO_JOBINPUTFACTORY_H

#include <workflow/Step.h>
#include "QueueFactory.h"


namespace mimo {

    class IJobInputs;
    using IJobInputsFactory = IFactory<IJobInputs, const workflow::InputMap &>;

    class JobInputFactory : public IJobInputsFactory {
    public:
        explicit JobInputFactory(std::shared_ptr<IQueueFactory> queue_factory = std::make_shared<QueueFactory>());

        IJobInputs *make_raw(const workflow::InputMap &inputs) const override;

        std::shared_ptr<IJobInputs> make_shared(const workflow::InputMap &inputs) const override;

        std::unique_ptr<IJobInputs> make_unique(const workflow::InputMap &inputs) const override;

    private:
        std::shared_ptr<IQueueFactory> queue_factory;
    };
}


#endif //MIMO_JOBINPUTFACTORY_H
