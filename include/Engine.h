/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * @brief
 */

#ifndef MIMO_ENGINE_H
#define MIMO_ENGINE_H

#include <unordered_map>
#include "workflow/Step.h"


namespace mimo {

    class Step;

    class Engine {
    public:

        template<typename T, typename P>
        void register_step<T>(std::shared_ptr<workflow::Step> identifier, P&&... args);

        void run();

    private:

        std::unordered_map<unsigned int, std::unique_ptr<Step> (*)()> constructors;

    };
}


#endif //MIMO_ENGINE_H
