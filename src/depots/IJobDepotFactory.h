/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBDEPOTFACTORY_H
#define MIMO_IJOBDEPOTFACTORY_H

#include <workflow/Step.h>
#include "../IFactory.h"


namespace mimo {

    class IJobDepot;
    class Step;

    template<typename Derived, typename ... Args>
    class IJobDepotFactory : public IFactory<Derived, Args...> {
    public:
        virtual void register_step(
            std::shared_ptr<workflow::Step> step_id,
            std::shared_ptr<Step> step
        ) = 0;
    };
}

#endif //MIMO_IJOBDEPOTFACTORY_H
