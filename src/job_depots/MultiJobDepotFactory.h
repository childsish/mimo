/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MULTIJOBDEPOTFACTORY_H
#define MIMO_MULTIJOBDEPOTFACTORY_H

#include "IMultiJobDepotFactory.h"
#include "SingleJobDepotFactory.h"


namespace mimo {

    class MultiJobDepotFactory : public IMultiJobDepotFactory {
    public:

        explicit MultiJobDepotFactory(
            std::shared_ptr<ISingleJobDepotFactory> factory = std::make_shared<SingleJobDepotFactory>(10)
        );

        ~MultiJobDepotFactory();

        void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) override;

        std::unique_ptr<IJobDepot> make_depot(
            std::shared_ptr<workflow::Workflow> workflow
        ) const override;

    private:

        std::shared_ptr<ISingleJobDepotFactory> factory;

    };
}


#endif //MIMO_MULTIJOBDEPOTFACTORY_H
