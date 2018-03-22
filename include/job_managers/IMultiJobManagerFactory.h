/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IMULTIJOBMANAGERFACTORY_H
#define MIMO_IMULTIJOBMANAGERFACTORY_H

#include <workflow/Workflow.h>


namespace mimo {

    class IJobManager;
    class ISingleJobManagerFactory;
    class Step;
    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class IMultiJobManagerFactory {
    public:

        virtual ~IMultiJobManagerFactory() = default;

        virtual void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) = 0;

        virtual std::unique_ptr<IJobManager> make_manager(
            std::shared_ptr<workflow::Workflow> workflow
        ) const = 0;

    };
}


#endif //MIMO_IMULTIJOBMANAGERFACTORY_H
