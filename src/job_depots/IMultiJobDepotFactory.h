/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IMULTIJOBDEPOTFACTORY_H
#define MIMO_IMULTIJOBDEPOTFACTORY_H

#include <workflow/Workflow.h>


namespace mimo {

    class IMultiJobDepot;
    class Step;

    class IMultiJobDepotFactory {
    public:

        virtual ~IMultiJobDepotFactory() = default;

        virtual void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) = 0;

        virtual std::unique_ptr<IMultiJobDepot> make_depot(
            std::shared_ptr<workflow::Workflow> workflow
        ) const = 0;

    };
}


#endif //MIMO_IMULTIJOBDEPOTFACTORY_H
