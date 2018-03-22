/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBMANAGERFACTORY_H
#define MIMO_IJOBMANAGERFACTORY_H

#include <memory>
#include <workflow/Step.h>


namespace mimo {

    class IJobManager;
    class Step;
    class Step;
    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class ISingleJobManagerFactory {
    public:

        virtual ~ISingleJobManagerFactory() = default;

        virtual void register_step(
            const std::shared_ptr<workflow::Step> &identifier,
            StepConstructor step_constructor
        ) = 0;

        virtual std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const = 0;

    };
}

#endif //MIMO_IJOBMANAGERFACTORY_H
