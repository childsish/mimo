/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MULTIJOBMANAGERFACTORY_H
#define MIMO_MULTIJOBMANAGERFACTORY_H

#include "IMultiJobManagerFactory.h"
#include "SingleJobManagerFactory.h"


namespace mimo {

    class MultiJobManagerFactory : public IMultiJobManagerFactory {
    public:

        explicit MultiJobManagerFactory(
            std::shared_ptr<ISingleJobManagerFactory> factory = std::make_shared<SingleJobManagerFactory>(10)
        );

        ~MultiJobManagerFactory();

        void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) override;

        std::unique_ptr<IJobManager> make_manager(
            std::shared_ptr<workflow::Workflow> workflow
        ) const override;

    private:

        std::shared_ptr<ISingleJobManagerFactory> factory;

    };
}


#endif //MIMO_MULTIJOBMANAGERFACTORY_H
