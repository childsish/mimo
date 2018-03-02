/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_STEPFACTORY_H
#define MIMO_STEPFACTORY_H

#include <memory>
#include <workflow/Step.h>
#include "Step.h"


namespace mimo {

    using StepConstructor = std::function<std::shared_ptr<Step>()>;

    class StepFactory {
    public:

        template<typename T, typename ... Args>
        void register_step(std::shared_ptr<workflow::Step> identifier, Args ... args) {
            this->step_constructors.emplace(
                identifier,
                [&]{ return std::make_shared<T>(std::forward<Args>(args)...); }
            );
        }

        std::shared_ptr<Step> make_step(std::shared_ptr<workflow::Step> identifier) const {
            return this->step_constructors.at(identifier)();
        }

    private:

        std::unordered_map<std::shared_ptr<workflow::Step>, StepConstructor> step_constructors;

    };
}


#endif //MIMO_STEPFACTORY_H
