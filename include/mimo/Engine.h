/** @author Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ENGINE_H
#define MIMO_ENGINE_H

#include <unordered_map>
#include <workflow/Workflow.h>
#include "../src/JobFactory.h"


namespace mimo {

    class IMultiJobDepotFactory;
    class Step;

    class Engine {
    public:

        explicit Engine(
            unsigned int capacity = 10,
            std::shared_ptr<IJobFactory> factory = std::make_shared<JobFactory>()
        );

        explicit Engine(std::shared_ptr<IMultiJobDepotFactory> factory);

        void register_step(const std::shared_ptr<workflow::Step> &identifier, std::shared_ptr<Step> step);

        void run(std::shared_ptr<workflow::Workflow> workflow);

    private:

        std::shared_ptr<IMultiJobDepotFactory> factory;

    };
}

#endif //MIMO_ENGINE_H
