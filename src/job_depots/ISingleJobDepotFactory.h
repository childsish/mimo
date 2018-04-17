/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBDEPOTFACTORY_H
#define MIMO_IJOBDEPOTFACTORY_H

#include <memory>
#include <workflow/Step.h>


namespace mimo {

    class IJobDepot;
    class Step;
    class Step;
    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class ISingleJobDepotFactory {
    public:

        virtual ~ISingleJobDepotFactory() = default;

        virtual void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) = 0;

        virtual std::unique_ptr<IJobDepot> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const = 0;

    };
}

#endif //MIMO_IJOBDEPOTFACTORY_H
