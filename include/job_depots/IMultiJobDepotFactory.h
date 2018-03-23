/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IMULTIJOBDEPOTFACTORY_H
#define MIMO_IMULTIJOBDEPOTFACTORY_H

#include <workflow/Workflow.h>


namespace mimo {

    class IJobDepot;
    class ISingleJobDepotFactory;
    class Step;
    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class IMultiJobDepotFactory {
    public:

        virtual ~IMultiJobDepotFactory() = default;

        virtual void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) = 0;

        virtual std::unique_ptr<IJobDepot> make_manager(
            std::shared_ptr<workflow::Workflow> workflow
        ) const = 0;

    };
}


#endif //MIMO_IMULTIJOBDEPOTFACTORY_H
