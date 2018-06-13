/** @author Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ENGINE_H
#define MIMO_ENGINE_H

#include <unordered_map>
#include <workflow/Workflow.h>
#include "../../src/depots/MultiJobDepotFactory.h"


namespace mimo {

    class Step;

    class Engine {
    public:

        explicit Engine(
            std::shared_ptr<IMultiJobDepotFactory > factory = std::make_shared<MultiJobDepotFactory>()
        );

        void register_step(std::shared_ptr<workflow::Step> step_id, std::shared_ptr<Step> step);

        void run(std::shared_ptr<workflow::Workflow> workflow);

    private:

        std::shared_ptr<IMultiJobDepotFactory> factory;

    };
}

#endif //MIMO_ENGINE_H
