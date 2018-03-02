/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBFACTORY_H
#define MIMO_JOBFACTORY_H

#include "IFactory.h"
#include "JobInputFactory.h"
#include "JobOutputFactory.h"


namespace mimo {

    class IJob;
    class Step;
    using IJobFactory = IFactory<IJob, const std::shared_ptr<workflow::Step>, std::shared_ptr<Step>>;

    class JobFactory : public IJobFactory {
    public:

        JobFactory(std::shared_ptr<IJobInputsFactory> inputs_factory = std::make_shared<JobInputFactory>(),
                   std::shared_ptr<IJobOutputsFactory> outputs_factory = std::make_shared<JobOutputFactory>());

        IJob *make_raw(const std::shared_ptr<workflow::Step> identifier, std::shared_ptr<Step> step) const override;

        std::shared_ptr<IJob> make_shared(const std::shared_ptr<workflow::Step> identifier, std::shared_ptr<Step> step) const override;

        std::unique_ptr<IJob> make_unique(const std::shared_ptr<workflow::Step> identifier, std::shared_ptr<Step> step) const override;

    private:
        const std::shared_ptr<IJobInputsFactory> inputs_factory;
        const std::shared_ptr<IJobOutputsFactory> outputs_factory;
    };
}

#endif //MIMO_JOBFACTORY_H
