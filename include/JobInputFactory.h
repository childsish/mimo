/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBINPUTFACTORY_H
#define MIMO_JOBINPUTFACTORY_H

#include <workflow/Step.h>
#include <queues/JobInputs.h>
#include <queues/Queue.h>
#include "IFactory.h"


namespace mimo {

    using IJobInputsFactory = IFactory<IJobInputs, const workflow::InputMap &>;

    class JobInputFactory : public IJobInputsFactory {
    public:
        explicit JobInputFactory(std::shared_ptr<IQueueFactory> queue_factory);

        IJobInputs *make_raw(const workflow::InputMap &inputs) const override;

        std::shared_ptr<IJobInputs> make_shared(const workflow::InputMap &inputs) const override;

        std::unique_ptr<IJobInputs> make_unique(const workflow::InputMap &inputs) const override;

    private:
        std::shared_ptr<IQueueFactory> queue_factory;
    };
}


#endif //MIMO_JOBINPUTFACTORY_H
