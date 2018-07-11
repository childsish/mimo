/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MULTIJOBDEPOTFACTORY_H
#define MIMO_MULTIJOBDEPOTFACTORY_H

#include <workflow/Workflow.h>
#include "IJobDepotFactory.h"
#include "MultiJobDepot.h"
#include "SingleJobDepotFactory.h"


namespace mimo {

    class IMultiJobDepot;
    class Step;
    using IMultiJobDepotFactory = IJobDepotFactory<
        IMultiJobDepot,
        std::shared_ptr<workflow::Workflow>
    >;

    class MultiJobDepotFactory : public IMultiJobDepotFactory {
    public:
        explicit MultiJobDepotFactory(
            std::shared_ptr<ISingleJobDepotFactory> factory = std::make_shared<SingleJobDepotFactory>()
        ) :
            factory(std::move(factory)) {}

        void register_step(
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step
        ) override {
            this->factory->register_step(std::move(step_id), std::move(step));
        }

        IMultiJobDepot *make_raw(
            std::shared_ptr<workflow::Workflow> workflow
        ) override {
            return new MultiJobDepot(workflow, this->factory);
        }
    private:
        std::shared_ptr<ISingleJobDepotFactory> factory;
    };
}


#endif //MIMO_MULTIJOBDEPOTFACTORY_H
