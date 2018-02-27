/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBOUTPUTFACTORY_H
#define MIMO_JOBOUTPUTFACTORY_H

#include <workflow/Step.h>
#include "QueueFactory.h"
#include "IFactory.h"


namespace mimo {

    class IJobOutputs;
    using IJobOutputsFactory = IFactory<IJobOutputs, const workflow::OutputMap &>;

    class JobOutputFactory : public IJobOutputsFactory {
    public:
        explicit JobOutputFactory(std::shared_ptr<IQueueFactory> queue_factory);

        IJobOutputs *make_raw(const workflow::OutputMap &outputs) const override;

        std::shared_ptr<IJobOutputs> make_shared(const workflow::OutputMap &outputs) const override;

        std::unique_ptr<IJobOutputs> make_unique(const workflow::OutputMap &outputs) const override;

    private:
        std::shared_ptr<IQueueFactory> queue_factory;
    };
}


#endif //MIMO_JOBOUTPUTFACTORY_H
