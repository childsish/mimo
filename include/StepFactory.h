/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_STEPFACTORY_H
#define MIMO_STEPFACTORY_H

#include <workflow/Step.h>

#include <memory>
#include <unordered_map>


namespace mimo {

    class Step;

    class StepFactory {
    public:

        template<typename T, typename ... Args>
        void register_step(std::shared_ptr<workflow::Step> identifier, Args&& ... args) {
            this->steps[identifier->identifier] = [args...](){
                return std::make_unique<T>(std::forward<Args>(args)...);
            };
        }

        template<typename T>
        std::unique_ptr<T> make_step(std::shared_ptr<workflow::Step> identifier) {
            return std::static_pointer_cast<T>(this->steps[identifier->identifier]());
        }


    private:

        std::unordered_map<unsigned int, std::unique_ptr<Step>(*)()> steps;
    };
}


#endif //MIMO_STEPFACTORY_H
