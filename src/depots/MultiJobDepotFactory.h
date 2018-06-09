/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MULTIJOBDEPOTFACTORY_H
#define MIMO_MULTIJOBDEPOTFACTORY_H

#include <workflow/Workflow.h>
#include "MultiJobDepot.h"
#include "SingleJobDepotFactory.h"


namespace mimo {

    class IMultiJobDepot;
    class Step;
    using IMultiJobDepotFactory = IFactory<IMultiJobDepot, std::shared_ptr<workflow::Workflow>>;

    class MultiJobDepotFactory : public IMultiJobDepotFactory {
    public:
        explicit MultiJobDepotFactory(
            std::shared_ptr<ISingleJobDepotFactory> factory = std::make_shared<SingleJobDepotFactory>()
        ) : factory(factory) {}

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
